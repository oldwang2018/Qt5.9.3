#ifndef QMYWIDGET_H
#define QMYWIDGET_H

#include <QWidget>
#include    "qperson.h"

namespace Ui {
class QmyWidget;
}

class QmyWidget : public QWidget
{
    Q_OBJECT

private:
    QPerson *boy;
    QPerson *girl;

public:
    explicit QmyWidget(QWidget *parent = 0);
    ~QmyWidget();

private:
    Ui::QmyWidget *ui;

signals:

private slots:
//自定义槽函数
    void   on_ageChanged(int  value);
//界面按钮的槽函数
    void on_btnClear_clicked();
    void on_btnBoyInc_clicked();
    void on_btnGirlInc_clicked();
    void on_btnClassInfo_clicked();

    void on_spinBoy_valueChanged(int arg1);
    void on_spinGirl_valueChanged(int arg1);
};

#endif // QMYWIDGET_H
