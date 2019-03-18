#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include<QLabel>
#include<QtNetwork/QHostInfo>
#include<QtNetwork/QTcpServer>
#include<QtNetwork/QTcpSocket>

namespace Ui {
class Server;
}

class Server : public QMainWindow
{
    Q_OBJECT
private:
    QLabel *LabListen;
    QLabel *LabSocketState;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QString getlocalIP();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
   void onNewConnection();
   void onSocketStateChange(QAbstractSocket::SocketState socketState);
   void onClientConnected();
   void onClientDisconnected();
   void onSocketReadyRead();

   void on_acHostInfo_triggered();
   void on_actStart_triggered();

   void on_actStop_triggered();

   void on_btnSendMsg_clicked();

   void on_btnSaveDir_clicked();

   void on_btnSelectFile_clicked();



public:
    explicit Server(QWidget *parent = 0);
    ~Server();

private:
    Ui::Server *ui;
};

#endif // SERVER_H
