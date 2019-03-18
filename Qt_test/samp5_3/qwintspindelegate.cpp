#include "qwintspindelegate.h"
#include<QSpinBox>
#include<QDebug>
QWIntSpinDelegate::QWIntSpinDelegate(QObject *parent)
    :QStyledItemDelegate(parent)//????????
{

}

QWidget *QWIntSpinDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSpinBox *editor=new QSpinBox(parent);
    editor->setFrame(false);
    editor->setMinimum(10);
    editor->setMaximum(10000);
    return editor;
}

void QWIntSpinDelegate::setEditorData(QWidget *editor,
                                      const QModelIndex &index) const
{
    /*QString*/qDebug()<<index.model()->data(index,Qt::EditRole);
    int value =index.model()->data(index,Qt::EditRole).toInt();
    QSpinBox *spinBox=static_cast<QSpinBox*>(editor);
    spinBox->setValue(value);
}

void QWIntSpinDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                     const QModelIndex &index) const
{
    QSpinBox *spinBox=static_cast<QSpinBox*>(editor);
    spinBox->interpretText();//??????解释数据，如果数据被修改后，就触发信号
    int value=spinBox->value();
    model->setData(index,value,Qt::EditRole);

    /*int*/qDebug()<<model->data(index,Qt::EditRole);
}

void QWIntSpinDelegate::updateEditorGeometry(QWidget *editor,
             const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
