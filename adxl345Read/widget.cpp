#include "widget.h"
#include "ui_widget.h"

#define pi 3.14159265358979323846
#define g 9.8

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    wiringPiSetup();
    fd = wiringPiI2CSetup(0x53);
    adxl345_init(fd);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(read()));
    timer->start(100);

    connect(this,SIGNAL(startcal()),this,SLOT(cal()));
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

void Widget::read(){
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
//    qDebug() << z;

    lx[t] = x;
    ly[t] = y;
    lz[t] = z+50;
    if(x > 65535/2) lx[t] = x - 65535;
    if(y > 65535/2) ly[t] = y - 65535;
    if(z > 65535/2) lz[t] = z - 65585;
//    qDebug() << lz[t];

    t = (t+1)%during;
    if(t==during-1){
        emit startcal();
    }
}

void Widget::on_pushButton_clicked(){
}

void Widget::cal(){
    int xsum = 0;
    int ysum = 0;
    int zsum = 0;

    for(int i=0;i<during;i++){
        if(lx[i] > 255) lx[i] = 255;
        if(ly[i] > 255) ly[i] = 255;
        if(lz[i] > 255) lz[i] = 255;
        if(lx[i] < -255) lx[i] = -255;
        if(ly[i] < -255) ly[i] = -255;
        if(lz[i] < -255) lz[i] = -255;

        xsum += lx[i];
        ysum += ly[i];
        zsum += lz[i];
    }

    float x1 = pi*xsum/(2*255*during);
    float y1 = pi*ysum/(2*255*during);
    float z1 = pi*zsum/(2*255*during);

    float ax = g*tan(x1);
    float ay = g*tan(y1);
    float az = g*cos(z1);

    QString xs;
    xs.sprintf("%f m/s^2",ax);
    QString ys;
    ys.sprintf("%f m/s^2",ay);
    QString zs;
    zs.sprintf("%f m/s^2",az);

    ui->label_2->setText(xs);
    ui->label_4->setText(ys);
    ui->label_6->setText(zs);

    xw = ax*0.25/2;
    yw = ay*0.25/2;
    zw = az*0.25/2;

}
