#include "bridge.h"
#include <QMessageBox>
#include <QJSEngine>
#include <QDebug>

bridge* bridge::instance()
{
    static bridge s_obj;
    return &s_obj;
}

bridge::bridge()
{
}

void bridge::showMsgBox()
{
    QMessageBox::aboutQt(0, tr("Qt"));
}

void bridge::getYourName()
{
    emit sigYourName("hello, world!");
}

void bridge::getCoordinate(QString lon, QString lat)
{
    qDebug()<<lon<<lat;

}
