#include "qwdialog.h"
#include "ui_qwdialog.h"

QWDialog::QWDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWDialog)
{
    ui->setupUi(this);
    connect(ui->chkBoxUnder,SIGNAL(clicked(bool)),this,SLOT(on_chkBoxUnder_clicked1(bool)));
    connect(ui->chkBoxBold,SIGNAL(clicked(bool)),this,SLOT(on_chkBoxBold_clicked1(bool)));
    connect(ui->chkBoxItalic,SIGNAL(clicked(bool)),this,SLOT(on_chkBoxItalic_clicked1(bool)));

    connect(ui->rBtnBlue,SIGNAL(clicked()),this,SLOT(setTextFontColor()));//信号与槽的关联
    connect(ui->rBtnRed,SIGNAL(clicked()),this,SLOT(setTextFontColor()));//信号与槽的关联
    connect(ui->rBtnBlack,SIGNAL(clicked()),this,SLOT(setTextFontColor()));//信号与槽的关联
}

QWDialog::~QWDialog()
{
    delete ui;
}

void QWDialog::on_chkBoxItalic_clicked1(bool checked)
{
     QFont font=ui->txtEdit->font();
     font.setItalic(checked);
     ui->txtEdit->setFont(font);
}

void QWDialog::on_chkBoxBold_clicked1(bool checked)
{
    QFont   font=ui->txtEdit->font();
    font.setBold(checked);
    ui->txtEdit->setFont(font);

}
void QWDialog::on_chkBoxUnder_clicked1(bool checked)
{
    QFont font=ui->txtEdit->font();
    font.setUnderline(checked);
    ui->txtEdit->setFont(font);
}


void QWDialog::setTextFontColor()
{ //设置字体颜色
    QPalette   plet=ui->txtEdit->palette();
    if (ui->rBtnBlue->isChecked())
        plet.setColor(QPalette::Text,Qt::blue);
    else if (ui->rBtnRed->isChecked())
        plet.setColor(QPalette::Text,Qt::red);
    else if (ui->rBtnBlack->isChecked())
        plet.setColor(QPalette::Text,Qt::black);
    else
        plet.setColor(QPalette::Text,Qt::black);

    ui->txtEdit->setPalette(plet);
}
