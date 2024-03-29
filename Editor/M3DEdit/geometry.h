#ifndef GEOMETRY_H
#define GEOMETRY_H

template <typename T>
class QVector;
class QVector3D;
class QVector4D;

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

    //! pure virtual method for rebounding geometry
    virtual void rebound(QVector3D start) = 0;

    /*!
     * \brief pure virtual method for resizing geometry
     * \param from point resizing from
     * \param to point resizing to
     */
    virtual void resize(QVector3D from, QVector3D to) = 0;

    //! pure virtual function to get the indicies for line drawing
    virtual QVector<unsigned int> getLineIndex() const = 0;

    //! pure virtual function to get the indicies for triangle mesh
    virtual QVector<unsigned int> getTriangleIndex() const = 0;

    //! pure virtual funciton to get the normals
    virtual QVector<QVector3D> getNormals() const = 0;

};
}

#endif // GEOMETRY_H
