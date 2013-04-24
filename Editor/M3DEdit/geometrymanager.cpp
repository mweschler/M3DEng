#include <stdexcept>
#include <QDebug>

#include "geometrymanager.h"
#include "materialmanager.h"

namespace M3DEditLevel{
GeometryManager *g_geoMgr = NULL;

GeometryManager::GeometryManager(QObject *parent) :
    QObject(parent)
{
}

int GeometryManager::addGeometry(Geometry *newGeo){
    int id;

    //if there is an unused id claim it, if not get a new one
    if(this->reclaimedIDs.size() != 0){
        id = reclaimedIDs.pop();
    }
    else{
        id = geometryList.size();//get new id based on size of list
    }

    this->geometryList[id] = newGeo;

    //create a material in the mat manager for this new piece of geometry
    M3DEditGUI::g_MatMgr->addMaterial(id, M3DEditGUI::Material());

    qDebug()<<"GeoMgr: Added Geo"<<id;

    //notify widgets about new geometry
    emit geometryAdded(id, newGeo);

    return id;
}

Geometry *GeometryManager::removeGeometry(int id){
    if(!geometryList.contains(id)){
        qDebug()<<"Can't remove geometry id "<<id<<" Not found";
        throw std::runtime_error("Cannot remove Gemoetry, ID does not exist");
    }

    //grab the geo out of the list then remove it and reclaim the id
    Geometry *geo = geometryList[id];
    geometryList.remove(id);
    reclaimedIDs.push(id);

    qDebug()<<"GeoMgr: removed Geo "<<id;

    //notify widgets of the removal
    emit geometryRemoved(id, geo);

    return geo;
}

void GeometryManager::updateGeometry(int id, Geometry *geo)
{
    if(!geometryList.contains(id)){
        qDebug()<<"Unable to update geometry, id "<< id <<" not found";
        throw std::runtime_error("Cann't update geometry, id not found");
    }

    //update local store of geometry
    geometryList[id] = geo;

    qDebug()<<"GeoMgr: Updated geo "<<id;

    //notify other classes of the update
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

//simple function to find a grid positon
static float snapToGrid(float pos){
    int gridOff = 1;

    if(pos < 1)
        gridOff = -1;

    int gridNum = pos /10;

    if(((int)pos % (10))*gridOff >= 5)
            gridNum += gridOff;

    return gridNum * 10;
}

int GeometryManager::findGeo(const QVector3D pos) const
{
    //find closest grid line coords
    QVector3D grid(pos);

    qDebug()<<"[GeoMgr] finding for"<<pos.x()<<pos.y()<<pos.z();

    //move pos to a grid point
    grid.setX(snapToGrid(grid.x()));
    grid.setY(snapToGrid(grid.y()));
    grid.setZ(snapToGrid(grid.z()));

    qDebug()<<"[GeoMgr] grid pos"<<grid.x()<<grid.y()<<grid.z();

    QVector<int> matchList;

    //iterate through geo list to find matches
    for(QMap<int,Geometry*>::Iterator itr = geometryList.begin(); itr != geometryList.end(); itr++){
        QVector<QVector3D> verts = itr.value()->getVerticies();
        for(int j = 0; j < 8; ++j){
            qDebug()<<"[GeoMgr] comparing vert"<<verts[j].x()<<verts[j].y()<<verts[j].z();
            if(verts[j] == grid){
                matchList.push_back(itr.key());
            }
        }
    }

    //no matches
    if(matchList.size() == 0)
        return -1;

    //dirty default, return the first match. Could be smarter!
    return matchList[0];

}
}
