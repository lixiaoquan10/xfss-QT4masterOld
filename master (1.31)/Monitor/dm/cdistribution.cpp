#include "cdistribution.h"
#include "cproject.h"
#include "ccanport.h"
#include "cloop.h"
#include "cdbsqlite.h"
#include "cdm.h"
#include "cglobal.h"
#include "widget/cprogramtreeview.h"

#include <QSqlRecord>
#include <QDebug>

const QString CDistribution::type_s = "CDistribution";

CDistribution::CDistribution(const int keyId) :
    CObject(keyId), m_deviceOnlineTotal(0)
{
    initDistribution();
}

CDistribution::CDistribution(CCanport *canport, const int keyId) :
    CObject(keyId), m_deviceOnlineTotal(0)
{
    initDistribution();
    int n = canport->maxDistributionId() + 1;
    setDistributionId(n);
    canport->addDistribution(this);
    setDistributionValue(DISTRIBUTION_VALUE_NAME, distributionValue(DISTRIBUTION_VALUE_NAME).toString().append(QString::number(n)));
}

CDistribution::~CDistribution()
{
    if(CGlobal::instance()->dm()->isClearing())
        return;
    if(m_canport)
        m_canport->removeDistribution(this);
    for(int i = 0; i < m_loops.size(); ++i)
        m_loops.at(i)->setDistribution(NULL);
}

bool CDistribution::setDistributionValue(QString name, QVariant value)
{
    QVariant old;
    if(name == OBJECT_VALUE_ONLINE)
    {
        CGlobal::instance()->programTreeView()->resetControllerTreeView();
    }
    if(m_distributionValue.contains(name))
    {
        old = m_distributionValue.value(name);
        if(value.isNull())
        {
            m_distributionValue.remove(name);
            return true;
        }
        if(old.type() != value.type() || old == value)
            return false;
        m_distributionValue[name] = value;
        return true;
    }
    else
    {
        m_distributionValue.insert(name, value);
        return true;
    }
}

QVariant CDistribution::distributionValue(const QString name) const
{
    return m_distributionValue.value(name);
}

void CDistribution::createTable(CDBSqlite *db)
{
    db->query("CREATE TABLE IF NOT EXISTS CanDeviceInfo ("
                  "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "CanPort INTEGER,"
                  "Address INTEGER,"
                  "CanDeviceTypeID INTEGER,"
                  "Area VARCHAR(255),"
                  "Location VARCHAR(255)"
              ");").exec();
    db->query("CREATE UNIQUE INDEX IF NOT EXISTS idx_CanDeviceInfo ON CanDeviceInfo ("
                  "ID ASC,"
              ");").exec();
}

void CDistribution::clearTable(CDBSqlite *db)
{
    db->query("drop table if exists distribution").exec();
}

int CDistribution::loadDistribution(CDBSqlite *db, CCanport *canport)
{
    QSqlQuery query = db->query(QString("SELECT l.ID,l.CanPort,l.Address,r.typeName, l.Area,l.Location "
                                        "FROM CanDeviceInfo l left join candevicetype r "
                                        " on l.CanDeviceTypeId = r.ID "
                                        " where l.CanPort = %1").arg(canport->canportValue(CANPORT_VALUE_PORT).toInt()));

    QSqlRecord record = query.record();
    int fieldKeyId = record.indexOf("ID");
    int fieldCanPortId = record.indexOf("CanPort");
    int fieldTypeName = record.indexOf("typeName");
    int fieldAddressId = record.indexOf("Address");
    int fieldAreaId = record.indexOf("Area");
    int fieldLocationId = record.indexOf("Location");
    while(query.next())
    {
        CDistribution* distribution = new CDistribution(CGlobal::instance()->loadKeyId(OBJT_Distribution,query.value(fieldKeyId).toInt()));
        distribution->setDistributionValue(DISTRIBUTION_VALUE_KEYID,query.value(fieldKeyId).toInt());
        distribution->setDistributionValue(DISTRIBUTION_PORT_ID, query.value(fieldCanPortId).toInt());
        distribution->setDistributionValue(DISTRIBUTION_VALUE_ID, query.value(fieldAddressId).toInt());
        distribution->setDistributionValue(DISTRIBUTION_VALUE_ADDR, query.value(fieldAddressId).toInt());
        distribution->setDistributionValue(DISTRIBUTION_VALUE_TYPENAME, query.value(fieldTypeName));
        distribution->setDistributionValue(DISTRIBUTION_VALUE_AREA, query.value(fieldAreaId));
        distribution->setDistributionValue(DISTRIBUTION_VALUE_LOCATION, query.value(fieldLocationId));
        distribution->setDistributionValue(DISTRIBUTION_VALUE_NAME, "集中电源");
        canport->addDistribution(distribution);
        distribution->setDistributionValue(OBJECT_VALUE_ONLINE,1);
        if(CLoop::loadLoop(db, distribution))
            return -1;
    }
    return 0;
}

CObject* CDistribution::parent()
{
    return m_canport;
}

QString CDistribution::codeStr() const
{
    return distributionValue(DISTRIBUTION_VALUE_NAME).toString() + tr("%1").arg(distributionAddress());
}

