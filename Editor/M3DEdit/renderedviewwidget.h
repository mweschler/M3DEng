#ifndef RENDEREDVIEWWIDGET_H
#define RENDEREDVIEWWIDGET_H

#include <QGLWidget>
#include <QGLBuffer>
#include <QGLShaderProgram>
#include <QMouseEvent>
#include <QPoint>
#include <QTimer>

#include "geometry.h"
#include "perspectiverenderer.h"
#include "persepectivecamera.h"


namespace M3DEditGUI{
class RenderedViewWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit RenderedViewWidget(QWidget *parent = 0);
    virtual void paintGL();
protected:
    virtual void resizeGL(int width, int height);
    virtual void initializeGL();
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
signals:

public slots:
    void addGeometry(int id, M3DEditLevel::Geometry *geo);
    void updateGeometry(int id, M3DEditLevel::Geometry *geo);
    void removeGeometry(int id, M3DEditLevel::Geometry *geo);
    virtual void updateGL();
    void newCamPos(QVector3D pos, QVector3D target);
private:
    QMap<int, QGLBuffer> geoVerts;
    QMap<int, QGLBuffer> geoNorms;
    QMap<int, QGLBuffer> geoIndex;

    M3DEditRender::PerspectiveRenderer *renderer;
    M3DEditRender::PerspectiveCamera camera;
    QGLShaderProgram program;
    bool rightTrack;
    bool leftTrack;
    QPoint lastMouse;
    QTimer mousepush;
};
}

#endif // RENDEREDVIEWWIDGET_H
