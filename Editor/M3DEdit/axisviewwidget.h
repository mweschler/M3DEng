#ifndef AXISVIEWWIDGET_H
#define AXISVIEWWIDGET_H

#include <QGLWidget>
#include <QGLBuffer>
#include <QGLShaderProgram>
#include <QMap>
#include <QMouseEvent>

#include "axisrenderer.h"
#include "axiscamera.h"
#include "geometry.h"

namespace M3DEditGUI{

//! Holds the vertex attributes for a vertex
struct vertAttribute{
    float x;
    float y;
    float z;
    float w;
};

/*!
 * /brief A display widget that shows a level's geometry.
 */
class AxisViewWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit AxisViewWidget(QWidget *parent = 0);
    /*!
     * /brief sets the axises to lock to
     * /param lock axis to lock to
     */
    void setAxisLock(M3DEditRender::AxisLock lock);
    virtual int heightForWidth(int) const;

protected:
    /*!
     * /brief draws the view via openGL
     */
    virtual void paintGL();

    /*!
     * /brief called when the widget is resized
     * /param width width of new size
     * /param height height of new size
     */
    virtual void resizeGL(int width, int height);

    /*!
     * /brief called when the widget is first displayed to initilize openGL
     */
    virtual void initializeGL();

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

public slots:
    /*!
     * \brief slot for adding new geometry to the view
     * \param id id number for new geometry
     * \param geo pointer to the new geometry data
     */
    void addGeometry(int id, M3DEditLevel::Geometry *geo);

    /*!
     * \brief slot for removing geometry from the view
     * \param id id of the removed geometry
     * \param geo data for the removed geometry
     */
    void removeGeometry(int id, M3DEditLevel::Geometry *geo);

    /*!
     * \brief updates the geometry in the view
     * \param id id of the updated geometry
     * \param geo data for the updated geometry
     */
    void updateGeometry(int id, M3DEditLevel::Geometry *geo);

private:
    //! holds vertex data
    QMap<int, vertAttribute> vertData;
    //! holds VBOs for all geometry
    QMap<int, QGLBuffer> geoVerts;
    //! holds IBOs for all geometry
    QMap<int, QGLBuffer> geoIndex;
    //! convience pointer to the axis renderer
    M3DEditRender::AxisRenderer *renderer;
    //! the camera used for this view
    M3DEditRender::AxisCamera camera;
    //! the program to render the view
    QGLShaderProgram program;
    QPoint lastMousePosition;
    bool mouseTrackRight;
    bool mouseTrackLeft;
};
}

#endif // AXISVIEWWIDGET_H
