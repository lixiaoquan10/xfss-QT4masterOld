#include "dlgsystemrecordmodel.h"
#include "struct/cmsglog.h"
#include <QFont>
#include "cglobal.h"

dlgSystemRecordModel::dlgSystemRecordModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    m_HeaderTemplet<<"设备名称"<<"类型"<<"发生时间"<<"设备"<<"回路"<<"地址"
                  <<"区域"<<"位置"<<"详细信息";
}

dlgSystemRecordModel::~dlgSystemRecordModel()
{
    int num = m_MsgLog.count();
    if(num > 0)
    {
        for(int i=0; i<num; ++i)
        {
            delete m_MsgLog.at(0);
            m_MsgLog.removeAt(0);
        }
    }
}

bool dlgSystemRecordModel::addData(CMsgLog* MsgLog)
{
    int rowcount=0;
//    CMsgLog *tempMsgLog = new CMsgLog;
//    tempMsgLog->EventTypeID = MsgLog->EventTypeID;
//    tempMsgLog->User = MsgLog->User;
//    tempMsgLog->Time = MsgLog->Time;
//    tempMsgLog->Remark = MsgLog->Remark;
//    tempMsgLog->Type = MsgLog->Type;
//    tempMsgLog->CanDeviceAddress = MsgLog->CanDeviceAddress;
//    tempMsgLog->Loop = MsgLog->Loop;
//    tempMsgLog->LampDeviceAddress = MsgLog->LampDeviceAddress;
//    tempMsgLog->Area = MsgLog->Area;
//    tempMsgLog->Location = MsgLog->Location;

//    delete MsgLog;

    beginInsertRows(QModelIndex(), rowcount, rowcount);
    m_MsgLog.append(MsgLog);
    endInsertRows();
    emit(dataChanged(index(0,0),index(rowCount()-1,m_HeaderTemplet.count()-1)));
    return true;
}

void dlgSystemRecordModel::removeData()
{
    int num = m_MsgLog.count();
    if(num > 0)
    {
        beginRemoveRows(QModelIndex(), 0, num-1);
        for(int i=0; i<num; ++i)
        {
            delete m_MsgLog.at(0);
            m_MsgLog.removeAt(0);
        }
        endRemoveRows();
    }
}

int dlgSystemRecordModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_MsgLog.count();
}

int dlgSystemRecordModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 9;
}

QVariant dlgSystemRecordModel::data(const QModelIndex &index, int role) const
{
    if(index.column()>=m_HeaderTemplet.count()) return QVariant();
    QVariant result;
    if(!index.isValid())return result;
    int row =index.row();
    int col=index.column();
    if(role== Qt::DisplayRole){
        if(col>-1&&col<9)
        {
            if(col == 0)
                result = m_MsgLog[row]->User;
            else if(col == 1)
                result = m_MsgLog[row]->Type;
            else if(col == 2)
                result = m_MsgLog[row]->Time.toString("yyyy-MM-dd HH:mm:ss");
            else if(col == 3 && m_MsgLog[row]->User != "系统")
            {
                if(m_MsgLog[row]->User == "火警点")
                    result = m_MsgLog[row]->CanDeviceAddress;
                else
                    result = "CAN" + QString::number(m_MsgLog[row]->Canport) + "-" + QString::number(m_MsgLog[row]->CanDeviceAddress);
            }

            else if(col == 4 && m_MsgLog[row]->User != "系统" && m_MsgLog[row]->User != "配电箱"
                    && m_MsgLog[row]->User != "A型集中电源" && m_MsgLog[row]->User != "B型集中电源"
                    && m_MsgLog[row]->User != "集中电源(MA)" && m_MsgLog[row]->User != "集中电源")
                result = m_MsgLog[row]->Loop;
            else if(col == 5 && (m_MsgLog[row]->User == "灯具" || m_MsgLog[row]->User == "火警点"))
                result = m_MsgLog[row]->LampDeviceAddress;
            else if(col == 6)
                result = m_MsgLog[row]->Area;
            else if(col == 7)
                result = m_MsgLog[row]->Location;
            else if(col == 8)
                result = m_MsgLog[row]->Remark;
        }
    }
    if(role == Qt::TextAlignmentRole){
        if(col < 6)
        {
            return Qt::AlignCenter;
        }
    }
    return result;
}

QVariant dlgSystemRecordModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;
    if(role== Qt::DisplayRole)
    {
        if(orientation==Qt::Horizontal && section<m_HeaderTemplet.count())
        {
            result=m_HeaderTemplet[section];
        }
        else if(orientation==Qt::Vertical)
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
