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

    //set resize policy
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
    qDebug()<<"[AxisView] paintGL";

    this->makeCurrent();//ensure context is current

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);//dark grey clear color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer->drawGrid(10, program, camera);

    glClear(GL_DEPTH_BUFFER_BIT);//clear depth to draw origin over grid

    renderer->renderOrigin(program, camera);

    //clera depth to draw geometry over origin and ensure only depth information for geometry
    //is in the buffer for finding position
    glClear(GL_DEPTH_BUFFER_BIT);


    //draw each piece of geometry from their buffers
    for(QMap<int, QGLBuffer>::Iterator itr = geoVerts.begin(); itr != geoVerts.end(); ++itr){
        QGLBuffer vertBuffer = itr.value(); //vertex buffer
        QGLBuffer indexBuffer = geoIndex[itr.key()]; //index buffer

        //see if this piece of geometry is selected
        bool selected = false;
        if(mainWnd->getSelected() == itr.key())
            selected = true;

        this->makeCurrent();
        renderer->render(camera, program, vertBuffer, indexBuffer, selected);
    }

    //draw camera line if needed
    if(camLine)
        renderer->drawCamLine(camFrom, camTo, program, camera);

    //draw brush if needed
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
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);//dark grey
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //test for shader support
    if(!(QGLShader::hasOpenGLShaders(QGLShader::Vertex) &&
         QGLShader::hasOpenGLShaders(QGLShader::Fragment)))
    {
        qDebug()<<"No vertex and fragment shader support!\n";
        QMessageBox::critical(this, "Error", "This computer does not support vertex and fragment shaders.\nExiting");
        qFatal("No vert/frag shader support found!");
    }

    //setup shaders then compile
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

    //add shaders to the program then link
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

    Q_ASSERT(!(mouseTrackLeft && mouseTrackRight));//left and right should never both be tracking

    //ignore this press if already tracking
    if(mouseTrackLeft || mouseTrackRight){
        event->ignore();
        return;
    }

    //handle per button
    switch(button){
    case Qt::RightButton:
        this->mouseTrackRight = true; break;

    case Qt::LeftButton:
        this->mouseTrackLeft = true;
        this->mouseMoved = false;

        //if in select mode see if we are clicking on same selection to enable resize mode
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

    //store relevent positions
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

    //handle both button releases
    switch(button){
    case Qt::RightButton:
        this->mouseTrackRight = false; break;

    case Qt::LeftButton:
        //different actions per tool in use
        switch(this->toolState) {
        case SELECT:
            if(mouseTrackLeft){
                this->mouseTrackLeft = false;

                //if moved, clear resize mode
                if(mouseMoved) {
                    mouseMoved = false;
                    resizeMode = false;
                }
                //if it hasn't moved user was trying to select, emit selection to other widgets
                else{
                    emit selectedGeo(selectFromPoint(event->pos()));
                }
            }
            break;

        case CAMERA:
            //if left mouse was being tracked update perspective camera position
            if(mouseTrackLeft){
                this->mouseTrackLeft = false;

                //set new camera positions based on mouse movements
                QVector3D camPos = this->getPosition( this->lastLocalMouse, false);
                QVector3D targPos = this->getPosition(event->pos(), false);

                camLine = false;

                //draw again to clear camera line
                this->paintGL();

                //notify other widgets of new camera positon
                emit newCamPos(camPos, targPos);
            }
            break;

        case GEOMETRY:
            //tracking left click mouse movements?
            if(mouseTrackLeft){
                this->mouseTrackLeft = false;
                this->brushToggle = false;
                emit brushStop();//inform widgets that brush is no longer being drawn

                //if this is a valid piece of geometry add it to the system
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
    qDebug()<<"[AxisView] mouseMoveEvent";

    this->makeCurrent();

    Q_ASSERT(!(mouseTrackLeft && mouseTrackRight));

    //mouse is being moved with left click held down?
    if(mouseTrackLeft){        
        //different actions depending on tool
        switch(this->toolState){
        case SELECT:
            mouseMoved = true; //mouse was moved

            if(resizeMode){
                QVector3D newPos = this->getPosition(event->pos());

                //depending on lock keep resize point the same as start
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

                //recenter point on nearest grid point
                newPos.setX(snapToGrid(newPos.x()));
                newPos.setY(snapToGrid(newPos.y()));
                newPos.setZ(snapToGrid(newPos.z()));

                qDebug()<<"[AxisView] newPos for resize ="<<newPos.x()<<newPos.y()<<newPos.z();

                //if position is different than starting point, resize the geometry and call an update
                if(newPos != resizeStart){

                    qDebug()<<"[AxisView] Using new position";

                    resizeTarget->resize(resizeStart, newPos);
                    M3DEditLevel::g_geoMgr->updateGeometry(mainWnd->getSelected(), resizeTarget);
                }
            }
            break;

        case CAMERA:
            //update camera line postions then draw
            camFrom = getPosition(lastLocalMouse, false);
            camTo = getPosition(event->pos(), false);
            camLine = true;

            this->paintGL();

            break;

        case GEOMETRY:
            //update postion
            this->lastLocalMouse = event->pos();

            //find the start and ending point of the brush on the grid
            QVector3D brushStart = this->findClosestGrid(this->firstMousePosition);
            QVector3D brushEnd = this->findClosestGrid(lastLocalMouse);

            //is it a valid size?
            if(brushStart != brushEnd)
                validBrush = true;

            //set missing axis value based on axis lock
            switch(camera.getLock()){
            case M3DEditRender::XY:
                brushEnd.setZ(brushEnd.z() - 10); break;
            case M3DEditRender::XZ:
                brushEnd.setY(brushEnd.y() - 10);break;
            case M3DEditRender::YZ:
                brushEnd.setX(brushEnd.x() + 10); break;
            }

            //update brush bounds
            QVector<QVector3D> brushBounds;
            brushBounds.push_back(brushStart);
            brushBounds.push_back(brushEnd);
            this->brush.setBounds(brushBounds);

            //set brush mode to true and redraw it
            this->brushToggle = true;
            this->paintGL();

            //inform other widgets that a brush needs to be drawn
            emit brushDraw(brush);
            break;
        }
    }

    //adjust camera based on movement distance then redraw
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

    //depending on mouse wheel direction change grid to zoom
    if (event->delta() > 0){
        camera.adjustGrid(1, 1);//zoom out
        this->paintGL();
        event->accept();
    }
    if (event->delta() < 0){
        camera.adjustGrid(-1, -1);//zoom in
        this->paintGL();
        event->accept();
    }
}



int AxisViewWidget::selectFromPoint(const QPoint point)
{
    int id = M3DEditLevel::g_geoMgr->findGeo(getPosition(point));
    qDebug()<<"[AxisView] selection gave id "<<id;

    return id;
}

QVector3D AxisViewWidget::getPosition(const QPoint point, bool useDepth)
{
    QVector3D position(point);

    QMatrix4x4 mvp = camera.getProjMatrix();
    QMatrix4x4 ident; //identity matrix
    QMatrix4x4 invY; //matrix to invert the Y value
    QMatrix4x4 yCoB; //matrix to performa a change of base for the y value

    //invert the y value
    invY.setToIdentity();
    invY.setRow(1, QVector4D(0.0f,-1.0f, 0.0f, 0.0f));

    //move the y value from top to bottom
    yCoB.setToIdentity();
    yCoB.translate(0, this->height(), 0);

    //moves y window value to a coordiante space with 0,0 at bottom left from top left
    position = yCoB * invY * position;

    ident.setToIdentity();

    //get the viewport's current values
    this->makeCurrent();
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    //if using the depth buffer get the z value using depth buffer
    if(useDepth){
        qDebug()<<"[AxisView] Using Depth";

        //read a 5 by 5 set of values from the depth buffer
        //allows us to find geometry close to the mouse position
        GLfloat z[5*5];
        glReadPixels(position.x() - 2, position.y() - 2, 5, 5, GL_DEPTH_COMPONENT, GL_FLOAT, &z);

        //try to find a value other than the camera position(1)
        float smallest = 1;
        for(int i = 0; i < 25; ++i)
            if(z[i]< smallest)
                smallest = z[i];

        position.setZ(smallest);
    }
    else
        position.setZ(0.5f);//default value

    qDebug()<<"[AxisView] new pos: x: "<<position.x()<<" y: "<< position.y()<<
              " z: "<<position.z();

    //reverse projections on the position to find world space xyz
    GLdouble worldX, worldY, worldZ;
    gluUnProject(position.x(), position.y(), position.z(), ident.data(),
                 mvp.data(),viewport, &worldX, &worldY, &worldZ);

    qDebug()<<"[AxisView] translated pos: x:"<<worldX<<
              " y:"<<worldY<<" z:"<<worldZ;

    return QVector3D(worldX, worldY, worldZ);
}

//! finds a grid position close to supplied position
static float snapToGrid(float pos){
    int gridOff = 1;

    if(pos < 1)
        gridOff = -1;

    int gridNum = pos /10;
    if(((int)pos % (10)) * gridOff >= 5)
            gridNum += gridOff;

    return gridNum * 10;
}

QVector3D AxisViewWidget::findClosestGrid(QPoint point)
{
    //get world space positon fo this point
    QVector3D pos = this->getPosition(point, false);

    //find the closest grid position for this position
    pos.setX(snapToGrid(pos.x()));
    pos.setY(snapToGrid(pos.y()));
    pos.setZ(snapToGrid(pos.z()));

    return pos;
}

//slot for new geometry being added from geo manager
void AxisViewWidget::addGeometry(int id, M3DEditLevel::Geometry *geo)
{
    Q_CHECK_PTR(geo);//never null


    QVector<QVector3D> verts = geo->getVerticies();

    //create buffer data from geometry verticies
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

    //get a line drawing index
    QVector<unsigned int> indicies = geo->getLineIndex();

    this->makeCurrent();

    //create, bind then fill vertex buffer
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

    //store buffer by geometry id
    geoVerts[id] = buffer;

    //create bind then fill index buffer
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

    //store index buffer by geo id
    geoIndex[id] = indexBuffer;

    qDebug()<<"AxisView: Added geo!";

    //redraw view to refelect new geo
    this->paintGL();
}

//geometry was removed slot
void AxisViewWidget::removeGeometry(int id, M3DEditLevel::Geometry *geo)
{
    Q_UNUSED(geo)

    //check if geometry is actually stored. Error if not.
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


    //remove buffers from storage and destroy them
    QGLBuffer buffer = geoVerts[id];
    geoVerts.remove(id);
    this->makeCurrent();

    buffer.destroy();

    buffer = geoIndex[id];
    buffer.destroy();

    qDebug()<<"[AxisView] removed geo "<<id;

    //redraw view to reflect missing geo
    this->paintGL();
}

//value of geometry was changed slot
void AxisViewWidget::updateGeometry(int id, M3DEditLevel::Geometry *geo)
{
    if(!geoVerts.contains(id)){
        qDebug()<<"Geometry id "<<id<<" not found to update\n";
        QMessageBox::critical(this, "Error", "Could not update geometry buffer. ID not found");
        qFatal("ID not found to update geo buffer");
    }


    Q_CHECK_PTR(geo);
    Q_ASSERT(geoIndex.contains(id));

    //get previous buffer object
    QGLBuffer buffer = geoVerts[id];
    QVector<QVector3D> verts = geo->getVerticies();
    std::vector<vertAttribute> vertAttribs;

    Q_ASSERT(verts.size() > 7);
    Q_ASSERT(buffer.isCreated());

    this->makeCurrent();
    buffer.bind();

    //prep new verticies for storage in buffer
    for(int i = 0; i < verts.size(); ++i){
        vertAttribute vert;
        vert.x = verts[i].x();
        vert.y = verts[i].y();
        vert.z = verts[i].z();
        vert.w = 1.0f;

        vertAttribs.push_back(vert);
    }

    //store updated buffer data
    buffer.write(0,vertAttribs.data(), sizeof(vertAttribute) * vertAttribs.size());

    qDebug()<<"[AxisView] updated geo "<<id;

    //redraw to reflect changes
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

    //set local copy of brush then draw it
    this->brush = brush;
    this->brushToggle = true;
    this->paintGL();
}

void AxisViewWidget::stopBrush()
{
    //clear brush drawing mode then redraw
    this->brushToggle = false;
    this->paintGL();
}

}
