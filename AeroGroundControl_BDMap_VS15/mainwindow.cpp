#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QSplitter>
#include<QDebug>
#include<QStatusBar>
#include<QFileDialog>
#include<QFile>

void MainWindow::closeEvent(QCloseEvent *event)
{
//    for(int i=0;i<UAVCount;++i)
//    {
//       UAV[i]->land();
//       delete UAV[i];
//    }
    qDebug()<<"window closed";
    event->accept();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    UAV[0]=new UAVClass("192.168.0.27",0);UAVCount=1;
    connect(UAV[0],SIGNAL(UAV_debug_signal(eventType,QString)),
                this,SLOT(slot_UAV_Debug(eventType,QString)));//Qt::QueuedConnection

    Timer=new QTimer(this);
    Timer->stop();
    Timer->setInterval(1000);
    connect(Timer,SIGNAL(timeout()),this,SLOT(slot_timer_out()));
    map=new BaiduMap();
    QSplitter *splitter=new QSplitter(Qt::Horizontal);
    splitter->addWidget(ui->groupBox);
    splitter->addWidget(map);
    this->setCentralWidget(splitter);
    ui->timeBar->setMinimumWidth(550);
    ui->timeLabel->setMinimumWidth(120);
    ui->statusBar->addWidget(ui->timeBar);
    ui->statusBar->addWidget(ui->timeLabel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnGenerate_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,"Select a file: ");
    QFile afile(filename);
    if(!afile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        ui->plainTextEdit->appendPlainText("IP配置文件出错");
        return ;
    }
    for(int i=0;i<UAVCount;i++)
        delete UAV[i];

    QString aIP;
    UAVCount=0;
    QTextStream aStream(&afile);
    while((!aStream.atEnd())&&(UAVCount<UAV_MaxNum))
    {
        aIP=aStream.readLine().trimmed();
        if(aIP!="")
        {
            ui->plainTextEdit->appendPlainText(QString("[%1] ").arg(UAVCount)+aIP);
            UAV[UAVCount]=new UAVClass(aIP,UAVCount);
            connect(UAV[UAVCount],SIGNAL(UAV_debug_signal(eventType,QString)),
                        this,SLOT(slot_UAV_Debug(eventType,QString)));
            UAVCount++;
        }
    }
    ui->spinUAVNumber->setMaximum(UAVCount-1);
}

void MainWindow::on_btnCommand_clicked()
{
    int n=ui->spinUAVNumber->value();
    UAV[n]->command(ui->editCommand->text());
}
void MainWindow::on_btnUAVStatus_clicked()
{
    int n=ui->spinUAVNumber->value();
    UAV[n]->get_status();
}
void MainWindow::on_btnUAVTakeoff_clicked()
{
    int n=ui->spinUAVNumber->value();
    UAV[n]->takeoff();
}
void MainWindow::on_btnUAVLand_clicked()
{
    int n=ui->spinUAVNumber->value();
    UAV[n]->land();
}

void MainWindow::on_btnUAVStatus_All_clicked()
{
    for(int i=0;i<UAVCount;i++)
    {
        UAV[i]->get_status();
    }
}
void MainWindow::on_btnUAVTakeoff_All_clicked()
{
    for(int i=0;i<UAVCount;++i)
    {
        UAV[i]->takeoff();
    }
}
void MainWindow::on_btnUAVLand_All_clicked()
{
    for(int i=0;i<UAVCount;++i)
    {
        UAV[i]->land();
    }
}
void MainWindow::on_btnGoto_clicked()
{//http://192.168.0.14:5000/goto?n=3&e=4&d=-3
//    UAV[0]->go_to(2,0,1);
//    UAV[1]->go_to(-2,0,-1);
//    UAV[2]->go_to(0,-2,1);
//    UAV[3]->go_to(0,2,-1);
}
void MainWindow::on_btnSaveStatus_clicked()
{
    QString dirname=QDir::currentPath()+"/status/";
    QDir dir(dirname);
    if(!dir.exists())
        dir.mkdir(dirname);

    for(int i=0;i<UAVCount;i++)
    {
        QString filename="status_"+UAV[i]->get_address();
        QString fullname=dirname+filename;
        QFile aFile(fullname);
        if(!aFile.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            ui->plainTextEdit->appendPlainText("could't write data to "+fullname);
            return;
        }
        QTextStream aStream(&aFile);

        QMap<QTime,QJsonObject>::iterator it;
        for ( it = UAV[i]->status_map.begin(); it != UAV[i]->status_map.end(); ++it )
        {
            aStream<<it.key().toString()<<"\n";
            aStream<<QJsonDocument(it.value()).toJson(QJsonDocument::Compact)<<"\n";
        }
        aFile.close();
        ui->plainTextEdit->appendPlainText(QString("写入文件[%1]成功").arg(i));
    }
}

void MainWindow::slot_UAV_Debug(eventType type, QString message)
{
    UAVClass *uav=qobject_cast<UAVClass*>(sender());
    switch (type) {
    case eventType::Info:
        ui->plainTextEdit->appendPlainText(uav->get_address()+":  "+message);
        break;
    case eventType::Command:
        ui->plainTextEdit->appendPlainText(uav->get_address()+":  "+message);
        break;
    case eventType::JSONinfo:
        {
        ui->plainTextEdit->appendPlainText(uav->get_address()+":  "+message);
        QString cmd=QString("addmarker(%0,%1,%2,%3,%4,%5)")
              .arg(uav->status_map.last().value("long").toDouble(),0,'g',11)
              .arg(uav->status_map.last().value("lat").toDouble(),0,'g',11)
              .arg(uav->status_map.last().value("alt").toDouble(),0,'g',11)
              .arg(uav->ID)
              .arg(1)  // 1-add  2-remove
              .arg(0); //  描述
        qDebug()<<cmd;
        map->m_view->page()->runJavaScript(cmd);
        }
        break;
    default:
        break;
    }
}



void MainWindow::slot_timer_out()
{
    for(int i=0;i<UAVCount;i++)
    {
        UAV[i]->get_status();
    }
   qDebug()<<"0000";
}
void MainWindow::on_actStartMonitor_triggered()
{
    Timer->start();
}
void MainWindow::on_actStopMonitor_triggered()
{
    Timer->stop();
}

void MainWindow::on_actSelectMap_triggered()
{
    QString filename=QFileDialog::getOpenFileName(this,"Select a file: ");
    QString filePath = "file:///" + filename;
    map->m_view->page()->load(QUrl(filePath));
    ui->plainTextEdit->appendPlainText("加载地图文件--"+filename);
}
