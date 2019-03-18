#include "dialog.h"
#include "ui_dialog.h"
#include<QFileInfo>
#include<QFileDialog>
#include<QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::on_pushButton_19_clicked()
{
    QString aFileName=QFileDialog::getOpenFileName();
    ui->editFile->setText(aFileName);
}

void Dialog::on_pushButton_20_clicked()
{
    QString aFileName=QFileDialog::getExistingDirectory();
    ui->lineEdit_2->setText(aFileName);
}


void Dialog::on_btnBaseName_clicked()
{
    QPushButton *btn=static_cast<QPushButton*>(sender());
    ui->plainTextEdit->appendPlainText(btn->text());
    ui->plainTextEdit->appendPlainText(btn->toolTip()+"\n");

    QFileInfo fileInfo(ui->editFile->text());
    QString str=fileInfo.baseName();
    ui->plainTextEdit->appendPlainText(str+"\n");
}

void Dialog::on_pushButton_clicked()
{//absoluteFilePath()
    QPushButton *btn=static_cast<QPushButton*>(sender());
    ui->plainTextEdit->appendPlainText(btn->text());

    QFileInfo fileInfo(ui->editFile->text());
    QString str=fileInfo.absoluteFilePath();
    ui->plainTextEdit->appendPlainText(str+"\n");

}

void Dialog::on_pushButton_15_clicked()
{
    QPushButton *btn=static_cast<QPushButton*>(sender());
    ui->plainTextEdit->appendPlainText(btn->text());

    QFileInfo fileInfo(ui->editFile->text());
    QString str=fileInfo.absolutePath();
    ui->plainTextEdit->appendPlainText(str+"\n");

}

void Dialog::on_pushButton_2_clicked()
{
    QPushButton *btn=static_cast<QPushButton*>(sender());
    ui->plainTextEdit->appendPlainText(btn->text());

    QFileInfo fileInfo(ui->editFile->text());
    QString str=fileInfo.fileName();
    ui->plainTextEdit->appendPlainText(str+"\n");
}

void Dialog::on_pushButton_18_clicked()
{
    QPushButton *btn=static_cast<QPushButton*>(sender());
    ui->plainTextEdit->appendPlainText(btn->text());

    QFileInfo fileInfo(ui->editFile->text());
    QString str=fileInfo.filePath();
    ui->plainTextEdit->appendPlainText(str+"\n");
}

void Dialog::on_pushButton_3_clicked()
{
    QPushButton *btn=static_cast<QPushButton*>(sender());
    ui->plainTextEdit->appendPlainText(btn->text());

    QFileInfo fileInfo(ui->editFile->text());
    int n =fileInfo.size();
    ui->plainTextEdit->appendPlainText(QString::asprintf("%d",n)+"\n");
}

void Dialog::on_pushButton_17_clicked()
{
    QPushButton *btn=static_cast<QPushButton*>(sender());
    ui->plainTextEdit->appendPlainText(btn->text());

    QFileInfo fileInfo(ui->editFile->text());
    QString str=fileInfo.path();
    ui->plainTextEdit->appendPlainText(str+"\n");
}

void Dialog::on_pushButton_4_clicked()
{
    QString afileName=ui->editFile->text();

    QFile aFile(afileName);
   // bool ok= QFile::rename(afileName,"/home/wangyu/桌面/2.txt");
  bool  ok= QFile::copy(afileName,"/home/wangyu/桌面/20.hh");
   if(ok)
    qDebug()<<"ok";
   else
    qDebug()<<"fail";
}
