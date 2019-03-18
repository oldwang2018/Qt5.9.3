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
    void initSocket();
private: //文件收发
    QTcpSocket *fileSocket;
    ///文件传送
        QFile *localFile;
        ///文件大小
        qint64 totalBytes;      //文件总字节数
        qint64 bytesWritten;    //已发送的字节数
        qint64 bytestoWrite;    //尚未发送的字节数
        qint64 filenameSize;    //文件名字的字节数
        qint64 bytesReceived;   //接收的字节数
        ///每次发送数据大小
        qint64 perDataSize;
        QString filename;
        ///数据缓冲区
        QByteArray inBlock;
        QByteArray outBlock;
        void initFileSocket();

private slots:
    void onConnected();
    void onDisconnected();
    void onSocketStateChange(QAbstractSocket::SocketState socketState);
    void onSocketReadyRead();//读取socket传入的数据
    ////更新文件发送进度
    void updateFileProgress(qint64);
    void updateFileProgress();
    void displayError(QAbstractSocket::SocketError socketError);


protected:
    void    closeEvent(QCloseEvent *event);
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

    void on_btnSendFile_clicked();

private:
    Ui::Client *ui;
};

#endif // CLIENT_H
