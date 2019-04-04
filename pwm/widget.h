#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "wiringPi.h"
#include "softPwm.h"
#include <QDebug>

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
    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::Widget *ui;
    int count = 0;
    int pwm = 0;
};

#endif // WIDGET_H
