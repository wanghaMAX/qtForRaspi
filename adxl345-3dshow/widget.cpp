#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QPainter>
#include <QDateTime>

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

    for(int s=0;s<20;s++){
        ii.insert(s,15.0*s);
    }

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(read()));
    timer->start(200);

    connect(this,SIGNAL(startcal()),this,SLOT(cal()));
    //connect(this,SIGNAL(startshow()),this,SLOT(show()));

    glWidget = new GLWidget;

    xSlider = createSlider();
    ySlider = createSlider();
    zSlider = createSlider();

    QLabel *forLine = new QLabel;
    forLine->setFixedSize(400,250);

    labela = new QLabel;
    labela->setText("Acceleration");
    labelax = new QLabel;
    labelax->setText("x: 0.0 m/s^2 .");
    labelay = new QLabel;
    labelay->setText("y: 0.0 m/s^2 .");
    labelaz = new QLabel;
    labelaz->setText("z: 0.0 m/s^2 .");

    labeld = new QLabel;
    labeld->setText("Displacement");
    labeldx = new QLabel;
    labeldx->setText("x: 0.0 m .");
    labeldy = new QLabel;
    labeldy->setText("y: 0.0 m .");
    labeldz = new QLabel;
    labeldz->setText("z: 0.0 m .");

    connect(xSlider, &QSlider::valueChanged, glWidget, &GLWidget::setXRotation);
    connect(ySlider, &QSlider::valueChanged, glWidget, &GLWidget::setYRotation);
    connect(zSlider, &QSlider::valueChanged, glWidget, &GLWidget::setZRotation);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QVBoxLayout *dataLayout = new QVBoxLayout;
    dataLayout->addWidget(labela);
    dataLayout->addWidget(labelax);
    dataLayout->addWidget(labelay);
    dataLayout->addWidget(labelaz);
    dataLayout->addWidget(labeld);
    dataLayout->addWidget(labeldx);
    dataLayout->addWidget(labeldy);
    dataLayout->addWidget(labeldz);

    QHBoxLayout *container = new QHBoxLayout;
    container->addWidget(glWidget);
    container->addWidget(xSlider);
    container->addWidget(ySlider);
    container->addWidget(zSlider);
    container->addLayout(dataLayout);

    QWidget *w = new QWidget;
    w->setLayout(container);
    mainLayout->addWidget(w);
    mainLayout->addWidget(forLine);
    this->setLayout(mainLayout);

    xSlider->setValue(180 * 16);
    ySlider->setValue(180 * 16);
    zSlider->setValue(180 * 16);

}

Widget::~Widget()
{
    delete ui;
}

QSlider *Widget::createSlider()
{
    QSlider *slider = new QSlider(Qt::Vertical);
    slider->setRange(0, 360 * 16);
    slider->setSingleStep(16);
    slider->setPageStep(15 * 16);
    slider->setTickInterval(15 * 16);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
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
//    qDebug() << x;
//    qDebug() << y;
//    qDebug() << z;

    lx[t] = x;
    ly[t] = y;
    lz[t] = z+50;
    if(x > 65535/2) lx[t] = x - 65535;
    if(y > 65535/2) ly[t] = y - 65535;
    if(z > 65535/2) lz[t] = z - 65570;
//    qDebug() << lz[t];

    t = (t+1)%during;
    if(t==during-1){
        emit startcal();
    }
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
    qDebug() << z1;

    float x2 = (x1/pi)>1?1:(x1/pi);
    x2 = (x1/pi)<(-1)?(-1):(x1/pi);
    xSlider->setValue(16*180*(1+x2));

    float y2 = (y1/pi)>1?1:(y1/pi);
    y2 = (y1/pi)<(-1)?(-1):(y1/pi);
    ySlider->setValue(16*180*(1+y2));

    float z2 = (z1/pi)>1?1:(z1/pi);
    z2 = (z1/pi)<(-1)?(-1):(z1/pi);
    zSlider->setValue(16*180*(1+z2));

    float ax = g*tan(x1);
    float ay = g*tan(y1);
    float az = g*sin(z1);

    ax = ax>50.0?50.0:ax;
    ay = ay>50.0?50.0:ay;
    az = az>50.0?50.0:az;

    ax = ax<-50.0?-50.0:ax;
    ay = ay<-50.0?-50.0:ay;
    az = az<-50.0?-50.0:az;

    axs.append(ax);
    ays.append(ay);
    azs.append(az);

    QString xa;
    QTextStream(&xa) << "x: " << ax << " m/s^2 .";
    QString ya;
    QTextStream(&ya) << "y: " << ay << " m/s^2 .";
    QString za;
    QTextStream(&za) << "z: " << az << " m/s^2 .";

    QFile file("/home/pi/wangha/qt/qtForRaspi/e.txt");
    file.open(QIODevice::Append | QIODevice::Text);
    QString txt;
    QTextStream(&txt) << ax << "|" << ay << "|" << az << "\n";
    file.write(txt.toUtf8());
    file.close();

    labelax->setText(xa);
    labelay->setText(ya);
    labelaz->setText(za);

    xw = ax*0.25/2.0;
    yw = ay*0.25/2.0;
    zw = az*0.25/2.0;

    QString xx;
    QTextStream(&xx) << "x: " << xw << " m.";
    QString yx;
    QTextStream(&yx) << "y: " << yw << " m.";
    QString zx;
    QTextStream(&zx) << "z: " << zw << " m.";

    labeldx->setText(xx);
    labeldy->setText(yx);
    labeldz->setText(zx);

    this->update();
    //emit startshow();
}

