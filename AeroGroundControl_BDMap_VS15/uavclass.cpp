#include "uavclass.h"
#include<QFile>
#include<QDebug>

#include<QJsonParseError>
#include<QJsonDocument>
#include<QJsonObject>
//#include<QJsonValue>
UAVClass::UAVClass(QString address,int n)
{
    this->address=address;
    this->ID=n;
    NetworkManager = new QNetworkAccessManager();
    connect(NetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_replayFinished(QNetworkReply*)));
}

void UAVClass::slot_replayFinished(QNetworkReply *reply )
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QString redirectionURL = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();
    emit UAV_debug_signal(Info,"statusCode: "+
                          QString::number(statusCode)+" -- "+redirectionURL);
    /*****************************
     * case 200   ok
     * case 302   http://192.168.0.14:5000/connect   redirection
     * case 400:      bad request
     * case 403:      permission denied
     * **************************/
    if(statusCode==302 && redirectionURL.contains("http"))
     {    
       QUrl url(redirectionURL);
       NetworkManager->get(QNetworkRequest(url));
       return ;
     }
/********************************************************************/
     if (reply->error() != QNetworkReply::NoError)
     {
        emit UAV_debug_signal(Info,"QNetworkReply::Error !");
        reply->deleteLater();
        return ;
     }
     QString url=reply->url().toString();

     QString message="from URL: "+url+"--> ";
     if(url.contains("connect"))
     {
         message+="connect mode";
     }
     if(url.contains("status"))
     {
         QByteArray bytes = reply->readAll();
         ByteArray_To_Json(bytes);
         emit UAV_debug_signal(JSONinfo, QString::fromUtf8(bytes).trimmed());
         message=message+"status mode\n"+"message  processed\n";
     }
     if(url.contains("land"))
     {
         QByteArray bytes = reply->readAll();
         message=message+"land mode\n"+QString::fromUtf8(bytes);
     }
     if(url.contains("takeoff"))
     {
         QByteArray bytes = reply->readAll();
         message=message+"takeoff mode\n"+QString::fromUtf8(bytes);
     }
     if(url.contains("goto"))
     {
         QByteArray bytes = reply->readAll();
         message=message+"goto mode\n"+QString::fromUtf8(bytes);
     }
     emit UAV_debug_signal(Info,message);
     reply->deleteLater();
}

void UAVClass::ByteArray_To_Json(QByteArray bytes)
{
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(bytes, &jsonError);  // 转化为 JSON 文档
    if (doucment.isNull() || (jsonError.error != QJsonParseError::NoError))
    {
      emit UAV_debug_signal(Info,"JSON解析发生错误");
      return;
    }
    if (!doucment.isObject())
    {
      emit UAV_debug_signal(Info,"非JSON文档");
      return;
    }
    QJsonObject object = doucment.object();  // 转化为对象
//    status_map.insert(QTime::currentTime(),object);
    status_map[QTime::currentTime()]=object;

    if (object.contains("alt"))
    {  // 包含指定的 key
        QJsonValue value = object.value("alt");
        if (value.isDouble()) {
            double alt_value = value.toDouble();
           qDebug()<<("alt_value :: "+QString::number(alt_value,'f',2));
        }
    }
    if (object.contains("armed"))
    {
        QJsonValue value = object.value("armed");
        if (value.isBool()) {
            bool armed_value = value.toBool();
           qDebug()<<("armed_value :: "+QString::number(armed_value));
        }
    }
    if (object.contains("connect"))
    {
        QJsonValue value = object.value("connect");
        if (value.isBool()) {
            bool connect_value = value.toBool();
           qDebug()<<("connect_value :: "+QString::number(connect_value));
        }
    }
    if (object.contains("lat"))
    {
        QJsonValue value = object.value("lat");
        if (value.isDouble()) {
            double lat_value = value.toDouble();
            qDebug()<<("lat_value :: "+QString::number(lat_value,'f',8));
        }
    }
    if (object.contains("long"))
    {
        QJsonValue value = object.value("long");
        if (value.isDouble()) {
            double long_value = value.toDouble();
            qDebug()<<("long_value :: "+QString::number(long_value,'f',8));
        }
    }
    if (object.contains("mode"))
    {
        QJsonValue value = object.value("mode");
        if (value.isString()) {
            QString mode_value = value.toString(); ;
           qDebug()<<("mode_value :: "+mode_value);
        }
    }
    if (object.contains("pitch"))
    {
        QJsonValue value = object.value("pitch");
        if (value.isDouble()) {
            double pitch_value = value.toDouble();
          qDebug()<<("pitch_value :: "+QString::number(pitch_value,'f',16));
        }
    }
    if (object.contains("roll"))
    {
        QJsonValue value = object.value("roll");
        if (value.isDouble()) {
            double roll_value = value.toDouble();
           qDebug()<<("roll_value :: "+QString::number(roll_value,'f',16));
        }
    }
    if (object.contains("yaw"))
    {
        QJsonValue value = object.value("yaw");
        if (value.isDouble()) {
            double yaw_value = value.toDouble();
           qDebug()<<("yaw_value :: "+QString::number(yaw_value,'f',16));
        }
        emit UAV_debug_signal(Info,"JSON解析成功");
    }
}

void UAVClass::command(QString str)
{
    QUrl url("http://"+address+":5000/"+str);
    QNetworkReply* reply = NetworkManager->get(QNetworkRequest(url));
    Q_UNUSED(reply);
    emit UAV_debug_signal(Command,"command -->  "+ url.toString());
}

void UAVClass::UAVconnect()
{
    command("connect");
}

void UAVClass::get_status()
{
    command("status");
}
void UAVClass::land()
{
    command("land");
}

void UAVClass::takeoff()
{
   command("takeoff");
}
void UAVClass::go_to(float N, float E, float D)
{//http://192.168.0.14:5000/goto?n=3&e=4&d=-3
    QString str=QString("goto?n=%1&e=%2&d=%3").arg(N).arg(E).arg(D);
    command(str);
}

QString UAVClass::get_address()
{
    return address;
}
////