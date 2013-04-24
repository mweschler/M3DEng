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

//hard codeded indicies for drawing as a series of lines
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

//hard codeded indicies for drawing as a triangle mesh
const static unsigned int triangleIndex[] ={
    0, 1, 4,
    0, 4, 5,

    8, 7, 2,
    8, 2, 9,

    11, 3, 6,
    11, 10, 3,

    21, 22, 23,
    21, 23, 20,

    16, 17, 18,
    16, 18, 19,

    12, 14, 15,
    12, 15, 13,
};

QVector<QVector3D> Box::getVerticies() const{

    //create the eight verticies from the bound points
    QVector3D vec1 (upperPoint);
    QVector3D vec2 (upperPoint.x(), upperPoint.y(), lowerPoint.z());
    QVector3D vec3 (upperPoint.x(), lowerPoint.y(), lowerPoint.z());
    QVector3D vec4 (lowerPoint);
    QVector3D vec5 (lowerPoint.x(), upperPoint.y(), lowerPoint.z());
    QVector3D vec6 (lowerPoint.x(), upperPoint.y(), upperPoint.z());
    QVector3D vec7 (lowerPoint.x(), lowerPoint.y(), upperPoint.z());
    QVector3D vec8 (upperPoint.x(), lowerPoint.y(), upperPoint.z());

    //create a list of verticies from these 8. Duplicates to match with normals
    QVector<QVector3D> verts;
    verts.append(vec1);
    verts.append(vec2);
    verts.append(vec3);
    verts.append(vec4);
    verts.append(vec5);
    verts.append(vec6);
    verts.append(vec7);
    verts.append(vec8);
    verts.append(vec1);
    verts.append(vec2);
    verts.append(vec5);
    verts.append(vec6);
    verts.append(vec3);
    verts.append(vec4);
    verts.append(vec8);
    verts.append(vec7);
    verts.append(vec2);
    verts.append(vec3);
    verts.append(vec4);
    verts.append(vec5);
    verts.append(vec1);
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

    const int edges = 12;//boxes have twelve edges

    //create a two idicies for each edge to draw a line between them
    for(int i = 0; i < (edges * 2); ++i)
    {
        indicies.push_back(linesIndex[i]);
    }

    return indicies;
}

QVector<unsigned int> Box::getTriangleIndex() const
{
    QVector<unsigned int> indicies;
    const int sides = 8;
    const int trianglesPerSide = 2;
    const int vertsPerTriangle = 3;
    const int verts = (sides * trianglesPerSide) * vertsPerTriangle;

    for(int i = 0; i < verts; ++i){
        indicies.push_back(triangleIndex[i]);
    }

    return indicies;
}

QVector<QVector3D> Box::getNormals() const
{
    //box normals are standard
    QVector<QVector3D> normals;
    QVector3D left(-1.0f, 0.0f, 0.0f);
    QVector3D right(1.0f, 0.0f, 0.0f);
    QVector3D up(0.0f, 1.0f, 0.0f);
    QVector3D down(0.0f, -1.0f, 0.0f);
    QVector3D back(0.0f, 0.0f, -1.0f);
    QVector3D foward(0.0f, 0.0f, 1.0f);

    normals.push_back(up);
    normals.push_back(up);
    normals.push_back(left);
    normals.push_back(right);
    normals.push_back(up);
    normals.push_back(up);
    normals.push_back(right);
    normals.push_back(left);
    normals.push_back(left);
    normals.push_back(left);
    normals.push_back(right);
    normals.push_back(right);
    normals.push_back(down);
    normals.push_back(down);
    normals.push_back(down);
    normals.push_back(down);
    normals.push_back(foward);
    normals.push_back(foward);
    normals.push_back(foward);
    normals.push_back(foward);
    normals.push_back(back);
    normals.push_back(back);
    normals.push_back(back);
    normals.push_back(back);

    return normals;
}

void Box::rebound(QVector3D start)
{
    int match = -1;
    QVector<QVector3D> verts = this->getVerticies();

    //find the vert index that matches the start value
    for(int i = 0; i < 8; ++i)
    {
        QVector3D vert = verts[i];
        if(vert == start)
            match = i;
    }

    //swap boundsing values depending on match
    switch(match){
    case 1: this->upperPoint = verts[1];
        this->lowerPoint = verts[6];
        break;
    case 2: this->upperPoint = verts[2];
        this->lowerPoint = verts[5];
        break;
    case 3: this->upperPoint = verts[3];
        this->lowerPoint = verts[0];
        break;
    case 4:
        this->upperPoint = verts[4];
        this->lowerPoint = verts[7];
        break;
    case 5:
        this->upperPoint = verts[5];
        this->lowerPoint = verts[2];
        break;
    case 6:
        this->upperPoint = verts[6];
        this->lowerPoint = verts[1];
        break;
    case 7:
        this->upperPoint = verts[7];
        this->lowerPoint = verts[4];
        break;
    case 0: break;//no change
    };
}

void Box::resize(QVector3D from, QVector3D to)
{
    this->rebound(from);//rebound upper bound
    this->upperPoint = to;//move it to resize
}


}
