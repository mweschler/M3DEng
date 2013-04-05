#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <QObject>

namespace M3DEditLevel{
class EntityManager : public QObject
{
    Q_OBJECT
public:
    explicit EntityManager(QObject *parent = 0);

signals:

public slots:

};
}

#endif // ENTITYMANAGER_H
