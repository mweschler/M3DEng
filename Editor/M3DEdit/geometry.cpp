#include <QtCore>
#include "geometry.h"

namespace M3DEditLevel{

Geometry::Geometry()
{
}

Geometry::Geometry(const QVector<QVector3D> &bounds)
{
    Q_UNUSED (bounds);
}

}
