#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    wiringPiSetup();
    fd = wiringPiI2CSetup(0x53);
    adxl345_init(fd);
}

Widget::~Widget()
{
    delete ui;
}

void  Widget::adxl345_init(int fd){
    wiringPiI2CWriteReg8(fd,0x31,0x0b);
    wiringPiI2CWriteReg8(fd,0x2d,0x08);
    wiringPiI2CWriteReg8(fd,0x1e,0x00);
    wiringPiI2CWriteReg8(fd,0x1f,0x00);
    wiringPiI2CWriteReg8(fd,0x20,0x00);
    wiringPiI2CWriteReg8(fd,0x21,0x00);
    wiringPiI2CWriteReg8(fd,0x22,0x00);
    wiringPiI2CWriteReg8(fd,0x23,0x00);
    wiringPiI2CWriteReg8(fd,0x24,0x01);
    wiringPiI2CWriteReg8(fd,0x25,0x0f);
    wiringPiI2CWriteReg8(fd,0x26,0x2b);
    wiringPiI2CWriteReg8(fd,0x27,0x00);
    wiringPiI2CWriteReg8(fd,0x28,0x09);
    wiringPiI2CWriteReg8(fd,0x29,0xff);
    wiringPiI2CWriteReg8(fd,0x2a,0x80);
    wiringPiI2CWriteReg8(fd,0x2c,0x0a);
    wiringPiI2CWriteReg8(fd,0x2f,0x00);
    wiringPiI2CWriteReg8(fd,0x38,0x9f);
}



void Widget::on_pushButton_clicked()
{
    char x0,y0,z0,x1,y1,z1;
    x0 = 0xff - wiringPiI2CReadReg8(fd,0x32);
    x1 = 0xff - wiringPiI2CReadReg8(fd,0x33);
    y0 = 0xff - wiringPiI2CReadReg8(fd,0x34);
    y1 = 0xff - wiringPiI2CReadReg8(fd,0x35);
    z0 = 0xff - wiringPiI2CReadReg8(fd,0x36);
    z1 = 0xff - wiringPiI2CReadReg8(fd,0x37);

    int x = (int)(x1 << 8) + (int)x0;
    int y = (int)(y1 << 8) + (int)y0;
    int z = (int)(z1 << 8) + (int)z0;

    if(x>65535/2){
        x = x - 65535;
    }

    if(y > 62235/2){
        y = y - 65535;
    }

    if(z > 65535/2){
        z = z - 65535;
    }

    qDebug() << x;
    qDebug() << y;
    qDebug() << z;

}
