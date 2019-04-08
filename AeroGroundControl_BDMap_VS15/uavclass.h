#ifndef UAVCLASS_H
#define UAVCLASS_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtWidgets/QWidget>
#include <QTextCodec>
#include <QString>
#include <QUrl>
#include<QMap>
#include<QJsonObject>
#include<QTime>
enum eventType{
    Info,
    JSONinfo,
    QNetworkReplyError,
    JSONerror,
    Command
};
class UAVClass:public QObject
{
    Q_OBJECT
public:
    void command(QString str);
    void UAVconnect();
    void get_status();
    void takeoff();
    void land();
    void go_to(float N,float E,float D);
    int ID=0;
    QString get_address();
    QMap<QTime,QJsonObject> status_map;

private:
    QString address;
    QNetworkAccessManager *NetworkManager;
    void ByteArray_To_Json(QByteArray str);
private slots:
    void slot_replayFinished(QNetworkReply *reply);

signals:
    void UAV_debug_signal(eventType type,QString message);

public:
    UAVClass(QString address, int n);
};

#endif // UAVCLASS_H
