#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QLabel>
#include<QStandardItemModel>
#include<QItemSelectionModel>


#include "qwintspindelegate.h"
#include "qwfloatspindelegate.h"
 #include "qwcomboboxdelegate.h"

#define     FixedColumnCount    6       //文件固定6列
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QLabel *LabCurFile;
    QLabel *LabCellPos;
    QLabel *LabCellText;
    QStandardItemModel *theModel;
    QItemSelectionModel *theSelection;
    void iniModelFromStringList(QStringList&);

private:
       QWIntSpinDelegate    intSpinDelegate; //整型数
       QWFloatSpinDelegate  floatSpinDelegate; //浮点数
       QWComboBoxDelegate   comboBoxDelegate; //列表选择

private slots:
    void on_currentChanged(const QModelIndex &current,const QModelIndex &previous);


    void on_actOpen_triggered();

    void on_actAppend_triggered();

    void on_actInsert_triggered();

    void on_actDelete_triggered();

    void on_actAlignLeft_triggered();

    void on_actAlignCenter_triggered();

    void on_actAlignRight_triggered();

    void on_actFontBold_triggered(bool checked);

    void on_actSave_triggered();

    void on_actModelData_triggered();


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
