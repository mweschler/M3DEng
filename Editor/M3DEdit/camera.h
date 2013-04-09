#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>

namespace M3DEditRender{
class Camera
{
public:
    explicit Camera();
protected:
    void setPosition(const QVector3D &newPos);
    void setPosition(float x, float y, float z);
    QVector3D getPosistion() const;
private:
    QVector3D position;
};
}

#endif // CAMERA_H