QString CDistribution::batteryStatus()
{
    if(m_backupPowerFault)
        return "断线";
    if(m_batUndervoltage || m_overDischargeFault)
        return "欠压";
    return "正常";
}


bool CDistribution::addLoop(CLoop *loop)
{
    if(!loop)
        return false;
    if(m_loops.contains(loop))
        return false;
    if(hasLoop(loop->loopId()))
        return false;
    if(loop->distribution())
        loop->distribution()->removeLoop(loop);
    m_loops.append(loop);
    loop->setDistribution(this);
    if(this->isUndefined())
        loop->setIsUndefined(true);
    m_childs.append(loop);
    addDeviceTypeAndOnline(loop);
    calcMaxLoopId(loop->loopId());
    emit objectChanged(CObject::OT_AddChildren, loop);
    return true;
}

void CDistribution::removeLoop(CLoop *loop)
{
    if(!loop)
        return;
    if(!m_loops.contains(loop))
        return;
    m_loops.removeOne(loop);
    loop->setDistribution(NULL);
    m_childs.removeOne(loop);
    removeDeviceTypeAndOnline(loop);
    setModify();
    emit objectChanged(CObject::OT_RemoveChildren, loop);
}

bool CDistribution::hasLoop(const int id) const
{
    return loopById(id);
}

CLoop *CDistribution::loopById(const int id) const
{
    for(int i = 0; i < m_loops.size(); ++i)
    {
        if(m_loops.at(i)->loopId() == id)
            return m_loops.at(i);
    }
    return NULL;
}

CLoop *CDistribution::loopByAdd(const int add) const
{
    for(int i = 0; i < m_loops.size(); ++i)
    {
        if(m_loops.at(i)->loopAdd() == add)
            return m_loops.at(i);
    }
    return NULL;
}

bool CDistribution::loopByKeyId(const int keyid) const
{
    CCanport* canport = CGlobal::instance()->controller()->canportById(CGlobal::instance()->panelAddress());
    if(!canport)
        return false;
    QList<CDistribution*> list = canport->distributions();
    for(int i=0; i<list.count(); i++){
        CDistribution* distribution = list.at(i);
        QList<CLoop*> loops = distribution->loops();
        for(int i=0; i<loops.count(); i++){
            CLoop* loop = loops.at(i);
                if(loop->keyId() == keyid)
                    return false;
        }
        loops.clear();
    }
    list.clear();
    return true;
}

void CDistribution::setDeviceTypeNumber(int type, int n)
{
    if(m_deviceTypeNumber[type] == n)
        return;
    int adj = n - m_deviceTypeNumber[type];
    adjustedDeviceTypeNumber(type, adj);
}

void CDistribution::adjustedDeviceTypeNumber(int type, int adj)
{
    m_deviceTypeNumber[type] += adj;
    if(m_canport)
        m_canport->adjustedDeviceTypeNumber(type, adj);
}

void CDistribution::setDeviceOnlineNumber(int type, int n)
{
    if(m_deviceOnlineNumber[type] == n)
        return;
    int adj = n - m_deviceOnlineNumber[type];
    adjustedDeviceOnlineNumber(type, adj);
}

void CDistribution::adjustedDeviceOnlineNumber(int type, int adj)
{
    m_deviceOnlineNumber[type] += adj;
    if(m_canport)
        m_canport->adjustedDeviceOnlineNumber(type, adj);
}

void CDistribution::setDeviceOnlineTotal(int n)
{
    if(m_deviceOnlineTotal == n)
        return;
    int adj = n - m_deviceOnlineTotal;
    adjustedDeviceOnlineTotal(adj);
}

void CDistribution::adjustedDeviceOnlineTotal(int adj)
{
    m_deviceOnlineTotal += adj;
    if(m_canport)
        m_canport->adjustedDeviceOnlineTotal(adj);
    setValue(OBJECT_VALUE_ONLINE_COUNT, m_deviceOnlineTotal);
}

void CDistribution::resetDeviceOnlineTotal()
{
    for(int i = 0; i < m_loops.size(); ++i)
        m_loops.at(i)->resetDeviceOnlineTotal();
    m_deviceOnlineNumber.clear();
    m_deviceOnlineTotal = 0;
    setValue(OBJECT_VALUE_ONLINE_COUNT, m_deviceOnlineTotal);
}

void CDistribution::afterSetValue(QString &name, QVariant& oldValue, QVariant& newValue)
{
    if(CGlobal::instance()->dm()->isLoading())
        return;
    if(name == DISTRIBUTION_VALUE_ID)
    {
        if(m_canport)
            m_canport->calcMaxDistributionId(newValue.toInt());
    }
    CObject::afterSetValue(name, oldValue, newValue);
}

