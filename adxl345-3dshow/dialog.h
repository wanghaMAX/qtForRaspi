#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPainter>
#include <QList>
#include "smoothcurvegenerator.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

private:
    int x;
    int y;
    QList<QPointF> points1;
    QPainterPath smoothCurvePath1;

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;

protected:
    void paintEvent(QPaintEvent *event);

};

#endif // DIALOG_H
