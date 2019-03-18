#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QCoreApplication>
#include<QFileDialog>
#include<QFile>
#include<QTextStream>
#include<QFont>
#include<QDebug>
//删除select怎么办？
void MainWindow::iniModelFromStringList(QStringList &aFileContent)
{
    int rowCnt=aFileContent.count();//文本行数
    theModel->setRowCount(rowCnt-1);

    QString header=aFileContent.at(0);
    QStringList headerList=header.split(QRegExp("\\s+"),QString::SkipEmptyParts); //？？？？？
    theModel->setHorizontalHeaderLabels(headerList);//设置表头文字

    for(int i=1;i<rowCnt;i++)//???????????  0-rowcnt-1
    {
        QString aLineText=aFileContent.at(i);
        // QStringList tmpList=aLineText.split(QRegExp("\\s+"),QString::SkipEmptyParts); //？？？？？
        QStringList tmpList=aLineText.split(QRegExp("\\s+"),QString::SkipEmptyParts); //？？？？？

        int j;
        QStandardItem *aItem;
        for(j=0;j<FixedColumnCount-1;j++)
        {//不包含最后一列
            aItem=new QStandardItem(tmpList[j]);//???????
            theModel->setItem(i-1,j,aItem);
        }
        aItem=new QStandardItem(headerList.at(j));
        aItem->setCheckable(true);
        if(tmpList[j]=="0")
            aItem->setCheckState(Qt::Unchecked);
        else
            aItem->setCheckState(Qt::Checked);
        theModel->setItem(i-1,j,aItem);
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    theModel=new QStandardItemModel(2,FixedColumnCount,this);
    theSelection=new QItemSelectionModel(theModel);
    connect(theSelection,SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(on_currentChanged(QModelIndex,QModelIndex)));

    ui->tableView->setModel(theModel);
    ui->tableView->setSelectionModel(theSelection);

    ui->tableView->setItemDelegateForColumn(0,&intSpinDelegate);
    ui->tableView->setItemDelegateForColumn(1,&floatSpinDelegate);
    ui->tableView->setItemDelegateForColumn(2,&floatSpinDelegate);
    ui->tableView->setItemDelegateForColumn(3,&floatSpinDelegate);
    ui->tableView->setItemDelegateForColumn(4,&comboBoxDelegate);

    LabCurFile = new QLabel("当前文件：",this);
    LabCurFile->setMinimumWidth(200);
    LabCellPos = new QLabel("当前单元格：",this);
    LabCellPos->setMinimumWidth(180);
    LabCellPos->setAlignment(Qt::AlignHCenter);
    LabCellText = new QLabel("单元格内容：",this);
    LabCellText->setMinimumWidth(150);

    ui->statusBar->addWidget(LabCurFile);
    ui->statusBar->addWidget(LabCellPos);
    ui->statusBar->addWidget(LabCellText);

    // setCentralWidget(ui->splitter); //
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if(current.isValid())
    {
        LabCellPos->setText(QString::asprintf("当前单元格：%d行,%d列",
                                              current.row(),current.column()));
        QStandardItem *aItem=theModel->itemFromIndex(current);
        this->LabCellText->setText("单元格内容："+aItem->text());
        QFont font=aItem->font();
        ui->actFontBold->setChecked(font.bold());
    }
}

void MainWindow::on_actOpen_triggered()
{
    QString curPath=QCoreApplication::applicationDirPath();
    QString aFileName=QFileDialog::getOpenFileName(this,"open a file",curPath);
    if(aFileName.isEmpty())
        return;

    QStringList fFileContent ;
    QFile aFile(aFileName);
    if(aFile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream aStream(&aFile);
        ui->plainTextEdit->clear();
        while(!aStream.atEnd())
        {
            QString str= aStream.readLine();
            ui->plainTextEdit->appendPlainText(str);
            fFileContent.append(str);
            //fFilecontent<<str;
        }
        aFile.close();
        this->LabCurFile->setText("当前文件："+aFileName);
        iniModelFromStringList(fFileContent);
    }
}

void MainWindow::on_actAppend_triggered()
{
    QList<QStandardItem*> aItemList;
    QStandardItem *aItem;
    for(int i=0;i<FixedColumnCount-1;i++)
    {
        aItem=new QStandardItem("0");
        aItemList<<aItem;
    }
    //获取最后一列的表头文字
    QString str=theModel->headerData(theModel->columnCount()-1,Qt::Horizontal
                                     ,Qt::DisplayRole).toString();
    aItem=new QStandardItem(str);//创建  测井取样 item
    aItem->setCheckable(true);
    aItemList<<aItem; //最后一列
    theModel->insertRow(theModel->rowCount(),aItemList); //插入一行，需要每个Cell的Item

    QModelIndex curIndex=theModel->index(theModel->rowCount()-1,0);//创建最后一行的ModelIndex
    theSelection->clearSelection();//清空选择项
    theSelection->setCurrentIndex(curIndex,QItemSelectionModel::Select);//设置刚插入的行为当前选择行
    //???????????
}

void MainWindow::on_actInsert_triggered()
{
    QList<QStandardItem*> aItemList;
    QStandardItem *aItem;
    for(int i=0;i<FixedColumnCount-1;i++)
    {
        aItem=new QStandardItem("0");
        aItemList<<aItem;
    }
    //获取最后一列的表头文字
    QString str=theModel->headerData(theModel->columnCount()-1,Qt::Horizontal
                                            ,Qt::DisplayRole).toString();
    aItem=new QStandardItem(str);//创建  测井取样 item
    aItem->setCheckable(true);
    aItemList<<aItem; //最后一列

    QModelIndex curIndex=theSelection->currentIndex(); //获取当前选中项的模型索引
    theModel->insertRow(curIndex.row(),aItemList);  //在当前行的前面插入一行
    theSelection->clearSelection();//清除已有选择
    theSelection->setCurrentIndex(curIndex,QItemSelectionModel::Select);

}

void MainWindow::on_actDelete_triggered()
{
    theModel->removeRow(theSelection->currentIndex().row());
   //theModel->removeRow(ui->tableView->currentIndex().row());
}

void MainWindow::on_actAlignLeft_triggered()
{
    if(!theSelection->hasSelection())
        return;
    QModelIndexList selectIndex=theSelection->selectedIndexes();
    for(int i=0;i<selectIndex.count();i++)
    {
        QModelIndex aIndex=selectIndex[i];
        QStandardItem *aItem=theModel->itemFromIndex(aIndex);
        aItem->setTextAlignment(Qt::AlignLeft);
    }
}
void MainWindow::on_actAlignCenter_triggered()
{
    if(!theSelection->hasSelection())
        return;
    QModelIndexList selectIndex=theSelection->selectedIndexes();
    for(int i=0;i<selectIndex.count();i++)
    {
        QModelIndex aIndex=selectIndex[i];
        QStandardItem *aItem=theModel->itemFromIndex(aIndex);
        aItem->setTextAlignment(Qt::AlignHCenter);
    }
}
void MainWindow::on_actAlignRight_triggered()
{

    if(!theSelection->hasSelection())
        return;
    QModelIndexList selectIndex=theSelection->selectedIndexes();
    for(int i=0;i<selectIndex.count();i++)
    {
        QModelIndex aIndex=selectIndex[i];
        QStandardItem *aItem=theModel->itemFromIndex(aIndex);
        aItem->setTextAlignment(Qt::AlignRight);
    }
}

void MainWindow::on_actFontBold_triggered(bool checked)
{
    if(!theSelection->hasSelection())
        return;
    QModelIndexList selectedIndex=theSelection->selectedIndexes();
    for(int i=0;i<selectedIndex.count();i++)
    {
        QModelIndex aIndex=selectedIndex[i];
        QStandardItem *aItem=theModel->itemFromIndex(aIndex);
        QFont font=aItem->font();
        font.setBold(checked);
        aItem->setFont(font);
    }
}

void MainWindow::on_actSave_triggered()
{
    QString aFileName=QFileDialog::getSaveFileName(this,"save a file");
    if(aFileName.isEmpty())
        return;
    QFile aFile(aFileName);
    if(!(aFile.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Truncate)))
        return ; //以读写、覆盖原有内容方式打开文件

    QTextStream aStream(&aFile);
    QStandardItem *aItem;
    QString str;
    ui->plainTextEdit->clear();
    for(int i=0;i<theModel->columnCount();i++)
    {//获取表头文字
        aItem=theModel->horizontalHeaderItem(i);
        str+=aItem->text()+"\t";
    }
    aStream<<str<<"\n";
    ui->plainTextEdit->appendPlainText(str);

    for(int i=0;i<theModel->rowCount();i++)
    {//获取数据区文字
        str="";
        int j;
        for(j=0;j<theModel->columnCount()-1;j++)
        {
            aItem=theModel->item(i,j);
            str+=aItem->text()+"\t";
        }
        aItem=theModel->item(i,j);
        if(aItem->checkState()==Qt::Checked)
            str+="1";
        else
            str+="0";
        ui->plainTextEdit->appendPlainText(str);
        aStream<<str<<"\n";
    }

}

void MainWindow::on_actModelData_triggered()
{//模型数据导出到PlainTextEdit显示
    ui->plainTextEdit->clear(); //清空
    QStandardItem   *aItem;
    QString str;

//获取表头文字
    int i,j;
    for (i=0;i<theModel->columnCount();i++)
    { //
        aItem=theModel->horizontalHeaderItem(i); //获取表头的一个项数据
        str=str+aItem->text()+"\t"; //用TAB间隔文字
    }
    ui->plainTextEdit->appendPlainText(str); //添加为文本框的一行

//获取数据区的每行
    for (i=0;i<theModel->rowCount();i++)
    {
        str="";
        for(j=0;j<theModel->columnCount()-1;j++)
        {
            aItem=theModel->item(i,j);
            str=str+aItem->text()+QString::asprintf("\t"); //以 TAB分隔
        }

        aItem=theModel->item(i,j); //最后一行是逻辑型
        if (aItem->checkState()==Qt::Checked)
            str=str+"1";
        else
            str=str+"0";

         ui->plainTextEdit->appendPlainText(str);
    }
}

