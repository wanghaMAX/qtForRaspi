#include "thread2.h"

thread2::thread2() : QThread()
{
    wiringPiSetup();

    pinMode(23,OUTPUT);
    pinMode(24,OUTPUT);
    pinMode(25,OUTPUT);

    digitalWrite(23,0);
    digitalWrite(24,0);
    digitalWrite(25,0);
}

thread2::~thread2(){

}

void thread2::run(){
    qDebug() << "2";
    liushuideng();
}

void thread2::liushuideng(){
    digitalWrite(23,HIGH);
    delay(500);
    digitalWrite(23,LOW);

    digitalWrite(24,HIGH);
    delay(500);
    digitalWrite(24,LOW);

    digitalWrite(25,HIGH);
    delay(500);
    digitalWrite(25,LOW);

    digitalWrite(25,HIGH);
    delay(500);
    digitalWrite(25,LOW);

    digitalWrite(24,HIGH);
    delay(500);
    digitalWrite(24,LOW);

    digitalWrite(23,HIGH);
    delay(500);
    digitalWrite(23,LOW);
}
