#include "qmywidget.h"
#include "ui_qmywidget.h"
#include<QMetaProperty>
#include "qperson.h"


QmyWidget::QmyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QmyWidget)
{
    ui->setupUi(this);
    boy=new QPerson("王小明") ;
    boy->setProperty("score",95);
    boy->setProperty("age",10);
    boy->setProperty("sex","Boy"); //动态属性
    connect(boy,SIGNAL(ageChanged(int)),this,SLOT(on_ageChanged(int)));

    girl=new QPerson("张小丽");
    girl->setProperty("score",81);
    girl->setProperty("age",20);
    girl->setProperty("sex","Girl");
    connect(girl,SIGNAL(ageChanged(int)),this,SLOT(on_ageChanged(int)));

    ui->spinBoy->setProperty("isBoy",true);//动态属性
    ui->spinGirl->setProperty("isBoy",false);


    connect(ui->spinBoy,SIGNAL(valueChanged(int)),this,SLOT(on_spin_valueChanged(int)));
    connect(ui->spinGirl,SIGNAL(valueChanged(int)),this,SLOT(on_spin_valueChanged(int)));
}


void QmyWidget::on_ageChanged(int value)
{
    Q_UNUSED(value);
   /***/ QPerson *aPerson=qobject_cast<QPerson*>(sender());
    QString hisName=aPerson->property("nameTEST").toString();
    QString hisSex=aPerson->property("sex").toString();
    int hisAge=aPerson->property("ageTEST").toInt();
   // int hisAge=aPerson->age();
    ui->textEdit->appendPlainText(hisName+","+
                                 hisSex+QString::asprintf(",年龄=%d",hisAge));
}

void QmyWidget::on_btnClassInfo_clicked()
{

    const QMetaObject *meta=girl->metaObject();  //************************
    ui->textEdit->clear();
    ui->textEdit->appendPlainText("==元对象信息==\n");
    ui->textEdit->appendPlainText(QString("类名称：%1\n").arg(meta->className()));

    ui->textEdit->appendPlainText("property");
    for(int i=meta->propertyOffset();i<meta->propertyCount();i++)
    {
        const char *propName=meta->property(i).name();
        ui->textEdit->appendPlainText(
            QString("属性名称=%1，属性值=%2").arg(propName).
                    arg(boy->property(propName).toString()));
    }

    ui->textEdit->appendPlainText("");
    ui->textEdit->appendPlainText("classInfo");
    for(int i=meta->classInfoOffset();i<meta->classInfoCount();i++)
    {
        QMetaClassInfo classInfo=meta->classInfo(i);
        ui->textEdit->appendPlainText(
             QString("Name=%1; Value=%2").arg(classInfo.name()).
                    arg(classInfo.value()));
    }
}

void QmyWidget::on_spinBoy_valueChanged(int arg1)
{//*****************************
     boy->setAge(ui->spinBoy->value());
}
void QmyWidget::on_spinGirl_valueChanged(int arg1)
{
    girl->setAge(ui->spinGirl->value());
}

void QmyWidget::on_btnClear_clicked()
{
    ui->textEdit->clear();
}

void QmyWidget::on_btnBoyInc_clicked()
{
     ui->spinBoy->setValue(ui->spinBoy->value()+1);
}
void QmyWidget::on_btnGirlInc_clicked()
{
    ui->spinGirl->setValue(ui->spinGirl->value()+1);
}
QmyWidget::~QmyWidget()
{
    delete ui;
}

