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
    qDebug()<<"[AxisRender] start of render";
    program.bind();
    GLint vertLoc = program.attributeLocation("vertex");
    Q_ASSERT(vertLoc != -1);
    //GLint colorLoc = program.uniformLocation("color");
    //Q_ASSERT(colorLoc != -1);
    GLint mvpLoc = program.uniformLocation("modelToCamera");
    Q_ASSERT(mvpLoc != -1);

    QVector4D color(0.0f, .5f, 1.0f, 1.0f);

    //program.setUniformValueArray(colorLoc, &color, 1);
    program.setUniformValueArray(mvpLoc, &camera.getProjMatrix(), 1);

    vertBuffer.bind();
    program.setAttributeBuffer(vertLoc, GL_FLOAT, 0, 4);

    indexBuffer.bind();
    qDebug()<<"[AxisRender] Drawing";
    glDrawElements(GL_LINE_STRIP, 8, GL_UNSIGNED_INT, NULL);
    qDebug()<<"Drew an axis view";
}


void AxisRenderer::addGeometry(int id){
    M3DEditLevel::Geometry *geo = geoMgr->getGeometry(id);
    //this->geoVert.
}

}
