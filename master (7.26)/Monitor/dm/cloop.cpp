#include "cglobal.h"


const QString CLoop::type_s = "CLoop";

CLoop::CLoop(const int keyId) :
    CObject(keyId), m_deviceOnlineTotal(0)
{
    initLoop();
    m_deviceOnlineTotalBak = 0;
    m_islogin = false;
    m_isOnline = false;
    m_power = 0;
}



CLoop::~CLoop()
{
    if(CGlobal::instance()->dm()->isClearing())
        return;
    if(m_distribution)
        m_distribution->removeLoop(this);
    for(int i = 0; i < m_devices.size(); ++i)
        m_devices.at(i)->setLoop(NULL);
}

bool CLoop::setloopValue(QString name, QVariant value)
{
    qDebug() << "CLoop::setloopValue" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
//    if(name == OBJECT_VALUE_ONLINE)
//    {
//        if(value == 1)
//            setDeviceOnlineTotal(m_deviceOnlineTotalBak);
//        else if(value == 0)
//            //回路灯具数量清零
//            setDeviceOnlineTotal(0, true);
//    }
    QVariant old;
    if(m_loopValue.contains(name))
    {
        old = m_loopValue.value(name);
        if(value.isNull())
        {
            m_loopValue.remove(name);
            return true;
        }
        if(old.type() != value.type() || old == value)
            return false;
        m_loopValue[name] = value;
        return true;
    }
    else
    {
        m_loopValue.insert(name, value);
        return true;
    }
}

QVariant CLoop::loopValue(const QString name) const
{
    return m_loopValue.value(name);
}

bool CLoop::getLoopCommunicationFault()
{
    qDebug() << "CLoop::getLoopCommunicationFault" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canportAdd());
    if(!canport)
        return false;
    CDistribution* distribution = canport->distributionByAddress(distributionAdd());
    if(!distribution)
        return false;
    if(!distribution->isDistributionOnline() ||  !isLoopOnline())
        return false;
    else
        return false;
}

bool CLoop::setLoopId(int id)
{
    return setloopValue(LOOP_VALUE_ID, id);
}


void CLoop::initDevices()
{
//    for(int i = 0; i < LOOP_DEVICE_NUM; ++i)
        new CDevice(this);
}

void CLoop::createTable(CDBSqlite *db)
{
    qDebug() << "CLoop::createTable" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    db->query("CREATE TABLE IF NOT EXISTS ChannelInfo ("
                  "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "Number INTEGER,"
                  "CanDeviceInfoID INTEGER,"
                  "Loop INTEGER,"
                  "Remark VARCHAR(255)"
              ");").exec();
    db->query("CREATE UNIQUE INDEX IF NOT EXISTS idx_ChannelInfo ON ChannelInfo ("
                  "CanDeviceInfoID ASC,"
                  "ID ASC"
              ");").exec();

}

void CLoop::clearTable(CDBSqlite *db)
{
    db->query("drop table if exists loop").exec();
}


int CLoop::loadLoop(CDBSqlite *db, CDistribution *distribution)
{
    qDebug() << "CLoop::loadLoop" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QSqlQuery query = db->query(QString("SELECT ID,Number,loop,remark "
                                        "FROM ChannelInfo WHERE CanDeviceInfoId = %1")
                                .arg(distribution->distributionKeyId()));
    QSqlRecord record = query.record();
    int filedKeyId = record.indexOf("ID");
    int fieldNumberId = record.indexOf("Number");
    int fieldLoopAdd = record.indexOf("loop");
//    int filedLoopName = record.indexOf("loop_name");
    int fieldRemark = record.indexOf("remark");
    while(query.next())
    {
        CLoop* loop = new CLoop(CGlobal::instance()->loadKeyId(OBJT_Loop,query.value(filedKeyId).toInt()));
        loop->setloopValue(LOOP_VALUE_ID, query.value(filedKeyId).toInt());
        loop->setloopValue(LOOP_VALUE_NUMBER, query.value(fieldNumberId).toInt());
        loop->setloopValue(LOOP_VALUE_ADDR, query.value(fieldLoopAdd).toInt());
        loop->setloopValue(LOOP_VALUE_NAME, tr("回路%1").arg(query.value(fieldLoopAdd).toInt()));
        loop->setloopValue(LOOP_VALUE_REMARK, query.value(fieldRemark));
        loop->setloopValue(LOOP_VALUE_DISTRIBUTIONADDR, distribution->distributionAddress());
        loop->setloopValue(LOOP_VALUE_CANPORTADDR, distribution->distributionValue(DISTRIBUTION_VALUE_CANPORTADDR).toInt());
        distribution->addLoop(loop);
        loop->setLoopLogin(true);
        if(CDevice::loadDevice(db, loop) != 0)
            return -1;
        if(loop->deviceCount() != 0)
        {
            loop->setLoopOnline(true);
//            loop->setDeviceOnlineTotal(loop->deviceCount());
        }

    }
    return 0;
}

