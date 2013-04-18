#include <QMessageBox>
#include <QDebug>
#include <QGLShader>
#include <vector>
#include <GL/GLU.h>
#include <QApplication>

#include "axisviewwidget.h"
#include "mainwindow.h"

namespace M3DEditGUI{
static float snapToGrid(float pos);

AxisViewWidget::AxisViewWidget(QWidget *parent):
    QGLWidget(parent), camera(M3DEditRender::UNSET)
{
    this->renderer = M3DEditRender::g_axisRenderer;

    QSizePolicy policy;
    policy.setHeightForWidth(true);
    this->setSizePolicy(policy);
    mouseTrackRight = false;
    mouseTrackLeft = false;
    mouseMoved = false;
    initialized = false;
    toolState = SELECT;
    camLine = false;
    brushToggle = false;
    validBrush = false;
    resizeMode = false;
}

void AxisViewWidget::setAxisLock(M3DEditRender::AxisLock lock)
{
    this->camera.setLock(lock);

}

int AxisViewWidget::heightForWidth(int w) const
{
    return w;
}

void AxisViewWidget::paintGL(){
    this->makeCurrent();
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    qDebug()<<"[AxisView] paintGL";
    //MainWindow *mainWnd = qobject_cast<MainWindow*>(QApplication::activeWindow());
    renderer->drawGrid(10, program, camera);
    glClear(GL_DEPTH_BUFFER_BIT);
    renderer->renderOrigin(program, camera);
    glClear(GL_DEPTH_BUFFER_BIT);


    for(QMap<int, QGLBuffer>::Iterator itr = geoVerts.begin(); itr != geoVerts.end(); ++itr){
        QGLBuffer vertBuffer = itr.value();
        QGLBuffer indexBuffer = geoIndex[itr.key()];

        bool selected = false;
        if(mainWnd->getSelected() == itr.key())
            selected = true;
        this->makeCurrent();
        renderer->render(camera, program, vertBuffer, indexBuffer, selected);
    }

    //Needs grid and ui

    if(camLine)
        renderer->drawCamLine(camFrom, camTo, program, camera);
    if(this->brushToggle)
        renderer->drawBrush(brush, program, camera);

    this->swapBuffers();
}

void AxisViewWidget::resizeGL(int width, int height){
    glViewport(0, 0, width, height);
    this->camera.updateProjection(width, height);
    qDebug()<<"[AxisView] set viewport to "<<width<<" "<<height;
    this->paintGL();
}

void AxisViewWidget::initializeGL(){
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    if(!(QGLShader::hasOpenGLShaders(QGLShader::Vertex) &&
         QGLShader::hasOpenGLShaders(QGLShader::Fragment)))
    {
        qDebug()<<"No vertex and fragment shader support!\n";
        QMessageBox::critical(this, "Error", "This computer does not support vertex and fragment shaders.\nExiting");
        qFatal("No vert/frag shader support found!");
    }

    QGLShader vertexShader(QGLShader::Vertex);
    QGLShader fragShader(QGLShader::Fragment);

    if(!vertexShader.compileSourceFile(":/shaders/axisvert.vert")){
        qDebug()<<"Failed to compile vertex shader:\n"<<vertexShader.log()<<"\n";
        QMessageBox::critical(this, "Error", "Failed to compile axis vertex shader");
        qFatal("Failed to compile axis vertex shader");
    }

    if(!fragShader.compileSourceFile(":/shaders/axisfrag.frag")){
        qDebug()<<"Failed to compile frag shader:\n"<<fragShader.log()<<"\n";
        QMessageBox::critical(this, "Error", "Failed to compile axis frag shader");
        qFatal("Failed to compile axis frag shader");
    }

    program.addShader(&vertexShader);
    program.addShader(&fragShader);

    if(!program.link()){
        qDebug()<<"Failed to link axis shader program:\n"<<program.log()<<"\n";
        QMessageBox::critical(this, "Error", "Failed to link axis shader program");
        qFatal("Failed to link axis shader program");
    }
    this->initialized = true;
}

void AxisViewWidget::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<"[AxisView] mousePressEvent";
    Qt::MouseButton button = event->button();
    Q_ASSERT(!(mouseTrackLeft && mouseTrackRight));
    //ignore this press if already tracking
    if(mouseTrackLeft || mouseTrackRight){
        event->ignore();
        return;
    }

