#include "dialog.h"
#include "ui_dialog.h"
#include "widget.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    x = -300;

    //readline
    qDebug() << "start read";
    int count = 0;
    int kk=0;
    QFile file("/home/pi/wangha/qt/qtForRaspi/e.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        for(int i = 0;!file.atEnd();i++){
            QByteArray line = file.readLine();
            count ++;
        }
    }
    file.close();
    if(count < 50){
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            for(int i = 0; i< 50 && !file.atEnd();i++){
                QByteArray line = file.readLine();
                QString str(line);
                str = str.trimmed();
                QStringList data = str.split('|');
                x += 30;
                QString ystr = data[0];
                y = ystr.toFloat()+150;
                points1 << QPointF(x, y);
            }
        }
    }else{
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            for(int i = 0;!file.atEnd()&&kk<50;i++){
                QByteArray line = file.readLine();
                if(i>count-50){
                    QString str(line);
                    str = str.trimmed();
                    QStringList data = str.split('|');
                    x += 30;
                    QString ystr = data[0];
                    y = ystr.toFloat()+150;
                    points1 << QPointF(x, y);
                    kk++;
                }
            }
        }
    }
    file.close();
    smoothCurvePath1 = SmoothCurveGenerator::generateSmoothCurve(points1);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 2));

    // 绘制第一条平滑曲线和曲线上的顶点
    painter.drawPath(smoothCurvePath1);
    painter.setBrush(Qt::gray);
    for (int i = 0; i < points1.length() ; i += 1) {
        painter.drawEllipse(points1[i].x()-3, points1[i].y()-3, 6, 6);
    }
}
