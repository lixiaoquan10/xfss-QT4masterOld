#ifndef CDEVICETYPEDELEGATE_H
#define CDEVICETYPEDELEGATE_H

#include <QStyledItemDelegate>

class CDeviceTypeDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CDeviceTypeDelegate(QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected slots:
    void indexChanged(int index);

};

#endif // CDEVICETYPEDELEGATE_H
