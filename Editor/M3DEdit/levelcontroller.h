#ifndef LEVELCONTROLLER_H
#define LEVELCONTROLLER_H

#include <QObject>

namespace M3DEditLevel{
class LevelController : public QObject
{
    Q_OBJECT
public:
    explicit LevelController(QObject *parent = 0);

signals:

public slots:

};
}

#endif // LEVELCONTROLLER_H
