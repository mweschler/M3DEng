#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>

namespace M3DEditRender{
/*!
 * \brief represents a camera to be used for rendering
 */
class Camera
{
public:
    explicit Camera();
protected:
    /*!
     * \brief sets the position of the camera
     * \param newPos the new position of the camera
     */
    void setPosition(const QVector3D &newPos);

    /*!
     * \brief sets the position of the camera
     * \param x x location
     * \param y y location
     * \param z z location
     */
    void setPosition(float x, float y, float z);

    /*!
     * \brief gets the current position of the camera
     * \return position of the camera
     */
    QVector3D getPosistion() const;
private:
    //! current position of the camera
    QVector3D position;
};
}

#endif // CAMERA_H
