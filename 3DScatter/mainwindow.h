#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QVector>
#include<QVector3D>
#include "scatter.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Scatter *scatter;
    QVector<QVector3D> Vector_pos;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnShow_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
