#ifndef BOX_H
#define BOX_H

#include "geometry.h"
#include <QVector3D>
template <typename T> class QVector;


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

    virtual QVector<unsigned int> getLineIndex() const;
private:
    //! upper point of the bounds
    QVector3D upperPoint;

    //! lower point of the bounds
    QVector3D lowerPoint;
};
}

#endif // BOX_H
