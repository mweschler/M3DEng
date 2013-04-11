#include "axiscamera.h"

namespace M3DEditRender{
AxisCamera::AxisCamera(AxisLock lock): lockAxis(lock)
{
    this->projection.setToIdentity();
    //this->projection.ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 200.0f);
}

void AxisCamera::setLock(AxisLock lock)
{
    this->lockAxis = lock;
}

void AxisCamera::updateProjection(int width, int height)
{
    //this->projection.ortho(0, width, 0, height, -1, 1);
    QVector3D target(this->getPosistion());
    QVector3D up(0, 0, 0);

    switch(this->lockAxis){
    case XY:
        target.setZ(target.z() + 10);
        up.setY(1.0f);
        break;
    case XZ:
        target.setY(target.y() + 10);
        up.setZ(1.0f);
        break;
    case YZ:
        target.setZ(target.x() + 10);
        up.setZ(-1.0f);
        break;
    }

    //this->projection.lookAt(this->getPosistion(), target, up);
    qDebug()<<"[AxisCamera] UpateProj: width: "<<width<<" height: "<<height<<
              " targ.x "<<target.x()<<" targ.y "<<target.y()<<" targ.z "<<target.z();
}
}
