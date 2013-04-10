#include "axiscamera.h"

namespace M3DEditRender{
AxisCamera::AxisCamera(AxisLock lock): lockAxis(lock)
{
}

void AxisCamera::setLock(AxisLock lock)
{
    this->lockAxis = lock;
}
}
