#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QListWidgetItem>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actListInit_triggered();

    void on_actListInsert_triggered();

    void on_actListDelete_triggered();

    void on_actListClear_triggered();

    void on_actListAppend_triggered();

    void on_actSelALL_triggered();

    void on_actSelNone_triggered();

    void on_actSelInvs_triggered();

    void on_actSelPopMenu_triggered();


    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_chkBoxListEditable_clicked(bool checked);

    void on_listWidget_customContextMenuRequested(const QPoint &pos);

    void on_toolBox_currentChanged(int index);

    void on_toolBox_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindow *ui;
    void  setActionsForButton();
    void createSelectionPopMenu();
};

#endif // MAINWINDOW_H
