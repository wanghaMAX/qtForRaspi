#include "widget.h"
#include "ui_widget.h"

#define PWMrange 100

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    wiringPiSetup();
    pinMode(25,OUTPUT);
    softPwmCreate(25,0,PWMrange);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    count ++;
    if(count % 2 == 1){
        softPwmWrite(25,pwm);
        qDebug() << "1";
    }else{
        softPwmWrite(25,0);
        qDebug() << "0";
    }
}

void Widget::on_horizontalSlider_valueChanged(int value)
{
    pwm = value;
    if(count % 2 == 0){
        softPwmWrite(25,0);
        return;
    }else {
        softPwmWrite(25,pwm);
    }
}
