#include "cglobal.h"


const QString CCanport::type_s = "CCanport";

CCanport::CCanport(const int keyId) :
    CObject(keyId), m_onlineDistributionCount(0), m_onlinePowerCount(0), m_deviceOnlineTotal(0), m_onlineTotalCount(0)
{
    initCanport();
}

CCanport::CCanport(CController *controller, const int keyId) :
    CObject(keyId), m_onlineDistributionCount(0), m_onlinePowerCount(0), m_deviceOnlineTotal(0), m_onlineTotalCount(0)
{
    initCanport();
    int n = controller->maxCanportId() + 1;
    setCanportId(n);
    controller->addCanport(this);
    setCanportValue(CANPORT_VALUE_NAME, canportValue(CANPORT_VALUE_NAME).toString().append(QString::number(n)));
}

CCanport::~CCanport()
{
    if(CGlobal::instance()->dm()->isClearing())
        return;
    if(m_controller)
        m_controller->removeCanport(this);
    for(int i = 0; i < m_distributions.size(); ++i)
        m_distributions.at(i)->setCanport(NULL);
}

bool CCanport::setCanportValue(QString name, QVariant value)
{
    qDebug() << "CCanport::setCanportValue"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QVariant old;
//    if(m_canportValue.value(name) != value && !CGlobal::instance()->m_bInLogin)
//    {
//        CGlobal::instance()->programTreeView()->resetControllerTreeView();
//    }
    if(m_canportValue.contains(name))
    {
        old = m_canportValue.value(name);
        if(value.isNull())
        {
            m_canportValue.remove(name);
            return true;
        }
        if(old.type() != value.type() || old == value)
            return false;
        m_canportValue[name] = value;
        return true;
    }
    else
    {
        m_canportValue.insert(name, value);
        return true;
    }
}

QVariant CCanport::canportValue(const QString name) const
{
    return m_canportValue.value(name);
}

void CCanport::createTable(CDBSqlite *db)
{
    qDebug() << "CCanport::createTable"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    db->query("CREATE TABLE IF NOT EXISTS canport ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "keyid INTEGER,"
                    "parent_keyid INTEGER,"
                    "canport_id INTEGER,"
                    "canport_name VARCHAR(255),"
                    "canport_description VARCHAR(255)"
                ");").exec();
    db->query("CREATE UNIQUE INDEX IF NOT EXISTS idx_canport ON canport ("
                    "parent_keyid ASC,"
                    "canport_id ASC"
                ");").exec();
}

void CCanport::clearTable(CDBSqlite *db)
{
    qDebug() << "CCanport::clearTable"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    db->query("drop table if exists canport").exec();
}

int CCanport::loadCanport(CDBSqlite *db, CController *controller)
{
    qDebug() << "CCanport::loadCanport"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i=0; i<CGlobal::instance()->m_nCanNumber; i++)
    {
        CCanport* canport = new CCanport(63+i);
        canport->setCanportValue(CANPORT_VALUE_ID,63+i);
        canport->setCanportValue(CANPORT_VALUE_PORT, i+1);     //can1-can4
        canport->setCanportValue(CANPORT_VALUE_ADDRESS, i+3);  //can1-can4对应3-6
        canport->setCanportValue(CANPORT_VALUE_AREA, "");
        canport->setCanportValue(CANPORT_VALUE_LOCATION, "");
        canport->setCanportValue(CANPORT_VALUE_NAME, "CAN" + QString::number(i+1));
        canport->setNone();
        controller->addCanport(canport);
        canport->setCanportValue(OBJECT_VALUE_ONLINE,1);

        if(CDistribution::loadDistribution(db, canport))
            return -1;
    }
    return 0;
}

CObject* CCanport::parent()
{
    return m_controller;
}

bool CCanport::addDistribution(CDistribution *distribution)
{
    qDebug() << "CCanport::addDistribution"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!distribution)
        return false;
    if(m_distributions.contains(distribution))
        return false;
    if(hasDistribution(distribution->distributionProductId()))
        return false;
    if(distribution->canport())
        distribution->canport()->removeDistribution(distribution);
    m_distributions.append(distribution);
    distribution->setCanport(this);
    if(this->isUndefined())
        distribution->setIsUndefined(true);
    m_childs.append(distribution);
    addDeviceTypeAndOnline(distribution);
    calcMaxDistributionId(distribution->distributionId());
    emit objectChanged(CObject::OT_AddChildren, distribution);
    return true;
}

