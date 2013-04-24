#include "persepectivecamera.h"

namespace M3DEditRender{
PerspectiveCamera::PerspectiveCamera(QObject *parent):
    Camera(parent)
{
    this->lastHeight = 1;
    this->lastWidth = 1;
    this->target = QVector3D(0.0f, 0.0f, 0.0f); //default target is the orign
    this->setPosition(-50.0f, 0.0f, 30.0f); //default postion
}


void PerspectiveCamera::updateProjection(int width, int height)
{
    QMatrix4x4 view;
    QMatrix4x4 proj;

    //setup view matrix
    view.setToIdentity();
    view.lookAt(this->getPosistion(), this->target, QVector3D(0.0f, 1.0f, 0.0f));

    //find aspect ratio of the view widget
    float aspect = (float)width / height;

    //set the projection matrix
    proj.setToIdentity();
    proj.perspective(45, aspect, 0.1f, 10000.0f);

    //store an updated verison for rendering
    this->projection = proj * view;

    //store the width and height for reuse
    this->lastWidth = width;
    this->lastHeight = height;
}


/* Rotates the camera target based on a movement vector.
 * To accomplish this a vector is calculated from position to the target.
 * This is then rotated around the origin based on the vector.
 * The new target vector is found by moving the position by this new rotated
 * vector.
*/
void PerspectiveCamera::moveTarget(QPoint point)
{
    QMatrix4x4 rotate;
    QVector3D distance = this->target - this->position;//vector between camera and target

    qDebug()<<"[PerspecCamera] Position"<<position.x()<<position.y()<<position.z();
    qDebug()<<"[PerspecCamera] Target"<<target.x()<<target.y()<<target.z();
    qDebug()<<"[PerspecCamear]: Distance ="<<distance.x()<<distance.y()<<distance.z();

    //create a rotation matrix based on movement vector
    rotate.setToIdentity();
    rotate.rotate(point.x(), 0.0f, 1.0f, 0.0f);
    rotate.rotate(point.y(), 1.0f, 0.0f, 0.0f);

    //rotate this distance vector
    distance = rotate * distance;

    qDebug()<<"[PerspecCamera] rotated distance"<<distance.x()<<distance.y()<<distance.z();
    qDebug()<<"[PerspecCamera] target pre"<<target.x()<<target.y()<<target.z();

    //set the target as the postion + the rotated distance
    this->target = position + distance;

    qDebug()<<"[PerspCamera] Distance"<<point.x()<<point.y()<<
              "Target"<<target.x()<<target.y()<<target.z();

    //update the projection
    this->updateProjection();

}

void PerspectiveCamera::moveFoward()
{
    //get a normalized direction vector for camera
    QVector3D direction = this->target - this->position;
    direction.normalize();

    //move the postion and target in this direction
    this->target = target + direction;
    this->position = position + direction;

    this->updateProjection();
}

QVector3D PerspectiveCamera::getTarget() const
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
