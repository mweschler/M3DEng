#include "persepectivecamera.h"

namespace M3DEditRender{
PerspectiveCamera::PerspectiveCamera()
{
    this->lastHeight = 1;
    this->lastWidth = 1;
    this->target = QVector3D(0.0f, 0.0f, 00.0f);
    this->setPosition(-50.0f, 0.0f, 30.0f);
}


void PerspectiveCamera::updateProjection(int width, int height)
{
    QMatrix4x4 view;
    QMatrix4x4 proj;

    view.setToIdentity();
    view.lookAt(this->getPosistion(), this->target, QVector3D(0.0f, 1.0f, 0.0f));

    float aspect = (float)width / height;

    proj.setToIdentity();
    proj.perspective(45, aspect, 0, 1000);

    this->projection = proj * view;

    this->lastWidth = width;
    this->lastHeight = height;
}

void PerspectiveCamera::movePos(QPoint point)
{

}

void PerspectiveCamera::moveTarget(QPoint point)
{
    QMatrix4x4 rotate;

    rotate.setToIdentity();

    rotate.rotate(point.x(), 0.0f, 1.0f, 0.0f);
    rotate.rotate(point.y(), 1.0f, 0.0f, 0.0f);
    rotate.translate(-this->position);
    qDebug()<<"[PerspecCamera] target pre"<<target.x()<<target.y()<<target.z();
    this->target = rotate * target;
    qDebug()<<"[PerspCamera] Distance"<<point.x()<<point.y()<<
              "Target"<<target.x()<<target.y()<<target.z();
    this->updateProjection();

}

QVector3D PerspectiveCamera::getTarget()
{
    return this->target;
}

void PerspectiveCamera::updateProjection()
{
    this->updateProjection(lastWidth, lastHeight);
}

void PerspectiveCamera::setTarget(QVector3D target)
{
    this->target = target;
}

}
