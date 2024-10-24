#include "cglobal.h"


CDevicesFindTableModel::CDevicesFindTableModel(CDM* dm) :
    CTableModel(), m_dm(dm)
{
    qDebug() << "CDevicesFindTableModel::CDevicesFindTableModel";
    initDM();
}

int CDevicesFindTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_devices.size();
}

int CDevicesFindTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

QVariant CDevicesFindTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == DEVICESFINDTABLE_COLUMN_DISTRIBUTION)
            return tr("distribution");
        else if(section == DEVICESFINDTABLE_COLUMN_LOOP)
            return tr("loop");
        else if(section == DEVICESFINDTABLE_COLUMN_ADDRESS)
            return tr("address");
        else if(section == DEVICESFINDTABLE_COLUMN_SECONDCODE)
            return tr("second code");
        else if(section == DEVICESFINDTABLE_COLUMN_TYPE)
            return tr("device type");
        else if(section == DEVICESFINDTABLE_COLUMN_DESCRIPTION)
            return tr("description");
    }
    if(orientation == Qt::Vertical && role == Qt::DisplayRole)
    {
        return section + 1;
    }
    return QVariant();
}

void CDevicesFindTableModel::setFilter(const QString &secondCode)
{
    m_secondCode = secondCode;
    filterDevice();
}

CDevice *CDevicesFindTableModel::getDevice(const int row) const
{
    if(!m_dm)
        return 0;
    QString secondCode = m_devices.value(row, "");
    if(secondCode.isEmpty())
        return NULL;
    return m_dm->device(secondCode);
}

QVariant CDevicesFindTableModel::getDisplayData(const QModelIndex &index) const
{
    CDevice* device = getDevice(index.row());
    if(!device)
        return QVariant();
    CDeviceType* devType = CGlobal::instance()->deviceType(device->deviceValue(DEVICE_VALUE_TYPE).toInt());
    if(!devType)
        devType = CGlobal::instance()->deviceType(0);
    if(index.column() == DEVICESFINDTABLE_COLUMN_DISTRIBUTION)
        return device->loop()->distribution()->distributionId();
    else if(index.column() == DEVICESFINDTABLE_COLUMN_LOOP)
        return device->loop()->loopId();
    else if(index.column() == DEVICESFINDTABLE_COLUMN_ADDRESS)
        return device->deviceId();
    else if(index.column() == DEVICESFINDTABLE_COLUMN_SECONDCODE)
        return device->deviceValue(DEVICE_VALUE_SECONDCODE);
    else if(index.column() == DEVICESFINDTABLE_COLUMN_TYPE)
        return devType->name();
    else if(index.column() == DEVICESFINDTABLE_COLUMN_DESCRIPTION)
        return device->deviceValue(DEVICE_VALUE_DESCRIPTION);
    return QVariant();
}

QVariant CDevicesFindTableModel::getDecorationData(const QModelIndex &index) const
{
    const CDevice* device = getDevice(index.row());
    if(!device)
        return QVariant();
    const CDeviceType* devType = CGlobal::instance()->deviceType(device->deviceValue(DEVICE_VALUE_TYPE).toInt());
    if(!devType)
        devType = CGlobal::instance()->deviceType(0);
    if(index.column() == DEVICESFINDTABLE_COLUMN_TYPE)
        return devType->imageReal(0.5);
    return QVariant();
}

CObject* CDevicesFindTableModel::getObject(const int row) const
{
    return getDevice(row);
}

void CDevicesFindTableModel::initDM()
{
    m_secondCode = "";
    m_backupDevices.clear();
    if(m_dm)
        m_backupDevices = m_dm->devices().keys();
    qSort(m_backupDevices.begin(), m_backupDevices.end());
    filterDevice();
}

void CDevicesFindTableModel::filterDevice()
{
    filterSecondCode();
    reset();
}

void CDevicesFindTableModel::filterSecondCode()
{
    m_devices.clear();
    if(m_secondCode.isEmpty())
    {
        m_devices = m_backupDevices;
        return;
    }
    m_secondCode.replace("*", "?");
    QRegExp reg(m_secondCode, Qt::CaseInsensitive, QRegExp::Wildcard);
    QString tmp;
    int x = 0;
    for(int i = m_backupDevices.size() - 1; i >= 0; --i)
    {
        tmp = m_backupDevices.at(i);
        x = tmp.indexOf(reg);
        if(x == 0)
            m_devices.prepend(tmp);
        else if(x > 0)
            m_devices.append(tmp);
    }
}
