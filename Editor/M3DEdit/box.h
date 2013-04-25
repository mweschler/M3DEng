#ifndef BOX_H
#define BOX_H

#include "geometry.h"
#include <QVector3D>
#include <QVector4D>

namespace M3DEditLevel{

//! Represents a piece of geometry as a rectangular box
class Box : public Geometry
{
public:
    //! Creates a box with default bounds
    explicit Box();

    /*!
     * \brief Initialize a box with supplied bounds
     * \param bounds bounds of the new box
     */
    explicit Box(const QVector<QVector3D> &bounds);

    /*!
     * \brief gets a vector with all of the verticies of this box
     * \return a vector with the verticies
     */
    virtual QVector<QVector3D> getVerticies() const;

    /*!
     * \brief sets the bounds of the box. Bounds should be two points
     * that form a diagnoal across the box
     * \param bounds a vector holding the box
     */
    virtual void setBounds(const QVector<QVector3D> &bounds);

    /*!
     * \brief gets the indicies to draw lines for this geometry
     * \return vector with line indicies
     */
    virtual QVector<unsigned int> getLineIndex() const;

    /*!
     * \brief gets the indicies to draw triangles for this geo
     * \return vector with triangle indicies
     */
    virtual QVector<unsigned int> getTriangleIndex() const;

    /*!
     * \brief gets the normals for this geomtery
     * \return vector containing normals
     */
    virtual QVector<QVector3D> getNormals() const;

    /*!
     * \brief rearanges bounds so that start value is the upper bound
     * \param start value to set as upper bound
     */
    virtual void rebound(QVector3D start);

    /*!
     * \brief resizes a box using an old and new position for a corner
     * \param from origional position of corner
     * \param to new position of corner
     */
    virtual void resize(QVector3D from, QVector3D to);

private:
    //! upper point of the bounds
    QVector3D upperPoint;

    //! lower point of the bounds
    QVector4D lowerPoint;

    //! color of this piece of geometry
    QVector4D color;
};
}

#endif // BOX_H
