#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QVector3D>
#include "scatter.h"
#include"uavclass.h"
#include<QTimer>
#include"baidumap.h"
const int UAV_MaxNum=8;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    int UAVCount=0;
    UAVClass *UAV[UAV_MaxNum];
    QVector<QVector3D> Vector_pos;
    QTimer *Timer;
    //Scatter *scatter;
    BaiduMap *map;

private slots:
    void slot_UAV_Debug(eventType,QString message);//消息处理
    void slot_timer_out();
    void on_btnGenerate_clicked();

    void on_btnUAVStatus_clicked();

    void on_btnCommand_clicked();

    void on_btnUAVLand_clicked();

    void on_btnUAVTakeoff_clicked();

    void on_btnUAVTakeoff_All_clicked();

    void on_btnUAVLand_All_clicked();

    void on_btnGoto_clicked();

    void on_btnSaveStatus_clicked();

    void on_btnUAVStatus_All_clicked();


    void on_actStartMonitor_triggered();
    void on_actStopMonitor_triggered();


    void on_actSelectMap_triggered();

protected:
    void closeEvent(QCloseEvent *event);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