void CCanport::removeDistribution(CDistribution *distribution)
{
    qDebug() << "CCanport::removeDistribution"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!distribution)
        return;
    if(!m_distributions.contains(distribution))
        return;
    m_distributions.removeOne(distribution);
    distribution->setCanport(NULL);
    m_childs.removeOne(distribution);
    removeDeviceTypeAndOnline(distribution);
    setModify();
    emit objectChanged(CObject::OT_RemoveChildren, distribution);
}

bool CCanport::hasDistribution(const QString str) const
{
    return distributionByProductId(str);
}

CDistribution *CCanport::distributionById(const int id) const
{
    qDebug() << "CCanport::distributionById"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_distributions.size(); ++i)
    {
        if(m_distributions.at(i)->distributionId() == id)
            return m_distributions.at(i);
    }
    return NULL;
}

CDistribution *CCanport::distributionByAddress(const int add) const
{
    qDebug() << "CCanport::distributionByAddress"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_distributions.size(); ++i)
    {
        if(m_distributions.at(i)->distributionAddress() == add)
            return m_distributions.at(i);
    }
    return NULL;
}

CDistribution *CCanport::distributionByProductId(const QString str) const
{
    qDebug() << "CCanport::distributionByProductId"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_distributions.size(); ++i)
    {
        if(m_distributions.at(i)->descriptionStr() == str && str != "")
            return m_distributions.at(i);
    }
    return NULL;
}

CDistribution *CCanport::distributionByKeyId(const int keyid) const
{
    qDebug() << "CCanport::creatdistributionByKeyIdeTable"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_distributions.size(); ++i)
    {
        if(m_distributions.at(i)->keyId() == keyid)
            return m_distributions.at(i);
    }
    return NULL;
}

bool CCanport::isdistributionByKeyId(const int keyid) const
{
    qDebug() << "CCanport::isdistributionByKeyId"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_distributions.size(); ++i)
    {
        if(m_distributions.at(i)->keyId() == keyid)
            return false;
    }
    return true;
}




void CCanport::setDeviceTypeNumber(int type, int n)
{
    qDebug() << "CCanport::setDeviceTypeNumber"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_deviceTypeNumber[type] == n)
        return;
    int adj = n - m_deviceTypeNumber[type];
    adjustedDeviceTypeNumber(type, adj);
}

void CCanport::adjustedDeviceTypeNumber(int type, int adj)
{
    qDebug() << "CCanport::adjustedDeviceTypeNumber"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_deviceTypeNumber[type] += adj;
    if(m_controller)
        m_controller->adjustedDeviceTypeNumber(type, adj);
    emit deviceTypeChanged();
}

void CCanport::setDeviceOnlineNumber(int type, int n)
{
    qDebug() << "CCanport::setDeviceOnlineNumber"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_deviceOnlineNumber[type] == n)
        return;
    int adj = n - m_deviceOnlineNumber[type];
    adjustedDeviceOnlineNumber(type, adj);
}

void CCanport::adjustedDeviceOnlineNumber(int type, int adj)
{
    qDebug() << "CCanport::adjustedDeviceOnlineNumber"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_deviceOnlineNumber[type] += adj;
    if(m_controller)
        m_controller->adjustedDeviceOnlineNumber(type, adj);
}

void CCanport::setDeviceOnlineTotal(int n)
{
    qDebug() << "CCanport::setDeviceOnlineTotal"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_deviceOnlineTotal == n)
        return;
    int adj = n - m_deviceOnlineTotal;
    adjustedDeviceOnlineTotal(adj);
}

void CCanport::adjustedDeviceOnlineTotal(int adj)
{
    qDebug() << "CCanport::adjustedDeviceOnlineTotal"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_deviceOnlineTotal += adj;
    if(m_controller)
        m_controller->adjustedOnlineCount(adj);
    updateOnlineTotalCount();
}

void CCanport::setOnlineDistributionCount(int n)
{
    qDebug() << "CCanport::setOnlineDistributionCount"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_onlineDistributionCount == n)
        return;
    int adj = n - m_onlineDistributionCount;
    adjustedOnlineDistributionCount(adj);
}

void CCanport::adjustedOnlineDistributionCount(int adj)
{
    qDebug() << "CCanport::adjustedOnlineDistributionCount"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_onlineDistributionCount += adj;
    if(m_controller)
        m_controller->adjustedOnlineCount(adj);
    updateOnlineTotalCount();
}

void CCanport::setOnlinePowerCount(int n)
{
    qDebug() << "CCanport::setOnlinePowerCount"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_onlinePowerCount == n)
        return;
    int adj = n - m_onlinePowerCount;
    adjustedOnlinePowerCount(adj);
}

