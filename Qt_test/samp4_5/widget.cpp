#include "widget.h"
#include "ui_widget.h"
#include    <QTextDocument>
#include    <QTextBlock>
#include    <QMenu>

#include    <QMap>
#include    <QVariant>
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

void Widget::on_pushButton_clicked()
{
    QIcon icon;
    icon.addFile(":/images/icons/aim.ico");
    ui->comboBox->clear();
    for(int i=0;i<20;i++){
        ui->comboBox->addItem(icon,QString::asprintf("Item %d",i));
    }
}

void Widget::on_pushButton_2_clicked()
{
        QMap<QString, int> City_Zone;
        City_Zone.insert("北京",10);
        City_Zone.insert("上海",21);
        City_Zone.insert("天津",22);
        City_Zone.insert("大连",411);
        City_Zone.insert("锦州",416);
        City_Zone.insert("徐州",516);
        City_Zone.insert("福州",591);
        City_Zone.insert("青岛",532);
        ui->comboBox_2->clear();
       foreach(const QString str,City_Zone.keys())
           ui->comboBox_2->addItem(str,City_Zone.value(str));


}

void Widget::on_pushButton_3_clicked()
{
     ui->comboBox->clear();
     ui->comboBox_2->clear();
}

void Widget::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->plainTextEdit->appendPlainText(arg1);
}

void Widget::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    QString zone=ui->comboBox_2->currentData().toString();
    ui->plainTextEdit->appendPlainText(arg1+":区号="+zone);
}

void Widget::on_pushButton_4_clicked()
{
        QTextDocument*  doc=ui->plainTextEdit->document(); //QPlainTextEdit 的内容保存在一个 QTextDocument 里
        int cnt=doc->blockCount();//QTextDocument分块保存内容，文本文件就是硬回车符是一个block,
        QIcon icon(":/images/icons/aim.ico");
        ui->comboBox->clear();  //清除条目
        for (int i=0; i<cnt;i++) //扫描所有 blobk
        {
             QTextBlock textLine=doc->findBlockByNumber(i);//用blobk编号获取block，就是获取一行
             QString str=textLine.text(); //转换为文本
             ui->comboBox->addItem(icon,str); //添加一个条目到comboBox
        }

}

void Widget::on_plainTextEdit_customContextMenuRequested(const QPoint &pos)
{
    QMenu *mene =ui->plainTextEdit->createStandardContextMenu();
    mene->exec(pos);
}

void Widget::on_checkBox_clicked(bool checked)
{
      ui->plainTextEdit->setReadOnly(checked);
}
