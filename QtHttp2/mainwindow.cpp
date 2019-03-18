#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFile>

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::MainWindow)
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
    QUrl url("https://translate.google.cn/");
    QNetworkReply* reply = NetworkManager->get(QNetworkRequest(url));
    // NOTE: Store QNetworkReply pointer (maybe into caller).
    // When this HTTP request is finished you will receive this same
    // QNetworkReply as response parameter.
    // By the QNetworkReply pointer you can identify request and response.
}

void MainWindow::on_pushButton_2_clicked()
{
    QUrl url("http://www.baidu.com/");
    QNetworkReply* reply = NetworkManager->get(QNetworkRequest(url));

}

void MainWindow::slot_replayFinished(QNetworkReply *reply)
{
     // Reading attributes of the reply
     // e.g. the HTTP status code
     QVariant statusCodeV =reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
     // Or the target URL if it was a redirect:
     QVariant redirectionTargetUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
     // see CS001432 on how to handle this
     // no error received?
     if (!(reply->error() == QNetworkReply::NoError))
     {
        return;
     }
     // read data from QNetworkReply here
     // Example 1: Creating QImage from the reply
     //QImageReader imageReader(reply);
     //QImage pic = imageReader.read();
     // Example 2: Reading bytes form the reply
     QByteArray bytes = reply->readAll();  // bytes
     QString str = QString::fromUtf8(bytes);//QString string(bytes);
     ui->textBrowser->setText(str);
     ui->plainTextEdit->appendPlainText(str);
     reply->deleteLater();

     QString filename="/home/wangyu/桌面/1.txt";
     QFile  afile(filename);
     if(afile.open(QIODevice::WriteOnly|QIODevice::Text))
     {
         QTextStream  out(&afile);
         out<<str;
         afile.close();
     }

}



