#include "bridge.h"
#include <QMessageBox>
#include <QJSEngine>
#include <QDebug>



bridge::bridge()
{
}

void bridge::getYourName()
{
    emit sigYourName("hello, world!");
}

void bridge::getCoordinate(QString lon, QString lat)
{
    qDebug()<<lon<<lat;
}
