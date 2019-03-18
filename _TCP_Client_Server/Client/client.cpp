#include "client.h"
#include "ui_client.h"
#include<QFileDialog>
#include<QAbstractSocket>
#include<QDebug>
QString Client::getlocalIP()
{
    QString hostName=QHostInfo::localHostName();
    QHostInfo hostInfo=QHostInfo::fromName(hostName);

    QList<QHostAddress> addList=hostInfo.addresses();
    if(!addList.isEmpty())
        for(int i=0;i<addList.count();i++)
        {
            QHostAddress localIP =addList.at(i);
            if(localIP.protocol()==QAbstractSocket::IPv4Protocol)
                return localIP.toString();
        }
    return "";
}

void Client::on_actHostInfo_triggered()
{//获取本机地址
    QString hostName=QHostInfo::localHostName();//本地主机名
    ui->plainTextEdit->appendPlainText("本机主机名："+hostName);
    QHostInfo   hostInfo=QHostInfo::fromName(hostName);

    QList<QHostAddress> addList=hostInfo.addresses();//
    if (!addList.isEmpty())
    for (int i=0;i<addList.count();i++)
    {
        QHostAddress aHost=addList.at(i);
       if (QAbstractSocket::IPv4Protocol==aHost.protocol())
        {
            QString IP=aHost.toString();
            ui->plainTextEdit->appendPlainText("本机IP地址："+aHost.toString());
//          if (ui->comboIP->findText(IP)<0)  ui->comboIP->addItem(IP);
        }
    }
}

