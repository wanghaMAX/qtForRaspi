#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    serial = new QSerialPort;
    serial->close();
    //serial->setPortName("/dev/tty1");
    //serial->setPortName("ttyAMA0");
    serial->setPortName("ttyS0");
    serial->open(QIODevice::ReadWrite);
    serial->setBaudRate(115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    connect(serial,&QSerialPort::readyRead,this,&Widget::read);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QByteArray a = QByteArray("\n");
    a.append(ui->textEdit->toPlainText().toLatin1());
    serial->write(a);
}

void Widget::read(){
    QByteArray buf;
    buf = serial->readAll();
    qDebug() << buf;
    QString a = QString(buf);
    ui->textEdit_2->setText(a);
}
