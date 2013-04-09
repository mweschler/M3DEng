#include <vector>

#include "axisviewwidget.h"

namespace M3DEditGUI{
AxisViewWidget::AxisViewWidget(M3DEditRender::AxisRenderer *renderer,
                               M3DEditRender::AxisLock lock, QWidget *parent):
    QGLWidget(parent), camera(lock), renderer(renderer)
{
}

void AxisViewWidget::paintGL(){

    this->swapBuffers();
}

void AxisViewWidget::resizeGL(int width, int height){
    glViewport(0, 0, width, height);
}

void AxisViewWidget::initializeGL(){
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void AxisViewWidget::addGeometry(int id, M3DEditLevel::Geometry *geo)
{
    QVector<QVector3D> verts = geo->getVerticies();
    std::vector<vertAttribute> vertAttrib;
    for(int i = 0; i < verts.size(); ++i){
        vertAttribute attrib;
        attrib.x = verts[i].x();
        attrib.y = verts[i].y();
        attrib.z = verts[i].z();

        vertAttrib.push_back(attrib);
    }

    //create a line drawing index
    std::vector<int> indexList;
    for(int i = 0; i < verts.size(); ++ i){
        indexList.push_back(i);
    }
    indexList.push_back(0);

    this->makeCurrent();

    QGLBuffer buffer;
    if(!buffer.create()){
        throw std::runtime_error("Could not create buffer");
    }

    if(!buffer.bind()){
        throw std::runtime_error("Could not bind buffer");
    }

    buffer.setUsagePattern(QGLBuffer::DynamicDraw);
    buffer.allocate(&vertAttrib, sizeof(vertAttribute) * verts.size());

    geoVerts[id] = buffer;

    QGLBuffer indexBuffer(QGLBuffer::IndexBuffer);
    if(!indexBuffer.create())
        throw std::runtime_error("Could not create index buffer");

    if(!buffer.bind())
        throw std::runtime_error("Could not bind index buffer");

    buffer.setUsagePattern(QGLBuffer::StaticDraw);
    buffer.allocate(&indexList, sizeof(int) * (verts.size() + 1) );

}

void AxisViewWidget::removeGeometry(int id, M3DEditLevel::Geometry *geo)
{
    if(!geoVerts.contains(id))
        throw std::runtime_error("Buffer does not exist for geometry to remove");

    QGLBuffer buffer = geoVerts[id];
    geoVerts.remove(id);
    this->makeCurrent();
    buffer.bind();
    buffer.destroy();
}

}