void Widget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 2));

    if(axs.length() >= 21){
        axs.removeAt(0);
        ays.removeAt(0);
        azs.removeAt(0);
    }

    for(int s = 0; s < axs.length(); s++){
        pointsx << QPointF(ii[s]+10.0,axs[s]+300.0);
        pointsy << QPointF(ii[s]+10.0,ays[s]+380.0);
        pointsz << QPointF(ii[s]+10.0,azs[s]+460.0);
    }

//    qDebug() << "here0";
//    qDebug() << axs.length();

    if(axs.length() == 0){
//        qDebug() << "here1";
        painter.drawEllipse(0+10, 0+30, 6, 6);
        painter.drawEllipse(0+10, 0+60, 6, 6);
        painter.drawEllipse(0+10, 0+90, 6, 6);
    }else if(axs.length() == 1){
//        qDebug() << "here2";
        painter.drawEllipse(pointsx[0].x(), pointsx[0].y(), 6, 6);
        painter.drawEllipse(pointsy[0].x(), pointsy[0].y(), 6, 6);
        painter.drawEllipse(pointsy[0].x(), pointsz[0].y(), 6, 6);

    }else if(axs.length() == 2){
//        qDebug() << "here3";
        for (int i = 0; i < pointsx.length() ; i += 1) {
            painter.drawEllipse(pointsx[i].x(), pointsx[i].y(), 6, 6);
            painter.drawEllipse(pointsy[i].x(), pointsy[i].y(), 6, 6);
            painter.drawEllipse(pointsz[i].x(), pointsz[i].y(), 6, 6);
        }
        painter.drawLine(pointsx[0],pointsx[1]);
        painter.drawLine(pointsy[0],pointsy[1]);
        painter.drawLine(pointsz[0],pointsz[1]);

    }else{
//        qDebug() << "here4";
        // 绘制第一条平滑曲线和曲线上的顶点
        smoothCurvePathx = SmoothCurveGenerator::generateSmoothCurve(pointsx);
        smoothCurvePathy = SmoothCurveGenerator::generateSmoothCurve(pointsy);
        smoothCurvePathz = SmoothCurveGenerator::generateSmoothCurve(pointsz);

        painter.drawPath(smoothCurvePathx);
        painter.drawPath(smoothCurvePathy);
        painter.drawPath(smoothCurvePathz);

        painter.setBrush(Qt::gray);
        for (int i = 0; i < pointsx.length() ; i += 1) {
            painter.drawEllipse(pointsx[i].x(), pointsx[i].y(), 6, 6);
            painter.drawEllipse(pointsy[i].x(), pointsy[i].y(), 6, 6);
            painter.drawEllipse(pointsz[i].x(), pointsz[i].y(), 6, 6);
        }
    }

    pointsx.clear();
    pointsy.clear();
    pointsz.clear();
}

void Widget::on_pushButton_clicked()
{
    Dialog *history = new Dialog;
    history->show();
}
