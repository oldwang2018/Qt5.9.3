#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    s=new myopenGL(this);//this
    setCentralWidget(s);
    s->setMinimumSize(400,400);
}

MainWindow::~MainWindow()
{

}
