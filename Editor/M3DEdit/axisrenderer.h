#ifndef AXISRENDERER_H
#define AXISRENDERER_H

#include <QObject>
#include <QGLBuffer>
#include <QGLShaderProgram>

#include "axiscamera.h"
#include "geometrymanager.h"

namespace M3DEditRender{

class AxisRenderer : public QObject
{
    Q_OBJECT
public:
    explicit AxisRenderer(M3DEditLevel::GeometryManager *geoMgr, QObject *parent = 0);
    void render(AxisCamera &camera, QGLShaderProgram &program, QGLBuffer &vertBuffer, QGLBuffer &indexBuffer);
public slots:
    void addGeometry(int id);
private:
    M3DEditLevel::GeometryManager *geoMgr;
};

extern AxisRenderer *g_axisRenderer;
}

#endif // AXISRENDERER_H
