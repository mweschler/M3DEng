#include <QtOpenGL/QGLFunctions>
#include <QMessageBox>
#include "axisrenderer.h"

namespace M3DEditRender{
AxisRenderer *g_axisRenderer = NULL;

AxisRenderer::AxisRenderer(M3DEditLevel::GeometryManager *geoMgr)
    : geoMgr(geoMgr)
{
}

void AxisRenderer::render(AxisCamera &camera, QGLShaderProgram &program, QGLBuffer &vertBuffer, QGLBuffer &indexBuffer)
{
    qDebug()<<"[AxisRender] start of render";
    initializeGLFunctions();

    if(!program.bind()){
        QMessageBox::critical(0, "Error", "Could not bind program for use");
        qFatal("Could not bind program for use");
    }

    /*
    const float vertexPositions[] = {
        0.75f, 0.75f, 0.0f, 1.0f,
        0.75f, -0.75f, 0.0f, 1.0f,
        -0.75f, -0.75f, 0.0f, 1.0f,
    };


    GLint vertLoc = program.attributeLocation("vertex");
    Q_ASSERT(vertLoc != -1);
    qDebug()<<"[AxisRender] setting attributes";
    QGLBuffer buffer(QGLBuffer::VertexBuffer);
    buffer.create();
    buffer.bind();
    buffer.allocate(vertexPositions, sizeof(vertexPositions));

    program.enableAttributeArray(vertLoc);
    program.setAttributeBuffer(vertLoc, GL_FLOAT, 0, 4); */

    /*
    GLuint positionBufferObject;
    glGenBuffers(1, &positionBufferObject);

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glEnableVertexAttribArray(vertLoc);
    glVertexAttribPointer(vertLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    */

    qDebug()<<"[AxisRender] Drawing";
    /*
    glDrawArrays(GL_TRIANGLES, 0, 3);
    program.disableAttributeArray(vertLoc);
    */

    /*
    const float square[] = {
        0.25f, 0.25f, 0.0f, 1.0f,
        0.25f, 0.75f, 0.0f, 1.0f,
        0.75f, 0.75f, 0.0f, 1.0f,
        0.75f, 0.25f, 0.0f, 1.0f
    };


    struct va {
        float x;
        float y;
        float z;
        float w;
    };

    std::vector<va> square;
    va vert1, vert2, vert3, vert4;
    vert1.x = 0.25f;
    vert1.y = 0.25f;
    vert1.z = 0.0f;
    vert1.w = 1.0f;

    vert2.x = 0.25f;
    vert2.y = 0.75f;
    vert2.z = 0.0f;
    vert2.w = 1.0f;

    vert3.x = 0.75f;
    vert3.y = 0.75f;
    vert3.z = 0.0f;
    vert3.w = 1.0f;

    vert4.x = 0.75f;
    vert4.y = 0.25f;
    vert4.z = 0.0f;
    vert4.w = 1.0f;

    square.push_back(vert1);
    square.push_back(vert2);
    square.push_back(vert3);
    square.push_back(vert4);

    const GLuint index[] = {
        0,1,2,3,0
    };

    QGLBuffer vb(QGLBuffer::VertexBuffer);
    QGLBuffer ib(QGLBuffer::IndexBuffer);

    vb.create();
    vb.bind();
    vb.allocate(square.data(), sizeof(va) * 4);

    ib.create();
    ib.bind();
    ib.allocate(index, sizeof(index));
    */

    program.bind();
    GLint vertLoc = program.attributeLocation("vertex");
    Q_ASSERT(vertLoc != -1);
    GLint colorLoc = program.uniformLocation("color");
    Q_ASSERT(colorLoc != -1);
    GLint mvpLoc = program.uniformLocation("modelToCamera");
    Q_ASSERT(mvpLoc != -1);

    QVector4D color(0.0f, .5f, 1.0f, 1.0f);

    program.setUniformValueArray(colorLoc, &color, 1);
    program.setUniformValueArray(mvpLoc, &camera.getProjMatrix(), 1);

    vertBuffer.bind();
    //vb.bind();
    program.enableAttributeArray(vertLoc);
    program.setAttributeBuffer(vertLoc, GL_FLOAT, 0, 4);

    indexBuffer.bind();
    //ib.bind();
    qDebug()<<"[AxisRender] Drawing";
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, NULL);

    qDebug()<<"Drew an axis view";
}

