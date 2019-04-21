#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "wiringPiI2C.h"
#include "wiringPi.h"
#include <QDebug>
#include <QTimer>
#include <math.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();
    void read();
    void cal();

private:
    Ui::Widget *ui;
    void adxl345_init(int fd);
    int fd;
    QTimer * timer;

    int during = 5;

    int lx[5];
    int ly[5];
    int lz[5];

    int xw=0;
    int yw=0;
    int zw=0;

    int t = 0;

signals:
    void startcal();

};

#endif // WIDGET_H