CObject* CLoop::parent()
{
    return m_distribution;
}

QString CLoop::codeStr() const
{
    qDebug() << "CLoop::codeStr" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_distribution)
        return m_distribution->codeStr().append(tr("回路%1").arg(loopAdd()));
    else
        return tr("回路%1").arg(loopAdd());
}

bool CLoop::addDevice(CDevice *device)
{
    qDebug() << "CLoop::addDevice" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!device)
        return false;
    device->calcSecondCode();
    if(m_devices.contains(device))
        return false;
    if(hasDevice(device->deviceId()))
        return false;
    if(device->loop())
        device->loop()->removeDevice(device);
    m_devices.append(device);
    device->setLoop(this);
    if(this->isUndefined())
        device->setIsUndefined(true);
    m_childs.append(device);
    int type = device->deviceTypeId();
    adjustedDeviceTypeNumber(type, 1);
    if(device->isDeviceOnline() && device->isDeviceLogin())
    {
        adjustedDeviceOnlineNumber(type, 1);
        if(device->isMAtype())
            setLoopHasMALamp(true);
        else
            addLoopPower(device->returnLampPower());
    }
    calcMaxDeviceId(device->deviceId());
    emit objectChanged(CObject::OT_AddChildren, device);
    return true;
}

void CLoop::removeDevice(CDevice *device)
{
    qDebug() << "CLoop::removeDevice" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!device)
        return;
    if(!m_devices.contains(device))
        return;
    m_devices.removeOne(device);
    device->setLoop(NULL);
    m_childs.removeOne(device);
    int type = device->deviceTypeId();
    adjustedDeviceTypeNumber(type, -1);
    if(device->isDeviceOnline())
    {
        adjustedDeviceOnlineNumber(type, -1);
        addLoopPower(0-device->returnLampPower());
    }
    setModify();
    emit objectChanged(CObject::OT_RemoveChildren, device);
}

bool CLoop::hasDevice(const int id) const
{
    return deviceById(id);
}

CDevice *CLoop::deviceById(const int id) const
{
    qDebug() << "CLoop::deviceById" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_devices.size(); ++i)
    {
        if(m_devices.at(i)->deviceId() == id)
            return m_devices.at(i);
    }
    return NULL;
}

CDevice *CLoop::deviceByAdd(const int add) const
{
    qDebug() << "CLoop::deviceByAdd" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_devices.size(); ++i)
    {
        if(m_devices.at(i)->deviceAdd() == add)
            return m_devices.at(i);
    }
    return NULL;
}

CDevice *CLoop::deviceByProductionId(const QString productionId) const
{
    qDebug() << "CLoop::deviceByProductionId" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_devices.size(); ++i)
    {
        if(m_devices.at(i)->deviceProductionId() == productionId)
            return m_devices.at(i);
    }
    return NULL;
}

bool CLoop::deviceByKeyId(const int keyid) const
{
    qDebug() << "CLoop::deviceByKeyId" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
        if(!canport)
            continue;
        QList<CDistribution*> list = canport->distributions();
        for(int i=0; i<list.count(); i++){
            CDistribution* distribution = list.at(i);
            if(!distribution)
                continue;
            QList<CLoop*> loops = distribution->loops();
            for(int i=0; i<loops.count(); i++){
                CLoop* loop = loops.at(i);
                if(!loop)
                    continue;
                QList<CDevice*> devices = loop->devices();
                for(int i=0; i<devices.count(); i++){
                    CDevice* device = devices.at(i);
                    if(!device)
                        continue;
                    if(device->keyId() == keyid)
                    {
                        list.clear();
                        loops.clear();
                        devices.clear();
                        return false;
                    }
                }
                devices.clear();
            }
            loops.clear();
        }
        list.clear();
    }
    return true;
}


void CLoop::setDeviceTypeNumber(int type, int n)
{
    qDebug() << "CLoop::setDeviceTypeNumber" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_deviceTypeNumber[type] == n)
        return;
    int adj = n - m_deviceTypeNumber[type];
    adjustedDeviceTypeNumber(type, adj);
}

void CLoop::adjustedDeviceTypeNumber(int type, int adj)
{
    qDebug() << "CLoop::adjustedDeviceTypeNumber" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_deviceTypeNumber[type] += adj;
    if(m_distribution)
        m_distribution->adjustedDeviceTypeNumber(type, adj);
}

