#include "axiscamera.h"

namespace M3DEditRender{
const int grid_size = 10;
AxisCamera::AxisCamera(AxisLock lock): lockAxis(lock)
{
    this->gridHeight = 5;
    this->gridWidth = 5;
    this->lastHeight = 1;
    this->lastWidth = 1;
    this->projection.setToIdentity();
    //this->projection.ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

}

void AxisCamera::setLock(AxisLock lock)
{
    this->lockAxis = lock;
    switch(lock){
    case XY:
        setPosition(0.0f, 0.0f, 1.0f); break;
    case XZ:
        setPosition(0.0f, 10.0f, 0.0f); break;
    case YZ:
        setPosition(-1.0f, 0.0f, 0.0f); break;
    }
}

AxisLock AxisCamera::getLock()
{
    return this->lockAxis;
}

void AxisCamera::updateProjection()
{
    this->updateProjection(lastWidth, lastHeight);
}

void AxisCamera::updateProjection(int width, int height)
{

    QVector3D target(this->getPosistion());
    QVector3D up(0, 0, 0);

    switch(this->lockAxis){
    case XY:
        target.setZ(target.z() - 1);
        up.setY(1.0f);
        break;
    case XZ:
        target.setY(target.y() - 1);
        up.setZ(-1.0f);
        break;
    case YZ:
        target.setX(target.x() + 1);
        up.setY(1.0f);
        break;
    }
    QMatrix4x4 view;
    view.setToIdentity();
    view.lookAt(this->getPosistion(), target, up);

    float aspectW = 1;
    float aspectH = 1;
    if( width > height && height != 0){
        aspectW = (float)width / height;
    }
    if( height > width && width != 0){
        aspectH = (float)height / width;
    }
    qDebug()<<"[AxisCamera] aspectWH: "<<aspectW<<", "<<aspectH;
    QMatrix4x4 proj;
    proj.setToIdentity();
    //proj.perspective(45,aspect, 0, 100);
    proj.ortho(-1.0f * aspectW * gridWidth * grid_size, 1.0f * aspectW  * gridWidth * grid_size,
               -1.0f * aspectH * gridHeight * grid_size, 1.0f * aspectH * gridHeight * grid_size, -1000.0f, 1000.0f);

    this->projection = proj * view ;
    lastWidth = width;
    lastHeight = height;
    qDebug()<<"[AxisCamera] UpateProj: width: "<<width<<" height: "<<height<<
              " pos.x "<<this->getPosistion().x()<<" pos.y "<<this->getPosistion().y()<<" pos.z "<<this->getPosistion().z()<<
              " targ.x "<<target.x()<<" targ.y "<<target.y()<<" targ.z "<<target.z();
}

void AxisCamera::moveVec(QPoint point)
{

    switch(lockAxis){
    case XY:
        this->position.setX(this->position.x() - point.x());
        this->position.setY(this->position.y() + point.y());
        this->updateProjection();
        break;
    case XZ:
        this->position.setX(this->position.x() + point.x());
        this->position.setZ(this->position.z() + point.y());
        this->updateProjection();
        break;
    case YZ:
        this->position.setZ(this->position.z() - point.x());
        this->position.setY(this->position.y() + point.y());
        this->updateProjection();
        break;
    };
}

void AxisCamera::adjustGrid(int width, int height)
{
    this->gridHeight += height;
    this->gridWidth += width;
    if(gridHeight < 1)
        gridHeight = 1;
    if(gridWidth < 1)
        gridWidth = 1;
    updateProjection();
}

int AxisCamera::getGridFactor()
{
    if(gridHeight >= gridWidth)
        return gridHeight;
    else
        return gridWidth;
}


}
