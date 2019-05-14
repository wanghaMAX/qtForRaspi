#ifndef READTHREAD_H
#define READTHREAD_H
#include <QObject>
#include <QThread>
#include <QDebug>

class readThread: public QThread
{
    Q_OBJECT
public:
    readThread();
    ~readThread();
protected:
    void run();
signals:
    void datass(int,int,int);
};

#endif // READTHREAD_H
