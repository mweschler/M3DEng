#include "axisrenderer.h"

namespace M3DEditRender{
AxisRenderer *g_axisRenderer = NULL;

AxisRenderer::AxisRenderer(M3DEditLevel::GeometryManager *geoMgr, QObject *parent)
    :QObject(parent), geoMgr(geoMgr)
{
}

void AxisRenderer::render(AxisCamera &camera, QGLShaderProgram &program, QGLBuffer &vertBuffer, QGLBuffer &indexBuffer)
{
    Q_UNUSED(camera)
    Q_UNUSED(program)
    Q_UNUSED(vertBuffer)
    Q_UNUSED(indexBuffer)
}


void AxisRenderer::addGeometry(int id){
    M3DEditLevel::Geometry *geo = geoMgr->getGeometry(id);
    //this->geoVert.
}

}