QVariant CDistribution::afterGetValue(QString name, QVariant value) const
{
    if(name == OBJECT_VALUE_DISPLAY_NAME)
        return m_distributionValue.value(DISTRIBUTION_VALUE_NAMEDETAIL);
    else if(name == OBJECT_VALUE_ADDRESS)
        return m_distributionValue.value(DISTRIBUTION_VALUE_ID);
    else if(name == OBJECT_VALUE_DESCRIPTION)
        return m_distributionValue.value(DISTRIBUTION_VALUE_DESCRIPTION);
    else if(name == OBJECT_VALUE_ONLINE_COUNT)
        return m_deviceOnlineTotal;
    else
        return value;
}

void CDistribution::afterSetStatus(short status, unsigned char value)
{
    if(status == OBJS_Online)
    {
        if(m_canport)
        {
            if(value)
                m_canport->adjustedOnlineDistributionCount(1);
            else
                m_canport->adjustedOnlineDistributionCount(-1);
        }
    }
}

void CDistribution::initDistribution()
{
    m_canport = NULL;
    m_maxLoopId = 0;
    m_outA = 0;
    m_linkageswitch = false;
    m_CANfaultswitch = false;
    m_linkageswitch = 0;
    m_powermode = 0;
    m_firetime = 0;
    m_unfiretime = 0;
    m_loopCommunication = 0;
    m_loopOpen = 0;

    m_mainPowerFault = false;
    m_backupPowerFault = false;
    m_overOut = false;
    m_outOpen = false;
    m_chargeStatus = false;
    m_emergencyStatus = false;
    m_36vOut = false;
    m_systemFault = false;
    m_runMode = false;
    m_overDischargeFault = false;
    m_batUndervoltage = false;
    m_batOverheating = false;

    m_loopOutStatus = false;

    m_bat1Undervoltage = false;
    m_bat2Undervoltage = false;
    m_bat3Undervoltage = false;
    m_chargeFault = false;

    setDistributionValue(OBJECT_VALUE_PIXMAP, ":/images/distribution.png");
    setDistributionValue(DISTRIBUTION_VALUE_NAME, "集中电源");
    setDistributionValue(OBJECT_VALUE_ONLINE_COUNT, 0);
}

int CDistribution::saveDistribution(CDBSqlite *db)
{
    int distributionkeyId = CGlobal::instance()->saveKeyId(OBJT_Distribution,keyId());
    QSqlQuery query = db->query("REPLACE INTO CanDeviceInfo (ID, CanPort, Address, CanDeviceTypeID, Area, Location) "
                                "VALUES (:ID, :CanPort, :Address, :CanDeviceTypeID, :Area, :Location)");
    query.bindValue(":ID", distributionkeyId);
    query.bindValue(":CanPort", 1);
    query.bindValue(":Address", distributionValue(DISTRIBUTION_VALUE_ADDR).toInt());
    query.bindValue(":CanDeviceTypeID", 1);
    query.bindValue(":Area", " ");
    query.bindValue(":Location", " ");
    if(distributionValue(DISTRIBUTION_VALUE_AREA).toString() == "")
        query.bindValue(":Area", " ");
    else
        query.bindValue(":Area", distributionValue(DISTRIBUTION_VALUE_AREA).toString());
    if(distributionValue(DISTRIBUTION_VALUE_LOCATION).toString() == "")
        query.bindValue(":Location", " ");
    else
        query.bindValue(":Location", distributionValue(DISTRIBUTION_VALUE_LOCATION).toString());
    qDebug() << "distribution========" << "distributionID:"<< distributionkeyId << "distributionAdd:"<< distributionValue(DISTRIBUTION_VALUE_ADDR).toInt();
    if(!query.exec())
        return -1;
    if(saveLoop(db) != 0)
        return -1;
    return 0;
}

int CDistribution::saveLoop(CDBSqlite *db)
{
    for(int i = 0; i < m_loops.size(); ++i)
    {
        if(m_loops.at(i)->save(db) != 0)
            return -1;
    }
    return 0;
}

void CDistribution::addDeviceTypeAndOnline(CLoop *loop)
{
    QHash<int, int> tmp = loop->getDeviceTypeNumber();
    QHash<int, int>::const_iterator iter = tmp.constBegin();
    while(iter != tmp.constEnd())
    {
        adjustedDeviceTypeNumber(iter.key(), iter.value());
        ++iter;
    }
    tmp = loop->getDeviceOnlineNumber();
    iter = tmp.constBegin();
    int n = 0;
    while(iter != tmp.constEnd())
    {
        n += iter.value();
        adjustedDeviceOnlineNumber(iter.key(), iter.value());
        ++iter;
    }
    adjustedDeviceOnlineTotal(n);
}

void CDistribution::removeDeviceTypeAndOnline(CLoop *loop)
{
    QHash<int, int> tmp = loop->getDeviceTypeNumber();
    QHash<int, int>::const_iterator iter = tmp.constBegin();
    while(iter != tmp.constEnd())
    {
        adjustedDeviceTypeNumber(iter.key(), -iter.value());
        ++iter;
    }
    tmp = loop->getDeviceOnlineNumber();
    iter = tmp.constBegin();
    int n = 0;
    while(iter != tmp.constEnd())
    {
        n += iter.value();
        adjustedDeviceOnlineNumber(iter.key(), -iter.value());
        ++iter;
    }
    adjustedDeviceOnlineTotal(-n);
}
