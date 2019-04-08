#include "serialport.h"
#include "ui_serialport.h"
#include<QSerialPortInfo>
#include<QDebug>
#include<QButtonGroup>
#include<QTextCodec>
#include<QMessageBox>

void SerialPort::serialport_init()
{
    foreach(const QSerialPortInfo info,QSerialPortInfo::availablePorts())//可以不使用
    {
        ui->comboBox_port->addItem(info.portName());
    }
}

void SerialPort::show_portStateLabel()
{
    QString info=QString("RX^%0    TX^%1 Bytes").arg(rxCount).arg(txCount);
    portStateLabel->setText(info);
}
SerialPort::SerialPort(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SerialPort)
{
    ui->setupUi(this);

    portStateLabel=new QLabel("RX ----  TX ----");
    portStateLabel->setContentsMargins(20,0,20,0);
    portStateLabel->setMaximumWidth(1000);
    ui->statusBar->addWidget(portStateLabel);

    QButtonGroup *btnGroupRev=new QButtonGroup(this);
    btnGroupRev->addButton(ui->radio_accept_ascii,0);
    btnGroupRev->addButton(ui->radio_accept_hex,1);
    ui->radio_accept_hex->setChecked(true);
    QButtonGroup *btnGroupSend=new QButtonGroup(this);
    btnGroupSend->addButton(ui->radio_send_ascii,0);
    btnGroupSend->addButton(ui->radio_send_hex,1);
    ui->radio_send_hex->setChecked(true);

    timer=new QTimer(this);
    timer->stop();
    timer->setInterval(ui->spinBox_timer->value());
    connect(timer,&QTimer::timeout, this,[=](){
        if(serial->isOpen()) on_btnSendData_clicked();});

    serial=new  QSerialPort(this);
    serialport_init();
    connect(serial,SIGNAL(readyRead()),this,SLOT(slot_read_from_port()));
}

SerialPort::~SerialPort()
{
    delete ui;
}

