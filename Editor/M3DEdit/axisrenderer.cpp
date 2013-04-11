#include <QGLFunctions>
#include "axisrenderer.h"

namespace M3DEditRender{
AxisRenderer *g_axisRenderer = NULL;

AxisRenderer::AxisRenderer(M3DEditLevel::GeometryManager *geoMgr, QObject *parent)
    :QObject(parent), geoMgr(geoMgr)
{
}

void AxisRenderer::render(AxisCamera &camera, QGLShaderProgram &program, QGLBuffer &vertBuffer, QGLBuffer &indexBuffer)
{
    //Q_UNUSED(camera)
    //Q_UNUSED(program)
    //Q_UNUSED(vertBuffer)
    //Q_UNUSED(indexBuffer)

    program.bind();
    GLint vertLoc = program.attributeLocation("vertex");
    GLint colorLoc = program.uniformLocation("color");
    QVector4D color(0.0f, .5f, 1.0f, 1.0f);

    program.setUniformValueArray(colorLoc, &color, 1);
    vertBuffer.bind();
    program.setAttributeBuffer(vertLoc, GL_VERTEX_ARRAY, 0, sizeof(float) *4);

    indexBuffer.bind();
    glDrawElements(GL_LINE_STRIP, 8, GL_INT, 0);
}


void AxisRenderer::addGeometry(int id){
    M3DEditLevel::Geometry *geo = geoMgr->getGeometry(id);
    //this->geoVert.
}

}
