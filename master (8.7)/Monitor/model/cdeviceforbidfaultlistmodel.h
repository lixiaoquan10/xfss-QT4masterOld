#ifndef CDEVICEFORBIDFAULTLISTMODEL_H
#define CDEVICEFORBIDFAULTLISTMODEL_H


#include <QAbstractItemModel>
#include <QStringList>
#include "cmodel.h"

typedef struct {
    int keyId;
    int statusId;
    QString distributionId;
    int loopId;
    int deviceId;
    QString productID;
    QString type;
} DeviceForbidFaultStatusInfo;

class CDeviceForbidFaultlistmodel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CDeviceForbidFaultlistmodel(QObject *parent = 0);
    ~CDeviceForbidFaultlistmodel();

    bool addData(const DeviceForbidFaultStatusInfo &StatusInfo);
    bool removeData(int keyId, int statusId = -1);
    void clear(){ m_data.clear(); reset(); }
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    DeviceForbidFaultStatusInfo getStatusInfo(int num){ return m_data.at(num); }

private:
    void setHeaderList(QStringList Header){m_HeaderTemplet=Header;}
    QStringList m_HeaderTemplet;
    QList<DeviceForbidFaultStatusInfo> m_data;
};

#endif // CDEVICEFORBIDFAULTLISTMODEL_H

