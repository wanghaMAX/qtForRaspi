#ifndef THREAD2_H
#define THREAD2_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include "wiringPi.h"

class thread2 : public QThread
{
    Q_OBJECT
public:
    thread2();
    ~thread2();

signals:

public slots:

protected:
    void run();

private:
    void liushuideng();
};

#endif // THREAD2_H
