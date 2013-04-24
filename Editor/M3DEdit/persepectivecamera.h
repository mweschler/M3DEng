#ifndef PERSEPECTIVECAMERA_H
#define PERSEPECTIVECAMERA_H

#include <QMatrix4x4>
#include <QPoint>
#include <QObject>
#include "camera.h"

namespace M3DEditRender{
/*!
 * \brief Represnts a camera used for perspective rendering in a view
 */
class PerspectiveCamera : public Camera
{
    Q_OBJECT
public:
    explicit PerspectiveCamera(QObject *parent = 0);

    /*!
     * \brief gets the current target point the camera is looking at
     * \return target point
     */
    QVector3D getTarget() const;

    /*!
     * \brief updates the projection using the last width and height given
     */
    void updateProjection();

    /*!
     * \brief updates the projection using the supplied width and height
     * \param width width of the view
     * \param height height of the view
     */
    void updateProjection(int width, int height);

    /*!
     * \brief moves the camera target based on a 2D movement vector
     * \param point movement vector
     */
    void moveTarget(QPoint point);

    /*!
     * \brief sets the target of the camera
     * \param target vector to set the target to
     */
    void setTarget(QVector3D target);

public slots:
    //! move the camera in the direction of its target
    void moveFoward();

private:
    //! the target the camera is looking at
    QVector3D target;

    //! the last view width used in the projection
    int lastWidth;

    //! the last view height used in the projection
    int lastHeight;
};
}

#endif // PERSEPECTIVECAMERA_H
