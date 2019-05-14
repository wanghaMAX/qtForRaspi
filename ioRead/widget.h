#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "readthread.h"
#include <QDebug>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    readThread *a;

private slots:
    void changes(int a,int b,int c);
    void on_pushButton_clicked();
};

#endif // WIDGET_H
