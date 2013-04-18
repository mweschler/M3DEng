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

enum ToolState{
    SELECT,
    CAMERA,
    GEOMETRY
};

/*!
 * /brief A display widget that shows a level's geometry.
 */
class AxisViewWidget : public QGLWidget, protected QGLFunctions
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
    /*!
     * /brief draws the view via openGL
     */
    virtual void paintGL();

protected:


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

    int selectFromPoint(QPoint point);
    QVector3D getPosition(QPoint point);

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
    void cameraToggle(bool checked);
    void geoToolToggle(bool checked);

signals:

    void selectedGeo(int id);
    void unselect(int id);
    void newCamPos(QVector3D, QVector3D);

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
    QPoint lastLocalMouse;
    bool mouseTrackRight;
    bool mouseTrackLeft;
    bool mouseMoved;
    bool initialized;
    bool camLine;
    ToolState toolState;
    QVector3D camFrom;
    QVector3D camTo;
};
}

#endif // AXISVIEWWIDGET_H
