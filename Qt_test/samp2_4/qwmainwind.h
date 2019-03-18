#ifndef QWMAINWIND_H
#define QWMAINWIND_H

#include <QMainWindow>
#include    <QLabel>
#include    <QProgressBar>
#include    <QSpinBox>
#include    <QFontComboBox>


namespace Ui {
class QWmainWind;
}

class QWmainWind : public QMainWindow
{
    Q_OBJECT

public:
    explicit QWmainWind(QWidget *parent = 0);
    ~QWmainWind();

private slots:
    void on_actFontBold_triggered(bool checked);

    void on_actFontItalic_triggered(bool checked);

    void on_actFontUnder_triggered(bool checked);

    void on_txtEdit_copyAvailable(bool b);

    void on_txtEdit_selectionChanged();

    //  自定义槽函数
        void on_spinBoxFontSize_valueChanged(int aFontSize);//改变字体大小的SpinBox的响应
        void on_comboFont_currentIndexChanged(const QString &arg1);//FontCombobox的响应，选择字体名称

private:
    Ui::QWmainWind *ui;

    QString     fCurFileName;//当前文件名
    QLabel      *fLabCurFile;//状态栏里显示当前文件的Label
    QProgressBar    *progressBar1;//状态栏上的进度条
    QSpinBox        *spinFontSize;//   字体大写
    QFontComboBox   *comboFont;//字体名称
    void    iniUI(); //程序设计的UI初始化

    void    iniSignalSlots(); //手工关联信号与槽
};

#endif // QWMAINWIND_H
