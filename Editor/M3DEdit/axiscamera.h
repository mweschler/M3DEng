#ifndef AXISCAMERA_H
#define AXISCAMERA_H

#include "camera.h"

namespace M3DEditRender
{
//! represents the axis lock of a camera view
enum AxisLock{
    XY,
    YZ,
    XZ,
    UNSET
};

//! A camera for an axis view
class AxisCamera : public Camera
{
public:
    //! creates a camera for a specific lock direction
    explicit AxisCamera(AxisLock lock);

    /*!
     * \brief sets the axis that this camera is locked to
     * \param lock axis to lock the camera to
     */
    void setLock(AxisLock lock);

    /*!
     * \brief gets the current locked axis for a camera
     * \return axis the camera is locked to
     */
    AxisLock getLock() const;

    /*!
     * \brief updates the projection using the last width and height given
     */
    void updateProjection();

    /*!
     * \brief updates the projection using the supplied width and height
     * \param width width of the widget
     * \param height height of the widget
     */
    void updateProjection(int width, int height);

    /*!
     * \brief Moves the camera based ona movement vecotr
     * \param point point to derive movement vector from
     */
    void moveVec(QPoint const point);

    /*!
     * \brief adjusts the grid based by the supplied width and height. Used for zooming in/out
     * \param width width to add to the grid
     * \param height height to add to the grid
     */
    void adjustGrid(int width, int height);

    /*!
     * \brief gets the grid factor(larege of width or height)
     * \return grid factor
     */
    int getGridFactor() const;

    /*!
     * \brief gets the current position of the camera
     * \return position of the camera
     */
    QVector3D getPos() const;
private:
    //! the axis that this camera is locked to
    AxisLock lockAxis;

    //! width of the grid
    int gridWidth;

    //! height of the grid
    int gridHeight;

    //! last view width given
    int lastWidth;

    //! last view height given
    int lastHeight;
};
}

#endif // AXISCAMERA_H
