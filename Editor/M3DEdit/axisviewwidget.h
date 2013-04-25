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
#include "box.h"

namespace M3DEditGUI{

//! Holds the vertex attributes for a vertex
struct vertAttribute{
    float x;
    float y;
    float z;
    float w;
};

//! the various tool states
enum ToolState{
    SELECT,
    CAMERA,
    GEOMETRY
};

/*!
 * \brief An opengl view widget showing a single locked axis
 */
class AxisViewWidget : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT
public:
    explicit AxisViewWidget(QWidget *parent = 0);

    /*!
     * \brief sets the axises to lock to
     * \param lock axis to lock to
     */
    void setAxisLock(M3DEditRender::AxisLock lock);

    //! used for drawing resizing the height based on the width
    virtual int heightForWidth(int) const;

    /*!
     * \brief draws the view via openGL
     */
    virtual void paintGL();

protected:


    /*!
     * \brief called when the widget is resized
     * \param width width of new size
     * \param height height of new size
     */
    virtual void resizeGL(int width, int height);

    /*!
     * \brief called when the widget is first displayed to initilize openGL
     */
    virtual void initializeGL();

    //! handles mouse press events
    virtual void mousePressEvent(QMouseEvent *event);

    //! handles mouse release events
    virtual void mouseReleaseEvent(QMouseEvent *event);

    //! handles mouse move events
    virtual void mouseMoveEvent(QMouseEvent *event);

    //! handles mouse wheel events
    virtual void wheelEvent(QWheelEvent *event);

    /*!
     * \brief finds the id of the geometry at supplied point
     * \param point point to look for geometry at
     * \return id of geometry at point or -1 if none
     */
    int selectFromPoint(const QPoint point);

    /*!
     * \brief gets the world position related to the point in the view
     * \param point point in the view to translate
     * \param useDepth determins if the depth buffer is used for calculation
     * \return position in world space
     */
    QVector3D getPosition(const QPoint point, bool useDepth = true);

    /*!
     * \brief finds the closest grid positon from a point
     * \param point point to find grid position from
     * \return grid positon
     */
    QVector3D findClosestGrid(QPoint point);

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

    /*!
     * \brief updates the status of the camera tool
     * \param checked true if tool is on
     */
    void cameraToggle(bool checked);

    /*!
     * \brief updates the status of the geometry tool
     * \param checked true if tool is on
     */
    void geoToolToggle(bool checked);

    /*!
     * \brief draws the brush
     * \param brush brush to draw
     */
    void drawBrush(M3DEditLevel::Box brush);

    /*!
     * \brief stop drawing any brushes
     */
    void stopBrush();

signals:

    //! geometry has been selected
    void selectedGeo(int id);

    //! geometry has been unselected
    void unselect(int id);

    //! a new camera position has been found
    void newCamPos(QVector3D, QVector3D);

    //! a brush is being drawn
    void brushDraw(M3DEditLevel::Box);

    //! a brush is no longer being drawn
    void brushStop();

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

    //! last global mouse positon
    QPoint lastMousePosition;

    //! last local mouse postion
    QPoint lastLocalMouse;

    //! first mouse position
    QPoint firstMousePosition;

    //! state of right click mouse tracking
    bool mouseTrackRight;

    //! state of left click mouse tracking
    bool mouseTrackLeft;

    //! state if the mouse has moved
    bool mouseMoved;

    //! if this view has been initialized
    bool initialized;

    //! if a camera line is being drawn
    bool camLine;

    //! if a brush is being drawn
    bool brushToggle;

    //! if a brush is valid
    bool validBrush;

    //! if geometry is being resized
    bool resizeMode;

    //! the current tool being used
    ToolState toolState;

    //! start of a camera line
    QVector3D camFrom;

    //! end of a camera line
    QVector3D camTo;

    //! brush being drawn
    M3DEditLevel::Box brush;

    //! geometry being resized
    M3DEditLevel::Geometry *resizeTarget;

    //! starting point of a resize
    QVector3D resizeStart;
};
}

#endif // AXISVIEWWIDGET_H
