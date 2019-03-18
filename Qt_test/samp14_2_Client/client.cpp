#include "client.h"
#include "ui_client.h"
#include<QFileDialog>
#include<QAbstractSocket>
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

Client::Client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    LabSocketState =new QLabel("Socket状态：");
    LabSocketState->setMinimumWidth(200);
    ui->statusBar->addWidget(LabSocketState);

    tcpClient=new QTcpSocket(this);
    ui->editIP->setText(getlocalIP());   this->setWindowTitle(this->windowTitle()+"--本机IP:"+getlocalIP());

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
}

void Client::on_actConnect_triggered()
{//连接主机
    QString addr=ui->editIP->text();
    quint16 port=ui->spinPort->value();
    tcpClient->connectToHost(addr,port);
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
    while(tcpClient->canReadLine())
        ui->plainTextEdit->appendPlainText("[in] "+tcpClient->readLine());
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
    str.append('\n');
    tcpClient->write(str);
}

void Client::closeEvent(QCloseEvent *event)
{
    if (tcpClient->state()==QAbstractSocket::ConnectedState)
        tcpClient->disconnectFromHost();
       event->accept();
}







void Client::on_btnSelectSaveDir_clicked()
{
    ui->editSaveDir->setText(QFileDialog::getExistingDirectory(this,"接收文件保存路径"));
}
void Client::on_btnSelectFile_clicked()
{
    ui->editFile->setText(QFileDialog::getOpenFileName(this,"选择待发送文件"));
}

