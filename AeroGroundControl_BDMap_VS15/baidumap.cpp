#include "baidumap.h"
#include "ui_baidumap.h"
#include <QSplitter>
#include<QDir>
#include<QDebug>
#include<QHBoxLayout>
#include "bridge.h"
BaiduMap::BaiduMap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaiduMap)
{
    ui->setupUi(this);
    m_view=new QWebEngineView(this);
    channel = new QWebChannel(this);
    bridge *JSbridge =new bridge();
    channel->registerObject("bridge", (QObject*)JSbridge);
connect((QObject*)JSbridge,SIGNAL(sendDate(double,double)),this,SLOT(revCoordinate(double,double)));
    m_view->page()->setWebChannel(channel);
    QDir temDir("../AeroGroundControl/map/baidumap.html");
    QString absDir = temDir.absolutePath();
    absDir="C:/Users/WangYu/Desktop/AeroGroundControl/map/baidumap.html";
    //absDir="./map/baidumap.html";
    QString filePath = "file:///" + absDir;
    //filePath="http://www.baidu.com";
    m_view->page()->load(QUrl(filePath));//load the map in relevant path

    QSplitter   *splitter=new QSplitter(Qt::Horizontal);
    splitter->addWidget(m_view);
    splitter->addWidget(ui->groupBox);
    QHBoxLayout *layout=new QHBoxLayout();
    layout->addWidget(splitter);
    layout->setContentsMargins(2,2,2,2);
    layout->setSpacing(2);
    this->setLayout(layout);

    ui->groupBox->resize(QSize(200,205));
}

BaiduMap::~BaiduMap()
{
    delete ui;
}

void BaiduMap::on_btnAddMarker_clicked()
{
    QString cmd=ui->plainTextEdit->document()->toPlainText();
    qDebug()<<cmd;
    m_view->page()->runJavaScript(cmd);
}

void BaiduMap::revCoordinate(double j, double w)
{
    qDebug()<<QString::number(j,'f',8)
           <<QString::number(w,'f',8)<<"from map";
}
