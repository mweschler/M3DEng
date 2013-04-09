#ifndef AXISVIEWWIDGET_H
#define AXISVIEWWIDGET_H

#include <QGLWidget>
#include <QGLBuffer>
#include <QMap>

#include "axisrenderer.h"
#include "axiscamera.h"
#include "geometry.h"

namespace M3DEditGUI{

struct vertAttribute{
    float x;
    float y;
    float z;
};

class AxisViewWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit AxisViewWidget(M3DEditRender::AxisRenderer *renderer,M3DEditRender::AxisLock lock, QWidget *parent = 0);
protected:
    virtual void paintGL();
    virtual void resizeGL(int width, int height);
    virtual void initializeGL();
public slots:
    void addGeometry(int id, M3DEditLevel::Geometry *geo);
    void removeGeometry(int id, M3DEditLevel::Geometry *geo);
private:
    QMap<int, vertAttribute> vertData;
    QMap<int, QGLBuffer> geoVerts;
    QMap<int, QGLBuffer> geoIndex;
    M3DEditRender::AxisRenderer *renderer;
    M3DEditRender::AxisCamera camera;
};
}

#endif // AXISVIEWWIDGET_H