void AxisRenderer::renderOrigin(QGLShaderProgram &program, AxisCamera &camera)
{
    qDebug()<<"[AxisRender] drawing origin lines";
    initializeGLFunctions();

    const GLfloat xline[] = {
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

    QGLBuffer xbuff(QGLBuffer::VertexBuffer);
    xbuff.create();
    xbuff.bind();
    xbuff.setUsagePattern(QGLBuffer::StaticDraw);
    xbuff.allocate(xline, sizeof(xline));

    QGLBuffer ybuff(QGLBuffer::VertexBuffer);
    ybuff.create();
    ybuff.bind();
    ybuff.setUsagePattern(QGLBuffer::StaticDraw);
    ybuff.allocate(yline, sizeof(yline));

    QGLBuffer zbuff(QGLBuffer::VertexBuffer);
    zbuff.create();
    zbuff.bind();
    zbuff.setUsagePattern(QGLBuffer::StaticDraw);
    zbuff.allocate(zline, sizeof(zline));

    program.bind();
    GLuint colorLoc = program.uniformLocation("color");
    Q_ASSERT(colorLoc != -1);
    GLuint mvpLoc = program.uniformLocation("modelToCamera");
    Q_ASSERT(mvpLoc != -1);
    program.setUniformValueArray(mvpLoc, &(camera.getProjMatrix()), 1);

    xbuff.bind();
    program.enableAttributeArray("vertex");
    program.setAttributeBuffer("vertex", GL_FLOAT, 0, 4);
    program.setUniformValueArray(colorLoc, xcolor, 1, 4);

    glDrawArrays(GL_LINES, 0, 2);

    ybuff.bind();
    program.setAttributeBuffer("vertex", GL_FLOAT, 0, 4);
    program.setUniformValueArray(colorLoc, ycolor, 1, 4);

    glDrawArrays(GL_LINES, 0, 2);

    zbuff.bind();
    program.setAttributeBuffer("vertex", GL_FLOAT, 0, 4);
    program.setUniformValueArray(colorLoc, zcolor, 1, 4);

    glDrawArrays(GL_LINES, 0, 2);

    xbuff.bind();
    xbuff.destroy();

    ybuff.bind();
    ybuff.destroy();

    zbuff.bind();
    zbuff.destroy();

}

void AxisRenderer::drawGrid(int gridSize, QGLShaderProgram &program, AxisCamera &camera)
{
    int gridNumber = camera.getGridFactor() * 2;
    qDebug()<<"[AxisRender] Drawing Grid";
    const int worldSize = 100;
    float lineLimit = gridNumber * gridSize;
    qDebug()<<"[AxisRender] lineLimit = "<<lineLimit;
    float lineX[] = {lineLimit, 0.0f, 0.0f, 1.0f,
                     -lineLimit, 0.0f, 0.0f, 1.0f};

    float lineY[] = { 0.0f, lineLimit, 0.0f, 1.0f,
                    0.0f, -lineLimit, 0.0f, 1.0f};

    float lineZ[] = {0.0f, 0.0f, lineLimit, 1.0f,
                    0.0f, 0.0f, -lineLimit, 1.0f};

    program.bind();
    QGLBuffer bufferX(QGLBuffer::VertexBuffer);
    bufferX.setUsagePattern(QGLBuffer::StaticDraw);
    bufferX.create();
    bufferX.bind();
    bufferX.allocate(lineX,sizeof(lineX));

    QGLBuffer bufferY(QGLBuffer::VertexBuffer);
    bufferY.setUsagePattern(QGLBuffer::StaticDraw);
    bufferY.create();
    bufferY.bind();
    bufferY.allocate(lineY, sizeof(lineY));

    QGLBuffer bufferZ(QGLBuffer::VertexBuffer);
    bufferZ.setUsagePattern(QGLBuffer::StaticDraw);
    bufferZ.create();
    bufferZ.bind();
    bufferZ.allocate(lineZ, sizeof(lineZ));

    QMatrix4x4 mvp;
    QMatrix4x4 vp = camera.getProjMatrix();
    QMatrix4x4 m;

    QVector3D pos = camera.getPosistion();
    int xoff = pos.x() / gridSize;
    int yoff = pos.y() / gridSize;
    int zoff = pos.z() / gridSize;

    QVector4D color(0.3f, 0.3f, 0.3f, 1.0f);
    program.setUniformValueArray("color", &color, 1);
    program.enableAttributeArray("vertex");
    for(int i = -gridNumber; i <= gridNumber; ++i){
        //x lines
        if(camera.getLock() == XY || camera.getLock() == XZ)
        {
            bufferX.bind();
            program.setAttributeBuffer("vertex", GL_FLOAT, 0, 4);

            m.setToIdentity();
            m.translate(0.0f + (xoff * gridSize), (i + yoff) * gridSize, (i + zoff) * gridSize);

            mvp = vp * m ;

            program.setUniformValueArray("modelToCamera", &mvp, 1);
            glDrawArrays(GL_LINES, 0, 2);
        }

        //y lines
        if(camera.getLock() == XY || camera.getLock() == YZ)
        {
            bufferY.bind();
            program.setAttributeBuffer("vertex", GL_FLOAT, 0, 4);
            m.setToIdentity();
            m.translate((i + xoff) * gridSize, 0.0f + (yoff * gridSize) ,(i + zoff) * gridSize);
            mvp = vp * m;

            program.setUniformValueArray("modelToCamera", &mvp, 1);
            glDrawArrays(GL_LINES, 0, 2);
        }

        //z lines
        if(camera.getLock() == XZ || camera.getLock() == YZ)
        {
            bufferZ.bind();
            program.setAttributeBuffer("vertex", GL_FLOAT, 0, 4);
            m.setToIdentity();
            m.translate((i + xoff) * gridSize, (i + yoff) * gridSize ,0.0f + (zoff * gridSize));
            mvp = vp * m;

            program.setUniformValueArray("modelToCamera", &mvp, 1);
            glDrawArrays(GL_LINES, 0, 2);
        }
    }
}




}
