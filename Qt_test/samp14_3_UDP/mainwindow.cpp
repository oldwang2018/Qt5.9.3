#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QLabel>
#include<QtNetwork/QHostInfo>
#include<QtNetwork/QUdpSocket>



void MainWindow::onSocketReadyRead()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());

        QHostAddress peerAddr;
        quint16 peerPort;
        udpSocket->readDatagram(datagram.data(),datagram.size(),&peerAddr,&peerPort);

        QString str=datagram.data();
        QString peer="[From "+peerAddr.toString()+":"+QString::number(peerPort)+"] ";
        ui->plainTextEdit->appendPlainText(peer+str);
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    LabSocketState=new QLabel("Socket状态");
    LabSocketState->setMinimumWidth(200);
    ui->statusBar->addWidget(LabSocketState);

    QString localIP=getLocalIP();   this->setWindowTitle(this->windowTitle()+"---本机IP："+localIP);
    ui->comboTargetIP->addItem(localIP);

    udpSocket=new QUdpSocket(this);
    connect(udpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));

    onSocketStateChanged(udpSocket->state());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actStart_triggered()
{
    quint16 port=ui->spinBindPort->value();
    if(udpSocket->bind(port))
    {
        ui->plainTextEdit->appendPlainText("**已成功绑定端口");
        ui->plainTextEdit->appendPlainText("**绑定端口："+QString::number(udpSocket->localPort()));
        ui->actStart->setEnabled(false);
        ui->actStop->setEnabled(true);
    }
    else
        ui->plainTextEdit->appendPlainText("**绑定失败");
}

void MainWindow::on_actStop_triggered()
{
    udpSocket->abort();
    ui->actStart->setEnabled(true);
    ui->actStop->setEnabled(false);
    ui->plainTextEdit->appendPlainText("**绑定已解除");
}


QString MainWindow::getLocalIP()
{
     QString hostName=QHostInfo::localHostName();//本地主机名
     QHostInfo   hostInfo=QHostInfo::fromName(hostName);
     QString   localIP="";

     QList<QHostAddress> addList=hostInfo.addresses();//

     if (!addList.isEmpty())
     for (int i=0;i<addList.count();i++)
     {
         QHostAddress aHost=addList.at(i);
         if (QAbstractSocket::IPv4Protocol==aHost.protocol())
         {
             localIP=aHost.toString();
             break;
         }
     }
     return localIP;
}

void MainWindow::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
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

void MainWindow::on_btnSend_clicked()
{
    QString targetIP=ui->comboTargetIP->currentText();
    QHostAddress targetAddr(targetIP);
    quint16 targetPort=ui->spinTargetPort->value();

    QString msg=ui->editMsg->text();
    QByteArray str=msg.toUtf8();
    udpSocket->writeDatagram(str,targetAddr,targetPort);

    ui->plainTextEdit->appendPlainText("[out] "+msg);
    ui->editMsg->clear();
    ui->editMsg->setFocus();
}

void MainWindow::on_btnBroadcast_clicked()
{
    quint16 targetPort=ui->spinTargetPort->value();

    QString msg=ui->editMsg->text();
    QByteArray str=msg.toUtf8();
    udpSocket->writeDatagram(str,QHostAddress::Broadcast,targetPort);

    ui->plainTextEdit->appendPlainText("[broadcast] "+msg);
    ui->editMsg->clear();
    ui->editMsg->setFocus();
}
