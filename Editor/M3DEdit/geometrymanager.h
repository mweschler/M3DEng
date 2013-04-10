#ifndef GEOMETRYMANAGER_H
#define GEOMETRYMANAGER_H

#include <QObject>
#include <QMap>
#include <QStack>
#include "geometry.h"

namespace M3DEditLevel{


class GeometryManager : public QObject
{
    Q_OBJECT
public:
    explicit GeometryManager(QObject *parent = 0);
    int addGeometry(Geometry *newGeo);
    Geometry *removeGeometry(int id);
    void updateGeometry(int id, Geometry *geo);
    Geometry *getGeometry(int id);
    int total() const;
signals:
    void geometryAdded(int, M3DEditLevel::Geometry*);
    void geometryRemoved(int, M3DEditLevel::Geometry*);
    void geometryUpdated(int, M3DEditLevel::Geometry*);
public slots:

private:
    QMap<int, Geometry*> geometryList;
    QStack<int> reclaimedIDs;
};

extern GeometryManager *g_geoMgr;
}

#endif // GEOMETRYMANAGER_H
