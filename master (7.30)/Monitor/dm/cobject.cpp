#include "cglobal.h"

int CObject::m_currentKeyId = 0;


CObject::CObject(const int keyId) :
    QObject(), m_isUndefined(false)
{
    m_editStatus = ES_None;
    if(keyId > 0)
        setKeyId(keyId);
    else
    {
        m_currentKeyId++;
        m_keyId = m_currentKeyId;
    }
}

CObject::~CObject()
{
    emit destrory(this);
}

QHash<short, unsigned char> CObject::compareStatusTable(const QHash<short, unsigned char> &oldStatusTable, const QHash<short, unsigned char> &newStatusTable)
{
    short status = OBJS_None;
    QHash<short, unsigned char> changedStatus;
    QHash<short, unsigned char>::const_iterator iter1 = oldStatusTable.constBegin();
    while(iter1 != oldStatusTable.constEnd())
    {
        status = iter1.key();
        if(!newStatusTable.contains(status))
            changedStatus.insert(status, OBJS_None);
        ++iter1;
    }
    QHash<short, unsigned char>::const_iterator iter2 = newStatusTable.constBegin();
    while(iter2 != newStatusTable.constEnd())
    {
        status = iter2.key();
        unsigned char oldValue = oldStatusTable.value(status, OBJS_None);
        unsigned char newValue = iter2.value();
        if(oldValue != newValue)
            changedStatus.insert(status, newValue);
        ++iter2;
    }
    return changedStatus;
}

void CObject::setEditStatus(const EditStatus status)
{
    CObject::EditStatus t = status;
    if(m_editStatus != t)
    {
        m_editStatus = t;
        emit editStatusChanged(m_editStatus);
    }
}

void CObject::setNew()
{
    setEditStatus(ES_New | ES_Modify);
}

void CObject::setModify()
{
    setEditStatus(ES_Modify);
}

void CObject::setDeleted()
{
    setEditStatus(ES_Deleted | ES_Modify);
}

void CObject::setNone()
{
    setEditStatus(ES_None);
}

bool CObject::setValue(QString name, QVariant value)
{
    if(!beforeSetValue(name, value))
        return false;
    QVariant old;
    if(m_value.contains(name))
    {
        old = m_value.value(name);
        if(value.isNull())
        {
            m_value.remove(name);
            afterSetValue(name, old, value);
            emit valueChanged(name, old, value);
            return true;
        }
        if(old.type() != value.type() || old == value)
            return false;
        m_value[name] = value;
        afterSetValue(name, old, value);
        emit valueChanged(name, old, value);
        return true;
    }
    else
    {
        m_value.insert(name, value);
        afterSetValue(name, old, value);
        emit valueChanged(name, old, value);
        return true;
    }
}

QVariant CObject::value(const QString name) const
{
    return afterGetValue(name, m_value.value(name));
}

void CObject::setStatus(short status, unsigned char value, unsigned int time)
{
    qDebug() << "setStatus" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!beforeSetStatus(status, value))
        return;
    unsigned char old = m_objectStatus.value(status, OBJS_None);
    if(value == old)
        return;
    if(value == OBJS_None)
        m_objectStatus.remove(status);
    else
        m_objectStatus[status] = value;
    if(time == 0)
        time = QDateTime::currentDateTime().toTime_t();
    m_objectStatusTime[status] = time;
    afterSetStatus(status, value);
//    emit objectStatusChanged(status, 0, value, time);
    CGlobal::instance()->ClientBusiness()->objectStatusChanged(this, status, old, value, time);
}

unsigned char CObject::getStatus(short status) const
{
    return afterGetStatus(status, m_objectStatus.value(status, OBJS_None));
}

unsigned int CObject::getStatusTime(short status) const
{
    return m_objectStatusTime.value(status, 0);
}

bool CObject::isStartStatus() const
{
    QSet<short> statusList = CGlobal::instance()->startStatusList();
    QSet<short>::const_iterator iter = statusList.constBegin();
    while(iter != statusList.constEnd())
    {
        if(getStatus(*iter))
            return true;
        ++iter;
    }
    return false;
}

