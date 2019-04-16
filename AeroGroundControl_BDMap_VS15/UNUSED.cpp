void MainWindow::on_btnUAVStatus_clicked()
{
    int n=ui->spinUAVNumber->value();
    UAV[n]->get_status();
//    qDebug()<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";
//    QMap<QTime,QJsonObject>::iterator it;
//    for ( it = UAV[0]->status_map.begin(); it != UAV[0]->status_map.end(); ++it )
//    {
//        QString strLong=QString::number(it.value().value("long").toDouble(),'f',8);
//        qDebug()<<it.key().toString()+" ：" +strLong+" "+it.value().value("mode").toString();
//    }
//    qDebug()<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";
}

QWebEngineView *m_view;
setCentralWidget(m_view);
QWebEnginePage *page = m_view->page();
//    QString abs="C:/Users/WangYu/Desktop/UAV_Formation_Ground_Station-master/Ground_Station/map/test.html";
//    QString filePath = "file:///" + abs;
page->load(QUrl(filePath));//load the map in relevant path

QVector<QVector3D> tmp;
tmp.append(QVector3D(0,50,80));
tmp.append(QVector3D(-30,50,50));
tmp.append(QVector3D(30,50,50));

tmp.append(QVector3D(-20,50,-10));
tmp.append(QVector3D(20,50,-10));
tmp.append(QVector3D(-80,50,-10));
tmp.append(QVector3D(80,50,-10));
scatter->show(tmp);
