#ifndef BAIDUMAP_H
#define BAIDUMAP_H

#include <QWidget>
#include<QtWebEngineWidgets/QWebEngineView>
#include <QWebChannel>
namespace Ui {
class BaiduMap;
}

class BaiduMap : public QWidget
{
    Q_OBJECT
public:
    QWebEngineView *m_view;
    QWebChannel *channel;
public:
    explicit BaiduMap(QWidget *parent = 0);
    ~BaiduMap();

private slots:
    void on_btnAddMarker_clicked();
    void revCoordinate(double j,double w);


private:
    Ui::BaiduMap *ui;
};

#endif // BAIDUMAP_H
