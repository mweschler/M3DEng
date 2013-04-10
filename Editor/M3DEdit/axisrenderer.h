#ifndef AXISRENDERER_H
#define AXISRENDERER_H

#include <QObject>
#include <QGLBuffer>
#include <QGLShaderProgram>

#include "axiscamera.h"
#include "geometrymanager.h"

namespace M3DEditRender{
//! This class is used to render a scene for an axis view
class AxisRenderer : public QObject
{
    Q_OBJECT
public:
    explicit AxisRenderer(M3DEditLevel::GeometryManager *geoMgr, QObject *parent = 0);

    /*!
     * \brief Renders the object as lines
     * \param camera camera to render from
     * \param program program to render with
     * \param vertBuffer buffer holding the vertex attributes to render
     * \param indexBuffer index buffer for the vertex buffer
     */
    void render(AxisCamera &camera, QGLShaderProgram &program, QGLBuffer &vertBuffer, QGLBuffer &indexBuffer);
public slots:
    void addGeometry(int id);
private:
    M3DEditLevel::GeometryManager *geoMgr;
};

//! global pointer to the axisrenderer
extern AxisRenderer *g_axisRenderer;
}

#endif // AXISRENDERER_H
