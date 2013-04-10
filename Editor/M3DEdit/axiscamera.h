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
private:
    //! the axis that this camera is locked to
    AxisLock lockAxis;
};
}

#endif // AXISCAMERA_H
