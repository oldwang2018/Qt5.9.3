#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QSplitter>
#include<QDebug>
#include<QStatusBar>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scatter=new Scatter();
    this->setCentralWidget(scatter);
    QSplitter *splitter=new QSplitter(Qt::Horizontal);
    splitter->addWidget(ui->groupBox);
    splitter->addWidget(scatter);
    this->setCentralWidget(splitter);

    ui->timeBar->setMinimumWidth(550);
    ui->timeLabel->setMinimumWidth(120);
    ui->statusBar->addWidget(ui->timeBar);
    ui->statusBar->addWidget(ui->timeLabel);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnShow_clicked()
{
    Vector_pos<<QVector3D(0,0,0);
    Vector_pos<<QVector3D(50,50,50);
    Vector_pos<<QVector3D(100,100,100);
    scatter->show(Vector_pos);
}
