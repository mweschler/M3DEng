#ifndef PERSPECTIVERENDERER_H
#define PERSPECTIVERENDERER_H

#include <QGLFunctions>
#include <QGLBuffer>
#include <QGLShaderProgram>

#include "persepectivecamera.h"

namespace M3DEditRender{
class PerspectiveRenderer
{
public:
    explicit PerspectiveRenderer();
    void render(PerspectiveCamera &camera, QGLShaderProgram &program,
                QGLBuffer &vertBuff, QGLBuffer &indexBuff, QGLBuffer &normBuff, QVector4D diffuse);
    void renderOrigin(PerspectiveCamera &camera, QGLShaderProgram &program);
};

extern PerspectiveRenderer *g_perspectiveRender;
}

#endif // PERSPECTIVERENDERER_H
