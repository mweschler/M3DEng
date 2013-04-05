#ifndef GEOMETRY_H
#define GEOMETRY_H

template <typename T> class QVector;
class QVector3D;

namespace M3DEditLevel{


class Geometry
{
public:
    explicit Geometry();
    explicit Geometry(const QVector<QVector3D> &bounds);
    virtual QVector<QVector3D> getVerticies() const = 0;
    virtual void setBounds(const QVector<QVector3D> &bounds) = 0;
};
}

#endif // GEOMETRY_H
