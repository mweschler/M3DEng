#include <QtOpenGL/QGLFunctions>
#include <QMessageBox>
#include "axisrenderer.h"

namespace M3DEditRender{
AxisRenderer *g_axisRenderer = NULL;

AxisRenderer::AxisRenderer()
{
}

void AxisRenderer::render(const AxisCamera &camera, QGLShaderProgram &program, QGLBuffer &vertBuffer, QGLBuffer &indexBuffer, bool selected)
{
    qDebug()<<"[AxisRender] start of render";

    initializeGLFunctions();

    //bind the program for use
    if(!program.bind()){
        QMessageBox::critical(0, "Error", "Could not bind program for use");
        qFatal("Could not bind program for use");
    }

    qDebug()<<"[AxisRender] Drawing";

    //set up shader locations
    program.bind();
    GLint vertLoc = program.attributeLocation("vertex");
    Q_ASSERT(vertLoc != -1);
    GLint colorLoc = program.uniformLocation("color");
    Q_ASSERT(colorLoc != -1);
    GLint mvpLoc = program.uniformLocation("modelToCamera");
    Q_ASSERT(mvpLoc != -1);

    //set default color to a cyan
    QVector4D color(0.0f, .5f, 1.0f, 1.0f);

    //modify the color for selected geometry
    if(selected){
        qDebug()<<"[AxisRender] adding selection color";
        color = color + QVector4D(1.0f, 0.0f, 0.0f, 0.0f);
    }

    //set the uniform values
    program.setUniformValueArray(colorLoc, &color, 1);
    program.setUniformValueArray(mvpLoc, &camera.getProjMatrix(), 1);

    //bind and set the vertex buffer for attribute
    vertBuffer.bind();
    program.enableAttributeArray(vertLoc);
    program.setAttributeBuffer(vertLoc, GL_FLOAT, 0, 4);

    //bind the index buffer for the draw call
    indexBuffer.bind();

    qDebug()<<"[AxisRender] Drawing";

    //draw the index buffer NOTE: Hard set to 24 values for a box!
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, NULL);
}

void AxisRenderer::renderOrigin(QGLShaderProgram &program, const AxisCamera &camera)
{
    qDebug()<<"[AxisRender] drawing origin lines";
    initializeGLFunctions();

    //data for the xline
    const GLfloat xline[] = {
        0.0f, 0.0f, 0.0f, 1.0f,
        5.0f, 0.0f, 0.0f, 1.0f
    };

    //data for the yline
    const GLfloat yline[] = {
        0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 5.0f, 0.0f, 1.0f
    };

    //data for the zline
    const GLfloat zline[] = {
        0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 5.0f, 1.0f
    };

    //data for each line color
    const GLfloat xcolor[] = { 1.0f, 0.0f, 0.0f, 1.0f};
    const GLfloat ycolor[] = { 0.0f, 1.0f, 0.0f, 1.0f};
    const GLfloat zcolor[] = { 1.0f, 0.0f, 1.0f, 1.0f};

    //fill buffer for xline
    QGLBuffer xbuff(QGLBuffer::VertexBuffer);
    xbuff.create();
    xbuff.bind();
    xbuff.setUsagePattern(QGLBuffer::StaticDraw);
    xbuff.allocate(xline, sizeof(xline));

    //fill buffer for yline
    QGLBuffer ybuff(QGLBuffer::VertexBuffer);
    ybuff.create();
    ybuff.bind();
    ybuff.setUsagePattern(QGLBuffer::StaticDraw);
    ybuff.allocate(yline, sizeof(yline));

    //fill buffer for zline
    QGLBuffer zbuff(QGLBuffer::VertexBuffer);
    zbuff.create();
    zbuff.bind();
    zbuff.setUsagePattern(QGLBuffer::StaticDraw);
    zbuff.allocate(zline, sizeof(zline));

    program.bind();

    //set the uniforms
    GLuint colorLoc = program.uniformLocation("color");
    Q_ASSERT(colorLoc != -1);
    GLuint mvpLoc = program.uniformLocation("modelToCamera");
    Q_ASSERT(mvpLoc != -1);
    program.setUniformValueArray(mvpLoc, &(camera.getProjMatrix()), 1);

    //bind then draw xline
    xbuff.bind();
    program.enableAttributeArray("vertex");
    program.setAttributeBuffer("vertex", GL_FLOAT, 0, 4);
    program.setUniformValueArray(colorLoc, xcolor, 1, 4);

    glDrawArrays(GL_LINES, 0, 2);

    //bind then draw yline
    ybuff.bind();
    program.setAttributeBuffer("vertex", GL_FLOAT, 0, 4);
    program.setUniformValueArray(colorLoc, ycolor, 1, 4);

    glDrawArrays(GL_LINES, 0, 2);

    //bind then draw zline
    zbuff.bind();
    program.setAttributeBuffer("vertex", GL_FLOAT, 0, 4);
    program.setUniformValueArray(colorLoc, zcolor, 1, 4);

    glDrawArrays(GL_LINES, 0, 2);

    //destroy the buffers(Probably should be created and stored for longer than one call but context issue)
    xbuff.bind();
    xbuff.destroy();

    ybuff.bind();
    ybuff.destroy();

    zbuff.bind();
    zbuff.destroy();

}

