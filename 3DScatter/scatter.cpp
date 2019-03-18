#include "scatter.h"
#include "ui_scatter.h"
#include    <QSplitter>
#include    <QColorDialog>
#include<QDebug>
#include<QStatusBar>
#include<QHBoxLayout>

Scatter::Scatter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Scatter)
{
    ui->setupUi(this);

    iniGraph3D();

    QSplitter   *splitter=new QSplitter(Qt::Horizontal);
    splitter->addWidget(ui->groupBox);
    splitter->addWidget(graphContainer);
    QHBoxLayout *layout=new QHBoxLayout();
    layout->addWidget(splitter);
    layout->setContentsMargins(2,2,2,2);
    layout->setSpacing(2);
    this->setLayout(layout);
}
void Scatter::iniGraph3D()
{
    graph3D = new Q3DScatter();
    graphContainer = QWidget::createWindowContainer(graph3D); //Q3DBars继承自QWindow，必须如此创建
    proxy = new QScatterDataProxy(); //数据代理
    series = new QScatter3DSeries(proxy);  //创建序列
    series->setItemLabelFormat(QStringLiteral("(X:@xLabel, Y:@zLabel, Z:@yLabel)"));// @yTitle: @yLabel
    graph3D->axisX()->setTitle("axis X");//创建坐标轴
    graph3D->axisX()->setTitleVisible(true);
    graph3D->axisZ()->setTitle("axis Y");
    graph3D->axisZ()->setTitleVisible(true);
    graph3D->axisY()->setTitle("axis Z");
    graph3D->axisY()->setTitleVisible(true);
    graph3D->addSeries(series);
    graph3D->activeTheme()->setLabelBackgroundEnabled(false);//轴标题背景
    series->setMesh(QAbstract3DSeries::MeshSphere);//散点形状 MeshPoint,MeshCylinder
    graph3D->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPreset(7));
    series->setItemSize(0.2);//散点大小 default 0. value 0~1

    itemCount=1;
    dataArray = new QScatterDataArray();
    dataArray->resize(itemCount);
    QScatterDataItem *ptrToDataArray = &dataArray->first();

    float x=0,y=0,z=0;
    (ptrToDataArray+0)->setPosition(QVector3D(x,z,y));
    series->dataProxy()->resetArray(dataArray);
    graph3D->axisX()->setRange(-100,100);
    graph3D->axisY()->setRange(-100,100);
    graph3D->axisZ()->setRange(-100,100);

}


Scatter::~Scatter()
{
    delete ui;
}

void Scatter::on_comboBox_currentIndexChanged(int index)
{//预设视角
    qDebug()<<index;
    graph3D->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPreset(index));
}


void Scatter::on_sliderH_valueChanged(int value)
{
    Q_UNUSED(value);
       int xRot =ui->sliderH->value();
       int yRot=ui->sliderV->value();
       int zoom=ui->sliderZoom->value(); //缩放
       graph3D->scene()->activeCamera()->setCameraPosition(xRot, yRot,zoom);
}

void Scatter::on_sliderV_valueChanged(int value)
{
    on_sliderH_valueChanged(value);
}

void Scatter::on_sliderZoom_valueChanged(int value)
{
    on_sliderH_valueChanged(value);
}

void Scatter::on_cBoxTheme_currentIndexChanged(int index)
{
    //设置主题
        Q3DTheme *currentTheme = graph3D->activeTheme();
        currentTheme->setType(Q3DTheme::Theme(index));

}

void Scatter::on_cBoxBarStyle_currentIndexChanged(int index)
{//散点形状
    //    QScatter3DSeries *series =graph3D->seriesList().at(0);
        QAbstract3DSeries::Mesh aMesh;
        aMesh=QAbstract3DSeries::Mesh(index+1);
        series->setMesh(aMesh);
}


void Scatter::on_spinFontSize_valueChanged(int arg1)
{//轴标签字体大小
    QFont font = graph3D->activeTheme()->font();
    font.setPointSize(arg1);
    graph3D->activeTheme()->setFont(font);
}

void Scatter::on_spinItemSize_valueChanged(double arg1)
{//散点大小
//  QScatter3DSeries *series =graph3D->seriesList().at(0);
    series->setItemSize(arg1);//default 0. value 0~1
}

void Scatter::on_btnBarColor_clicked()
{ //设置序列柱状图的颜色
//    QScatter3DSeries *series =graph3D->seriesList().at(0);
    QColor  color=series->baseColor();
    color=QColorDialog::getColor(color);
    if (color.isValid())
        series->setBaseColor(color);
}

void Scatter::on_btnAddPoint_clicked()
{
    //QScatter3DSeries *series =graph3D->seriesList().at(0);
    itemCount++;
    dataArray->resize(itemCount);
    QScatterDataItem *ptrToDataArray = &dataArray->first();
    float x=itemCount*20-20,y=itemCount*20-20,z=itemCount*20-20;
    (ptrToDataArray+itemCount-1)->setPosition(QVector3D(x,z,y));
    series->dataProxy()->resetArray(dataArray);
}

void Scatter::on_btnReducePoint_clicked()
{
    itemCount--;
    if(itemCount==0)
        itemCount=1;
    dataArray->resize(itemCount);
    series->dataProxy()->resetArray(dataArray);
}

void Scatter::on_btnInit_clicked()
{
   graph3D->activeTheme()->setBackgroundEnabled(false);//图表的背景
   graph3D->setShadowQuality(QAbstract3DGraph::ShadowQualityMedium);
   series->setMeshSmooth(true);//柱状图的光滑性
   //轴标题
       graph3D->axisX()->setTitleVisible(true);
       graph3D->axisY()->setTitleVisible(true);
       graph3D->axisZ()->setTitleVisible(true);
   //项的标签
   //    QScatter3DSeries *series =graph3D->seriesList().at(0);
   series->setItemLabelVisible(true);
   //图表的网格
   graph3D->activeTheme()->setGridEnabled(true);
   //Z轴反向
   graph3D->axisZ()->setReversed(false);
   //轴标题背景
   graph3D->activeTheme()->setLabelBackgroundEnabled(false);
}
void Scatter::show(QVector<QVector3D> Vec)
{
    itemCount=Vec.count();
     dataArray->resize(itemCount);
    QScatterDataItem *ptrToDataArray = &dataArray->first();
    for(int i=0;i<itemCount;i++)
    {
       (ptrToDataArray+i)->setPosition(Vec.at(i));
    }
    series->dataProxy()->resetArray(dataArray);
}



