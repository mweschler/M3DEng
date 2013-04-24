#include <QMessageBox>

#include "renderedviewwidget.h"
#include "materialmanager.h"

namespace M3DEditGUI{
RenderedViewWidget::RenderedViewWidget(QWidget *parent) :
    QGLWidget(parent)
{
    this->renderer = M3DEditRender::g_perspectiveRender;
    leftTrack = false;
    rightTrack = false;

    //connect the timer with the camera and this view
    QObject::connect(&mousepush, SIGNAL(timeout()), &camera, SLOT(moveFoward()));
    QObject::connect(&mousepush, SIGNAL(timeout()), this, SLOT(updateGL()));
}

void RenderedViewWidget::paintGL()
{
    this->makeCurrent();

    qDebug()<<"[RenderedView] paintGL";

    //clear the screen and depth buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //for each piece of geometry, render it
    for(QMap<int, QGLBuffer>::Iterator itr = geoVerts.begin(); itr != geoVerts.end(); ++itr){
        QGLBuffer vertBuffer = itr.value();
        QGLBuffer indexBuffer = geoIndex[itr.key()];
        QGLBuffer normalBuffer = geoNorms[itr.key()];
        Material mat = M3DEditGUI::g_MatMgr->getMaterial(itr.key());

        this->makeCurrent();
        renderer->render(camera, program, vertBuffer,
                         indexBuffer, normalBuffer, mat.getDiffuseColor());
    }

    this->swapBuffers();
}

void RenderedViewWidget::resizeGL(int width, int height)
{
    this->makeCurrent();

    qDebug()<<"[RenderedView] resizeGL";

    //update viewport and projection then render view
    glViewport(0, 0 , width, height);
    this->camera.updateProjection(width, height);

    this->paintGL();
}

void RenderedViewWidget::initializeGL()
{
    this->makeCurrent();

    qDebug()<<"[RenderedView] initGL";

    //cler the screen and depth buffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //setup the vertex and fragment shaders
    QGLShader vertexShader(QGLShader::Vertex);
    QGLShader fragShader(QGLShader::Fragment);

    //compile vertex shaer and report errors
    if(!vertexShader.compileSourceFile(":/shaders/perview.vert")){
        qDebug()<<"Failed to compile vertex shader:\n"<<vertexShader.log()<<"\n";
        QMessageBox::critical(this, "Error", "Failed to compile axis vertex shader");
        qFatal("Failed to compile axis vertex shader");
    }

    //compile fragment shader and report errors
    if(!fragShader.compileSourceFile(":/shaders/perview.frag")){
        qDebug()<<"Failed to compile frag shader:\n"<<fragShader.log()<<"\n";
        QMessageBox::critical(this, "Error", "Failed to compile axis frag shader");
        qFatal("Failed to compile axis frag shader");
    }

    //add shaders to the program
    program.addShader(&vertexShader);
    program.addShader(&fragShader);

    //link and report errors
    if(!program.link()){
        qDebug()<<"Failed to link axis shader program:\n"<<program.log()<<"\n";
        QMessageBox::critical(this, "Error", "Failed to link axis shader program");
        qFatal("Failed to link axis shader program");
    }
}

void RenderedViewWidget::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<"[RenderView] Mouse Press";

    //for left mouse, start timeer to move camera foward every 10ms
    if(event->button() == Qt::LeftButton)
    {
        leftTrack = true;
        mousepush.setInterval(10);
        mousepush.start();
    }

    //for right mouse start tracking its movements
    if(event->button() == Qt::RightButton)
    {
        rightTrack = true;
        lastMouse = event->globalPos();
    }
}

void RenderedViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug()<<"[RenderView] mouseRelease";

    //if left mouse then stop the move foward timer
    if(event->button() == Qt::LeftButton)
    {
        leftTrack = false;
        mousepush.stop();
    }

    //if right mouse stop tracking its movements
    if(event->button() == Qt::RightButton)
        rightTrack = false;


}

void RenderedViewWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(leftTrack){
        //nothing for now
    }

    //rotate the camera target based on a movement vector
    if(rightTrack){
        qDebug()<<"[RenderView] tracking right";

        QPoint distance = this->lastMouse - event->globalPos();
        camera.moveTarget(distance);

        this->paintGL();

        lastMouse = event->globalPos();
    }
}