Client::Client(QWidget *parent) :QMainWindow(parent),ui(new Ui::Client)
{
    ui->setupUi(this);
    ui->editIP->setText(getlocalIP());   this->setWindowTitle(this->windowTitle()+"--本机IP:"+getlocalIP());

    initSocket();
    initFileSocket();

}
void Client::initFileSocket()
{
    /////文件传送相关变量初始化
    ///每次发送数据大小为64kb
    perDataSize = 64*1024;
    totalBytes = 0;
    bytestoWrite = 0;
    bytesWritten = 0;
    bytesReceived = 0;
    filenameSize = 0;

    fileSocket = new QTcpSocket(this);
    fileSocket->abort();
    ////文件传送进度更新
    connect(fileSocket,SIGNAL(bytesWritten(qint64)),this,SLOT(updateFileProgress(qint64)));
    connect(fileSocket,SIGNAL(readyRead()),this,SLOT(updateFileProgress()));

    connect(fileSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    ui->progressBarSend->setValue(0);ui->progressBarRec->setValue(0);
}
void Client::initSocket()
{
    LabSocketState =new QLabel("Socket状态：");
    LabSocketState->setMinimumWidth(200);
    ui->statusBar->addWidget(LabSocketState);

    tcpClient=new QTcpSocket(this);
    connect(tcpClient,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(tcpClient,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));
    connect(tcpClient,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,SLOT(onSocketStateChange(QAbstractSocket::SocketState)));
    onSocketStateChange(tcpClient->state());
}

Client::~Client()
{
    delete ui;
    delete tcpClient;
    delete fileSocket;
}

void Client::on_actConnect_triggered()
{//连接主机
    QString addr=ui->editIP->text();
    quint16 port=ui->spinPort->value();
    tcpClient->connectToHost(addr,port);

    fileSocket->connectToHost(ui->editIP->text(),ui->spinPort->value()+1);
}
void Client::on_actDisconnect_triggered()
{
  if(tcpClient->state()==QAbstractSocket::ConnectedState)
      tcpClient->disconnectFromHost();
}

void Client::onConnected()
{
    ui->plainTextEdit->appendPlainText("**已连接到服务器");
    ui->plainTextEdit->appendPlainText("**peer address:"+
                                   tcpClient->peerAddress().toString());
    ui->plainTextEdit->appendPlainText("**peer port:"+
                                   QString::number(tcpClient->peerPort()));
    ui->actConnect->setEnabled(false);
    ui->actDisconnect->setEnabled(true);

}

void Client::onDisconnected()
{
    ui->plainTextEdit->appendPlainText("**已断开与服务器的连接");
    ui->actConnect->setEnabled(true);
    ui->actDisconnect->setEnabled(false);
}

void Client::onSocketReadyRead()
{
//    qDebug()<<"data to read....";
//    while(tcpClient->canReadLine())
//        ui->plainTextEdit->appendPlainText("[in] "+tcpClient->readLine());
//    无法读取python，必须换行
//    QByteArray buffer;
//    buffer = tcpClient->readAll();
//    if(!buffer.isEmpty())
//    {
//        QString str = tr(buffer);
//        ui->plainTextEdit->appendPlainText(str);
//    }
    ui->plainTextEdit->appendPlainText("[in] "+tcpClient->readAll());
}

void Client::onSocketStateChange(QAbstractSocket::SocketState socketState)
{//stateChange()信号槽函数
    switch(socketState)
    {
    case QAbstractSocket::UnconnectedState:
        LabSocketState->setText("scoket状态：UnconnectedState");
        break;
    case QAbstractSocket::HostLookupState:
        LabSocketState->setText("scoket状态：HostLookupState");
        break;
    case QAbstractSocket::ConnectingState:
        LabSocketState->setText("scoket状态：ConnectingState");
        break;
    case QAbstractSocket::ConnectedState:
        LabSocketState->setText("scoket状态：ConnectedState");
        break;
    case QAbstractSocket::BoundState:
        LabSocketState->setText("scoket状态：BoundState");
        break;
    case QAbstractSocket::ClosingState:
        LabSocketState->setText("scoket状态：ClosingState");
        break;
    case QAbstractSocket::ListeningState:
        LabSocketState->setText("scoket状态：ListeningState");
    }
}
void Client::on_btnSendMsg_clicked()
{//发送数据
    QString  msg=ui->editMsg->text();
    ui->plainTextEdit->appendPlainText("[out] "+msg);
    ui->editMsg->clear();
    ui->editMsg->setFocus();

    QByteArray  str=msg.toUtf8();
    //str.append('\n');
    tcpClient->write(str);
}

void Client::closeEvent(QCloseEvent *event)
{
    if (tcpClient->state()==QAbstractSocket::ConnectedState)
        tcpClient->disconnectFromHost();
    if (fileSocket->state()==QAbstractSocket::ConnectedState)
        fileSocket->disconnectFromHost();
       event->accept();
}

void Client::on_btnSelectSaveDir_clicked()
{
    ui->editSaveDir->setText(QFileDialog::getExistingDirectory(this,"接收文件保存路径"));
}

void Client::on_btnSelectFile_clicked()
{
   // ui->editFile->setText(QFileDialog::getOpenFileName(this,"选择待发送文件"));
   filename = QFileDialog::getOpenFileName(this);
   ui->editFile->setText(filename);
}

void Client::on_btnSendFile_clicked()
{

       localFile = new QFile(filename);
        if(!localFile->open(QFile::ReadOnly))
        {
            ui->plainTextEdit->appendPlainText("Client:open file error!");
            return;
        }
        ///获取文件大小
        this->totalBytes = localFile->size();
        QDataStream sendout(&outBlock,QIODevice::WriteOnly);
        sendout.setVersion(QDataStream::Qt_4_8);
        QString currentFileName = filename.right(filename.size()-filename.lastIndexOf('/')-1);

        //qDebug()<<sizeof(currentFileName);
        ////保留总代大小信息空间、文件名大小信息空间、文件名
        sendout<<qint64(0)<<qint64(0)<<currentFileName;
        totalBytes += outBlock.size();
        sendout.device()->seek(0);
        sendout<<totalBytes<<qint64((outBlock.size()-sizeof(qint64)*2));

        bytestoWrite = totalBytes-fileSocket->write(outBlock);
        outBlock.resize(0);
}

void Client::updateFileProgress(qint64 numBytes)
{
    ////已经发送的数据大小
    bytesWritten += (int)numBytes;
    ////如果已经发送了数据
    if(bytestoWrite > 0)
    {
        outBlock = localFile->read(qMin(bytestoWrite,perDataSize));
        ///发送完一次数据后还剩余数据的大小
        bytestoWrite -= ((int)fileSocket->write(outBlock));
        ///清空发送缓冲区
        outBlock.resize(0);
    }
    else
        localFile->close();
    ////更新进度条
    ui->progressBarSend->setMaximum(totalBytes);
    ui->progressBarSend->setValue(bytesWritten);

    ////如果发送完毕
    qDebug()<<"bytesWritten"<<bytesWritten<<"--"<<totalBytes<<totalBytes;
    if(bytesWritten == totalBytes)
    {
       bytesWritten=0;totalBytes=0;
       localFile->close();
      // fileSocket->close();
       qDebug()<<ui->progressBarRec->value()<<"value";
       qDebug()<<ui->progressBarRec->maximum()<<"maximum";
    }
}

void Client::updateFileProgress()
{
    QDataStream inFile(fileSocket);
    inFile.setVersion(QDataStream::Qt_4_8);

    ///如果接收到的数据小于16个字节，保存到来的文件头结构
    if(bytesReceived <= sizeof(qint64)*2)
    {
        if((fileSocket->bytesAvailable()>=(sizeof(qint64))*2) && (filenameSize==0))
        {
            inFile>>totalBytes>>filenameSize;
            bytesReceived += sizeof(qint64)*2;
        }
        if((fileSocket->bytesAvailable()>=filenameSize) && (filenameSize != 0))
        {
            inFile>>filename;
            bytesReceived += filenameSize;
            ////接收的文件放在指定目录下
            filename = ui->editSaveDir->text()+"/"+filename;
            localFile = new QFile(filename);
            if(!localFile->open(QFile::WriteOnly))
            {
                ui->plainTextEdit->appendPlainText("Server::open file error!");
                return;
            }
        }
        else
            return;
    }
    /////如果接收的数据小于总数据，则写入文件
    if(bytesReceived < totalBytes)
    {
        bytesReceived += fileSocket->bytesAvailable();
        inBlock = fileSocket->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }
    ////更新进度条显示
    this->ui->progressBarRec->setMaximum(totalBytes);
    this->ui->progressBarRec->setValue(bytesReceived);
    ////数据接收完成时
    if(bytesReceived == totalBytes)
    {
        this->ui->plainTextEdit->appendPlainText("Receive file successfully!");
        bytesReceived = 0;
        totalBytes = 0;
        filenameSize = 0;
        localFile->close();
        //fileSocket->close();
    }
}
void Client::displayError(QAbstractSocket::SocketError socketError)
{
    ui->plainTextEdit->appendPlainText(fileSocket->errorString());
    //socket->close();
}
//
