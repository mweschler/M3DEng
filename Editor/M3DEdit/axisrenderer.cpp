#include "axisrenderer.h"

namespace M3DEditRender{
AxisRenderer::AxisRenderer(M3DEditLevel::GeometryManager *geoMgr, QObject *parent)
    :QObject(parent), geoMgr(geoMgr)
{
}

void AxisRenderer::render(AxisCamera camera){
    Q_UNUSED(camera)

}

void AxisRenderer::addGeometry(int id){
    M3DEditLevel::Geometry *geo = geoMgr->getGeometry(id);
    //this->geoVert.
}

}
