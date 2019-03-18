#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_SliderRed_valueChanged(int value)
{
    Q_UNUSED(value);
    QColor color;
    int r=ui->SliderRed->value();
    int g=ui->SliderGreen->value();
    int b=ui->SliderBlue->value();
    int alpha=ui->SliderAlpha->value();
    color.setRgb(r,g,b,alpha);
    QPalette pal=ui->textEdit->palette();
    pal.setColor(QPalette::Base,color);
    ui->textEdit->setPalette(pal);
}

void Widget::on_SliderGreen_valueChanged(int value)
{
    Widget::on_SliderRed_valueChanged(value);
}

void Widget::on_SliderBlue_valueChanged(int value)
{
    Widget::on_SliderRed_valueChanged(value);
}

void Widget::on_dial_valueChanged(int value)
{
    ui->LCDDisplay->display(value);
}
void Widget::on_SliderAlpha_valueChanged(int value)
{
     Widget::on_SliderRed_valueChanged(value);
}
void Widget::on_radioButton_clicked()
{
    ui->LCDDisplay->setDigitCount(3);
    ui->LCDDisplay->setDecMode();
}

void Widget::on_radioButton_2_clicked()
{
    ui->LCDDisplay->setDigitCount(8);
    ui->LCDDisplay->setBinMode();
}

void Widget::on_radioButton_4_clicked()
{  ui->LCDDisplay->setDigitCount(4);
    ui->LCDDisplay->setOctMode();
}
void Widget::on_radioButton_3_clicked()
{
    ui->LCDDisplay->setDigitCount(3);
    ui->LCDDisplay->setHexMode();
}

void Widget::on_horizontalScrollBar_valueChanged(int value)
{
    ui->progressBar->setValue(value);
}


