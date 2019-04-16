#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QStatusBar>
#include<QFileDialog>
#include<QFile>
#include"bridge.h"
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
    scatter=new Scatter();
    ui->stkWidget_display->addWidget(map);
    ui->stkWidget_display->addWidget(scatter);
    ui->stkWidget_display->setContentsMargins(2,2,2,2);
    QSplitter *splitter=new QSplitter(Qt::Horizontal);
    splitter->addWidget(ui->stkWidget_tool);
    splitter->addWidget(ui->stkWidget_display);
    this->setCentralWidget(splitter);
    ui->stkWidget_display->setCurrentIndex(0);

    ui->timeBar->setMinimumWidth(550);
    ui->timeLabel->setMinimumWidth(120);
    ui->statusBar->addWidget(ui->timeBar);
    ui->statusBar->addWidget(ui->timeLabel);
//    connect(map->channel->registeredObjects().begin().value(),
//            SIGNAL(sendDate(double,double)),this,SLOT(;));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scatter;
    delete map;
}

void MainWindow::on_btnGenerate_clicked()
{
    QString filter="文本文件(*.txt);;全部文件(*.*)";
    QString filename=QFileDialog::getOpenFileName(this,"Select a file: ",QDir::currentPath(),filter);
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
        if(ui->action_Map_3D->text()=="地图模式")
        {
            QString cmd=QString("addmarker(%0,%1,%2,%3,%4,%5)")
                    .arg(uav->status_map.last().value("long").toDouble(),0,'g',11)
                    .arg(uav->status_map.last().value("lat").toDouble(),0,'g',11)
                    .arg(uav->status_map.last().value("alt").toDouble(),0,'g',11)
                    .arg(uav->ID)
                    .arg(1)  // 1-add  2-remove
                    .arg(0); //  描述
            map->m_view->page()->runJavaScript(cmd);
            qDebug()<<cmd;
        }
    }
        break;
    default:
        break;
    }
}

void MainWindow::on_actStartMonitor_triggered()
{
    Timer->start();
    ui->actStartMonitor->setEnabled(false);
    ui->actStopMonitor->setEnabled(true);
}
void MainWindow::on_actStopMonitor_triggered()
{
    Timer->stop();
    ui->actStartMonitor->setEnabled(true);
    ui->actStopMonitor->setEnabled(false);
}

void MainWindow::on_actSelectMap_triggered()
{
    QString filter="html文件(*.html);;全部文件(*.*)";
    QString filename=QFileDialog::getOpenFileName(this,"Select a file: ",QDir::currentPath(),filter);
    if(filename=="")
        return;
    QString filePath = "file:///" + filename;
    map->m_view->page()->load(QUrl(filePath));
    ui->plainTextEdit->appendPlainText("加载地图文件--"+filename);
}


void MainWindow::on_act_show3DPostion_triggered()
{
    static double dg[UAV_MaxNum]={0};
    static int flag=1;
    if(flag==1)
    {
        for(int i=0;i<UAVCount;i++)
        {
            dg[i]=UAV[i]->status_map.last().value("alt").toDouble();
            ui->plainTextEdit->appendPlainText(QString("uav %0....%1")
                                               .arg(i).arg(dg[i]));
        }
        flag=0;
    }  //高度校准

    static double dj=UAV[0]->status_map.last().value("long").toDouble();// dj=118.7907568000;
    static double dw=UAV[0]->status_map.last().value("lat").toDouble();// dw=31.93959810000;
    ui->plainTextEdit->appendPlainText(QString::number(dj,'f',10));
    ui->plainTextEdit->appendPlainText(QString::number(dw,'f',10));

    //double Rcd=6377830.0;
    double Rj =6356908.8;
    double RE =6371393.0;
    static double rx=sqrt(  RE*RE-(dw/90.0*Rj)*(dw/90.0*Rj)  );
    static double dRj =rx*3.14159265358979/180;
    double dRw=111201.785789; // RE*PI/180
    qDebug()<<(QString::number(dRj,'f',10));

    Vector_pos.clear();
    Vector_pos.append(QVector3D(0,0,0));
    for(int i=0;i<UAVCount;i++)
    {
        UAV[i]->get_status();
        double dj_i=UAV[i]->status_map.last().value("long").toDouble();
        double dw_i=UAV[i]->status_map.last().value("lat").toDouble();
        double dg_i=UAV[i]->status_map.last().value("alt").toDouble();
        QVector3D Vec=QVector3D(
                    (dj_i-dj)*dRj,
                    (dg_i>dg[i]? (dg_i-dg[i])*1.15:0.0),
                    (dw_i-dw)*dRw
                    );
        Vector_pos.append(Vec);
    }
    scatter->show(Vector_pos);
}
void MainWindow::slot_timer_out()
{
    if(ui->action_Map_3D->text()=="地图模式")
    {
        on_btnUAVStatus_All_clicked();
        qDebug()<<"on_btnUAVStatus_All_clicked()";
    }
    else if(ui->action_Map_3D->text()=="三维模式")
    {
       on_act_show3DPostion_triggered();
       qDebug()<<"on_act_show3DPostion_triggered";
    }
}

void MainWindow::on_action_Map_3D_triggered()
{
    if(ui->action_Map_3D->text()=="三维模式")
    {
        ui->action_Map_3D->setText("地图模式");
        ui->act_show3DPostion->setEnabled(false);
        ui->actSelectMap->setEnabled(true);
        ui->stkWidget_display->setCurrentIndex(0);
    }
    else if(ui->action_Map_3D->text()=="地图模式")
    {
        ui->action_Map_3D->setText("三维模式");
        ui->act_show3DPostion->setEnabled(true);
        ui->actSelectMap->setEnabled(false);
        ui->stkWidget_display->setCurrentIndex(1);
    }
}
