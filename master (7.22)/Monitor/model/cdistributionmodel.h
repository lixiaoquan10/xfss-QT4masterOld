#ifndef CDISTRIBUTIONMODEL_H
#define CDISTRIBUTIONMODEL_H

#include "cmodel.h"

#define DISTRIBUTION_COLUMN_ADDRESS 0
#define DISTRIBUTION_COLUMN_NAME 1
#define DISTRIBUTION_COLUMN_DESCRIPTION 2
#define DISTRIBUTION_COLUMN_AREA 3
#define DISTRIBUTION_COLUMN_LOCATION 4

class CCanport;
class CController;
class CDistributionModel : public CModel
{
    Q_OBJECT
public:
    explicit CDistributionModel(QObject *parent = 0);
    CDistributionModel(CCanport* canport, bool track = true, QObject *parent = 0);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setController(CController *controller);
protected slots:
    void objectChanged(CObject::ObjectType type, CObject* object);
    void objectDestroy(CObject *object);
protected:
    bool beforeLoadChildren(CObject *object, QModelIndex &parent);
    QVariant data(const QModelIndex &index, int role) const;
//    QVariant getDisplayData(const QModelIndex &index) const;
//    QVariant getTextColorData(const QModelIndex &index) const;
//    QVariant getEditData(const QModelIndex &index) const;
//    bool setEditData(const QModelIndex &index, const QVariant &value);
private:
    CController* m_controller;
};


#endif // CDISTRIBUTIONMODEL_H
