#include "cglobal.h"


CDeviceListModel::CDeviceListModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    m_HeaderTemplet<<"集中电源"<<"回路"<<"地址"<<"设备类型"<<"通讯"
                  <<"应急"<<"光源"<<"电池状态"<<"电池电量"<<"区域"<<"位置";
}

CDeviceListModel::~CDeviceListModel()
{
    m_data.clear();
}

bool CDeviceListModel::addData(const DeviceStatusInfo &StatusInfo)
{
    int rowcount=0;
    DeviceStatusInfo tempInfo = StatusInfo;
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

bool CDeviceListModel::removeData(int keyId, int statusId)
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

int CDeviceListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

int CDeviceListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_HeaderTemplet.count();
}

QVariant CDeviceListModel::data(const QModelIndex &index, int role) const
{

    if(index.column()>=m_HeaderTemplet.count()) return QVariant();
    QVariant result;
    int col=index.column();
    if(!index.isValid())return result;
    if(role == Qt::TextAlignmentRole){
        if(col != 3 && col < 9){
            QVariant value = Qt::AlignCenter;
            return value;
        }
    }
    if(role== Qt::DisplayRole){
        int row =index.row();
        DeviceStatusInfo tempInfo = m_data[row];
        if(col>-1&&col<11)
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
                result = tempInfo.communicationStatus;
            else if(col == 5)
                result = tempInfo.emergencyStatus;
            else if(col == 6)
                result = tempInfo.lightStatus;
            else if(col == 7)
                result = tempInfo.batteryStatus;
            else if(col == 8)
                result = tempInfo.batteryLevel;
            else if(col == 9)
                result = tempInfo.area;
            else if(col == 10)
                result = tempInfo.location;
        }
    }
    if(role == Qt::BackgroundColorRole){
        int row = index.row();
        DeviceStatusInfo tempInfo = m_data[row];
        if(col>-1&&col<9)
        {
            if(col == 4)
            {
                if(tempInfo.communicationStatus == "正常")
                    result = QColor(127,255,0);   //绿色
                else
                    result = QColor(245,222,179);  //黄色
            }
            else if(col == 5)
            {
                if(tempInfo.emergencyStatus == "正常")
                    result = QColor(127,255,0);   //绿色
                else
                    result = QColor(255,99,71);   //红色
            }
            else if(col == 6)
            {
                if(tempInfo.lightStatus == "正常")
                    result = QColor(127,255,0);   //绿色
                else
                    result = QColor(245,222,179);  //黄色
            }
            else if(col == 7)
            {
                if(tempInfo.batteryStatus == "正常")
                    result = QColor(127,255,0);   //绿色
                else if(tempInfo.batteryStatus == "电池短路" || tempInfo.batteryStatus == "电池断线")
                    result = QColor(245,222,179);  //黄色
                else if(tempInfo.batteryStatus == "电池放电")
                    result = QColor(255,99,71);   //红色
                else if(tempInfo.batteryStatus == "电池充电")
                    result = QColor(227,207,87);   //香蕉色
            }
            else if(col == 8)
            {
                if(tempInfo.batteryLevel == "电池过放电")
                    result = QColor(245,222,179);  //黄色
                else if(tempInfo.batteryLevel == "%25" ||
                        tempInfo.batteryLevel == "%50" || tempInfo.batteryLevel == "%75")
                    result = QColor(227,207,87);   //香蕉色
                else if(tempInfo.batteryLevel == "正常")
                    result = QColor(127,255,0);   //绿色
            }
        }
    }
    return result;
}

QVariant CDeviceListModel::headerData(int section, Qt::Orientation orientation, int role) const
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
