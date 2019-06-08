#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "wiringPiI2C.h"
#include "wiringPi.h"
#include <QDebug>
#include <QTimer>
#include <math.h>
#include <QList>
#include <QPainter>
#include <QPainterPath>
#include "smoothcurvegenerator.h"

#include <glwidget.h>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

namespace Ui {
class Widget;
}

QT_BEGIN_NAMESPACE
class QSlider;
class QPushButton;
class QLabel;
QT_END_NAMESPACE

class GLWidget;
class MainWindow;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void read();
    void cal();
    //void show();

private:
    Ui::Widget *ui;void adxl345_init(int fd);
    int fd;
    QTimer * timer;

    int during = 5;

    int lx[5];
    int ly[5];
    int lz[5];

    int xw=0;
    int yw=0;
    int zw=0;

    QList<float> axs;
    QList<float> ays;
    QList<float> azs;

    QList<QPointF> pointsx;
    QList<QPointF> pointsy;
    QList<QPointF> pointsz;

    int t = 0;
    QList<float> ii;

    QPainterPath smoothCurvePathx;
    QPainterPath smoothCurvePathy;
    QPainterPath smoothCurvePathz;

    QSlider *createSlider();
    GLWidget *glWidget;
    QSlider *xSlider;
    QSlider *ySlider;
    QSlider *zSlider;

    QLabel *labela;
    QLabel *labelax;
    QLabel *labelay;
    QLabel *labelaz;
    QLabel *labeld;
    QLabel *labeldx;
    QLabel *labeldy;
    QLabel *labeldz;

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

signals:
    void startcal();
    //void startshow();
};

#endif // WIDGET_H
