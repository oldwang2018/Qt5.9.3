#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QStringList theStrList;
    theStrList<<"北京"<<"上海"<<"天津"<<"河北"<<"山东"<<"四川"<<"重庆";
    theModel=new QStringListModel(this);
    theModel->setStringList(theStrList);

    ui->listView->setModel(theModel);
    ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked|
                                  QAbstractItemView::SelectedClicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btnListAppend_clicked()
{
    theModel->insertRow(theModel->rowCount());
    QModelIndex index=theModel->index(theModel->rowCount()-1,0);
    theModel->setData(index,"new item",Qt::DisplayRole);
    ui->listView->setCurrentIndex(index);
}

void Widget::on_btnListInsert_clicked()
{
    QModelIndex index=ui->listView->currentIndex();
    theModel->insertRow(index.row());
    theModel->setData(index,"inserted item",Qt::DisplayRole);
    ui->listView->setCurrentIndex(index);
}

void Widget::on_btnListDelete_clicked()
{
    QModelIndex index=ui->listView->currentIndex();
    theModel->removeRow(index.row());
}

void Widget::on_btnListClear_clicked()
{
    theModel->removeRows(0,theModel->rowCount());
}

void Widget::on_btnTextImport_clicked()
{
    QStringList strList=theModel->stringList();
    ui->plainTextEdit->clear();
    foreach (QString str, strList) {
        ui->plainTextEdit->appendPlainText(str);
    }
}

void Widget::on_listView_clicked(const QModelIndex &index)
{
   // ui->label->setText(QString("Row=%d, Column=%d").arg(index.row()).arg(index.column()));
    ui->label->setText(QString::asprintf("Row=%d, Column=%d",index.row(),index.column()));
    //ui->plainTextEdit->appendPlainText( QString::asprintf("Row=%d, Column=%d",index.row(),index.column()));
}
