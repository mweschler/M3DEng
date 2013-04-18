#include "materialmanager.h"

namespace M3DEditGUI{
MaterialManager *g_MatMgr = NULL;
MaterialManager::MaterialManager(QObject *parent) :
    QObject(parent)
{
}

void MaterialManager::addMaterial(int id, Material material)
{
    this->materials[id] = Material(material);
}

Material MaterialManager::getMaterial(int id)
{
    return materials[id];
}

void MaterialManager::updateDiffuse(int id, QVector4D diffuse)
{
    this->materials[id].setDiffuseColor(diffuse);
}
}
