#ifndef PERSPECTIVERENDERER_H
#define PERSPECTIVERENDERER_H

#include <QGLFunctions>
#include <QGLBuffer>
#include <QGLShaderProgram>

#include "persepectivecamera.h"

namespace M3DEditRender{

//! A class used to render a perspective view
class PerspectiveRenderer
{
public:
    explicit PerspectiveRenderer();

    /*!
     * \brief renders a piece of geometry in the perspective view using the current context
     * \param camera camera to use for rendering
     * \param program program to use for rendering
     * \param vertBuff a buffer holding the verticies of the object
     * \param indexBuff the buffer holding the indicies used to render
     * \param normBuff the buffer holding the normals of this object
     * \param diffuse the diffuse color of this object
     */
    void render(const PerspectiveCamera &camera, QGLShaderProgram &program,
                QGLBuffer &vertBuff, QGLBuffer &indexBuff, QGLBuffer &normBuff,const QVector4D diffuse);

};

//! global pointer to the perspective renderer singelton
extern PerspectiveRenderer *g_perspectiveRender;
}

#endif // PERSPECTIVERENDERER_H
