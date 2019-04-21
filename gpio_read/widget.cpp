#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    wiringPiSetup();
    pinMode(21,INPUT);
    pinMode(22,INPUT);
    pinMode(23,INPUT);
    QTimer *timer1 = new QTimer(this);
    connect(timer1,SIGNAL(timeout()),this,SLOT(read()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QString v21;
    v21.sprintf("%d",analogRead(21));
    QString v22;
    v22.sprintf("%d",digitalRead(22));
    QString v23;
    v23.sprintf("%d",digitalRead(23));
    ui->textBrowser->setText(v21);
    ui->textBrowser_2->setText(v22);
    ui->textBrowser_3->setText(v23);
}

void read(){

}
