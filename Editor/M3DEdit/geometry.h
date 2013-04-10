#ifndef GEOMETRY_H
#define GEOMETRY_H

template <typename T> class QVector;
class QVector3D;

namespace M3DEditLevel{

/*!
 * \brief abstract class represing geometry in the level
 */
class Geometry
{
public:
    //! creates new geometry with default bounds
    explicit Geometry();

    //! creates new geometry with supplied bounds
    explicit Geometry(const QVector<QVector3D> &bounds);

    //! pure virtual method to get verticies for geometry
    virtual QVector<QVector3D> getVerticies() const = 0;

    //! pure pirtual method to set the bounds of geometry
    virtual void setBounds(const QVector<QVector3D> &bounds) = 0;
};
}

#endif // GEOMETRY_H
