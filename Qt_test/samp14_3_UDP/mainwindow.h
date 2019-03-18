#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QLabel>
#include<QtNetwork/QHostInfo>
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
    QString getLocalIP();
private slots:
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);
    void onSocketReadyRead();

    void on_actStart_triggered();

    void on_actStop_triggered();

    void on_btnSend_clicked();

    void on_btnBroadcast_clicked();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