void RenderedViewWidget::addGeometry(int id, M3DEditLevel::Geometry *geo)
{
    this->makeCurrent();

    qDebug()<<"[RenderedView] addGeo";

    QGLBuffer verts(QGLBuffer::VertexBuffer);
    QGLBuffer normals(QGLBuffer::VertexBuffer);
    QGLBuffer indicies(QGLBuffer::IndexBuffer);

    //get data for buffers
    QVector<QVector3D> vertData = geo->getVerticies();
    QVector<QVector3D> normData = geo->getNormals();
    QVector<unsigned int> indexData = geo->getTriangleIndex();

    //prepare vertex data for the buffer
    std::vector<float> vertAttrib;
    for(int i = 0; i<vertData.size(); i++){
        QVector3D vert = vertData[i];

        vertAttrib.push_back(vert.x());
        vertAttrib.push_back(vert.y());
        vertAttrib.push_back(vert.z());
        vertAttrib.push_back(1.0f);

        qDebug()<<"[RenderView] Vert:"<<vert.x()<<vert.y()<<vert.z();
    }

    //create vertex buffer and add data to it
    verts.create();
    verts.setUsagePattern(QGLBuffer::DynamicDraw);
    verts.bind();
    verts.allocate(vertAttrib.data(), sizeof(float) *vertAttrib.size());

    this->geoVerts[id] = verts;

    //prepare the nomal data for the buffer
    std::vector<float> normAttrib;
    for(int i=0; i < normData.size(); ++i){
        QVector3D normal = normData[i];

        normAttrib.push_back(normal.x());
        normAttrib.push_back(normal.y());
        normAttrib.push_back(normal.z());

        qDebug()<<"[RenderView] Normal:"<<normal.x()<<normal.y()<<normal.z();
    }

    //create the normal buffer and add data to it
    normals.create();
    normals.bind();
    normals.allocate(normAttrib.data(), sizeof(float) * normAttrib.size());

    this->geoNorms[id] = normals;

    //create the index buffer then add data to it
    indicies.create();
    indicies.bind();
    indicies.allocate(indexData.data(), sizeof(unsigned int) * indexData.size());

    qDebug()<<"[RenderView] total indicies"<<indexData.size();

    this->geoIndex[id] = indicies;

    //redraw view with new geometry
    this->paintGL();
}

void RenderedViewWidget::updateGeometry(int id, M3DEditLevel::Geometry *geo)
{
    this->makeCurrent();

    qDebug()<<"[RenderedView] updateGeo";

    //get already active vertex buffer for this piece of geometry
    QGLBuffer verts = geoVerts[id];

    QVector<QVector3D> vertData = geo->getVerticies();

    //prepare new vertex data for the buffer
    std::vector<float> vertAttrib;
    for(int i = 0; i<vertData.size(); i++){
        QVector3D vert = vertData[i];
        vertAttrib.push_back(vert.x());
        vertAttrib.push_back(vert.y());
        vertAttrib.push_back(vert.z());
        vertAttrib.push_back(1.0f);
        qDebug()<<"[RenderView] Vert:"<<vert.x()<<vert.y()<<vert.z();
    }

    //update the buffer with the new data
    verts.bind();
    verts.write(0, vertAttrib.data(), sizeof(float) * vertAttrib.size());

    this->geoVerts[id] = verts;

    //redraw the view with new geometry
    this->paintGL();
}

void RenderedViewWidget::removeGeometry(int id, M3DEditLevel::Geometry *geo)
{
    Q_UNUSED(geo)

    //destroy buffers and remove them from storage
    geoVerts[id].destroy();
    this->geoVerts.remove(id);

    geoIndex[id].destroy();
    this->geoIndex.remove(id);

    geoNorms[id].destroy();
    this->geoNorms.remove(id);

    //redraw to reflect changes
    this->paintGL();
}

void RenderedViewWidget::updateGL()
{
    this->paintGL();
}

void RenderedViewWidget::newCamPos(QVector3D pos, QVector3D target)
{
    //update the camera values based on the arguments
    this->camera.setPosition(pos);
    this->camera.setTarget(target);

    //update projection and redraw
    this->camera.updateProjection();
    this->paintGL();
}
}
