#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&threadA,SIGNAL(started()),this,SLOT(onthreadA_started()));
    connect(&threadA,SIGNAL(finished()),this,SLOT(onthreadA_finished()));
    connect(&threadA,SIGNAL(newValue(int,int)),this,SLOT(onthreadA_newValue(int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(threadA.isRunning())
    {
        threadA.stopThread();
        //threadA.quit();
        threadA.wait();
    }
    event->accept();
}

void MainWindow::onthreadA_started()
{
    ui->LabA->setText("开始线程");
}

void MainWindow::onthreadA_finished()
{
     ui->LabA->setText("结束线程");
}

void MainWindow::onthreadA_newValue(int seq, int diceValue)
{
    ui->plainTextEdit->appendPlainText(
                QString::asprintf("第 %d次筛子，： %d",seq,diceValue));
}
void MainWindow::on_btnStartTread_clicked()
{
    threadA.start();
}

void MainWindow::on_btnStopTread_clicked()
{
    threadA.stopThread();
    threadA.wait();
}

void MainWindow::on_btnBegin_clicked()
{
    threadA.diceBegin();
}

void MainWindow::on_btnPause_clicked()
{
    threadA.dicePause();
}
