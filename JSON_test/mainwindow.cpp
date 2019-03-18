#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QJsonDocument>
#include<QJsonObject>
#include<QFile>
#include<QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

     QJsonObject json;
      json.insert("Name", "Qt");
      json.insert("From", 1991);
      json.insert("Cross Platform", true);
      QJsonDocument document(json);
      QByteArray byteArray = document.toJson();  //QJsonDocument::Compact  压缩不换行
      QFile loadFile("/home/wangyu/桌面/2.json");
      if(!loadFile.open(QIODevice::ReadWrite|QIODevice::Text))
      {
          qDebug() << "could't open projects json";
          return;
      }
      QTextStream aStream(&loadFile);
      aStream<<document.toJson();
     // loadFile.close();
      /*************************保存文件*********************************************/



      QFile readFile("/home/wangyu/桌面/2.json");
      if(!readFile.open(QIODevice::ReadWrite|QIODevice::Text))
      {
          qDebug() << "could't open projects json  read";
          return;
      }
      QByteArray byteArray2=readFile.readAll(); // qDebug()<<byteArray2;
      QJsonParseError jsonError;
      QJsonDocument doucment = QJsonDocument::fromJson(byteArray2, &jsonError);  // 转化为 JSON 文档
      if (doucment.isNull() || (jsonError.error != QJsonParseError::NoError))
      {qDebug() <<" 解析发生错误1";
        return;
      }
      if (!doucment.isObject())
      {qDebug() <<"非JSON 文档";
        return;
      }
      QJsonObject object = doucment.object();  // 转化为对象

       QJsonObject json2;
       json2.insert("10:0:0", object);
       QJsonDocument document2(json2);
       QFile loadFile2("/home/wangyu/桌面/3.json");
       if(!loadFile2.open(QIODevice::ReadWrite|QIODevice::Text))
       {
           qDebug() << "could't open projects json";
           return;
       }
       QTextStream aStream2(&loadFile2);
       aStream2<<document2.toJson(QJsonDocument::Compact);
       //loadFile2.close();
       /*******************读取重新保存*********************************/
       QFile afile("/home/wangyu/桌面/3.json");
       if(!afile.open(QIODevice::ReadWrite|QIODevice::Text))
       {
           qDebug() << "could't open projects json  read";
           return;
       }
       QByteArray byteArray0=afile.readAll(); // qDebug()<<byteArray2;
       qDebug()<<byteArray0;
       QJsonParseError jsonErro;
       QJsonDocument doc = QJsonDocument::fromJson(byteArray0, &jsonErro);  // 转化为 JSON 文档
       if (doc.isNull() || (jsonErro.error != QJsonParseError::NoError))
       {qDebug() <<" 解析发生错误2";
         return;
       }
       if (!doucment.isObject())
       {qDebug() <<"非JSON 文档";
         return;
       }
       QJsonObject object00 = doc.object();  // 转化为对象
       QJsonObject subObj=object00.value("10:0:0").toObject();
       qDebug()<<subObj.value("Name");
}

MainWindow::~MainWindow()
{
    delete ui;



}
