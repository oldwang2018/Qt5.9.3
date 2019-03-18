#ifndef QWCOMBOBOXSPINDELEGATE_H
#define QWCOMBOBOXSPINDELEGATE_H
#include<QStyledItemDelegate>

class QWComboBoxDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    QWComboBoxDelegate(QObject *parent=0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option
                          , const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setEditorData(QWidget *editor,
                      const QModelIndex &index)const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                     const QModelIndex &index)const Q_DECL_OVERRIDE;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // QWCOMBOBOXSPINDELEGATE_H
