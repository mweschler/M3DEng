#include "box.h"

#include <QVector>
#include <stdexcept>

namespace M3DEditLevel{

Box::Box()
{
    QVector<QVector3D> bounds;
    bounds.append(QVector3D(0, 0, 0));
    bounds.append(QVector3D(1,1,1));
    this->setBounds(bounds);
}

Box::Box(const QVector<QVector3D> &bounds){
    this->setBounds(bounds);
}

QVector<QVector3D> Box::getVerticies() const{
    QVector3D vec1 (upperPoint);
    QVector3D vec2 (upperPoint.x(), upperPoint.y(), lowerPoint.z());
    QVector3D vec3 (upperPoint.x(), lowerPoint.y(), upperPoint.z());
    QVector3D vec4 (upperPoint.x(), lowerPoint.y(), lowerPoint.z());
    QVector3D vec5 (lowerPoint.x(), upperPoint.y(), upperPoint.z());
    QVector3D vec6 (lowerPoint.x(), upperPoint.y(), lowerPoint.z());
    QVector3D vec7 (lowerPoint.x(), lowerPoint.y(), upperPoint.z());
    QVector3D vec8 (lowerPoint);

    QVector<QVector3D> verts;
    verts.append(vec1);
    verts.append(vec2);
    verts.append(vec3);
    verts.append(vec4);
    verts.append(vec5);
    verts.append(vec6);
    verts.append(vec7);
    verts.append(vec8);

    return verts;
}

void Box::setBounds(const QVector<QVector3D> &bounds){
    if(bounds.size() != 2)
        throw std::invalid_argument("bounds for box must contain at least two vectors");

    this->upperPoint.setX(bounds[0].x());
    this->upperPoint.setY(bounds[0].y());
    this->upperPoint.setZ(bounds[0].z());

    this->lowerPoint.setX(bounds[1].x());
    this->lowerPoint.setY(bounds[1].y());
    this->lowerPoint.setZ(bounds[1].z());
}
}
