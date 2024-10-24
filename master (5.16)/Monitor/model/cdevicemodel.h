#ifndef CDEVICEMODEL_H
#define CDEVICEMODEL_H

#include "cmodel.h"

#include <QHash>

#define DEVICE_COLUMN_ADDRESS 0
#define DEVICE_COLUMN_LOGIN_ADDRESS 1
#define DEVICE_COLUMN_TYPE 2
#define DEVICE_COLUMN_LOGIN_TYPE 3
#define DEVICE_COLUMN_PRODUCTID 4
#define DEVICE_COLUMN_VERSION 5
//#define DEVICE_COLUMN_LOCATION 4
#define DEVICE_COLUMN_CONFIG_LOGIN 6
#define DEVICE_COLUMN_STATUS 7

class CLoop;
class CDevice;
class CDeviceModel : public CModel
{
    Q_OBJECT
public:
    explicit CDeviceModel(QObject *parent = 0);
    CDeviceModel(CLoop* loop, bool track = true, QObject *parent = 0);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setLoop(CLoop* loop);
    CLoop* loop(){ return m_loop; }
    void setupdate() { isupdate = true; }
protected slots:
    void objectChanged(CObject::ObjectType type, CObject* object);
    void objectDestroy(CObject *object);
protected:
    bool beforeLoadChildren(CObject *object, QModelIndex &parent);
    QVariant data(const QModelIndex &index, int role) const;
private:
    CLoop* m_loop;
    bool isupdate;
};


#endif // CDEVICEMODEL_H
