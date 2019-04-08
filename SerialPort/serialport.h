#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QMainWindow>
#include<QLabel>
#include<QSerialPort>
#include<QTimer>
namespace Ui {
class SerialPort;
}

class SerialPort : public QMainWindow
{
    Q_OBJECT
private:

    QSerialPort *serial;
    QTimer *timer;
    void serialport_init();
    int rxCount=0,txCount=0;
    QLabel *portStateLabel;
    void show_portStateLabel();


public:
    explicit SerialPort(QWidget *parent = 0);
    ~SerialPort();
private slots:
    void slot_read_from_port();
    void on_btnReloadPort_clicked();

    void on_btnOpenPort_clicked();

    void on_btnClosePort_clicked();

    void on_btnSendData_clicked();

    void on_chk_auto_send_clicked(bool checked);

    void on_btnResetInfo_clicked();

    void on_btn_pause_rev_clicked();

private:
    Ui::SerialPort *ui;
     void closeEvent(QCloseEvent *event);
};

#endif // SERIALPORT_H
