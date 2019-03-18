#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QMessageBox>
#include<QUrl>
#include<QDesktopServices>
#include<QDir>
#include<QFileDialog>
#include<QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnDefaultPath_clicked()
{
    QString curPath=QDir::currentPath();     qDebug()<<curPath;
    QDir dir(curPath);
    QString sub="temp";
    dir.mkdir(sub);
    ui->editPath->setText(curPath+"/"+sub+"/");
}

void MainWindow::on_btnDownload_clicked()
{
    QString urlSpec=ui->editURL->text().trimmed();
    if(urlSpec.isEmpty())
    {
        QMessageBox::information(this,"错误","请指定需要下载的URL");
        return;
    }
    QUrl newUrl(urlSpec);           //QUrl newUrl=QUrl::fromUserInput(urlSpec);
    if(!newUrl.isValid())
    {
        QMessageBox::information(this,"错误",
                      QString("无效URL: %1 \n 错误信息： %2").arg(urlSpec,newUrl.errorString()));
        return ;
    }

    QString tempDir=ui->editPath->text().trimmed();
    if(tempDir.isEmpty())
    {
      QMessageBox::information(this,"错误","请指定保存下载文件的目录");
      return ;
    }

    QString fullFilename=tempDir+newUrl.fileName();
    if(QFile::exists(fullFilename)) QFile::remove(fullFilename);

    downloadFile=new QFile(fullFilename);
    if(!downloadFile->open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this,"错误","临时文件打开错误");
        return;
    }
    ui->btnDownload->setEnabled(false);
    reply=networkManager.get(QNetworkRequest(newUrl));
    connect(reply,SIGNAL(finished()),this,SLOT(on_finished()));
    connect(reply,SIGNAL(readyRead()),this,SLOT(on_readyRead()));
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),
            this,SLOT(on_downloadProgress(qint64,qint64)));
}
void MainWindow::on_readyRead()
{
    downloadFile->write(reply->readAll());
}

void MainWindow::on_downloadProgress(qint64 bytesRead, qint64 totalBytes)
{
    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setValue(bytesRead);
}
void MainWindow::on_finished()
{
   QFileInfo fileInfo;
   fileInfo.setFile(downloadFile->fileName());
   downloadFile->close();

   delete downloadFile;     downloadFile=Q_NULLPTR;
   reply->deleteLater();    reply=Q_NULLPTR;
   if(ui->checkOpen->isChecked())
       QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absolutePath()));
   ui->btnDownload->setEnabled(true);
}

void MainWindow::on_btnSetDir_clicked()
{
    QString Path=QFileDialog::getExistingDirectory();
    QDir dir(Path);
    QString sub="QTDownload_5";
    dir.mkdir(sub);
    ui->editPath->setText(Path+"/"+sub+"/");
}
