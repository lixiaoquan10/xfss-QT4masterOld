#ifndef CDEVICESFINDTABLEMODEL_H
#define CDEVICESFINDTABLEMODEL_H

#include "ctablemodel.h"

#include <QList>

#define DEVICESFINDTABLE_COLUMN_DISTRIBUTION 0
#define DEVICESFINDTABLE_COLUMN_LOOP 1
#define DEVICESFINDTABLE_COLUMN_ADDRESS 2
#define DEVICESFINDTABLE_COLUMN_SECONDCODE 3
#define DEVICESFINDTABLE_COLUMN_TYPE 4
#define DEVICESFINDTABLE_COLUMN_DESCRIPTION 5

class CDM;
class CDevice;
class CDevicesFindTableModel : public CTableModel
{
    Q_OBJECT
public:
    explicit CDevicesFindTableModel(CDM *dm = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setDM(CDM* dm) { m_dm = dm; initDM(); }
    void setFilter(const QString& secondCode);
    void clearSecondCode() { m_secondCode.clear(); }
    CDevice* getDevice(const int row) const;
protected:
    QVariant getDisplayData(const QModelIndex &index) const;
    QVariant getDecorationData(const QModelIndex &index) const;
    CObject* getObject(const int row) const;
private:
    void initDM();
    void filterDevice();
    void filterSecondCode();
private:
    CDM* m_dm;
    QString m_secondCode;
    QList<QString> m_devices;
    QList<QString> m_backupDevices;
};

#endif // CDEVICESFINDTABLEMODEL_H
