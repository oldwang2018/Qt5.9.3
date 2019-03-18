#include "qwfloatspindelegate.h"

#include<QDoubleSpinBox>

QWFloatSpinDelegate::QWFloatSpinDelegate(QObject *parent)
    :QStyledItemDelegate(parent)//????????
{

}

QWidget *QWFloatSpinDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDoubleSpinBox *editor=new QDoubleSpinBox(parent);
    editor->setFrame(false);
    editor->setMinimum(10);
    editor->setDecimals(2);
    editor->setMaximum(10000);
    return editor;
}

void QWFloatSpinDelegate::setEditorData(QWidget *editor,
                                      const QModelIndex &index) const
{
    float value =index.model()->data(index,Qt::EditRole).toFloat();
    QDoubleSpinBox *spinBox=static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}

void QWFloatSpinDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                     const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox=static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();//??????解释数据，如果数据被修改后，就触发信号
    float value=spinBox->value();
    model->setData(index,value,Qt::EditRole);
}

void QWFloatSpinDelegate::updateEditorGeometry(QWidget *editor,
             const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
