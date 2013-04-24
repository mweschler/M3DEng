#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <QObject>
#include <QMap>

#include "material.h"

namespace M3DEditGUI{

/*!
 * \brief Manages materials for objects in a level
 */
class MaterialManager : public QObject
{
    Q_OBJECT
public:
    explicit MaterialManager(QObject *parent = 0);

    /*!
     * \brief adds a new material for a piece of geometry
     * \param id id of the geometry this material is for
     * \param material material data for the geometry
     */
    void addMaterial(int id, Material material);

    /*!
     * \brief gets the material for a piece of geometry
     * \param id id of the geometry to retrieve
     * \return the material
     */
    Material getMaterial(int id) const;

public slots:
    /*!
     * \brief updates the diffuse color for a piece of geometry
     * \param id the id or the geometry whoes color is being updated
     * \param diffuse the color to change to
     */
    void updateDiffuse(int id, QVector4D diffuse);

private:
    //! a map to hold materials, id is same as geometries ID
    QMap<int, Material> materials;

};

//! global pointer for the material manager singelton
extern MaterialManager *g_MatMgr;
}

#endif // MATERIALMANAGER_H
