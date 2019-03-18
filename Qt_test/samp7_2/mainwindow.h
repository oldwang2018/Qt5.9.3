#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include    <QMainWindow>
#include    <QLabel>
#include    <QStandardItemModel>
#include    <QItemSelectionModel>

#include    "qwintspindelegate.h"
#include    "qwfloatspindelegate.h"
#include    "qwcomboboxdelegate.h"

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
//用于状态栏的信息显示
//    QLabel  *LabCurFile;  //当前文件
    QLabel  *LabCellPos;    //当前单元格行列号
    QLabel  *LabCellText;   //当前单元格内容
    QWIntSpinDelegate    intSpinDelegate; //整型数
    QWFloatSpinDelegate  floatSpinDelegate; //浮点数
    QWComboBoxDelegate   comboBoxDelegate; //列表选择
//    QString fCurFile;//当前文件名
    QStandardItemModel  *theModel;//数据模型
    QItemSelectionModel *theSelection;//Item选择模型
//    void    iniModelFromStringList(QStringList&);//从StringList初始化数据模型
    void    resetTable(int aRowCount);  //表格复位，设定行数
    bool    saveDataAsStream(QString& aFileName);//将数据保存为数据流文件
    bool    openDataAsStream(QString& aFileName);//读取数据流文件
    bool    saveBinaryFile(QString& aFileName);//保存为二进制文件
    bool    openBinaryFile(QString& aFileName);//打开二进制文件

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