void AxisRenderer::drawGrid(int gridSize, QGLShaderProgram &program,const AxisCamera &camera)
{
    qDebug()<< "[AxisRender] Drawing Grid";

    int gridNumber = camera.getGridFactor() * 2; //double size of grid to ensure it covers entire view
    float lineLimit = gridNumber * gridSize; //bounds of grid for this size and number

    qDebug()<< "[AxisRender] lineLimit = " << lineLimit;

    //general xline for grid
    float lineX[] = {lineLimit, 0.0f, 0.0f, 1.0f,
                     -lineLimit, 0.0f, 0.0f, 1.0f};

    //general yline for grid
    float lineY[] = { 0.0f, lineLimit, 0.0f, 1.0f,
                    0.0f, -lineLimit, 0.0f, 1.0f};

    //general zline for grid
    float lineZ[] = {0.0f, 0.0f, lineLimit, 1.0f,
                    0.0f, 0.0f, -lineLimit, 1.0f};

    //bind and fill xline buffer
    program.bind();
    QGLBuffer bufferX(QGLBuffer::VertexBuffer);
    bufferX.setUsagePattern(QGLBuffer::StaticDraw);
    bufferX.create();
    bufferX.bind();
    bufferX.allocate(lineX,sizeof(lineX));

    //bind and fill yline buffer
    QGLBuffer bufferY(QGLBuffer::VertexBuffer);
    bufferY.setUsagePattern(QGLBuffer::StaticDraw);
    bufferY.create();
    bufferY.bind();
    bufferY.allocate(lineY, sizeof(lineY));

    //bind and fill zline buffer
    QGLBuffer bufferZ(QGLBuffer::VertexBuffer);
    bufferZ.setUsagePattern(QGLBuffer::StaticDraw);
    bufferZ.create();
    bufferZ.bind();
    bufferZ.allocate(lineZ, sizeof(lineZ));

    QMatrix4x4 mvp;
    QMatrix4x4 vp = camera.getProjMatrix();
    QMatrix4x4 m;

    //find the offest based on camera position
    QVector3D pos = camera.getPosistion();
    int xoff = pos.x() / gridSize;
    int yoff = pos.y() / gridSize;
    int zoff = pos.z() / gridSize;

    //set color uniform to grey
    QVector4D color(0.3f, 0.3f, 0.3f, 1.0f);
    program.setUniformValueArray("color", &color, 1);

    //draw each line
    program.enableAttributeArray("vertex");
    for(int i = -gridNumber; i <= gridNumber; ++i){
        //x lines only for XY and XZ Axis
        if(camera.getLock() == XY || camera.getLock() == XZ)
        {
            bufferX.bind();
            program.setAttributeBuffer("vertex", GL_FLOAT, 0, 4);

            //use model matrix to translate each line with offset
            m.setToIdentity();
            m.translate(0.0f + (xoff * gridSize), (i + yoff) * gridSize, (i + zoff) * gridSize);

            mvp = vp * m ;//set mvp up

            //set mvp uniform then draw
            program.setUniformValueArray("modelToCamera", &mvp, 1);
            glDrawArrays(GL_LINES, 0, 2);
        }

        //y lines for XY and YZ axis only
        if(camera.getLock() == XY || camera.getLock() == YZ)
        {
            bufferY.bind();
            program.setAttributeBuffer("vertex", GL_FLOAT, 0, 4);

            //use model matrix to translate each line with offset
            m.setToIdentity();
            m.translate((i + xoff) * gridSize, 0.0f + (yoff * gridSize) ,(i + zoff) * gridSize);
            mvp = vp * m;

            //set mvp uniform and draw
            program.setUniformValueArray("modelToCamera", &mvp, 1);
            glDrawArrays(GL_LINES, 0, 2);
        }

        //z lines
        if(camera.getLock() == XZ || camera.getLock() == YZ)
        {
            bufferZ.bind();
            program.setAttributeBuffer("vertex", GL_FLOAT, 0, 4);

            //use model matrix to translate each line with offset
            m.setToIdentity();
            m.translate((i + xoff) * gridSize, (i + yoff) * gridSize ,0.0f + (zoff * gridSize));
            mvp = vp * m;

            //set mvp uniform then draw
            program.setUniformValueArray("modelToCamera", &mvp, 1);
            glDrawArrays(GL_LINES, 0, 2);
        }
    }
}

