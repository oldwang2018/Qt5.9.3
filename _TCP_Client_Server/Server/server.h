#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include<QLabel>
#include<QtNetwork/QHostInfo>
#include<QtNetwork/QTcpServer>
#include<QtNetwork/QTcpSocket>
#include<QFile>

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
private:  //file
    QTcpServer *fileServer;
    QTcpSocket *fileSocket;
    qint64 totalBytes;
    qint64 bytesReceived;
    qint64 bytestoWrite;
    qint64 bytesWritten;
    qint64 filenameSize;
    QString filename;
    ///每次发送数据大小
    qint64 perDataSize;
    QFile *localFile;
    ////本地缓冲区
    QByteArray inBlock;
    QByteArray outBlock;

    void initSocket();
    void initFileSocket();

private slots:
    void onNewConnection();
    void onSocketStateChange(QAbstractSocket::SocketState socketState);
    void onClientConnected();
    void onClientDisconnected();
    void onSocketReadyRead();

    void acceptFileConnection();
    void updateFileProgress();
    void displayError(QAbstractSocket::SocketError socketError);
    void updateFileProgress(qint64);


protected:
    void closeEvent(QCloseEvent *event);

private slots:
   void on_acHostInfo_triggered();
   void on_actStart_triggered();
   void on_actStop_triggered();
   void on_btnSendMsg_clicked();
   void on_btnSaveDir_clicked();
   void on_btnSelectFile_clicked();
   void on_btnSendFile_clicked();

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

private:
    Ui::Server *ui;
};

#endif // SERVER_H
