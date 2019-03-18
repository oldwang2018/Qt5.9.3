#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QLabel>
#include<QtNetwork/QUdpSocket>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QLabel *LabSocketState;
    QUdpSocket *udpSocket;
    QHostAddress groupAddress;
    QString getlocalIP();
private slots:
    void onStateChanged(QAbstractSocket::SocketState socketState);
    void onReadyRead();

    void on_actStart_triggered();

    void on_actStop_triggered();

   /**/  void on_actHostInfo_triggered();

    void on_pushButton_clicked();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
