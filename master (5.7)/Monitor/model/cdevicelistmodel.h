#ifndef CDEVICELISTMODEL_H
#define CDEVICELISTMODEL_H


#include <QAbstractItemModel>
#include <QStringList>
#include "cmodel.h"

typedef struct {
    int keyId;
    int statusId;
    QString distributionId;
    int loopId;
    int deviceId;
    QString type;
    QString emergencyStatus;
    QString communicationStatus;
    QString lightStatus;
    QString batteryStatus;
    QString batteryLevel;
    QString area;
    QString location;
} DeviceStatusInfo, *pDeviceStatusInfo;

class CDeviceListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CDeviceListModel(QObject *parent = 0);
    ~CDeviceListModel();

    bool addData(const DeviceStatusInfo &StatusInfo);
    bool removeData(int keyId, int statusId = -1);
    void clear(){ m_data.clear(); reset(); }
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    DeviceStatusInfo getStatusInfo(int num){ return m_data.at(num); }

private:
    void setHeaderList(QStringList Header){m_HeaderTemplet=Header;}
    QStringList m_HeaderTemplet;
    QList<DeviceStatusInfo> m_data;
};

#endif // CDEVICELISTMODEL_H

