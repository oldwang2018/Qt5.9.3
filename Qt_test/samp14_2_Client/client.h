#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include<QLabel>
#include<QtNetwork>

namespace Ui {
class Client;
}

class Client : public QMainWindow
{
    Q_OBJECT
private:
    QLabel *LabSocketState;
    QTcpSocket *tcpClient;
    QString  getlocalIP();
protected:
    void    closeEvent(QCloseEvent *event);
private slots:
    void onConnected();
    void onDisconnected();
    void onSocketStateChange(QAbstractSocket::SocketState socketState);
    void onSocketReadyRead();//读取socket传入的数据


public:
    explicit Client(QWidget *parent = 0);
    ~Client();

private slots:
    void on_actHostInfo_triggered();
    void on_actConnect_triggered();
    void on_actDisconnect_triggered();

    void on_btnSendMsg_clicked();

    void on_btnSelectSaveDir_clicked();
    void on_btnSelectFile_clicked();

private:
    Ui::Client *ui;
};

#endif // CLIENT_H
