#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QtNetwork/QHostInfo>


void MainWindow::onStateChanged(QAbstractSocket::SocketState socketstate)
{
    switch(socketstate)
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


QString MainWindow::getlocalIP()
{
    QString name=QHostInfo::localHostName();
    QHostInfo aHost=QHostInfo::fromName(name);
    QList<QHostAddress> addrList=aHost.addresses();
    for(int i=0;i<addrList.count();i++)
    {
        QHostAddress addr=addrList.at(i);
        if(addr.protocol()==QAbstractSocket::IPv4Protocol)
            return addr.toString();
    }
    return "";
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LabSocketState=new QLabel("Socket状态：");
    LabSocketState->setMinimumWidth(200);
    ui->statusBar->addWidget(LabSocketState);

    QString localIP=getlocalIP();
    this->setWindowTitle(this->windowTitle()+"--本机IP"+localIP);

    udpSocket=new QUdpSocket(this);
    udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption,1);

    connect(udpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,SLOT(onStateChanged(QAbstractSocket::SocketState)));
    onStateChanged(udpSocket->state());
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
}

MainWindow::~MainWindow()
{
    delete udpSocket;
    delete ui;
}

void MainWindow::on_actStart_triggered()
{
    QString IP=ui->comboIP->currentText();
    groupAddress=QHostAddress(IP);
    quint16 groupPort=ui->spinPort->value();

    if(udpSocket->bind(QHostAddress::AnyIPv4,groupPort,QUdpSocket::ShareAddress))
    {
        udpSocket->joinMulticastGroup(groupAddress);
        ui->plainTextEdit->appendPlainText("**加入组播成功");
        ui->plainTextEdit->appendPlainText("**组播地址IP："+IP);
        ui->plainTextEdit->appendPlainText("绑定端口: "+QString::number(groupPort));
        ui->actStart->setEnabled(false);
        ui->actStop->setEnabled(true);
    }
    else
        ui->plainTextEdit->appendPlainText("**绑定端口失败");
}

void MainWindow::on_actStop_triggered()
{
    udpSocket->leaveMulticastGroup(groupAddress);
    udpSocket->abort();
    ui->plainTextEdit->appendPlainText("**已退出组播，解除端口绑定");
    ui->actStart->setEnabled(true);
    ui->actStop->setEnabled(false);
}
void MainWindow::onReadyRead()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress peerAddr;
        quint16 peerPort;
        udpSocket->readDatagram(datagram.data(),datagram.size(),&peerAddr,&peerPort);

        QString str=datagram.data();
        QString peer="[from "+peerAddr.toString()+':'+QString::number(peerPort)+"] ";
        ui->plainTextEdit->appendPlainText(peer+str);
    }

}

void MainWindow::on_pushButton_clicked()
{
    quint16 groupPort=ui->spinPort->value();
    QString msg=ui->editMsg->text();
    QByteArray datagram=msg.toUtf8();
    udpSocket->writeDatagram(datagram,groupAddress,groupPort);

    ui->plainTextEdit->appendPlainText("[multicst] "+msg);
    ui->editMsg->clear();
    ui->editMsg->setFocus();
}


void MainWindow::on_actHostInfo_triggered()
{
    QString hostName=QHostInfo::localHostName();//本地主机名
    ui->plainTextEdit->appendPlainText("本机主机名："+hostName+"\n");
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
            if (ui->comboIP->findText(IP)<0)
                ui->comboIP->addItem(IP);
        }
    }
}
