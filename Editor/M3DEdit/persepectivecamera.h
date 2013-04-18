#ifndef PERSEPECTIVECAMERA_H
#define PERSEPECTIVECAMERA_H

#include <QMatrix4x4>
#include <QPoint>
#include <QObject>
#include "camera.h"

namespace M3DEditRender{
class PerspectiveCamera : public Camera
{
    Q_OBJECT
public:
    explicit PerspectiveCamera(QObject *parent = 0);
    QVector3D getTarget();
    void updateProjection();
    void updateProjection(int width, int height);
    void movePos(QPoint point);
    void moveTarget(QPoint point);
    void setTarget(QVector3D target);
public slots:
    void moveFoward();
protected:

private:
    QVector3D target;
    int lastWidth;
    int lastHeight;
};
}

#endif // PERSEPECTIVECAMERA_H
