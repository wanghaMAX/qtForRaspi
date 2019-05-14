#include "readthread.h"

readThread::readThread():QThread()
{

}

readThread::~readThread()
{

}

void readThread::run(){
    int x = 1;
    emit readThread::datass(x,x,x);
}

