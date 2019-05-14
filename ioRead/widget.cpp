#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->progressBar->setRange(0,1023);
    ui->progressBar->setValue(533);
    ui->label->setStyleSheet("background: red");

    a = new readThread;
    connect(a,SIGNAL(datass(int,int,int)),this,SLOT(changes(int,int,int)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::changes(int a,int b,int c){
    ui->progressBar->setValue(a);
    ui->progressBar_2->setValue(b);
    ui->progressBar_3->setValue(c);
}

void Widget::on_pushButton_clicked()
{
    a->start();
}
