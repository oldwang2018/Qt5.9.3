#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->timeBar->setMinimumWidth(600);
    ui->timeLabel->setMinimumWidth(150);
    ui->statusBar->addWidget(ui->timeBar);
    ui->statusBar->addWidget(ui->timeLabel);

    ui->textBrowser->setText("<!DOCTYPE html>\n<html lang=\"en\">\n\n<head>\n    <meta charset=\"UTF-8\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n    <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">\n    <title>Document</title>\n</head>\n\n<body>\n    <a href=\"/\">index</a>\n    <br>\n    <a href=\"/connect\">connect</a>\n    <br>\n    <a href=\"/takeoff\">takeoff</a>\n    <br>\n    <a href=\"/land\">land</a>\n    <br>\n    <a href=\"/goto\">goto</a>\n    <br>\n    <a href=\"/status\">status</a>\n</body>\n\n</html>");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_timeBar_valueChanged(int value)
{
      ui->timeLabel->setText(QString::asprintf(
                               "%d / %d",value,ui->timeBar->maximum()));
}
