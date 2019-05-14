#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    wiringPiSetup();

    threadone = new thread1;
    threadtwo = new thread2;

    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(change()));
    timer->start(3200);
    connect(this,SIGNAL(sig(int)),this,SLOT(switchs(int)));
}

Widget::~Widget()
{
    delete ui;
    threadone->exit();
    threadtwo->exit();
}

void Widget::on_pushButton_clicked() // thread1
{
    i = 1;
}

void Widget::on_pushButton_2_clicked() // thread2
{
    i = 0;
}

void Widget::change(){
    emit sig(i);
}

void Widget::switchs(int a){
    if(a == 0){
        threadone->start();
    }else if (a == 1) {
        threadtwo->start();
    }
}

void Widget::on_pushButton_3_clicked()
{
    j++;
    j = j%2;
    if(j == 1){
        timer->stop();
        qDebug() << "stop";
    }else{
        timer->start(3200);
        qDebug() << "start";
    }
}
