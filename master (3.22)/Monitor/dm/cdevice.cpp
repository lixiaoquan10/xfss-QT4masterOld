#include "cglobal.h"


const QString CDevice::type_s = "CDevice";

CDevice::CDevice(const int keyId) :
    CObject(keyId)
{
    initDevice();
    m_isDeviceForbid = false;
    m_isDeviceControl = false;
    m_enableUpdate = true;
}

CDevice::CDevice(CLoop *loop, const int keyId) :
    CObject(keyId)
{
    initDevice();
    setDeviceId(loop->maxDeviceId() + 1);
    setDeviceType(0);
    loop->addDevice(this);
    m_isDeviceForbid = false;
}

CDevice::~CDevice()
{
    if(CGlobal::instance()->dm()->isClearing())
        return;
    if(m_loop)
        m_loop->removeDevice(this);
}

bool CDevice::setDeviceValue(QString name, QVariant value)
{
    if(name == OBJECT_VALUE_ONLINE)
    {
        if(m_loop)
        {
            if(value == 1 && deviceValue(OBJECT_VALUE_ONLINE) != 1)
                m_loop->adjustedDeviceOnlineNumber(deviceTypeID(), 1);
            else if(value == 0 && deviceValue(OBJECT_VALUE_ONLINE) == 1)
                m_loop->adjustedDeviceOnlineNumber(deviceTypeID(), -1);
        }
    }
    if(m_deviceValue.value(name) != value && m_enableUpdate && !CGlobal::instance()->m_bInLogin)
    {
        CGlobal::instance()->programTreeView()->resetControllerTreeView();
    }
    QVariant old;
    if(m_deviceValue.contains(name))
    {
        old = m_deviceValue.value(name);
        if(value.isNull())
        {
            m_deviceValue.remove(name);
            return true;
        }
        if(old.type() != value.type() || old == value)
            return false;
        m_deviceValue[name] = value;
        return true;
    }
    else
    {
        m_deviceValue.insert(name, value);
        return true;
    }
}

QVariant CDevice::deviceValue(const QString name) const
{
    return m_deviceValue.value(name);
}

bool CDevice::getDeviceCommunicationFault()
{
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(deviceValue(DEVICE_VALUE_CANPORTADDR).toInt());
    CDistribution* distribution = canport->distributionByAddress(deviceValue(DEVICE_VALUE_DISTRIBUTIONADDR).toInt());
    CLoop* loop = distribution->loopByAdd(deviceValue(DEVICE_VALUE_LOOPADDR).toInt());
    if(distribution->distributionValue(OBJECT_VALUE_ONLINE) == 0 ||  loop->loopValue(OBJECT_VALUE_ONLINE) == 0
            || loop->loopValue(LOOP_VALUE_NOLOGIN) == 1 ||
            getStatus(OBJS_LightCommunicationFault) == 1)
        return true;
    else
        return false;
}

void CDevice::slot_finished()
{
    QObject::sender()->deleteLater();  // 删除线程对象
}

bool CDevice::setDeviceId(int id)
{
    return setDeviceValue(DEVICE_VALUE_ID, id);
}
bool CDevice::setDeviceType(int type)
{
    return setDeviceValue(DEVICE_VALUE_TYPE, type);
}

void CDevice::createTable(CDBSqlite *db)
{
    db->query("CREATE TABLE IF NOT EXISTS LampInfo ("
                  "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "ChannelInfoID INTEGER,"
                  "Address INTEGER,"
                  "LampTypeID INTEGER,"
                  "Area VARCHAR(255),"
                  "Location VARCHAR(255),"
                  "Remark VARCHAR(255),"
                  "IsReverse INTEGER,"
                  "IsUsed INTEGER"
              ");").exec();
    db->query("CREATE UNIQUE INDEX IF NOT EXISTS idx_LampInfo ON LampInfo ("
                  "ChannelInfoID ASC,"
              ");").exec();
}

void CDevice::clearTable(CDBSqlite *db)
{
    db->query("drop table if exists device").exec();
}

