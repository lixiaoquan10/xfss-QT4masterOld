#include "cdevicetypedelegate.h"
#include "cdevicetype.h"
#include "cglobal.h"

#include <QComboBox>
#include <QDebug>

CDeviceTypeDelegate::CDeviceTypeDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget* CDeviceTypeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QComboBox* combobox = new QComboBox(parent);
    QVector<CDeviceType*> deviceTypes = CGlobal::instance()->deviceTypes();
    for(int i = 0; i < deviceTypes.size(); ++i)
    {
        CDeviceType* deviceType = deviceTypes.value(i, NULL);
        if(!deviceType) continue;
        combobox->addItem(deviceType->imageReal(0.5), deviceType->name(), deviceType->type());
    }
    connect(combobox, SIGNAL(activated(int)), this, SLOT(indexChanged(int)));
    return combobox;
}

void CDeviceTypeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int type = index.data(Qt::EditRole).toInt();
    QComboBox* combobox = static_cast<QComboBox*>(editor);
    int i = combobox->findData(type);
    combobox->setCurrentIndex(i);
}

void CDeviceTypeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox* combobox = static_cast<QComboBox*>(editor);
    int type = combobox->itemData(combobox->currentIndex()).toInt();
    model->setData(index, type);
}

void CDeviceTypeDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    QComboBox* combobox = static_cast<QComboBox*>(editor);
    combobox->setGeometry(option.rect);
}

void CDeviceTypeDelegate::indexChanged(int index)
{
    Q_UNUSED(index);
    QComboBox* combobox = static_cast<QComboBox*>(sender());
    emit commitData(combobox);
}
