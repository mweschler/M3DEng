#include "material.h"

namespace M3DEditGUI{
Material::Material()
{
    this->diffuseColor = QVector4D(1.0f, 1.0f, 1.0f, 1.0f);
}

QVector4D Material::getDiffuseColor()
{
    return this->diffuseColor;
}

void Material::setDiffuseColor(QVector4D diffuse)
{
    this->diffuseColor = diffuse;
}
}
