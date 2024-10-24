#ifndef CLOOPMODEL_H
#define CLOOPMODEL_H

#include "cmodel.h"

#define LOOP_COLUMN_ADDRESS 0
#define LOOP_COLUMN_NAME 1
#define LOOP_COLUMN_DESCRIPTION 2

class CCanport;
class CDistribution;
class CLoopModel : public CModel
{
    Q_OBJECT
public:
    explicit CLoopModel(QObject *parent = 0);
    CLoopModel(CDistribution* distribution, bool track = true, QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setDistribution(CDistribution* distribution);
    CDistribution* distribution() { return m_distribution; }

protected slots:
    void objectChanged(CObject::ObjectType type, CObject* object);
    void objectDestroy(CObject *object);
protected:
    bool beforeLoadChildren(CObject *object, QModelIndex &parent);
    QVariant getDisplayData(const QModelIndex &index) const;
    QVariant getTextColorData(const QModelIndex &index) const;
//    QVariant getEditData(const QModelIndex &index) const;
//    bool setEditData(const QModelIndex &index, const QVariant &value);
private:
    CDistribution* m_distribution;
    CCanport* m_canport;
};


#endif // CLOOPMODEL_H
