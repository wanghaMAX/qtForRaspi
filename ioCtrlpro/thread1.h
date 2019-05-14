#ifndef THREAD1_H
#define THREAD1_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include "wiringPi.h"

class thread1 : public QThread
{
    Q_OBJECT
public:
    thread1();
    ~thread1();

private:
    void paomadeng();
signals:

public slots:

protected:
    void run();
};

#endif // THREAD1_H
