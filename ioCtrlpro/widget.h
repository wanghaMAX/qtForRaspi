#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "wiringPi.h"
#include <QTimer>
#include <QDebug>

#include "thread1.h"
#include "thread2.h"

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
    void change();
    void on_pushButton_2_clicked();
    void switchs(int);

    void on_pushButton_3_clicked();

private:
    int i = 0;
    int j = 0;
    Ui::Widget *ui;
    thread1 *threadone;
    thread2 *threadtwo;
    QTimer *timer;

signals:
    void sig(int);
};

#endif // WIDGET_H
