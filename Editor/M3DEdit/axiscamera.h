#ifndef AXISCAMERA_H
#define AXISCAMERA_H

#include "camera.h"

namespace M3DEditRender
{
enum AxisLock{
    XY,
    YZ,
    XZ,
    UNSET
};

class AxisCamera : public Camera
{
public:
    explicit AxisCamera(AxisLock lock);
    void setLock(AxisLock lock);
private:
    AxisLock lockAxis;
};
}

#endif // AXISCAMERA_H
