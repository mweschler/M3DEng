#include <QMessageBox>

#include "renderedviewwidget.h"

namespace M3DEditGUI{
RenderedViewWidget::RenderedViewWidget(QWidget *parent) :
    QGLWidget(parent)
{
    this->renderer = M3DEditRender::g_perspectiveRender;
    leftTrack = false;
    rightTrack = false;
}

void RenderedViewWidget::paintGL()
{
    this->makeCurrent();
    qDebug()<<"[RenderedView] paintGL";
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(QMap<int, QGLBuffer>::Iterator itr = geoVerts.begin(); itr != geoVerts.end(); ++itr){
        QGLBuffer vertBuffer = itr.value();
        QGLBuffer indexBuffer = geoIndex[itr.key()];
        QGLBuffer normalBuffer = geoNorms[itr.key()];


        this->makeCurrent();
        renderer->render(camera, program, vertBuffer,
                         indexBuffer, normalBuffer, QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
    }

    this->swapBuffers();
}

void RenderedViewWidget::resizeGL(int width, int height)
{
    this->makeCurrent();
    qDebug()<<"[RenderedView] resizeGL";
    glViewport(0, 0 , width, height);
    this->camera.updateProjection(width, height);
    this->paintGL();
}

void RenderedViewWidget::initializeGL()
{
    this->makeCurrent();
    qDebug()<<"[RenderedView] initGL";
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_CCW);

    QGLShader vertexShader(QGLShader::Vertex);
    QGLShader fragShader(QGLShader::Fragment);

    if(!vertexShader.compileSourceFile(":/shaders/perview.vert")){
        qDebug()<<"Failed to compile vertex shader:\n"<<vertexShader.log()<<"\n";
        QMessageBox::critical(this, "Error", "Failed to compile axis vertex shader");
        qFatal("Failed to compile axis vertex shader");
    }

    if(!fragShader.compileSourceFile(":/shaders/perview.frag")){
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

void RenderedViewWidget::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<"[RenderView] Mouse Press";
    if(!(leftTrack || rightTrack)){
        switch(event->button()){
        case (Qt::LeftButton):
            leftTrack = true;
            break;
        case (Qt::RightButton):
            rightTrack = true;
            break;
        }
        lastMouse = event->globalPos();
    }
}

void RenderedViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug()<<"[RenderView] mouseRelease";
    switch(event->button()){
    case (Qt::LeftButton):
        leftTrack = false;
        break;
    case (Qt::RightButton):
        rightTrack = false;
        break;
    }
}

void RenderedViewWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(leftTrack){

    }
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

    QVector<QVector3D> vertData = geo->getVerticies();
    QVector<QVector3D> normData = geo->getNormals();
    QVector<unsigned int> indexData = geo->getTriangleIndex();

    std::vector<float> vertAttrib;
    for(int i = 0; i<vertData.size(); i++){
        QVector3D vert = vertData[i];
        vertAttrib.push_back(vert.x());
        vertAttrib.push_back(vert.y());
        vertAttrib.push_back(vert.z());
        vertAttrib.push_back(1.0f);
        qDebug()<<"[RenderView] Vert:"<<vert.x()<<vert.y()<<vert.z();
    }

    verts.create();
    verts.bind();
    verts.allocate(vertAttrib.data(), sizeof(float) *vertAttrib.size());

    this->geoVerts[id] = verts;

    std::vector<float> normAttrib;
    for(int i=0; i < normData.size(); ++i){
        QVector3D normal = normData[i];
        normAttrib.push_back(normal.x());
        normAttrib.push_back(normal.y());
        normAttrib.push_back(normal.z());
        qDebug()<<"[RenderView] Normal:"<<normal.x()<<normal.y()<<normal.z();
    }

    normals.create();
    normals.bind();
    normals.allocate(normAttrib.data(), sizeof(float) * normAttrib.size());

    this->geoNorms[id] = normals;

    indicies.create();
    indicies.bind();
    indicies.allocate(indexData.data(), sizeof(unsigned int) * indexData.size());
    qDebug()<<"[RenderView] total indicies"<<indexData.size();
    for(int i = 0; i< indexData.size(); ++i){
        qDebug()<<"[RenderView] indicie"<<i<<indexData[i];
    }
    this->geoIndex[id] = indicies;
    this->paintGL();
}

void RenderedViewWidget::updateGeometry(int id, M3DEditLevel::Geometry *geo)
{
}
}
