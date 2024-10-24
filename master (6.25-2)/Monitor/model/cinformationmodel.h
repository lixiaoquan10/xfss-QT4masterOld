#ifndef STARTINFORMATIONMODEL_H
#define STARTINFORMATIONMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

#include "business/cclientbusiness.h"

class CInformationModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CInformationModel(QObject *parent = 0);
    ~CInformationModel();

    bool addData(const StatusInfo &StartInfo);
    bool removeData(int keyId, int statusId = -1);
    void clear(){ m_data.clear(); reset(); }
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    StatusInfo getStatusInfo(int num){ return m_data.at(num); }
    int count() { return m_data.count(); }

private:
    void setHeaderList(QStringList Header){m_HeaderTemplet=Header;}
    QStringList m_HeaderTemplet;
    QList<StatusInfo> m_data;
};

#endif // STARTINFORMATIONMODEL_H
