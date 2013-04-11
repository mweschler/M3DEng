#include "camera.h"

namespace M3DEditRender{
Camera::Camera()
{
}

void Camera::setPosition(const QVector3D &newPos){
    this->position = QVector3D(newPos);
}

void Camera::setPosition(float x, float y, float z){
    QVector3D newPos(x,y,z);
    this->setPosition(newPos);
}

QVector3D Camera::getPosistion() const{
    return this->position;
}

QMatrix4x4 Camera::getProjMatrix() const
{
    return this->projection;
}

}