    switch(button){
    case Qt::RightButton:
        this->mouseTrackRight = true; break;
    case Qt::LeftButton:
        this->mouseTrackLeft = true;
        this->mouseMoved = false;
        if(toolState == SELECT){
            int currentSelection = mainWnd->getSelected();
            if (currentSelection != -1 && currentSelection == selectFromPoint(event->pos())){
                qDebug()<<"[AxisView] entering resize mode";
                this->resizeMode = true;
                this->resizeTarget = M3DEditLevel::g_geoMgr->getGeometry(currentSelection);
                this->resizeStart = getPosition(event->pos());
                resizeStart.setX(snapToGrid(resizeStart.x()));
                resizeStart.setY(snapToGrid(resizeStart.y()));
                resizeStart.setZ(snapToGrid(resizeStart.z()));
                qDebug()<<"[AxisView] resizeStart ="<<resizeStart.x()<<resizeStart.y()<<resizeStart.z();
            }
        }
        break;
    }

    this->lastLocalMouse = event->pos();
    this->lastMousePosition = event->globalPos();
    this->firstMousePosition = event->pos();


    event->accept();
}

void AxisViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug()<<"[AxisView] mouseReleaseEvent";
    Q_ASSERT(!(mouseTrackLeft && mouseTrackRight));
    Qt::MouseButton button = event->button();
    switch(button){
    case Qt::RightButton:
        this->mouseTrackRight = false; break;
    case Qt::LeftButton:
        switch(this->toolState)
        {
        case SELECT:
            if(mouseTrackLeft){
                this->mouseTrackLeft = false;
                if(mouseMoved)
                {
                    mouseMoved = false;
                    resizeMode = false;
                    qDebug()<<"[AxisView] exit resizeMode";
                }
                else{
                    emit selectedGeo(selectFromPoint(event->pos()));
                }
            }
            break;
        case CAMERA:
            if(mouseTrackLeft){
                this->mouseTrackLeft = false;
                QVector3D camPos = this->getPosition( this->lastLocalMouse, false);
                QVector3D targPos = this->getPosition(event->pos(), false);
                camLine = false;
                this->paintGL();
                emit newCamPos(camPos, targPos);
            }
            break;
        case GEOMETRY:
            if(mouseTrackLeft){
                this->mouseTrackLeft = false;
                this->brushToggle = false;
                emit brushStop();

                if(validBrush)
                {
                    M3DEditLevel::Geometry *newGeo = new M3DEditLevel::Box(brush);
                    M3DEditLevel::g_geoMgr->addGeometry(newGeo);
                    validBrush = false;
                }
            }
            break;
        }
    }
}



void AxisViewWidget::mouseMoveEvent(QMouseEvent *event)
{
    this->makeCurrent();
    qDebug()<<"[AxisView] mouseMoveEvent";
    Q_ASSERT(!(mouseTrackLeft && mouseTrackRight));

    if(mouseTrackLeft){
        switch(this->toolState){
        case SELECT:
            mouseMoved = true;
            if(resizeMode){
                QVector3D newPos = this->getPosition(event->pos());
                switch(camera.getLock()){
                case M3DEditRender::XY:
                    newPos.setZ(resizeStart.z());
                    break;
                case M3DEditRender::XZ:
                    newPos.setY(resizeStart.y());
                    break;
                case M3DEditRender::YZ:
                    newPos.setX(resizeStart.x());
                    break;
                }

                newPos.setX(snapToGrid(newPos.x()));
                newPos.setY(snapToGrid(newPos.y()));
                newPos.setZ(snapToGrid(newPos.z()));
                qDebug()<<"[AxisView] newPos for resize ="<<newPos.x()<<newPos.y()<<newPos.z();
                if(newPos != resizeStart){
                    qDebug()<<"[AxisView] Using new position";
                    resizeTarget->resize(resizeStart, newPos);
                    M3DEditLevel::g_geoMgr->updateGeometry(mainWnd->getSelected(), resizeTarget);
                }
            }
            break;
        case CAMERA:
            camFrom = getPosition(lastLocalMouse, false);
            camTo = getPosition(event->pos(), false);
            camLine = true;
            this->paintGL();
            break;
        case GEOMETRY:
            this->lastLocalMouse = event->pos();
            QVector3D brushStart = this->findClosestGrid(this->firstMousePosition);
            QVector3D brushEnd = this->findClosestGrid(lastLocalMouse);
            if(brushStart != brushEnd)
                validBrush = true;

            switch(camera.getLock()){
            case M3DEditRender::XY:
                brushEnd.setZ(brushEnd.z() - 10); break;
            case M3DEditRender::XZ:
                brushEnd.setY(brushEnd.y() - 10);break;
            case M3DEditRender::YZ:
                brushEnd.setX(brushEnd.x() + 10); break;
            }
            QVector<QVector3D> brushBounds;
            brushBounds.push_back(brushStart);
            brushBounds.push_back(brushEnd);
            this->brush.setBounds(brushBounds);
            this->brushToggle = true;
            this->paintGL();
            emit brushDraw(brush);
            break;
        }
    }

    if(mouseTrackRight){
        QPoint pos = event->globalPos();
        QPoint distance = pos - lastMousePosition;
        camera.moveVec(distance);
        lastMousePosition = pos;
        this->paintGL();
    }
}

