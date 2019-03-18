#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include    <QtNetwork/QHostInfo>
#include    <QtNetwork/QNetworkInterface>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
private:
    QString protocolName(QAbstractSocket::NetworkLayerProtocol protocal);

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_btnGetHostInfo_clicked();

    void on_btnLookup_clicked();

        void lookedUpHostInfo(const QHostInfo &host);

    void on_btnALLInterface_clicked();

    void on_btnAllAddresses_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
