#include <QMessageBox>
#include <QDebug>
#include <QGLShader>
#include <vector>

#include "axisviewwidget.h"

namespace M3DEditGUI{
AxisViewWidget::AxisViewWidget(QWidget *parent):
    QGLWidget(parent), camera(M3DEditRender::UNSET)
{
    this->renderer = M3DEditRender::g_axisRenderer;

    QSizePolicy policy;
    policy.setHeightForWidth(true);
    this->setSizePolicy(policy);
    mouseTrackRight = false;
    mouseTrackLeft = false;
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
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT );
    qDebug()<<"[AxisView] paintGL";

    renderer->drawGrid(10, program, camera);
    renderer->renderOrigin(program, camera);


    this->makeCurrent();
    for(QMap<int, QGLBuffer>::Iterator itr = geoVerts.begin(); itr != geoVerts.end(); ++itr){
        QGLBuffer vertBuffer = itr.value();
        QGLBuffer indexBuffer = geoIndex[itr.key()];

        renderer->render(camera, program, vertBuffer, indexBuffer);
    }

    //Needs grid and ui

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
        this->mouseTrackLeft = true; break;
    }

    this->lastMousePosition = event->globalPos();


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
        this->mouseTrackLeft = false; break;
    }
}

void AxisViewWidget::mouseMoveEvent(QMouseEvent *event)
{
    this->makeCurrent();
    qDebug()<<"[AxisView] mouseMoveEvent";
    Q_ASSERT(!(mouseTrackLeft && mouseTrackRight));

    if(mouseTrackLeft){

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

    buffer.write(0,&vertAttribs, sizeof(vertAttribute) * vertAttribs.size());

    qDebug()<<"[AxisView] updated geo "<<id;
    this->paintGL();
}

}