int CDevice::loadDevice(CDBSqlite *db, CLoop *loop)
{
    QSqlQuery query = db->query(QString("SELECT l.ID,l.Address,r.number,r.type,l.area,l.location,l.remark,l.isreverse,l.isused,d.lampdirection from lampinfo l "
                                        "left join lamptype r on l.lamptypeid=r.id "
                                        "left join lampdefaultdirection d on d.lampinfoid=l.id "
                                        "where l.channelinfoid=%1"
                                        ).arg(loop->loopId()));
    QSqlRecord record = query.record();
    int fieldKeyId = record.indexOf("ID");
    int fieldAddress = record.indexOf("Address");
    int fieldDeviceType = record.indexOf("type");
    int fieldRemark = record.indexOf("remark");
    int fieldReverse = record.indexOf("isreverse");
    int fieldUsed = record.indexOf("isused");
    int fieldTypeId = record.indexOf("number");
    int fieldArea = record.indexOf("area");
    int fieldLocation = record.indexOf("location");
    int fieldLampDirection = record.indexOf("lampdirection");
    while(query.next())
    {
        CDevice* device = new CDevice(CGlobal::instance()->loadKeyId(OBJT_Device,query.value(fieldKeyId).toInt()));
        device->setDeviceValue(DEVICE_VALUE_ID, query.value(fieldKeyId).toInt());
        device->setDeviceValue(DEVICE_VALUE_TYPE_ID, query.value(fieldTypeId).toInt());
        device->setDeviceValue(DEVICE_VALUE_DESCRIPTION, query.value(fieldRemark));
        device->setDeviceValue(DEVICE_VALUE_ADDR, query.value(fieldAddress).toInt());
        device->setDeviceValue(DEVICE_VALUE_TYPE, query.value(fieldDeviceType));
        device->setDeviceValue(DEVICE_VALUE_ISREVERSE, query.value(fieldReverse).toInt());
        device->setDeviceValue(DEVICE_VALUE_ISUSED, query.value(fieldUsed).toInt());
        device->setDeviceValue(DEVICE_VALUE_AREA, query.value(fieldArea));
        device->setDeviceValue(DEVICE_VALUE_LOCATION, query.value(fieldLocation));

        device->setDeviceValue(DEVICE_VALUE_LOOPADDR, loop->loopAdd());
        device->setDeviceValue(DEVICE_VALUE_DISTRIBUTIONADDR, loop->distribution()->distributionAddress());
        device->setDeviceValue(DEVICE_VALUE_CANPORTADDR, loop->loopValue(LOOP_VALUE_CANPORTADDR).toInt());

        device->setDeviceValue(DEVICE_VALUE_NAME, QString("%1_%2").arg(device->deviceValue(DEVICE_VALUE_TYPE).toString()).arg(device->deviceValue(DEVICE_VALUE_ADDR).toInt()));
        device->setDeviceValue(DEVICE_VALUE_EMERGENCY, "正常");
        device->setDeviceValue(DEVICE_VALUE_LIGHT, "正常");
        device->setDeviceValue(DEVICE_VALUE_DEFAULTDIRECTION,query.value(fieldLampDirection).toInt());
        if(query.value(fieldLampDirection).toInt())
        {
            CMsgObjectStatus msgobjectstatus;
            msgobjectstatus.nCanportAdd = device->canportAdd();
            msgobjectstatus.nDisID = device->distributionAdd();
            msgobjectstatus.nLoopID = device->loopAdd();
            msgobjectstatus.nDeviceID = device->deviceAdd();
            if(query.value(fieldLampDirection).toInt() == 2)
                msgobjectstatus.nValue = 0x05;
            else if(query.value(fieldLampDirection).toInt() == 1)
                msgobjectstatus.nValue = 0x06;
            else if(query.value(fieldLampDirection).toInt() == 3)
                msgobjectstatus.nValue = 0x04;
            CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDefaultDrection, &msgobjectstatus);
        }
        device->setDeviceValue(OBJECT_VALUE_ONLINE,1);
        loop->addDevice(device);
    }
    return 0;
}

CObject* CDevice::parent()
{
    return m_loop;
}

QString CDevice::codeStr() const
{
    if(m_loop)
        return m_loop->codeStr().append(tr("设备%1").arg(deviceAdd()));
    else
        return tr("设备%1").arg(deviceAdd());
}

QString CDevice::typeStr() const
{
    CDeviceType* devType = CGlobal::instance()->deviceType(deviceTypeId());
    if(devType)
        return devType->name();
    else
        return "未定义";
}

bool CDevice::isValueContain(QString value)
{
    if(m_value.contains(value))
        return true;
    return false;
}

void CDevice::calcSecondCode()
{
    QString secondCode;
    if(m_loop)
    {
        CDistribution* distribution = m_loop->distribution();
        if(distribution)
        {
            CCanport* canport = distribution->canport();
            if(canport)
                secondCode.append(QString::number(canport->canportId()).rightJustified(2, '0', true));
            else
                secondCode.append("00");
            secondCode.append(QString::number(distribution->distributionId()).rightJustified(3, '0', true));
        }
        else
        {
            secondCode.append("00000");
        }
        secondCode.append(QString::number(m_loop->loopId()).rightJustified(1, '0', true));
    }
    else
    {
        secondCode.append("000000");
    }
    secondCode.append(QString::number(deviceId()).rightJustified(2, '0', true));
    CDM* dm = CGlobal::instance()->dm();
    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());
    while(dm->hasSecondCode(secondCode))
    {
        secondCode = (QString::number(qrand()) + QString::number(qrand())).rightJustified(8, '0', true);
    }
    setSecondCode(secondCode);
}

