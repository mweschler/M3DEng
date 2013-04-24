#ifndef GEOMETRYMANAGER_H
#define GEOMETRYMANAGER_H

#include <QObject>
#include <QMap>
#include <QStack>
#include <QVector3D>
#include "geometry.h"

namespace M3DEditLevel{

//! a class to manage the geometry within a level
class GeometryManager : public QObject
{
    Q_OBJECT
public:
    explicit GeometryManager(QObject *parent = 0);

    /*!
     * \brief adds new geometry to the level
     * \param newGeo geometry to add
     * \return the id for this newly added geometry
     */
    int addGeometry(Geometry *newGeo);

    /*!
     * \brief removes geometry from the level
     * \param id id of the goemetry to remove
     * \return the removed geometry
     */
    Geometry *removeGeometry(int id);

    /*!
     * \brief updates geometry in the level
     * \param id id of the geometry to update
     * \param geo update geometry object
     */
    void updateGeometry(int id, Geometry *geo);

    /*!
     * \brief gets the a piece of geometry in the level
     * \param id the id of the geometry to retrieve
     * \return the requested geometry
     */
    Geometry *getGeometry(int id);

    /*!
     * \brief gets the total count of geometry pieces in a level
     * \return total pieces of geometry in the level
     */
    int total() const;

    /*!
     * \brief finds a piece of geometry at this position
     * \param pos the position for the geometry
     * \return the id of the geometry found or -1 for none
     */
    int findGeo(const QVector3D pos) const;

signals:

    //! signals new geometry was added to the level with the data
    void geometryAdded(int, M3DEditLevel::Geometry*);

    //! signals gemoetry was removed from the level
    void geometryRemoved(int, M3DEditLevel::Geometry*);

    //! signals geometry was updated in the level
    void geometryUpdated(int, M3DEditLevel::Geometry*);

private:
    //! a map of all geometry in the level by id number
    QMap<int, Geometry*> geometryList;

    //! a stack of reclaimed ids from removed geometry
    QStack<int> reclaimedIDs;
};

//! a global point for the geo manager singleton
extern GeometryManager *g_geoMgr;
}

#endif // GEOMETRYMANAGER_H
