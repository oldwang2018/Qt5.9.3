#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDir>
#include "bridge.h"
#include <QWebChannel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    view = new QWebEngineView(ui->widget);
//    QDir temDir("../TestMap3/baidumap_apiV2_offline/baidu_map_demo.html");
 //   QDir temDir("../webengine/index.html");
        QDir temDir("../TestMap3/Baidu_JS/sample.html");

    QString absDir = temDir.absolutePath();


    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject("bridge", (QObject*)bridge::instance());
   view->page()->setWebChannel(channel);
   view->page()->load(QUrl(QString("file:///%0").arg(absDir)));
   connect(ui->pushButton, &QPushButton::clicked, [this]() {
       view->page()->runJavaScript("showAlert()");
   });
}

MainWindow::~MainWindow()
{
    delete ui;
}
