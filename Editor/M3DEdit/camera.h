#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QObject>

namespace M3DEditRender{
/*!
 * \brief represents a camera to be used for rendering
 */
class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = 0);
    //! gets this cameras projection matrix;
    QMatrix4x4 getProjMatrix() const;
    /*!
     * \brief gets the current position of the camera
     * \return position of the camera
     */
    QVector3D getPosistion() const;
    /*!
     * \brief sets the position of the camera
     * \param newPos the new position of the camera
     */
    void setPosition(const QVector3D &newPos);
protected:


    /*!
     * \brief sets the position of the camera
     * \param x x location
     * \param y y location
     * \param z z location
     */
    void setPosition(float x, float y, float z);

    QMatrix4x4 projection;
    //! current position of the camera
    QVector3D position;
private:

};
}

#endif // CAMERA_H
