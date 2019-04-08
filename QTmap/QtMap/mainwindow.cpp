#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bridge.h"
#include<QDebug>
#include<QtWebEngineWidgets/QWebEnginePage>
#include <QDir>
#include <QWebChannel>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_view=new QWebEngineView(this);
    setCentralWidget(m_view);
    channel = new QWebChannel(this);   //创建通道对象用于与JS交互
    bridge *mybride=new bridge();        //创建通道对象用于与JS交互
    channel->registerObject("bridge_name",(QObject*)mybride);
    // 这里注册对象名"bridge_name"需要与JS函数用到的名称一致
    //见baidumap.html文件第94行 window.bridge_js = channel.objects.bridge_name;
    // C++对象"bridge_name", JS对象bridge_js。101行，JS调用QT中C++函数只需
    //  bridge_js.getCoordinate(e.point.lng, e.point.lat);网页向QT传递经纬度坐标
    //这就实现了JS调用C++函数，实现网页向QT传递数据
    m_view->page()->setWebChannel(channel);

    QDir temDir("../baidumap.html");
    QString absDir = temDir.absolutePath();
    //absDir="C:/Users/WangYu/Desktop/tmp/TestMap3/Baidu_JS/sample.html";
    QString filePath = "file:///" + absDir;  qDebug()<<filePath;
    m_view->page()->load(QUrl(filePath));//load the map in relevant path

    qDebug()<<sizeof(m_view);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString cmd=QString("addmarker(%0,%1)")
            .arg(ui->spinlong->value())
            .arg(ui->spinlat->value());
    //把要调用的JS命令当做QString传递给网页
    //对应baidumap.html 第77行
    //这就实现了QT通过C++调用JS函数
    m_view->page()->runJavaScript(cmd);
}


void MainWindow::on_btnRunCmd_clicked()
{
    QString cmd=ui->plainTextEdit->document()->toPlainText();
    m_view->page()->runJavaScript(cmd);
}
