#ifndef _DRIVER_H
#define _DRIVER_H

#include <QObject>

/*!
 * \brief The Driver class A driver for a Qt program to run HW2's tests
 */
class Driver : public QObject{

    Q_OBJECT

public:
    Driver(QObject *parent = 0);

public slots:
    void run();

signals:
    void finished();
};
#endif // DRIVER_H
