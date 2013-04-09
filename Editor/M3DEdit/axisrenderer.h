#ifndef AXISRENDERER_H
#define AXISRENDERER_H

#include <QObject>
#include <QGLBuffer>

#include "axiscamera.h"
#include "geometrymanager.h"

namespace M3DEditRender{

class AxisRenderer : public QObject
{
    Q_OBJECT
public:
    explicit AxisRenderer(M3DEditLevel::GeometryManager *geoMgr, QObject *parent = 0);
    void render(AxisCamera camera);
public slots:
    void addGeometry(int id);
private:
    M3DEditLevel::GeometryManager *geoMgr;
};
}

#endif // AXISRENDERER_H
