#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include <QJSValue>
#include <functional>
#include <QVariant>

class bridge : QObject
{
Q_OBJECT
public:
    bridge();
signals:
    void sigYourName(const QString &name);
public slots:
    void getYourName();

    void getCoordinate(QString lon,QString lat);
};

#endif // BRIDGE_H
