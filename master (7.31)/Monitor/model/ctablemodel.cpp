#include "ctablemodel.h"
#include <QDebug>

CTableModel::CTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

CTableModel::~CTableModel()
{
}

int CTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 0;
}

int CTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
}

bool CTableModel::insertRows(int row, int count, const QModelIndex& parent)
{
    if(count <= 0)
        return false;

    bool nRet;
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();
    nRet = insertObjects(row, count);
    return nRet;
}

bool CTableModel::removeRows(int row, int count, const QModelIndex& parent)
{
    if(count <= 0)
        return false;

    QList<CObject*> objects;
    for(int i = row; i < row + count; ++i)
    {
        CObject* object = getObject(i);
        if(object)
            objects.append(object);
    }

    bool nRet;
    beginRemoveRows(parent, row, row + count - 1);
    endRemoveRows();
    nRet = removeObjects(objects);
    return nRet;
}

Qt::ItemFlags CTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return QAbstractItemModel::flags(index);
}

QVariant CTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::DisplayRole)
        return getDisplayData(index);
    if(role == Qt::DecorationRole)
        return getDecorationData(index);
    if(role == Qt::EditRole)
        return getEditData(index);
    if(role == Qt::TextColorRole)
        return getTextColorData(index);
    if(role == Qt::BackgroundRole)
        return getBackgroundData(index);
    if(role == Qt::ToolTipRole)
        return getToolTipData(index);
    return QVariant();
}

bool CTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;
    if(role == Qt::EditRole)
        return setEditData(index, value);
    return true;
}
