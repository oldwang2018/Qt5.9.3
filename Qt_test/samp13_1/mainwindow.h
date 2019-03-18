#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"qdicethread.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QDiceThread threadA;
private slots:
    void onthreadA_started();
    void onthreadA_finished();
    void onthreadA_newValue(int seq,int diceValue);

    void on_btnStartTread_clicked();

    void on_btnStopTread_clicked();

    void on_btnBegin_clicked();

    void on_btnPause_clicked();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