void SerialPort::closeEvent(QCloseEvent *event)
{
    if(serial->isOpen()){
        int  result;
        result=QMessageBox::question(this,"提示","端口已打开，确认退出？",
                   QMessageBox::Yes|QMessageBox::No);
        if(result==QMessageBox::Yes)
        {
            serial->close();
            event->accept();
        }else{
            event->ignore();
        }
    }
}
void SerialPort::on_btnReloadPort_clicked()
{
    ui->comboBox_port->clear();
    serialport_init();
}
void SerialPort::on_btnOpenPort_clicked()
{
    serial->setPortName(ui->comboBox_port->currentText());
    if (serial->open(QIODevice::ReadWrite))
    {
        qint32 baudrate=ui->comboBox_baud->currentText().toInt();
        serial->setBaudRate(baudrate);
        switch(ui->comboBox_databit->currentText().toInt())
        {
            case 8:
                serial->setDataBits(QSerialPort::Data8);  break;
            case 7:
                serial->setDataBits(QSerialPort::Data7);  break;
            case 6:
                serial->setDataBits(QSerialPort::Data6);  break;
            case 5:
                serial->setDataBits(QSerialPort::Data5);  break;
            default : qDebug()<<"default"<<"databit";
                break;
        }
        switch (ui->comboBox_checkbit->currentIndex()) {
            case 0:
                serial->setParity(QSerialPort::NoParity);   break;
            case 1: //odd
                serial->setParity(QSerialPort::OddParity);  break;
            case 2: //even
                serial->setParity(QSerialPort::EvenParity); break;
            case 3:
                serial->setParity(QSerialPort::SpaceParity);break;
            case 4:
                serial->setParity(QSerialPort::MarkParity); break;
            default:    qDebug()<<"default"<<"checkbit";
                break;
        }
        switch(ui->comboBox_stopbit->currentText().toInt())
        {
            case 1:
                serial->setStopBits(QSerialPort::OneStop); break;
            case 2:
                serial->setStopBits(QSerialPort::TwoStop); break;
            default: qDebug()<<"default"<<"stopbit";
                break;
        }
        switch(ui->comboBox_flow->currentIndex())
        {
            case 0:
                serial->setFlowControl(QSerialPort::NoFlowControl);break;
            case 1:
                serial->setFlowControl(QSerialPort::HardwareControl);break;
            case 2:
                serial->setFlowControl(QSerialPort::SoftwareControl);break;
            default: qDebug()<<"default"<<"flow";
                break;
        }
        ui->btnOpenPort->setEnabled(false);
        ui->btnClosePort->setEnabled(true);
        ui->label_com_baud->setText(QString(" %0  %1").
                 arg(ui->comboBox_port->currentText()).
                arg(ui->comboBox_baud->currentText()));
     }
    else
    {
        ui->label_com_baud->setText(QString("Open Failed").
                   arg(ui->comboBox_port->currentText()));
    }
}
void SerialPort::on_btnClosePort_clicked()
{
    serial->close();
    ui->btnOpenPort->setEnabled(true);
    ui->btnClosePort->setEnabled(false);
    ui->label_com_baud->setText("        **********");
}
void SerialPort::on_btnResetInfo_clicked()
{
    txCount=0; rxCount=0;
    ui->lcdRevNum->display(0);
    ui->lcdSendNum->display(0);
}
void SerialPort::on_chk_auto_send_clicked(bool checked)
{
    if(checked){
        timer->setInterval(ui->spinBox_timer->value());
        timer->start();
        ui->spinBox_timer->setReadOnly(true);
    }
    else{
        timer->stop();
        ui->spinBox_timer->setReadOnly(false);
    }
}
void SerialPort::on_btn_pause_rev_clicked()
{
    if(ui->btn_pause_rev->text()=="暂停接收"){
        disconnect(serial,SIGNAL(readyRead()),this,SLOT(slot_read_from_port()));
        ui->btn_pause_rev->setText("继续接收");
    }
    else if(ui->btn_pause_rev->text()=="继续接收"){
        ui->btn_pause_rev->setText("暂停接收");
        connect(serial,SIGNAL(readyRead()),this,SLOT(slot_read_from_port()));
    }
}
void SerialPort::slot_read_from_port()
{
    ui->lcdRevNum->display(ui->lcdRevNum->value()+1);

    QByteArray  byteArray = serial->readAll();
    rxCount+=byteArray.length(); show_portStateLabel();

    QTextCursor cursor = ui->plainTextEdit_rev->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->plainTextEdit_rev->setTextCursor(cursor);
    if(ui->chk_showNo->isChecked()){
        ui->plainTextEdit_rev->insertPlainText(QString("[%0]^")
                            .arg(ui->lcdRevNum->value()));
    }
    if(ui->radio_accept_ascii->isChecked())
    {
        ui->plainTextEdit_rev->insertPlainText(QString::fromLocal8Bit(byteArray));
    }
    else if( ui->radio_accept_hex->isChecked())
    {
        QDataStream out(&byteArray,QIODevice::ReadWrite);
        while(!out.atEnd())
        {
            qint8 outChar = 0;
            out>>outChar;
            QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
            ui->plainTextEdit_rev->insertPlainText(str+" ");
        }
        //ui->plainTextEdit_rev->appendPlainText(byteArray.toHex()+"  ");
    }
    if(ui->chk_auto_line->isChecked())
    {
        ui->plainTextEdit_rev->insertPlainText("\n");
    }
}

void SerialPort::on_btnSendData_clicked()
{
    ui->lcdSendNum->display(ui->lcdSendNum->value()+1);
    QString sendstr=ui->plainTextEdit_send->toPlainText();
    if(ui->radio_send_ascii->isChecked())
    {   //Unicode转GBK 参考博客问答http://bbs.csdn.net/topics/390024555
//        QTextCodec *codec = QTextCodec::codecForName("GBK"); //建立一个unicode与GBK之间的转换器
//        QByteArray bytes = codec->fromUnicode(sendstr); //unicode转换成gbk
        QByteArray bytes=sendstr.toLocal8Bit();//toLatin1
        serial->write(bytes);
        txCount+=bytes.length();show_portStateLabel();
    }
    else if(ui->radio_send_hex->isChecked())
    {
        QByteArray bytes;
        bytes = QByteArray::fromHex(sendstr.toLatin1());//toLatin1
        serial->write(bytes);
        txCount+=bytes.length();  show_portStateLabel();
        qDebug()<<bytes.toHex();
    }
}

