#include "cglobal.h"


const QString CDevice::type_s = "CDevice";

CDevice::CDevice(const int keyId) :
    CObject(keyId)
{
    initDevice();
    m_isDeviceForbid = false;
    m_isDeviceControl = false;
    m_isDeviceTypeChanged = false;
    m_islogin = false;
    m_isconfig = false;
    m_enableNumberChange = false;
}

CDevice::CDevice(CLoop *loop, const int keyId) :
    CObject(keyId)
{
    initDevice();
    setDeviceId(loop->maxDeviceId() + 1);
    setDeviceType(0);
    m_isOnline = false;
    m_isDeviceForbid = false;
    m_isDeviceControl = false;
    m_isDeviceTypeChanged = false;
    m_islogin = false;
    m_isconfig = false;
    m_enableNumberChange = false;
    loop->addDevice(this);
}

CDevice::~CDevice()
{
    if(CGlobal::instance()->dm()->isClearing())
        return;
    if(m_loop)
        m_loop->removeDevice(this);
}

void CDevice::setDeviceOnline(bool isOnline)
{
    if(m_enableNumberChange)
    {
        if(m_loop && m_loop->isLoopOnline())
        {
            if(isOnline && !m_isOnline)
            {
                m_loop->adjustedDeviceOnlineNumber(deviceTypeId(), 1);
                m_loop->addLoopPower(returnLampPower(deviceTypeId()));
            }
            else if(!isOnline && m_isOnline)
            {
                m_loop->adjustedDeviceOnlineNumber(deviceTypeId(), -1);
                m_loop->addLoopPower(0-returnLampPower(deviceTypeId()));
            }
        }
        m_enableNumberChange = false;
    }
    m_isOnline = isOnline;
}

bool CDevice::setDeviceValue(QString name, QVariant value)
{
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

int CDevice::returnLampPower(int lampType)
{
    int power = 0;
    if(lampType < 50 || (lampType >= 100 && lampType <= 138))
        power = 1;
    else if(lampType == 50 || lampType == 70 || lampType == 80 || lampType == 150 || lampType == 170)
        power = 3;
    else if(lampType == 51 || lampType == 151)
        power = 5;
    else if(lampType == 52 || lampType == 71 || lampType == 81 || lampType == 152 || lampType == 171)
        power = 6;
    else if(lampType == 53 || lampType == 72 || lampType == 82 || lampType == 153 || lampType == 172)
        power = 9;
    else if(lampType == 54 || lampType == 154)
        power = 10;
    else if(lampType == 55 || lampType == 73 || lampType == 83 || lampType == 155 || lampType == 173)
        power = 12;
    else if(lampType == 56 || lampType == 74 || lampType == 84 || lampType == 156 || lampType == 174)
        power = 15;
    else if(lampType == 57 || lampType == 157 || (lampType >= 180 && lampType <= 182))
        power = 30;
    else if(lampType == 183)
        power = 50;
    else if(lampType == 184)
        power = 100;
    return power;
}

QVariant CDevice::deviceValue(const QString name) const
{
    return m_deviceValue.value(name);
}

bool CDevice::getDeviceCommunicationFault()
{
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(deviceValue(DEVICE_VALUE_CANPORTADDR).toInt());
    if(!canport)
        return false;
    CDistribution* distribution = canport->distributionByAddress(deviceValue(DEVICE_VALUE_DISTRIBUTIONADDR).toInt());
    if(!distribution)
        return false;
    CLoop* loop = distribution->loopByAdd(deviceValue(DEVICE_VALUE_LOOPADDR).toInt());
    if(!loop)
        return false;
    if(!distribution->isDistributionOnline() ||  !loop->isLoopOnline()
            || !loop->isLoopLogin() || !isDeviceLogin() || getStatus(OBJS_LightCommunicationFault) == 1)
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
//        device->setDeviceValue(DEVICE_VALUE_ISUSED, query.value(fieldUsed).toInt());
        device->setDeviceConfig(true);
        if(query.value(fieldUsed).toInt())
        {
            device->setDeviceLogin(true);
        }
        device->setDeviceValue(DEVICE_VALUE_AREA, query.value(fieldArea));
        device->setDeviceValue(DEVICE_VALUE_LOCATION, query.value(fieldLocation));

        device->setDeviceValue(DEVICE_VALUE_LOOPADDR, loop->loopAdd());
        device->setDeviceValue(DEVICE_VALUE_DISTRIBUTIONADDR, loop->distribution()->distributionAddress());
        device->setDeviceValue(DEVICE_VALUE_CANPORTADDR, loop->loopValue(LOOP_VALUE_CANPORTADDR).toInt());

        device->setDeviceValue(DEVICE_VALUE_NAME, QString("%1_%2").arg(device->deviceValue(DEVICE_VALUE_TYPE).toString()).arg(device->deviceValue(DEVICE_VALUE_ADDR).toInt()));
        device->setDeviceValue(DEVICE_VALUE_EMERGENCY, "正常");
        device->setDeviceValue(DEVICE_VALUE_LIGHT, "正常");
        device->setDeviceValue(DEVICE_VALUE_DEFAULTDIRECTION,query.value(fieldLampDirection).toInt());
//        device->setDeviceValue(DEVICE_VALUE_SOFTWARENUMBER,"0");
//        device->setDeviceValue(DEVICE_VALUE_SOFTWAREVERSION,"0");
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
        device->setDeviceOnline(true);
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
    if(m_deviceValue.contains(value))
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
                m_loop->adjustedDeviceOnlineNumber(deviceTypeId(), 1);
            else
                m_loop->adjustedDeviceOnlineNumber(deviceTypeId(), -1);
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
    if(!canport)
        return -1;
    CDistribution* distribution = canport->distributionByAddress(distributionAdd());
    if(!distribution)
        return -1;
    CLoop* loop = distribution->loopByAdd(loopAdd());
    if(!loop)
        return -1;
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
    if(loop->isLoopLogin() && isDeviceLogin() && distribution->isDistributionLogin())
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
