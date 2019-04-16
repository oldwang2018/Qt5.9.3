#ifndef SCATTER_H
#define SCATTER_H

#include <QWidget>
#include<QScatterDataProxy>
#include <QtDataVisualization>
#include<QVector>
#include<QVector3D>

namespace Ui {
class Scatter;
}
using namespace QtDataVisualization;
class Scatter : public QWidget
{
    Q_OBJECT
private:
    QWidget     *graphContainer;//
    Q3DScatter  *graph3D;       //散点图
    QScatter3DSeries *series;  //散点序列
    QScatterDataProxy *proxy;
    QScatterDataArray *dataArray ;
    QVector<QVector3D> Vector_position; //记录位置
    int itemCount=1;
    void    iniGraph3D(); //初始化绘图
public:
    void show(QVector<QVector3D> Vec);


public:
    explicit Scatter(QWidget *parent = 0);
    ~Scatter();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_sliderH_valueChanged(int value);

    void on_sliderV_valueChanged(int value);

    void on_sliderZoom_valueChanged(int value);

    void on_cBoxTheme_currentIndexChanged(int index);

    void on_cBoxBarStyle_currentIndexChanged(int index);

    void on_spinFontSize_valueChanged(int arg1);

    void on_spinItemSize_valueChanged(double arg1);

    void on_btnBarColor_clicked();

    void on_btnAddPoint_clicked();

    void on_btnReducePoint_clicked();

    void on_btnInit_clicked();



private:
    Ui::Scatter *ui;
};

#endif // SCATTER_H
/*
//******************************************************************
//using namespace QtDataVisualization;


//private slots:
//    void on_spinFontSize_valueChanged(int arg1);

//    void on_cBoxTheme_currentIndexChanged(int index);

//    void on_btnBarColor_clicked();

//    void on_cBoxBarStyle_currentIndexChanged(int index);

//    void on_spinItemSize_valueChanged(double arg1);

//    void on_comboBox_currentIndexChanged(int index);

//    void on_sliderH_valueChanged(int value);

//    void on_sliderV_valueChanged(int value);

//    void on_sliderZoom_valueChanged(int value);

//    void on_timeBar_valueChanged(int value);

//    void on_btnInit_clicked();

//    void on_btnAddPoint_clicked();

//    void on_btnReducePoint_clicked();
*/
