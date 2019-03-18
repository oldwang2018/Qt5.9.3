#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFile>
#include<QDebug>

#include<QJsonParseError>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonValue>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    NetworkManager = new QNetworkAccessManager(this);
    connect(NetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_replayFinished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QUrl url("http://192.168.0.8:5000/status");
    QNetworkReply* reply = NetworkManager->get(QNetworkRequest(url));
    // NOTE: Store QNetworkReply pointer (maybe into caller).
    // When this HTTP request is finished you will receive this same
    // QNetworkReply as response parameter.
    // By the QNetworkReply pointer you can identify request and response.
}
void MainWindow::slot_replayFinished(QNetworkReply *reply)
{
     // Reading attributes of the reply
     // e.g. the HTTP status code
     QVariant statusCodeV =reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
     // Or the target URL if it was a redirect:
     QVariant redirectionTargetUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
     // see CS001432 on how to handle this
     if (reply->error() != QNetworkReply::NoError)
     {
        qDebug()<<"QNetworkReply::Error !";
        return ;
     }
     // Example 1: Creating QImage from the reply
     //QImageReader imageReader(reply);
     //QImage pic = imageReader.read();
     QString URL=reply->url().toString();
     ui->plainTextEdit->appendPlainText("*********************from URL：："+URL);
     QByteArray bytes = reply->readAll();  // bytes
     QString str = QString::fromUtf8(bytes);//QString string(bytes);
     ui->plainTextEdit->appendPlainText(str);
     ui->plainTextEdit->appendPlainText("***************JSON*********************");
     QJsonParseError jsonError;
     QJsonDocument doucment = QJsonDocument::fromJson(bytes, &jsonError);  // 转化为 JSON 文档
     if (doucment.isNull() || (jsonError.error != QJsonParseError::NoError))
     {qDebug() <<" 解析发生错误";
       return;
     }
     if (!doucment.isObject())
     {qDebug() <<"非JSON 文档";
       return;
     }
     QJsonObject object = doucment.object();  // 转化为对象
     if (object.contains("Name"))
     {  // 包含指定的 key
         QJsonValue value = object.value("Name");  // 获取指定 key 对应的 value
         if (value.isString()) {  // 判断 value 是否为字符串
             QString strName = value.toString(); ; // 将 value 转化为字符串
             qDebug() << "Name : " << strName;
         }
     }


     reply->deleteLater();

}


