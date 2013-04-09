#include <stdexcept>
#include <QDebug>
#include "geometrymanager.h"

namespace M3DEditLevel{
GeometryManager::GeometryManager(QObject *parent) :
    QObject(parent)
{
}

int GeometryManager::addGeometry(Geometry *newGeo){
    int id;
    if(this->reclaimedIDs.size() != 0){
        id = reclaimedIDs.pop();
    }
    else{
        id = geometryList.size();
    }

    this->geometryList[id] = newGeo;

    emit geometryAdded(id, newGeo);

    return id;
}

Geometry *GeometryManager::removeGeometry(int id){
    if(!geometryList.contains(id)){
        qDebug()<<"Can't remove geometry id "<<id<<" Not found";
        throw std::runtime_error("Cannot remove Gemoetry, ID does not exist");
    }
    Geometry *geo = geometryList[id];
    geometryList.remove(id);
    reclaimedIDs.push(id);

    emit geometryRemoved(id, geo);

    return geo;
}

Geometry *GeometryManager::getGeometry(int id){
    if(!geometryList.contains(id)){
        qDebug() << "Could not find geometry with id "<<id;
        throw std::runtime_error("ID not found for Geometry");
    }

    return geometryList[id];
}

int GeometryManager::total() const{
    return geometryList.size();
}
}
