#ifndef BOX_H
#define BOX_H

#include "geometry.h"
#include <QVector3D>
template <typename T> class QVector;


namespace M3DEditLevel{


class Box : public Geometry
{

public:
    explicit Box();
    explicit Box(const QVector<QVector3D> &bounds);
    virtual QVector<QVector3D> getVerticies() const;
    virtual void setBounds(const QVector<QVector3D> &bounds);
private:
    QVector3D upperPoint;
    QVector3D lowerPoint;
};
}

#endif // BOX_H
