#include "server.h"
#include "ui_server.h"
#include<QFileDialog>

QString Server::getlocalIP()
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


void Server::on_acHostInfo_triggered()
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

Server::Server(QWidget *parent) :QMainWindow(parent), ui(new Ui::Server)
{
    ui->setupUi(this);
    ui->editIP->setText(getlocalIP());   this->setWindowTitle(this->windowTitle()+"---本机IP："+getlocalIP());

    initSocket();
    initFileSocket();
}
void Server::initSocket()
{
    LabListen=new QLabel("监听状态:");
    LabListen->setMinimumWidth(150);
    ui->statusBar->addWidget(LabListen);
    LabSocketState=new QLabel("Socket状态：");//
    LabSocketState->setMinimumWidth(200);
    ui->statusBar->addWidget(LabSocketState);

    tcpServer=new QTcpServer(this);
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(onNewConnection()));
}

void Server::initFileSocket()
{
    //// 文件传送相关变量初始化
    perDataSize = 64*1024;
    totalBytes = 0;
    bytestoWrite = 0;
    bytesWritten = 0;
    bytesReceived = 0;
    filenameSize = 0;
    ///文件传送套接字
    fileSocket = new QTcpSocket(this);
    fileServer = new QTcpServer(this);
    fileServer->listen(QHostAddress::Any,ui->spinPort->value()+1);
    connect(fileServer,SIGNAL(newConnection()),this,SLOT(acceptFileConnection()));
}
Server::~Server()
{
    delete ui;
    delete tcpServer;
    delete fileServer;
}

void Server::on_actStart_triggered()
{//开始监听
    QString     IP=ui->editIP->text();//IP地址
    quint16     port=ui->spinPort->value();//端口
    QHostAddress    addr(IP);
    tcpServer->listen(QHostAddress::Any,port);//
//  tcpServer->listen(QHostAddress::LocalHost,port);// Equivalent to QHostAddress("127.0.0.1").
    ui->plainTextEdit->appendPlainText("**开始监听...");
    ui->plainTextEdit->appendPlainText("**服务器地址："+tcpServer->serverAddress().toString());
    ui->plainTextEdit->appendPlainText("**服务器端口："+QString::number(tcpServer->serverPort()));
    ui->actStart->setEnabled(false);
    ui->actStop->setEnabled(true);
    LabListen->setText("监听状态：正在监听");
}
void Server::on_actStop_triggered()
{//停止监听
    if (tcpServer->isListening()) //tcpServer正在监听
    {
        tcpServer->close();//停止监听
        ui->actStart->setEnabled(true);
        ui->actStop->setEnabled(false);
        LabListen->setText("监听状态：已停止监听");
    }
}

void Server::onNewConnection()
{
    ui->plainTextEdit->appendPlainText("有新连接");
    tcpSocket = tcpServer->nextPendingConnection(); //创建socket
    connect(tcpSocket, SIGNAL(connected()),this, SLOT(onClientConnected()));//
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
    connect(tcpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,SLOT(onSocketStateChange(QAbstractSocket::SocketState)));
    onSocketStateChange(tcpSocket->state());

    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));
}
void Server::onSocketStateChange(QAbstractSocket::SocketState socketState)
{//socket状态变化时
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

void Server::onClientConnected()
{//客户端接入时
    ui->plainTextEdit->appendPlainText("**client socket connected");
    ui->plainTextEdit->appendPlainText("**peer address:"+tcpSocket->peerAddress().toString());
    ui->plainTextEdit->appendPlainText("**peer port:"+QString::number(tcpSocket->peerPort()));
}

void Server::onClientDisconnected()
{//客户端断开连接时
    ui->plainTextEdit->appendPlainText("**client socket disconnected");
    tcpSocket->deleteLater();
   // deleteLater();
}

void Server::onSocketReadyRead()
{
    qDebug()<<"data to read....";
//    while(tcpSocket->canReadLine())
//        ui->plainTextEdit->appendPlainText("[in] "+tcpSocket->readLine());
    //无法读取python，必须换行
//    QByteArray buffer;
//    buffer = tcpSocket->readAll();
//    if(!buffer.isEmpty())
//    {
//        QString str = tr(buffer);
//        ui->plainTextEdit->appendPlainText(str);
//    }
    ui->plainTextEdit->appendPlainText("[in] "+tcpSocket->readAll());
}

void Server::on_btnSendMsg_clicked()
{
    QString  msg=ui->editMsg->text();
    ui->plainTextEdit->appendPlainText("[out] "+msg);
    ui->editMsg->clear();
    ui->editMsg->setFocus();

    QByteArray  str=msg.toUtf8();
    //str.append('\n');//添加一个换行符
    tcpSocket->write(str);
}

void Server::closeEvent(QCloseEvent *event)
{//关闭窗口时停止监听
    if (tcpServer->isListening())
        tcpServer->close();//停止网络监听
    if (fileServer->isListening())
        fileServer->close();//停止网络监听
    event->accept();
}

void Server::acceptFileConnection()
{
    bytesWritten = 0;
    ///每次发送数据大小为64kb
    perDataSize = 64*1024;
    fileSocket =fileServer->nextPendingConnection();
    ///接受文件
    connect(fileSocket,SIGNAL(readyRead()),this,SLOT(updateFileProgress()));
    connect(fileSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    connect(fileSocket,SIGNAL(bytesWritten(qint64)),this,SLOT(updateFileProgress(qint64)));
}

void Server::updateFileProgress()
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
void Server::updateFileProgress(qint64 numBytes)
{ ////已经发送的数据大小
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
    this->ui->progressBarSend->setMaximum(totalBytes);
    this->ui->progressBarSend->setValue(bytesWritten);

    ////如果发送完毕
    qDebug()<<"bytesWritten"<<bytesWritten<<"--"<<"totalBytes"<<totalBytes;
    if(bytesWritten == totalBytes)
    {
       bytesWritten=0;totalBytes=0;
       localFile->close();
      // fileSocket->close();
       qDebug()<<ui->progressBarRec->value()<<"value";
       qDebug()<<ui->progressBarRec->maximum()<<"maximum";
    }
}

void Server::displayError(QAbstractSocket::SocketError socketError)
{
    ui->plainTextEdit->appendPlainText(fileSocket->errorString());
    //socket->close();
}

void Server::on_btnSaveDir_clicked()
{
    ui->editSaveDir->setText(QFileDialog::getExistingDirectory(this,"请选择保存路径"));
}
void Server::on_btnSelectFile_clicked()
{
   //  ui->editFile->setText(QFileDialog::getOpenFileName(this,"请选择待发送文件"));
    //???fileSocket->open(QIODevice::WriteOnly);
    ///文件传送进度更新
    //connect(fileSocket,SIGNAL(bytesWritten(qint64)),this,SLOT(updateFileProgress(qint64)));
    filename = QFileDialog::getOpenFileName(this,"Open a file");
    ui->editFile->setText(filename);
}

void Server::on_btnSendFile_clicked()
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

    qDebug()<<sizeof(currentFileName);
    ////保留总代大小信息空间、文件名大小信息空间、文件名
    sendout<<qint64(0)<<qint64(0)<<currentFileName;
    totalBytes += outBlock.size();
    sendout.device()->seek(0);
    sendout<<totalBytes<<qint64((outBlock.size()-sizeof(qint64)*2));

    bytestoWrite = totalBytes-fileSocket->write(outBlock);
    outBlock.resize(0);
}