void CCanport::adjustedOnlinePowerCount(int adj)
{
    qDebug() << "CCanport::adjustedOnlinePowerCount"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_onlinePowerCount += adj;
    if(m_controller)
        m_controller->adjustedOnlineCount(adj);
    updateOnlineTotalCount();
}

void CCanport::resetOnline()
{
    qDebug() << "CCanport::resetOnline"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_distributions.size(); ++i)
        m_distributions.at(i)->resetDeviceOnlineTotal();
    m_deviceOnlineNumber.clear();
    m_onlineDistributionCount = 0;
    m_onlinePowerCount = 0;
    m_deviceOnlineTotal = 0;
    updateOnlineTotalCount();
}

void CCanport::afterSetValue(QString &name, QVariant& oldValue, QVariant& newValue)
{
    if(CGlobal::instance()->dm()->isLoading())
        return;
    if(name == CANPORT_VALUE_ID)
    {
        if(m_controller)
            m_controller->calcMaxCanportId(newValue.toInt());
    }
    CObject::afterSetValue(name, oldValue, newValue);
}

QVariant CCanport::afterGetValue(QString name, QVariant value) const
{
    if(name == OBJECT_VALUE_DISPLAY_NAME)
        return m_canportValue.value(CANPORT_VALUE_NAME);
    else if(name == OBJECT_VALUE_ADDRESS)
        return m_canportValue.value(CANPORT_VALUE_ID);
    else if(name == OBJECT_VALUE_DESCRIPTION)
        return m_canportValue.value(CANPORT_VALUE_DESCRIPTION);
    else if(name == OBJECT_VALUE_ONLINE_COUNT)
        return m_onlineTotalCount;
    else
        return value;
}

void CCanport::initCanport()
{
    qDebug() << "CCanport::initCanport"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_controller = NULL;
    m_maxDistributionId = 0;
    setCanportValue(OBJECT_VALUE_PIXMAP, ":/images/CanDeviceConfig.png");
    setCanportValue(CANPORT_VALUE_NAME, "CAN");
    setValue(OBJECT_VALUE_ONLINE_COUNT, 0);
}


int CCanport::saveCanport(CDBSqlite *db)
{
    qDebug() << "CCanport::saveCanport"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(isUndefined())
        return 0;
    if(saveDistribution(db) != 0)
        return -1;
    return 0;
}

int CCanport::saveDistribution(CDBSqlite *db)
{
    qDebug() << "CCanport::saveDistribution"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_distributions.size(); ++i)
    {
        if(m_distributions.at(i)->save(db) != 0)
            return -1;
    }
    return 0;
}



void CCanport::updateOnlineTotalCount()
{
    qDebug() << "CCanport::updateOnlineTotalCount"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_onlineTotalCount = m_deviceOnlineTotal;
    setValue(OBJECT_VALUE_ONLINE_COUNT, m_onlineTotalCount);
    emit onlineChanged();
}

void CCanport::addDeviceTypeAndOnline(CDistribution *distribution)
{
    qDebug() << "CCanport::addDeviceTypeAndOnline"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QHash<int, int> tmp = distribution->getDeviceTypeNumber();
    QHash<int, int>::const_iterator iter = tmp.constBegin();
    while(iter != tmp.constEnd())
    {
        adjustedDeviceTypeNumber(iter.key(), iter.value());
        ++iter;
    }
    tmp = distribution->getDeviceOnlineNumber();
    iter = tmp.constBegin();
    int n = 0;
    while(iter != tmp.constEnd())
    {
        n += iter.value();
        adjustedDeviceOnlineNumber(iter.key(), iter.value());
        ++iter;
    }
    adjustedOnlineDistributionCount(n);
}

void CCanport::removeDeviceTypeAndOnline(CDistribution *distribution)
{
    qDebug() << "CCanport::removeDeviceTypeAndOnline"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QHash<int, int> tmp = distribution->getDeviceTypeNumber();
    QHash<int, int>::const_iterator iter = tmp.constBegin();
    while(iter != tmp.constEnd())
    {
        adjustedDeviceTypeNumber(iter.key(), -iter.value());
        ++iter;
    }
    tmp = distribution->getDeviceOnlineNumber();
    iter = tmp.constBegin();
    int n = 0;
    while(iter != tmp.constEnd())
    {
        n += iter.value();
        adjustedDeviceOnlineNumber(iter.key(), -iter.value());
        ++iter;
    }
    adjustedOnlineDistributionCount(-n);
}

