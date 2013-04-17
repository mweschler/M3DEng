#ifndef PERSEPECTIVECAMERA_H
#define PERSEPECTIVECAMERA_H

#include <QMatrix4x4>
#include <QPoint>
#include "camera.h"

namespace M3DEditRender{
class PerspectiveCamera : public Camera
{
public:
    explicit PerspectiveCamera();
    QVector3D getTarget();
    void updateProjection();
    void updateProjection(int width, int height);
    void movePos(QPoint point);
    void moveTarget(QPoint point);
protected:
    void setTarget(QVector3D target);
private:
    QVector3D target;
    int lastWidth;
    int lastHeight;
};
}

#endif // PERSEPECTIVECAMERA_H
