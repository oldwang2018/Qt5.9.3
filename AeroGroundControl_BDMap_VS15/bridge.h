#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include <QJSValue>
#include <functional>
#include <QVariant>

class bridge : QObject
{
Q_OBJECT
public slots:
    void getCoordinate(QString lon,QString lat);
signals:
    void sendDate(double lon,double lat);


public slots:  //备用
    void getYourName();
signals:
    void sigYourName(const QString &name);
public:
    bridge();
};

#endif // BRIDGE_H