bool CDevice::setSecondCode(QString &secondCode)
{
    QString old = this->secondCode();
    if(!setDeviceValue(DEVICE_VALUE_SECONDCODE, secondCode.rightJustified(8, '0', true)))
        return false;
    QString newStr = this->secondCode();
    if(old != newStr)
        emit secondCodeChanged(old, newStr);
    return true;
}

void CDevice::setLoop(CLoop *loop)
{
    m_loop = loop;
    if(secondCode().isEmpty())
        calcSecondCode();
}

void CDevice::afterSetValue(QString &name, QVariant& oldValue, QVariant& newValue)
{
    if(CGlobal::instance()->dm()->isLoading())
        return;
    if(name == DEVICE_VALUE_ID)
    {
        if(m_loop)
            m_loop->calcMaxDeviceId(newValue.toInt());
    }
    else if(name == DEVICE_VALUE_TYPE)
    {
        if(m_loop)
        {
            m_loop->adjustedDeviceTypeNumber(oldValue.toInt(), -1);
            m_loop->adjustedDeviceTypeNumber(newValue.toInt(), 1);
        }
    }
    CObject::afterSetValue(name, oldValue, newValue);
}

QVariant CDevice::afterGetValue(QString name, QVariant value) const
{
    if(name == OBJECT_VALUE_DISPLAY_NAME)
        return m_deviceValue.value("device_name");
    else
        return value;
}

void CDevice::afterSetStatus(short status, unsigned char value)
{
    if(status == OBJS_Online)
    {
        if(m_loop)
        {
            if(value)
                m_loop->adjustedDeviceOnlineNumber(deviceTypeID(), 1);
            else
                m_loop->adjustedDeviceOnlineNumber(deviceTypeID(), -1);
        }
    }
    qlonglong n = this->deviceValue(DEVICE_VALUE_ITEM_POINTER).toLongLong();
    if(!n)
        return;
    CDevice* object = reinterpret_cast<CDevice*>(n);
    object->afterSetStatus(status, value);
}

void CDevice::initDevice()
{
    m_loop = NULL;
    setDeviceValue(OBJECT_VALUE_PIXMAP, ":/images/zone.png");
}

int CDevice::saveDevice(CDBSqlite *db)
{
    int lampTypeId = 0;
    QString cmd = QString("SELECT ID, Number, Type "
                          "FROM LampType WHERE Number = %1"
                          ).arg(deviceValue(DEVICE_VALUE_TYPE_ID).toInt());

    QSqlQuery querydata = db->query(cmd);
    while (querydata.next())
    {
        lampTypeId = querydata.value(0).toInt();
    }

    int devicekeyId = CGlobal::instance()->saveKeyId(OBJT_Device,keyId());
    int loopkeyId = CGlobal::instance()->saveKeyId(OBJT_Loop,m_loop->keyId());
    //设置修改设备类型状态为0，修改地址状态为0
    setDeviceValue(DEVICE_VALUE_CHANGETYPE,0);
    setDeviceValue(DEVICE_VALUE_CHANGEADD,0);

    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canportAdd());
    CDistribution* distribution = canport->distributionByAddress(distributionAdd());
    CLoop* loop = distribution->loopByAdd(loopAdd());
    QSqlQuery query = db->query("REPLACE INTO LampInfo (ID, ChannelInfoID, Address, LampTypeID, Area, Location, Remark, IsReverse, IsUsed) "
                                "VALUES (:ID, :ChannelInfoID, :Address, :LampTypeID, :Area, :Location, :remark, :isreverse, :isused)");
    query.bindValue(":ID", devicekeyId);
    query.bindValue(":ChannelInfoID", loopkeyId);
    query.bindValue(":Address", deviceValue(DEVICE_VALUE_ADDR).toInt());
    query.bindValue(":LampTypeID", lampTypeId);
    if(deviceValue(DEVICE_VALUE_AREA).toString() == "")
        query.bindValue(":Area", " ");
    else
        query.bindValue(":Area", deviceValue(DEVICE_VALUE_AREA).toString());
    if(deviceValue(DEVICE_VALUE_LOCATION).toString() == "")
        query.bindValue(":Location", " ");
    else
        query.bindValue(":Location", deviceValue(DEVICE_VALUE_LOCATION).toString());
    query.bindValue(":remark", deviceValue(DEVICE_VALUE_DESCRIPTION).toString());
    QString isReverse = deviceValue(DEVICE_VALUE_ISREVERSE).toString();
    if(isReverse == NULL)
        isReverse = "0";
    query.bindValue(":isreverse", isReverse);
    if(loop->loopValue(LOOP_VALUE_NOLOGIN) != 1 && deviceValue(DEVICE_VALUE_NOLOGIN) != 1
            && distribution->distributionValue(DISTRIBUTION_VALUE_NOLOGIN) != 1)
        query.bindValue(":isused", 1);
    else
        query.bindValue(":isused", 0);
    if(!query.exec())
    {
        qDebug()<<query.lastError().text();
        return -1;
    }
    return 0;
}
