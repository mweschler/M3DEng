#include "perspectiverenderer.h"

namespace M3DEditRender{
const static QVector4D ambientLight(1.0f, 1.0f, 1.0f, 1.0f);
const static QVector4D ambientIntensity(0.2f, 0.2f, 0.2f, 1.0f);
const static QVector3D lightDir(0.5f, 1.0f, 0.5);

PerspectiveRenderer *g_perspectiveRender = NULL;

PerspectiveRenderer::PerspectiveRenderer()
{
    g_perspectiveRender = this;
}

void PerspectiveRenderer::render(PerspectiveCamera &camera, QGLShaderProgram &program, QGLBuffer &vertBuff, QGLBuffer &indexBuff, QGLBuffer &normBuff, QVector4D diffuse)
{
    qDebug()<<"[PerspecRender] diffuse:"<<diffuse.x()<<diffuse.y()<<diffuse.z();
    program.bind();

    vertBuff.bind();
    program.enableAttributeArray("vertex");
    program.setAttributeBuffer("vertex", GL_FLOAT, 0, 4);

    normBuff.bind();
    program.enableAttributeArray("vertNormal");
    program.setAttributeBuffer("vertNormal", GL_FLOAT, 0, 3);

    program.setUniformValueArray("mvp", &camera.getProjMatrix(), 1);
    program.setUniformValueArray("diffuse", &diffuse, 1);
    program.setUniformValueArray("lightDir", &lightDir, 1);
    program.setUniformValueArray("ambientLight", &ambientLight, 1);
    program.setUniformValueArray("ambientIntensity", &ambientIntensity, 1);

    indexBuff.bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void PerspectiveRenderer::renderOrigin(PerspectiveCamera &camera, QGLShaderProgram &program)
{
    QGLBuffer xbuffer;const GLfloat xline[] = {
        0.0f, 0.0f, 0.0f, 1.0f,
        5.0f, 0.0f, 0.0f, 1.0f
    };

    const GLfloat yline[] = {
        0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 5.0f, 0.0f, 1.0f
    };

    const GLfloat zline[] = {
        0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 5.0f, 1.0f
    };

    const GLfloat xcolor[] = { 1.0f, 0.0f, 0.0f, 1.0f};
    const GLfloat ycolor[] = { 0.0f, 1.0f, 0.0f, 1.0f};
    const GLfloat zcolor[] = { 1.0f, 0.0f, 1.0f, 1.0f};
    program.bind();
}
}
