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

const static unsigned int linesIndex[] ={
    0, 1,
    0, 5,
    1, 4,
    4, 5,
    2, 3,
    2, 7,
    6, 3,
    6, 7,
    1, 2,
    4, 3,
    5, 6,
    0, 7
};

const static unsigned int triangleIndex[] ={
    1, 2, 5,
    1, 5, 6,
    1, 8, 3,
    1, 3, 2,
    6,4, 7,
    6, 5, 4,
    6,7,8,
    6,8,1,
    2,3,4,
    2,4,5,
    3,8,7,
    3,7,4,
};

QVector<QVector3D> Box::getVerticies() const{
    QVector3D vec1 (upperPoint);
    QVector3D vec2 (upperPoint.x(), upperPoint.y(), lowerPoint.z());
    QVector3D vec3 (upperPoint.x(), lowerPoint.y(), lowerPoint.z());
    QVector3D vec4 (lowerPoint);
    QVector3D vec5 (lowerPoint.x(), upperPoint.y(), lowerPoint.z());
    QVector3D vec6 (lowerPoint.x(), upperPoint.y(), upperPoint.z());
    QVector3D vec7 (lowerPoint.x(), lowerPoint.y(), upperPoint.z());
    QVector3D vec8 (upperPoint.x(), lowerPoint.y(), upperPoint.z());

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

QVector<unsigned int> Box::getLineIndex() const
{
    QVector<unsigned int> indicies;
    const int edges = 12;
    for(int i = 0; i < (edges * 2); ++i)
    {
        indicies.push_back(linesIndex[i]);
    }

    return indicies;
}

QVector<unsigned int> Box::getTriangleIndex() const
{
    return QVector<unsigned int>();
}
}
