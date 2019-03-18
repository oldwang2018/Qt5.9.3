#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"myopengl.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    myopenGL *s;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
