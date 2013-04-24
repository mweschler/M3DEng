#include "perspectiverenderer.h"

namespace M3DEditRender{
//! default ambient light color
const static QVector4D ambientLight(1.0f, 1.0f, 1.0f, 1.0f);

//! default ambient light intensity
const static QVector4D ambientIntensity(0.2f, 0.2f, 0.2f, 1.0f);

//! default direction for the ambient light
const static QVector3D lightDir(0.5f, 1.0f, 0.5);

PerspectiveRenderer *g_perspectiveRender = NULL;

PerspectiveRenderer::PerspectiveRenderer()
{
    g_perspectiveRender = this;
}

void PerspectiveRenderer::render(const PerspectiveCamera &camera, QGLShaderProgram &program,
                                 QGLBuffer &vertBuff, QGLBuffer &indexBuff, QGLBuffer &normBuff,
                                 const QVector4D diffuse)
{
    qDebug()<<"[PerspecRender] diffuse:"<<diffuse.x()<<diffuse.y()<<diffuse.z();

    program.bind();

    //setup vertex buffer to render from
    vertBuff.bind();
    program.enableAttributeArray("vertex");
    program.setAttributeBuffer("vertex", GL_FLOAT, 0, 4);

    //setup normal buffer to render from
    normBuff.bind();
    program.enableAttributeArray("vertNormal");
    program.setAttributeBuffer("vertNormal", GL_FLOAT, 0, 3);

    //set uniforms for shader
    program.setUniformValueArray("mvp", &camera.getProjMatrix(), 1);
    program.setUniformValueArray("diffuse", &diffuse, 1);
    program.setUniformValueArray("lightDir", &lightDir, 1);
    program.setUniformValueArray("ambientLight", &ambientLight, 1);
    program.setUniformValueArray("ambientIntensity", &ambientIntensity, 1);

    //draw! NOTE: Hardcoded to 36 indcies for a box triangle mesh only!
    indexBuff.bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

}
