#ifndef CTABLEMODEL_H
#define CTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>

class CObject;
class CTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CTableModel(QObject *parent = 0);
    virtual ~CTableModel();

    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());
    virtual bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const {
        Q_UNUSED(section); Q_UNUSED(orientation); Q_UNUSED(role); return QVariant();}

protected:
    virtual QVariant getDisplayData(const QModelIndex& index) const { Q_UNUSED(index); return QVariant(); }
    virtual QVariant getDecorationData(const QModelIndex& index) const { Q_UNUSED(index); return QVariant(); }
    virtual QVariant getEditData(const QModelIndex& index) const { Q_UNUSED(index); return QVariant(); }
    virtual QVariant getTextColorData(const QModelIndex& index) const { Q_UNUSED(index); return QVariant(); }
    virtual QVariant getBackgroundData(const QModelIndex& index) const { Q_UNUSED(index); return QVariant(); }
    virtual QVariant getToolTipData(const QModelIndex& index) const { Q_UNUSED(index); return QVariant(); }
    virtual bool setEditData(const QModelIndex& index, const QVariant& value) { Q_UNUSED(index); Q_UNUSED(value); return true; }
    virtual bool insertObjects(const int row, const int count) { Q_UNUSED(row); Q_UNUSED(count); return true; }
    virtual bool removeObjects(const QList<CObject*> objects) { Q_UNUSED(objects); return true; }
    virtual CObject* getObject(const int row) const { Q_UNUSED(row); return NULL; }
};


#endif // CTABLEMODEL_H
