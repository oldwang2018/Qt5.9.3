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
    //qDebug()<<lon<<lat;
    double j=lon.toDouble();
    double w=lat.toDouble();
    emit sendDate(j,w);
}
