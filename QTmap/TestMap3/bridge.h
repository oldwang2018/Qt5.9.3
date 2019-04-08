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
    static bridge* instance();

signals:
    void sigYourName(const QString &name);

public slots:
    void showMsgBox();
    void getYourName();
    void getCoordinate(QString lon,QString lat);
private:
    bridge();
};

#endif // BRIDGE_H
