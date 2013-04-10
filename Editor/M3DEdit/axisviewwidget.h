#ifndef AXISVIEWWIDGET_H
#define AXISVIEWWIDGET_H

#include <QGLWidget>
#include <QGLBuffer>
#include <QGLShaderProgram>
#include <QMap>

#include "axisrenderer.h"
#include "axiscamera.h"
#include "geometry.h"

namespace M3DEditGUI{

struct vertAttribute{
    float x;
    float y;
    float z;
    float w;
};

class AxisViewWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit AxisViewWidget(QWidget *parent = 0);
    void setAxisLock(M3DEditRender::AxisLock lock);
protected:
    virtual void paintGL();
    virtual void resizeGL(int width, int height);
    virtual void initializeGL();
public slots:
    void addGeometry(int id, M3DEditLevel::Geometry *geo);
    void removeGeometry(int id, M3DEditLevel::Geometry *geo);
    void updateGeometry(int id, M3DEditLevel::Geometry *geo);
private:
    QMap<int, vertAttribute> vertData;
    QMap<int, QGLBuffer> geoVerts;
    QMap<int, QGLBuffer> geoIndex;
    M3DEditRender::AxisRenderer *renderer;
    M3DEditRender::AxisCamera camera;
    QGLShaderProgram program;
};
}

#endif // AXISVIEWWIDGET_H
