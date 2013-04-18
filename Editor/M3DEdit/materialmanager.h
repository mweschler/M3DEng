#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <QObject>
#include <QMap>

#include "material.h"

namespace M3DEditGUI{
class MaterialManager : public QObject
{
    Q_OBJECT
public:
    explicit MaterialManager(QObject *parent = 0);
    void addMaterial(int id, Material material);
    Material getMaterial(int id);
signals:

public slots:
    void updateDiffuse(int id, QVector4D diffuse);
private:
    QMap<int, Material> materials;

};
extern MaterialManager *g_MatMgr;
}

#endif // MATERIALMANAGER_H