void AxisViewWidget::wheelEvent(QWheelEvent *event)
{
    this->makeCurrent();
    qDebug()<<"[AxisView] Wheel event delta: "<<event->delta();
    if (event->delta() > 0){
        camera.adjustGrid(1, 1);
        this->paintGL();
        event->accept();
    }
    if (event->delta() < 0){
        camera.adjustGrid(-1, -1);
        this->paintGL();
        event->accept();
    }
}



int AxisViewWidget::selectFromPoint(QPoint point)
{
    int id = M3DEditLevel::g_geoMgr->findGeo(getPosition(point));
    qDebug()<<"[AxisView] selection gave id "<<id;

    return id;
}

QVector3D AxisViewWidget::getPosition(QPoint point, bool useDepth)
{
    QVector3D position(point);

    QMatrix4x4 mvp = camera.getProjMatrix();
    QMatrix4x4 ident;
    QMatrix4x4 invY;
    QMatrix4x4 yCoB;

    invY.setToIdentity();
    invY.setRow(1, QVector4D(0.0f,-1.0f, 0.0f, 0.0f));

    yCoB.setToIdentity();
    yCoB.translate(0, this->height(), 0);

    position = yCoB * invY * position;

    ident.setToIdentity();

    this->makeCurrent();
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);



    if(useDepth){
        qDebug()<<"[AxisView] Using Depth";
        GLfloat z[5*5];
        glReadPixels(position.x() - 2, position.y() - 2, 5, 5, GL_DEPTH_COMPONENT, GL_FLOAT, &z);

        float smallest = 1;
        for(int i = 0; i < 25; ++i)
            if(z[i]< smallest)
                smallest = z[i];

        position.setZ(smallest);
    }
    else
        position.setZ(0.5f);

    qDebug()<<"[AxisView] new pos: x: "<<position.x()<<" y: "<< position.y()<<
              " z: "<<position.z();

    GLdouble worldX, worldY, worldZ;
    gluUnProject(position.x(), position.y(), position.z(), ident.data(),
                 mvp.data(),viewport, &worldX, &worldY, &worldZ);

    qDebug()<<"[AxisView] translated pos: x:"<<worldX<<
              " y:"<<worldY<<" z:"<<worldZ;

    return QVector3D(worldX, worldY, worldZ);
}

static float snapToGrid(float pos){
    int gridOff = 1;

    if(pos < 1)
        gridOff = -1;

    int gridNum = pos /10;
    if(((int)pos % (10))*gridOff >= 5)
            gridNum += gridOff;

    return gridNum * 10;
}

QVector3D AxisViewWidget::findClosestGrid(QPoint point)
{
    QVector3D pos = this->getPosition(point, false);


    pos.setX(snapToGrid(pos.x()));
    pos.setY(snapToGrid(pos.y()));
    pos.setZ(snapToGrid(pos.z()));

    return pos;
}


