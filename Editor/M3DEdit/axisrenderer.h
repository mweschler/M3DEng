#ifndef AXISRENDERER_H
#define AXISRENDERER_H

#include <QObject>
#include <QGLBuffer>
#include <QGLFunctions>
#include <QGLShaderProgram>

#include "axiscamera.h"
#include "geometrymanager.h"
#include "box.h"

namespace M3DEditRender  {
//! This singleton class is used to render a scene for an axis view
class AxisRenderer : protected QGLFunctions
{
public:
    explicit AxisRenderer();

    /*!
     * \brief Renders the geometry object as lines
     * \param camera camera to render from
     * \param program program to render with
     * \param vertBuffer buffer holding the vertex attributes to render
     * \param indexBuffer index buffer for the vertex buffer
     * \param selected true if this geometry is selected
     */
    void render(const AxisCamera &camera, QGLShaderProgram &program, QGLBuffer &vertBuffer,
                QGLBuffer &indexBuffer, bool selected = false);

    /*!
     * \brief renders origin lines for a view
     * \param program program to render with
     * \param camera camera to render with
     */
    void renderOrigin(QGLShaderProgram &program, const AxisCamera &camera);

    /*!
     * \brief draws the grid lines for a view
     * \param gridSize size of the grid
     * \param program program to render with
     * \param camera camera to render with
     */
    void drawGrid(int gridSize, QGLShaderProgram &program, const AxisCamera &camera);

    /*!
     * \brief draws a line for the camera tool
     * \param from point to draw from
     * \param to point to draw to
     * \param program program to render with
     * \param camera camera to render with
     */
    void drawCamLine(QVector3D from, QVector3D to, QGLShaderProgram &program, const AxisCamera &camera);

    /*!
     * \brief drawBrush renders the brush outline
     * \param brush brush to render
     * \param program program to render with
     * \param camera camera to render with
     */
    void drawBrush(const M3DEditLevel::Box &brush, QGLShaderProgram &program, const AxisCamera &camera);
};

//! global pointer to the axisrenderer
extern AxisRenderer *g_axisRenderer;
}

#endif // AXISRENDERER_H
