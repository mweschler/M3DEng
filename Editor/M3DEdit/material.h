#ifndef MATERIAL_H
#define MATERIAL_H

#include <QVector4D>

namespace M3DEditGUI{
/*!
 * \brief Represents a material for some level object. A material defines how an object should be rendered
 */
class Material
{
public:
    explicit Material();
    /*!
     * \brief gets the diffuse color for this material
     * \return the diffuse color
     */
    QVector4D getDiffuseColor() const;

    /*!
     * \brief sets the diffuse color for this material
     * \param diffuse the color to set
     */
    void setDiffuseColor(QVector4D diffuse);

private:
    //! holds the diffuse color
    QVector4D diffuseColor;
};
}

#endif // MATERIAL_H
