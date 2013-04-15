#include <stdexcept>
#include <QDebug>

#include "geometrymanager.h"

namespace M3DEditLevel{
GeometryManager *g_geoMgr = NULL;

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

    qDebug()<<"GeoMgr: Added Geo"<<id;
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

    qDebug()<<"GeoMgr: removed Geo "<<id;
    emit geometryRemoved(id, geo);

    return geo;
}

void GeometryManager::updateGeometry(int id, Geometry *geo)
{
    if(!geometryList.contains(id)){
        qDebug()<<"Unable to update geometry, id "<< id <<" not found";
        throw std::runtime_error("Cann't update geometry, id not found");
    }

    geometryList[id] = geo;

    qDebug()<<"GeoMgr: Updated geo "<<id;
    emit geometryUpdated(id, geo);
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

static float snapToGrid(float pos){
    int gridOff = 1;

    if(pos < 1)
        gridOff = -1;

    int gridNum = pos /10;
    qDebug()<<"[GeoMgr] comparing "<<((int)pos % (10))*gridOff<<">=5";
    if(((int)pos % (10))*gridOff >= 5)
            gridNum += gridOff;

    return gridNum * 10;
}
int GeometryManager::findGeo(QVector3D pos)
{
    //find closest grid line coords
    QVector3D grid(pos);

    qDebug()<<"[GeoMgr] finding for"<<pos.x()<<pos.y()<<pos.z();

    grid.setX(snapToGrid(grid.x()));
    grid.setY(snapToGrid(grid.y()));
    grid.setZ(snapToGrid(grid.z()));

    qDebug()<<"[GeoMgr] grid pos"<<grid.x()<<grid.y()<<grid.z();

    QVector<int> matchList;
    for(int i = 0; i< this->geometryList.size(); ++i){
        QVector<QVector3D> verts = geometryList[i]->getVerticies();
        for(int j = 0; j < verts.size(); ++j){
            qDebug()<<"[GeoMgr] comparing vert"<<verts[j].x()<<verts[j].y()<<verts[j].z();
            if(verts[j] == grid){
                matchList.push_back(i);
            }
        }
    }

    if(matchList.size() == 0)
        return -1;

    return matchList[0];

}
}