void AxisViewWidget::addGeometry(int id, M3DEditLevel::Geometry *geo)
{
    Q_CHECK_PTR(geo);

    QVector<QVector3D> verts = geo->getVerticies();
    std::vector<vertAttribute> vertAttrib;
    for(int i = 0; i < verts.size(); ++i){
        vertAttribute attrib;
        attrib.x = verts[i].x();
        attrib.y = verts[i].y();
        attrib.z = verts[i].z();
        attrib.w = 1.0f;
        qDebug()<<"[AxisView]: bufferVerts ("<<i<<") x "<<attrib.x<<" y: "<<attrib.y<< " z"<<attrib.z;
        vertAttrib.push_back(attrib);
    }

    //create a line drawing index
    QVector<unsigned int> indicies = geo->getLineIndex();
    qDebug()<<"[AxisView] indicies size: "<<indicies.size();
    for(int i = 0; i< indicies.size(); i += 2){
        qDebug()<<"[AxisView]: linesIndex "<< i / 2 <<" ("<<indicies[i]<<
                   ", "<<indicies[i+1]<<")";
    }
    this->makeCurrent();

    QGLBuffer buffer;
    if(!buffer.create()){
        qDebug()<<"could not create buffer for geo "<<id<<"\n";
        QMessageBox::critical(this, "Fatal Error", "Could not create a buffer. Exiting.");
        qFatal("Could not create a buffer");
    }

    if(!buffer.bind()){
        qDebug()<<"Could not bind buffer for geo "<<id<<"\n";
        QMessageBox::critical(this, "Fatal Error", "Could not bind buffer. Exiting");
        qFatal("Could not bind buffer");
    }

    buffer.setUsagePattern(QGLBuffer::DynamicDraw);
    buffer.allocate(vertAttrib.data(), sizeof(vertAttribute) * verts.size());

    geoVerts[id] = buffer;

    QGLBuffer indexBuffer(QGLBuffer::IndexBuffer);
    if(!indexBuffer.create()){
        qDebug()<<"Could not create index buffer\n";
        QMessageBox::critical(this, "Fatal Error", "Could not create index buffer");
        qFatal("Could not create index buffer");
    }

    if(!indexBuffer.bind()){
        qDebug()<<"Could not bind index buffer\n";
        QMessageBox::critical(this, "Fatal Error", "Could not create bind buffer");
        qFatal("Could not bind index buffer");
    }

    indexBuffer.setUsagePattern(QGLBuffer::StaticDraw);
    indexBuffer.allocate(indicies.data(), sizeof(unsigned int) * indicies.size());

    geoIndex[id] = indexBuffer;
    qDebug()<<"AxisView: Added geo!";
    this->paintGL();
}

void AxisViewWidget::removeGeometry(int id, M3DEditLevel::Geometry *geo)
{
    if(!geoVerts.contains(id)){
        qDebug()<<"Geometry id "<<id<<"not found to remove\n";
        QMessageBox::critical(this, "Error", "Could not remove geomoetry, id not found");
        qFatal("ID not found remove geometry");
    }

    if(!geoIndex.contains(id)){
        qDebug()<<"Geometry index id "<<id<<" not found to remove\n";
        QMessageBox::critical(this, "Error", "Could not remove geomoetry index, id not found");
        qFatal("ID not found remove geometry index");
    }

    Q_CHECK_PTR(geo);

    QGLBuffer buffer = geoVerts[id];
    geoVerts.remove(id);
    this->makeCurrent();

    buffer.destroy();

    buffer = geoIndex[id];
    buffer.destroy();

    qDebug()<<"[AxisView] removed geo "<<id;
    this->paintGL();
}

void AxisViewWidget::updateGeometry(int id, M3DEditLevel::Geometry *geo)
{
    if(!geoVerts.contains(id)){
        qDebug()<<"Geometry id "<<id<<" not found to update\n";
        QMessageBox::critical(this, "Error", "Could not update geometry buffer. ID not found");
        qFatal("ID not found to update geo buffer");
    }


    Q_CHECK_PTR(geo);
    Q_ASSERT(geoIndex.contains(id));

    QGLBuffer buffer = geoVerts[id];
    QVector<QVector3D> verts = geo->getVerticies();
    std::vector<vertAttribute> vertAttribs;

    Q_ASSERT(verts.size() > 7);
    Q_ASSERT(buffer.isCreated());

    this->makeCurrent();
    buffer.bind();

    for(int i = 0; i < verts.size(); ++i){
        vertAttribute vert;
        vert.x = verts[i].x();
        vert.y = verts[i].y();
        vert.z = verts[i].z();
        vert.w = 1.0f;

        vertAttribs.push_back(vert);
    }

    buffer.write(0,vertAttribs.data(), sizeof(vertAttribute) * vertAttribs.size());

    qDebug()<<"[AxisView] updated geo "<<id;
    this->paintGL();
}

void AxisViewWidget::cameraToggle(bool checked)
{
    if(checked)
        this->toolState = CAMERA;
    else
        this->toolState = SELECT;

    this->mouseTrackLeft = false;
    this->mouseTrackRight = false;
}

void AxisViewWidget::geoToolToggle(bool checked)
{
    if(checked)
        this->toolState = GEOMETRY;
    else
        this->toolState = SELECT;

    this->mouseTrackLeft = false;
    this->mouseTrackRight = false;
}

void AxisViewWidget::drawBrush(M3DEditLevel::Box brush)
{
    qDebug()<<"[AxisView] drawBrush";
    this->brush = brush;
    this->brushToggle = true;
    this->paintGL();
}

void AxisViewWidget::stopBrush()
{
    this->brushToggle = false;
    this->paintGL();
}

}