void CLoop::setDeviceOnlineNumber(int type, int n)
{
    qDebug() << "CLoop::setDeviceOnlineNumber" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_deviceOnlineNumber[type] == n)
        return;
    int adj = n - m_deviceOnlineNumber[type];
    adjustedDeviceOnlineNumber(type, adj);
}

void CLoop::adjustedDeviceOnlineNumber(int type, int adj)
{
    qDebug() << "CLoop::adjustedDeviceOnlineNumber" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_deviceOnlineNumber[type] += adj;
    if(m_distribution)
        m_distribution->adjustedDeviceOnlineNumber(type, adj);
    adjustedDeviceOnlineTotal(adj);
}

void CLoop::setDeviceOnlineTotal(int n)
{
    qDebug() << "CLoop::setDeviceOnlineTotal" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_deviceOnlineTotal == n)
        return;
    int adj = n - m_deviceOnlineTotal;
    adjustedDeviceOnlineTotal(adj);
}

void CLoop::adjustedDeviceOnlineTotal(int adj)
{
    qDebug() << "CLoop::adjustedDeviceOnlineTotal" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_deviceOnlineTotal += adj;
    if(m_distribution)
        m_distribution->adjustedDeviceOnlineTotal(adj);
    setValue(OBJECT_VALUE_ONLINE_COUNT, m_deviceOnlineTotal);
}

void CLoop::resetDeviceOnlineTotal()
{
    qDebug() << "CLoop::resetDeviceOnlineTotal" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_deviceOnlineNumber.clear();
    m_deviceOnlineTotal = 0;
    m_deviceOnlineTotalBak = 0;
    setValue(OBJECT_VALUE_ONLINE_COUNT, m_deviceOnlineTotal);
}

void CLoop::afterSetValue(QString &name, QVariant& oldValue, QVariant& newValue)
{
    if(CGlobal::instance()->dm()->isLoading())
        return;
    if(name == LOOP_VALUE_ID)
    {
        if(m_distribution)
            m_distribution->calcMaxLoopId(newValue.toInt());
    }
    CObject::afterSetValue(name, oldValue, newValue);
}

QVariant CLoop::afterGetValue(QString name, QVariant value) const
{
    if(name == OBJECT_VALUE_DISPLAY_NAME)
        return m_loopValue.value(LOOP_VALUE_NAME);
    else if(name == OBJECT_VALUE_ADDRESS)
        return m_loopValue.value(LOOP_VALUE_ADDR);
    else if(name == OBJECT_VALUE_DESCRIPTION)
        return m_loopValue.value(LOOP_VALUE_DESCRIPTION);
    else if(name == OBJECT_VALUE_ONLINE_COUNT)
        return m_deviceOnlineTotal;
    else
        return value;
}

void CLoop::initLoop()
{
    qDebug() << "CLoop::initLoop" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_distribution = NULL;
    m_maxDeviceId = 0;
    setloopValue(OBJECT_VALUE_PIXMAP, ":/images/loop.png");
    setloopValue(LOOP_VALUE_NAME, "回路");
    setValue(OBJECT_VALUE_ONLINE_COUNT, 0);
}

int CLoop::saveLoopdevice(CDBSqlite *db)
{
    return saveLoop(db);
}

int CLoop::saveLoop(CDBSqlite *db)
{
    qDebug() << "CLoop::saveLoop" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int distributionkeyId = CGlobal::instance()->saveKeyId(OBJT_Distribution,m_distribution->keyId());
    int loopkeyId = CGlobal::instance()->saveKeyId(OBJT_Loop,keyId());

    QSqlQuery query = db->query("REPLACE INTO ChannelInfo (ID, Number, CanDeviceInfoID, Loop, Remark) "
                                "VALUES (:ID, :Number, :CanDeviceInfoID, :Loop, :Remark)");
    query.bindValue(":ID", loopkeyId);
    query.bindValue(":Number", loopkeyId);
    query.bindValue(":CanDeviceInfoID", distributionkeyId);
    query.bindValue(":Loop", loopValue(LOOP_VALUE_ADDR));
    query.bindValue(":Remark", " ");
    qDebug() << "loop==========" << "LoopID:"<< loopkeyId << "distributionID:"<< distributionkeyId << "loopAdd:"<< loopValue(LOOP_VALUE_ADDR).toInt();
    if(!query.exec())
        return -1;
    if(saveDevice(db) != 0)
        return -1;
    return 0;
}

int CLoop::saveDevice(CDBSqlite *db)
{
    qDebug() << "CLoop::saveDevice" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_devices.size(); ++i)
    {
        if(!CGlobal::instance()->m_isDelUnregLampFlag || (CGlobal::instance()->m_isDelUnregLampFlag &&
              m_devices.at(i)->isDeviceLogin()))
        {
            if(m_devices.at(i)->save(db) != 0)
                return -1;
        }
    }
    return 0;
}
