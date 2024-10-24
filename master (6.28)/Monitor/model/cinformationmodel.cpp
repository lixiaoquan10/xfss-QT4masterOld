#include "cglobal.h"


CInformationModel::CInformationModel(QObject *parent) :
    QAbstractTableModel(parent)
{
//    m_HeaderTemplet<<tr("Time")<<tr("Coded Message")<<tr("Device Type")<<tr("Device Description")<<tr("Device State")<<tr("Location Layer");
    m_HeaderTemplet<<"时间"<<"设备"<<"回路"<<"地址"<<"设备类型"<<"区域"<<"位置"<<"设备状态";
}

CInformationModel::~CInformationModel()
{
    m_data.clear();
}

bool CInformationModel::addData(const StatusInfo &StartInfo)
{
    int rowcount=0;
    StatusInfo tempInfo = StartInfo;
    for(int i=m_data.count()-1; i>-1; --i)
    {
        if(m_data.at(i).keyId==tempInfo.keyId && m_data.at(i).statusId==tempInfo.statusId
                && m_data.at(i).type==tempInfo.type)
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

bool CInformationModel::removeData(int keyId, int statusId)
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

int CInformationModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

int CInformationModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_HeaderTemplet.count();
}

QVariant CInformationModel::data(const QModelIndex &index, int role) const
{
    if(index.column()>=m_HeaderTemplet.count()) return QVariant();
    int col=index.column();
    QVariant result;
    if(!index.isValid())return result;
    if(role == Qt::TextAlignmentRole){
        if(col < 4){
            QVariant value = Qt::AlignCenter;
            return value;
        }
    }
    if(role== Qt::DisplayRole){
        int row =index.row();
        int col=index.column();
        StatusInfo tempInfo = m_data[row];
        if(col>-1&&col<8)
        {
            if(col == 0)
                result = tempInfo.time.toString("yyyy-MM-dd HH:mm:ss");
            else if(col == 1 && tempInfo.distributionId != "")
                result = tempInfo.distributionId;
            else if(col == 2 && tempInfo.loopID != 0)
                result = tempInfo.loopID;
            else if(col == 3 && tempInfo.deviceID != 0)
                result = tempInfo.deviceID;
            else if(col == 4)
                result = tempInfo.type;
            else if(col == 5)
                result = tempInfo.area;
            else if(col == 6)
                result = tempInfo.location;
            else if(col == 7)
                result = tempInfo.status;
//            else if(col == 5)
//                result = tempInfo.layer;
        }
    }
    return result;
}

QVariant CInformationModel::headerData(int section, Qt::Orientation orientation, int role) const
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
