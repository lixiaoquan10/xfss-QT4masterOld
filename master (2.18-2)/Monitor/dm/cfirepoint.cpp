#include "cfirepoint.h"
#include "ccontroller.h"
#include "cdbsqlite.h"
#include "cdm.h"
#include "cglobal.h"

#include <QSqlRecord>
#include <QDebug>

const QString CFirePoint::type_s = "CFirePoint";

CFirePoint::CFirePoint(const int keyId) :
    CObject(keyId)
{
    initFirePoint();
}

CFirePoint::CFirePoint(CController *controller, const int keyId) :
    CObject(keyId)
{
    initFirePoint();
    int n = controller->maxFirePointId() + 1;
    setFirePointId(n);
    controller->addFirePoint(this);
}

CFirePoint::~CFirePoint()
{
    if(CGlobal::instance()->dm()->isClearing())
        return;
    if(m_controller)
        m_controller->removeFirePoint(this);
}

void CFirePoint::createTable(CDBSqlite *db)
{
    db->query("CREATE TABLE IF NOT EXISTS firepoint ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "Id INTEGER,"
                  "Device INTEGER,"
                  "Loop INTEGER,"
                  "Address INTEGER,"
                  "Area VARCHAR(255),"
                  "Location VARCHAR(255)"
              ");").exec();
    db->query("CREATE UNIQUE INDEX IF NOT EXISTS idx_firepoint ON firepoint ("
                  "parent_keyid ASC,"
                  "firepoint_id ASC"
              ");").exec();
}

void CFirePoint::clearTable(CDBSqlite *db)
{
    db->query("drop table if exists firepoint").exec();
}

int CFirePoint::loadFirePoint(CDBSqlite *db, CController *controller)
{
    QSqlQuery query = db->query(QString("SELECT id, device, loop, address, area, location FROM firepoint"));
    QSqlRecord record = query.record();
    int filedId = record.indexOf("id");
    int filedFirePointDeviceAddress = record.indexOf("device");
    int filedFirePointLoopAddress = record.indexOf("loop");
    int filedFirePointAddress = record.indexOf("address");
    int filedFirePointArea = record.indexOf("area");
    int filedFirePointLocation = record.indexOf("location");
    while(query.next())
    {
        CFirePoint* firepoint = new CFirePoint(query.value(filedId).toInt());
        firepoint->setFirePointValue(FIREPOINT_VALUE_ID, query.value(filedId).toInt());
        firepoint->setFirePointValue(FIREPOINT_VALUE_ADDRESS, query.value(filedFirePointAddress).toInt());
        firepoint->setFirePointValue(FIREPOINT_VALUE_DEVICEADDRESS, query.value(filedFirePointDeviceAddress).toInt());
        firepoint->setFirePointValue(FIREPOINT_VALUE_LOOPADDRESS, query.value(filedFirePointLoopAddress).toInt());
        firepoint->setFirePointValue(FIREPOINT_VALUE_AREA, query.value(filedFirePointArea).toString());
        firepoint->setFirePointValue(FIREPOINT_VALUE_LOCATION, query.value(filedFirePointLocation).toString());
        controller->addFirePoint(firepoint);
    }
    return 0;
}

bool CFirePoint::setFirePointValue(QString name, QVariant value)
{
    QVariant old;
    if(m_firePointValue.contains(name))
    {
        old = m_firePointValue.value(name);
        if(value.isNull())
        {
            m_firePointValue.remove(name);
            return true;
        }
        if(old.type() != value.type() || old == value)
            return false;
        m_firePointValue[name] = value;
        return true;
    }
    else
    {
        m_firePointValue.insert(name, value);
        return true;
    }
}

QVariant CFirePoint::firePointValue(const QString name) const
{
    return m_firePointValue.value(name);
}

CObject* CFirePoint::parent()
{
    return m_controller;
}

QString CFirePoint::codeStr() const
{
    if(m_controller)
        return m_controller->codeStr().append(tr("AlarmPoint %1").arg(address()));
    else
        return tr("AlarmPoint %1").arg(address());
}

bool CFirePoint::setAddress(const int address)
{
    setFirePointValue(FIREPOINT_VALUE_ADDRESS, address);
}

void CFirePoint::afterSetValue(QString &name, QVariant &oldValue, QVariant &newValue)
{
    if(CGlobal::instance()->dm()->isLoading())
        return;
    if(name == FIREPOINT_VALUE_ID)
    {
        if(m_controller)
            m_controller->calcMaxFirePointId(newValue.toInt());
    }
    CObject::afterSetValue(name, oldValue, newValue);
}

QVariant CFirePoint::afterGetValue(QString name, QVariant value) const
{
    if(name == OBJECT_VALUE_DISPLAY_NAME)
        return m_value.value("firepoint_name");
    else
        return value;
}

void CFirePoint::afterSetStatus(short status, unsigned char value)
{
    if(status == OBJS_Fire)
    {
//        if(value)
//        {
//            if(!CGlobal::instance()->dm()->firstFireAlarmPoint())
//            {
//                setStatus(OBJS_FirstFire, 1);
//                CGlobal::instance()->dm()->setFirstFireAlarmPoint(this);
//            }
//        }
//        else
//        {
//            if(CGlobal::instance()->dm()->firstFireAlarmPoint() == this)
//            {
//                setStatus(OBJS_FirstFire, 0);
//                CGlobal::instance()->dm()->setFirstFireAlarmPoint(NULL);
//            }
//        }
    }
    CFirePoint* object = reinterpret_cast<CFirePoint*>(0);
    object->afterSetStatus(status, value);
}

void CFirePoint::initFirePoint()
{
    m_controller = NULL;
    int n = keyId();
    while(CGlobal::instance()->dm()->hasFirePoint(QString::number(n)))
    {
        ++n;
    }
    setAddress(n);
}

int CFirePoint::saveFirePoint(CDBSqlite *db)
{
    if(isUndefined())
        return 0;
    QSqlQuery query = db->query("REPLACE INTO firepoint (id, device, loop, address, area, location) "
                                        "VALUES (:id, :device, :loop, :address, :area, :location)");
    query.bindValue(":id", firePointValue(FIREPOINT_VALUE_ID));
    query.bindValue(":device", firePointValue(FIREPOINT_VALUE_DEVICEADDRESS));
    query.bindValue(":loop", firePointValue(FIREPOINT_VALUE_LOOPADDRESS));
    query.bindValue(":address", firePointValue(FIREPOINT_VALUE_ADDRESS));
    query.bindValue(":area", firePointValue(FIREPOINT_VALUE_AREA));
    query.bindValue(":location", firePointValue(FIREPOINT_VALUE_LOCATION));
    if(!query.exec())
        return -1;
    return 0;
}
