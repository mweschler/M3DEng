#ifndef MATERIAL_H
#define MATERIAL_H

#include <QVector4D>
namespace M3DEditGUI{
class Material
{
public:
    explicit Material();
    QVector4D getDiffuseColor();
    void setDiffuseColor(QVector4D diffuse);
private:
    QVector4D diffuseColor;
};
}

#endif // MATERIAL_H
