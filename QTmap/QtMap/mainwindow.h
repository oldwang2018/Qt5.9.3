#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QWebEngineView>
//#include<QtWebEngineWidgets/QWebEngineView>
#include <QWebChannel>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QWebEngineView *m_view;
    QWebChannel *channel;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_pushButton_clicked();
    void on_btnRunCmd_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

