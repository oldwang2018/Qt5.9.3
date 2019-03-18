#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_4_clicked()
{
    QString str=ui->editNum->text();
    int num=str.toInt();
    str=ui->editPrice->text();
    float price=str.toFloat();
    float total=num*price;
    str=str.sprintf("%.3f",total);
    ui->editTotal->setText(str);
}

void Widget::on_pushButton_clicked()
{
  QString str=ui->editDec->text();
  int val=str.toInt();
  str=str.setNum(val,16).toUpper();

  ui->editHex->setText(str);
  str=str.setNum(val,2);
  ui->editBin->setText(str);
}

void Widget::on_pushButton_2_clicked()
{
    QString str=ui->editBin->text();
    bool ok;
    int val=str.toInt(&ok,2);

    str=str.setNum(val,10);
    ui->editDec->setText(str);

    str=str.setNum(val,16).toUpper();
    ui->editHex->setText(str);

}

void Widget::on_pushButton_3_clicked()
{
    bool ok;
    int val=ui->editHex->text().toInt(&ok,16);//以十六进制数读入
    QString str=QString::number(val,10);//显示为10进制字符串
    ui->editDec->setText(str);//显示为10进制字符串
    str=str.setNum(val,2);//显示为2进制字符串
    ui->editBin->setText(str);//显示二进制字符串

}
