#include "cprocessbase.h"
#include "struct/cmsgnull.h"
#include "struct/cmsgobjectstatus.h"
#include "struct.h"
#include "cglobal.h"

#include <QDebug>

CMsgNull* CProcessBase::m_msgNull = new CMsgNull();

CProcessBase::CProcessBase(const QString &name) :
    QObject(), m_name(name), m_timeOut(0)
{
}

CProcessBase::~CProcessBase()
{
    qDebug("~CProcessBase");
}

void CProcessBase::recvEvent(const QString &processname, const int &infoType, const QHash<QString, QVariant> &control, const QByteArray &data)
{
    if(processname != m_name)
        return;
    procRecvEvent(infoType, control, data);
}

int CProcessBase::firstCardPort() const
{
    if(m_cardPorts.isEmpty())
        return 0;
    QSet<int>::const_iterator iter = m_cardPorts.constBegin();
    return *iter;
}


//////////////
///ntype 1:panel status
/////////2:distribution status
/////////3:power status
/////////4:device status
/////////5:loop status
void CProcessBase::uploadStatus(int ntype, int nCanportAdd, int nDisID, int nLoopID, int nDeviceID, int nStatus, int nValue, int nTimes, QString strtext, int nLoginStatus, int nCommunicationStatus, QString strdata)
{
    qDebug() << "uploadStatus" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgObjectStatus objectStatus;
    objectStatus.nType = ntype;
    objectStatus.nCanportAdd = nCanportAdd + 2;
    objectStatus.nDisID = nDisID;
    objectStatus.nLoopID = nLoopID;
    objectStatus.nDeviceID = nDeviceID;
    objectStatus.nStatusID = nStatus;
    objectStatus.nValue = nValue;
    objectStatus.nTimes = nTimes;
    objectStatus.strText = strtext;
    objectStatus.nLoginStatus = nLoginStatus;
    objectStatus.nCommunicationStatus = nCommunicationStatus;
    objectStatus.strdata = strdata;
    CGlobal::instance()->DealEvent(NCT_ObjectStatus, objectStatus.data(NCT_ObjectStatus));
}
