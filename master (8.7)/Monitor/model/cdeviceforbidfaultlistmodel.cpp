#include "cglobal.h"
#include "cdeviceforbidfaultlistmodel.h"


CDeviceForbidFaultlistmodel::CDeviceForbidFaultlistmodel(QObject *parent) :
    QAbstractTableModel(parent)
{
    m_HeaderTemplet<<"集中电源"<<"回路"<<"地址"<<"设备类型"<<"产品编号";
}

CDeviceForbidFaultlistmodel::~CDeviceForbidFaultlistmodel()
{
    m_data.clear();
}

bool CDeviceForbidFaultlistmodel::addData(const DeviceForbidFaultStatusInfo &StatusInfo)
{
    int rowcount=0;
    DeviceForbidFaultStatusInfo tempInfo = StatusInfo;
    for(int i=m_data.count()-1; i>-1; --i)
    {
        if(m_data.at(i).keyId==tempInfo.keyId && m_data.at(i).statusId==tempInfo.statusId)
        {
            beginRemoveRows(QModelIndex(), i, i);
            m_data.removeAt(i);
            endRemoveRows();
        }
    }
    beginInsertRows(QModelIndex(), rowcount, rowcount);
    m_data.push_front(tempInfo);
    endInsertRows();
    return true;
}

bool CDeviceForbidFaultlistmodel::removeData(int keyId, int statusId)
{
    for(int i=m_data.count()-1; i>-1; --i)
    {
        if(m_data.at(i).keyId == keyId)
        {
            if(statusId < 0)
            {
                beginRemoveRows(QModelIndex(), i, i);
                m_data.removeAt(i);
                endRemoveRows();
            }
            else if(m_data.at(i).statusId == statusId)
            {
                beginRemoveRows(QModelIndex(), i, i);
                m_data.removeAt(i);
                endRemoveRows();
            }
        }
    }
    return true;
}

int CDeviceForbidFaultlistmodel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

int CDeviceForbidFaultlistmodel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_HeaderTemplet.count();
}

QVariant CDeviceForbidFaultlistmodel::data(const QModelIndex &index, int role) const
{

    if(index.column()>=m_HeaderTemplet.count()) return QVariant();
    QVariant result;
    int col=index.column();
    if(!index.isValid())return result;
    if(role == Qt::TextAlignmentRole){
        if(col < 3){
            QVariant value = Qt::AlignCenter;
            return value;
        }
    }
    if(role== Qt::DisplayRole){
        int row =index.row();
        DeviceForbidFaultStatusInfo tempInfo = m_data[row];
        if(col>-1&&col<5)
        {
            if(col == 0)
                result = tempInfo.distributionId;
            else if(col == 1)
                result = tempInfo.loopId;
            else if(col == 2)
                result = tempInfo.deviceId;
            else if(col == 3)
                result = tempInfo.type;
            else if(col == 4)
                result = tempInfo.productID;
        }
    }
    return result;
}

QVariant CDeviceForbidFaultlistmodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;
    if(role== Qt::DisplayRole)
    {
        if(orientation==Qt::Horizontal && section<m_HeaderTemplet.count())
        {
            result=m_HeaderTemplet[section];
        }
        else if(orientation == Qt::Vertical)
        {
            result = section + 1;
        }
    }
    else if (role == Qt::FontRole && orientation == Qt::Horizontal)
    {
        QFont font;
        font.setPointSize(12);  // 设置字号为12
        result = font;
    }
    return result;
}
