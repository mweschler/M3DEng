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
    Geometry *getGeometry(int id);
    int total() const;
signals:
    void geometryAdded(int id);
    void geometryRemoved(int id);
public slots:

private:
    QMap<int, Geometry*> geometryList;
    QStack<int> reclaimedIDs;
};
}

#endif // GEOMETRYMANAGER_H
