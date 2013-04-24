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

//! a view widget that will display a perspective rendered view of the level
class RenderedViewWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit RenderedViewWidget(QWidget *parent = 0);

    //! redraws the view via opengl
    virtual void paintGL();
protected:
    /*!
     * \brief updates the view when it has been resized
     * \param width new width of the view
     * \param height new height of the view
     */
    virtual void resizeGL(int width, int height);

    //! initialize the view for opengl rendering
    virtual void initializeGL();

    //! handles mouse buttone presses
    virtual void mousePressEvent(QMouseEvent *event);

    //! handles mouse button release events
    virtual void mouseReleaseEvent(QMouseEvent *event);

    //! handles mouse movement events
    virtual void mouseMoveEvent(QMouseEvent *event);
signals:

public slots:
    //! called when geometry has been added
    void addGeometry(int id, M3DEditLevel::Geometry *geo);

    //! called when geometry has been updated
    void updateGeometry(int id, M3DEditLevel::Geometry *geo);

    //! called when geometry has been removed
    void removeGeometry(int id, M3DEditLevel::Geometry *geo);

    //! redraw the view via opengl
    virtual void updateGL();

    //! called when a new cameara positon is needed
    void newCamPos(QVector3D pos, QVector3D target);

private:
    //! holds all the vertex buffers for geometry
    QMap<int, QGLBuffer> geoVerts;

    //! holds all the normal buffers for geometry
    QMap<int, QGLBuffer> geoNorms;

    //! holds all the index buffers for geometry
    QMap<int, QGLBuffer> geoIndex;

    //! pointer the the perspective renderer
    M3DEditRender::PerspectiveRenderer *renderer;

    //! camera to use for this view
    M3DEditRender::PerspectiveCamera camera;

    //! shaer program to use in rendering
    QGLShaderProgram program;

    //! state for tracking mouse movements when right button is held
    bool rightTrack;

    //! state for tracking mouse movements when left button is held
    bool leftTrack;

    //! last mouse postion
    QPoint lastMouse;

    //! timer used to keep moving camera foward as long as left button is held
    QTimer mousepush;
};
}

#endif // RENDEREDVIEWWIDGET_H