void AxisRenderer::drawCamLine(QVector3D from, QVector3D to, QGLShaderProgram &program, const AxisCamera &camera)
{
    const GLfloat color[] ={1.0f, 0.0f, 0.0f, 1.0f}; //default red color

    //temp vector to hold line verticies
    std::vector<GLfloat> bufferData;

    //from verticies
    bufferData.push_back(from.x());
    bufferData.push_back(from.y());
    bufferData.push_back(from.z());
    bufferData.push_back(1.0f);

    //to verticies
    bufferData.push_back(to.x());
    bufferData.push_back(to.y());
    bufferData.push_back(to.z());
    bufferData.push_back(1.0f);

    program.bind();

    //create and fill line buffer with verticies
    QGLBuffer line(QGLBuffer::VertexBuffer);
    line.create();
    line.bind();
    line.allocate(bufferData.data(), sizeof(GLfloat) * 8);

    //get locations
    GLuint colorLoc = program.uniformLocation("color");
    Q_ASSERT(colorLoc != -1);
    GLuint mvpLoc = program.uniformLocation("modelToCamera");
    Q_ASSERT(mvpLoc != -1);

    //setup line for drawing
    line.bind();
    program.enableAttributeArray("vertex");
    program.setAttributeBuffer("vertex", GL_FLOAT, 0, 4);
    program.setUniformValueArray(colorLoc, color, 1, 4);
    program.setUniformValueArray(mvpLoc, &(camera.getProjMatrix()), 1);

    glDrawArrays(GL_LINES, 0, 2);
}

void AxisRenderer::drawBrush(const M3DEditLevel::Box &brush, QGLShaderProgram &program, const AxisCamera &camera)
{
    QVector<QVector3D> temp = brush.getVerticies();
    QVector<GLfloat> verts;

    //fill vector with brush verticies
    for(int i = 0; i < temp.size(); ++i){
        QVector3D vert = temp[i];
        verts.push_back(vert.x());
        verts.push_back(vert.y());
        verts.push_back(vert.z());
        verts.push_back(1.0f);
    }

    QVector<unsigned int> indicies = brush.getLineIndex();

    //bind brush buffer and fill with verticies
    QGLBuffer brushBuffer(QGLBuffer::VertexBuffer);
    brushBuffer.create();
    brushBuffer.bind();
    brushBuffer.allocate(verts.data(), sizeof(GLfloat) * verts.size());

    //bind index buffer and fill with indicies
    QGLBuffer brushIndex(QGLBuffer::IndexBuffer);
    brushIndex.create();
    brushIndex.bind();
    brushIndex.allocate(indicies.data(), sizeof(unsigned int) * indicies.size());

    //set up uniforms
    program.bind();
    program.setUniformValueArray("color", &QVector4D(1.0f, 1.0f, 1.0f, 1.0f), 1);
    program.setUniformValueArray("modelToCamera", &camera.getProjMatrix(), 1);

    //set up buffers
    brushBuffer.bind();
    program.enableAttributeArray("vertex");
    program.setAttributeBuffer("vertex", GL_FLOAT, 0, 4);

    //draw brush. NOTE: Fixed 24 indicies for a box only!
    brushIndex.bind();
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);


}

}
