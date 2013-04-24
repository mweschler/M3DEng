#include "axiscamera.h"

namespace M3DEditRender{

const int grid_size = 10;//! world units per grid point

AxisCamera::AxisCamera(AxisLock lock): lockAxis(lock)
{
    this->gridHeight = 5;
    this->gridWidth = 5;
    this->lastHeight = 1;
    this->lastWidth = 1;
    this->projection.setToIdentity();//set default projection to identity
}

void AxisCamera::setLock(AxisLock lock)
{
    this->lockAxis = lock;

    //when the lock is set, put the camera into a default positon
    switch(lock){
    case XY:
        setPosition(0.0f, 0.0f, 1.0f); break;
    case XZ:
        setPosition(0.0f, 10.0f, 0.0f); break;
    case YZ:
        setPosition(-1.0f, 0.0f, 0.0f); break;
    }
}

AxisLock AxisCamera::getLock() const
{
    return this->lockAxis;
}

void AxisCamera::updateProjection()
{
    //use last width and height to update the projection if none given
    this->updateProjection(lastWidth, lastHeight);
}

void AxisCamera::updateProjection(int width, int height)
{

    QVector3D target(this->getPosistion());//initialize camera target to the position
    QVector3D up(0, 0, 0);

    //depending on axislock, set the axis to look down and the up direction
    switch(this->lockAxis){
    case XY:
        target.setZ(target.z() - 1);//down -z
        up.setY(1.0f);//up is +y
        break;
    case XZ:
        target.setY(target.y() - 1); //down is -y
        up.setZ(-1.0f);//up is -z
        break;
    case YZ:
        target.setX(target.x() + 1); //down is +x
        up.setY(1.0f); //up is +y
        break;
    }

    //setup view matrix
    QMatrix4x4 view;
    view.setToIdentity();
    view.lookAt(this->getPosistion(), target, up);

    //find aspect ratio
    float aspectW = 1;
    float aspectH = 1;
    if( width > height && height != 0){
        aspectW = (float)width / height;
    }
    if( height > width && width != 0){
        aspectH = (float)height / width;
    }

    qDebug()<<"[AxisCamera] aspectWH: "<<aspectW<<", "<<aspectH;

    //set to an orthographic projection using aspect ratios and grid width/height
    QMatrix4x4 proj;
    proj.setToIdentity();
    proj.ortho(-1.0f * aspectW * gridWidth * grid_size, 1.0f * aspectW  * gridWidth * grid_size,
               -1.0f * aspectH * gridHeight * grid_size, 1.0f * aspectH * gridHeight * grid_size, -1000.0f, 1000.0f);

    this->projection = proj * view ;//combine projection and view matricies for final use

    //store the width and height for future use
    lastWidth = width;
    lastHeight = height;

    qDebug()<<"[AxisCamera] UpateProj: width: "<<width<<" height: "<<height<<
              " pos.x "<<this->getPosistion().x()<<" pos.y "<<this->getPosistion().y()<<" pos.z "<<this->getPosistion().z()<<
              " targ.x "<<target.x()<<" targ.y "<<target.y()<<" targ.z "<<target.z();
}

void AxisCamera::moveVec(const QPoint point)
{
    //adjust the position based on the movement vector from the mouse point and
    //axis orientation
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

    //bound grid zoom to at leat 1
    if(gridHeight < 1)
        gridHeight = 1;
    if(gridWidth < 1)
        gridWidth = 1;

    //update projection using the new grid zoom
    updateProjection();
}


int AxisCamera::getGridFactor() const
{
    if(gridHeight >= gridWidth)
        return gridHeight;
    else
        return gridWidth;
}


}
