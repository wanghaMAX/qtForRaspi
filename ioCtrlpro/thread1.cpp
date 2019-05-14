#include "thread1.h"

thread1::thread1() : QThread()
{
    wiringPiSetup();

    pinMode(23,OUTPUT);
    pinMode(24,OUTPUT);
    pinMode(25,OUTPUT);

    digitalWrite(23,0);
    digitalWrite(24,0);
    digitalWrite(25,0);
}

thread1::~thread1(){

}

void thread1::run(){
    qDebug() << "1";
    paomadeng();
}

void thread1::paomadeng(){
    digitalWrite(23,HIGH);
    delay(500);
    digitalWrite(23,LOW);

    digitalWrite(24,HIGH);
    delay(500);
    digitalWrite(24,LOW);

    digitalWrite(25,HIGH);
    delay(500);
    digitalWrite(25,LOW);
    digitalWrite(23,HIGH);
    delay(500);
    digitalWrite(23,LOW);

    digitalWrite(24,HIGH);
    delay(500);
    digitalWrite(24,LOW);

    digitalWrite(25,HIGH);
    delay(500);
    digitalWrite(25,LOW);
}