bool CObject::isFaultStatus() const
{
    QSet<short> statusList = CGlobal::instance()->faultStatusList();
    QSet<short>::const_iterator iter = statusList.constBegin();
    while(iter != statusList.constEnd())
    {
        if(getStatus(*iter))
            return true;
        ++iter;
    }
    return false;
}

bool CObject::isEmergencyStatus() const
{
    QSet<short> statusList = CGlobal::instance()->emergencyStatusList();
    QSet<short>::const_iterator iter = statusList.constBegin();
    while(iter != statusList.constEnd())
    {
        if(getStatus(*iter))
            return true;
        ++iter;
    }
    return false;
}

bool CObject::isFireStatus() const
{
    if(getStatus(OBJS_Fire))
        return true;
    return false;
}

void CObject::setStatusTable(const QHash<short, unsigned char> &statusTable, const QHash<short, unsigned int> &statusTime)
{
    QHash<short, unsigned char> changedStatus = compareStatusTable(m_objectStatus, statusTable);
    QHash<short, unsigned char>::const_iterator iter = changedStatus.constBegin();
    while(iter != changedStatus.constEnd())
    {
        setStatus(iter.key(), iter.value(), statusTime.value(iter.key(), 0));
        ++iter;
    }
}

QString CObject::getStatusDes()
{
    QString strStatus = "";
    QHash<short, unsigned char>::iterator statusIter = m_objectStatus.begin();
    while(statusIter != m_objectStatus.end())
    {
        short key = statusIter.key();
        if(key == OBJS_LightCommunicationFault || key == OBJS_LightFault || key == OBJS_ManualEmergency || key == OBJS_ForceEmergency ||
                key == OBJS_DIEmergency || key == OBJS_TestEmergency || key == OBJS_SystemEmergency || key == OBJS_UndervoltageEmergency ||
                key == OBJS_AutoEmergency)
            strStatus += CGlobal::instance()->getObjectStatusStr(key);
        ++statusIter;
    }
    if(strStatus.isEmpty()) {
        if(getStatus(OBJS_Online) == 0) strStatus = "未定义";
        else  strStatus = "正常";
    }
    return strStatus;
}

QString CObject::getCommunicationStatus()
{
    QString strStatus = "";
    QHash<short, unsigned char>::iterator statusIter = m_objectStatus.begin();
    while(statusIter != m_objectStatus.end())
    {
        short key = statusIter.key();
        if(key == OBJS_LightCommunicationFault || key == OBJS_LoopCommunicationFault || key == OBJS_DistributionCommunicationFault)
            strStatus += CGlobal::instance()->getObjectStatusStr(key);
        ++statusIter;
    }
    if(strStatus.isEmpty()) {
        if(getStatus(OBJS_Online) == 0) strStatus = "通讯故障";
        else  strStatus = "正常";
    }
    return strStatus;
}

void CObject::clearStatus(bool reserveLogin)
{
    if(reserveLogin)
    {
        QHash<short, unsigned char>::iterator statusIter = m_objectStatus.begin();
        while(statusIter != m_objectStatus.end())
        {
            short key = statusIter.key();
            if(key != OBJS_Online && key != OBJS_LoginDeviceType && key != OBJS_PowerChargerValue && key != OBJS_PowerAutoValue)
                statusIter = m_objectStatus.erase(statusIter);
            else
                ++statusIter;
        }
        QHash<short, unsigned int>::iterator timeIter = m_objectStatusTime.begin();
        while(timeIter != m_objectStatusTime.end())
        {
            short key = timeIter.key();
            if(key != OBJS_Online && key != OBJS_LoginDeviceType && key != OBJS_PowerChargerValue && key != OBJS_PowerAutoValue)
                timeIter = m_objectStatusTime.erase(timeIter);
            else
                ++timeIter;
        }
    }
    else
    {
        m_objectStatus.clear();
        m_objectStatusTime.clear();
    }
//    afterSetStatus(OBJS_None, OBJS_None);
//    emit objectStatusChanged(OBJS_None, OBJS_None, OBJS_None, QDateTime::currentDateTime().toTime_t());
}
