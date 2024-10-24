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
    m_isDatabaseLogin = false;
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
    qDebug() << "CDevice::setDeviceOnline"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_enableNumberChange)
    {
        if(m_loop && m_loop->isLoopOnline())
        {
            if(isMAtype())
                m_loop->setLoopHasMALamp(true);
            if(isOnline && !m_isOnline)
            {
                m_loop->adjustedDeviceOnlineNumber(deviceTypeId(), 1);
                m_loop->addLoopPower(returnLampPower());
            }
            else if(!isOnline && m_isOnline)
            {
                m_loop->adjustedDeviceOnlineNumber(deviceTypeId(), -1);
                m_loop->addLoopPower(0-returnLampPower());
            }
        }
        m_enableNumberChange = false;
    }
    m_isOnline = isOnline;
}

bool CDevice::setDeviceValue(QString name, QVariant value)
{
    qDebug() << "CDevice::setDeviceValue"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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

bool CDevice::isMAtype()
{
    qDebug() << "CDevice::isMAtype"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int lampType = deviceTypeId();
    if((lampType >= 15 && lampType <= 19) || (lampType >= 40 && lampType <= 47)
            || lampType == 79 || (lampType >= 115 && lampType <= 119) || (lampType >= 122 && lampType <= 124)
            || (lampType >= 140 && lampType <= 147) || lampType == 179)
        return true;
    return false;
}

int CDevice::returnLampPower()
{
    qDebug() << "CDevice::returnLampPower"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int lampType = deviceTypeId();
    int power = 0;
    if((lampType > 0 && lampType <= 12) || (lampType >= 30 && lampType <= 37)
            || (lampType >= 100 && lampType <= 121) || (lampType >= 130 && lampType <= 138))
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
    qDebug() << "CDevice::getDeviceCommunicationFault"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    qDebug() << "CDevice::createTable"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    qDebug() << "CDevice::loadDevice"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
        if(!device)
            continue;
        device->setDeviceValue(DEVICE_VALUE_ID, query.value(fieldKeyId).toInt());
        device->setDeviceValue(DEVICE_VALUE_TYPE_ID, query.value(fieldTypeId).toInt());
        device->setDeviceValue(DEVICE_VALUE_DESCRIPTION, query.value(fieldRemark));
        device->setDeviceValue(DEVICE_VALUE_ADDR, query.value(fieldAddress).toInt());
        device->setDeviceValue(DEVICE_VALUE_TYPE, query.value(fieldDeviceType));
        device->setDeviceValue(DEVICE_VALUE_ISREVERSE, query.value(fieldReverse).toInt());
//        device->setDeviceValue(DEVICE_VALUE_ISUSED, query.value(fieldUsed).toInt());
        device->setDeviceConfig(true);
//        if(query.value(fieldUsed).toInt())
//        {
            device->setDeviceLogin(true);
            device->setDatabaseLogin(true);
            device->setDeviceValue(DEVICE_VALUE_LOGINTYPE_ID, query.value(fieldTypeId).toInt());
            device->setDeviceValue(DEVICE_VALUE_LOGINTYPE, query.value(fieldDeviceType));
//        }
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
    qDebug() << "CDevice::codeStr"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_loop)
        return m_loop->codeStr().append(tr("设备%1").arg(deviceAdd()));
    else
        return tr("设备%1").arg(deviceAdd());
}

QString CDevice::typeStr() const
{
    qDebug() << "CDevice::typeStr"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CDeviceType* devType = CGlobal::instance()->deviceType(deviceTypeId());
    if(devType)
        return devType->name();
    else
        return "未定义";
}

bool CDevice::isValueContain(QString value)
{
    qDebug() << "CDevice::isValueContain"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_deviceValue.contains(value))
        return true;
    return false;
}

void CDevice::calcSecondCode()
{
    qDebug() << "CDevice::calcSecondCode"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    qDebug() << "CDevice::setSecondCode"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    qDebug() << "CDevice::setLoop"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    qDebug() << "CDevice::initDevice"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_loop = NULL;
    setDeviceValue(OBJECT_VALUE_PIXMAP, ":/images/zone.png");
}

int CDevice::saveDevice(CDBSqlite *db)
{
    qDebug() << "CDevice::saveDevice"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int lampTypeNumber = 0;
    int lampTypeID = deviceValue(DEVICE_VALUE_LOGINTYPE_ID).toInt();
    int configTypeID = deviceValue(DEVICE_VALUE_TYPE_ID).toInt();
    if(configTypeID != lampTypeID)
    {
        //未知MA灯具已配置过型号
        if((lampTypeID == 19 && ((configTypeID <= 18 && configTypeID >= 15) || (configTypeID >= 40 && configTypeID <= 47)))
                || (lampTypeID == 119 && ((configTypeID <= 118 && configTypeID >= 115) || (configTypeID >= 140 && configTypeID <= 147)))
                || (lampTypeID == 124 && (configTypeID == 122 || configTypeID == 123)))
        {
            lampTypeID = configTypeID;
        }
        else
        {
            QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");;
            db.setDatabaseName(fileName);
            if(!db.open())
                return 0;
            QSqlQuery query(db);

            int id = CGlobal::instance()->saveKeyId(OBJT_Device,keyId());
            query.exec(QString("DELETE FROM EvacuationPathItem WHERE LampInfoID=%1").arg(id));
            query.exec(QString("DELETE FROM LampConnection WHERE StartLampID=%1").arg(id));
            query.exec(QString("DELETE FROM LampConnection WHERE EndLampID=%1").arg(id));
            query.exec(QString("DELETE FROM LampDefaultDirection WHERE LampInfoID=%1").arg(id));
            query.exec(QString("DELETE FROM LampToFirePoint WHERE LampInfoID=%1").arg(id));
            query.exec(QString("DELETE FROM LampToLaunchGroup WHERE LampInfoID=%1").arg(id));
            if(!query.exec())
            {
                qDebug() << query.lastError();
            }
            db.close();
        }
    }

    QString cmd = QString("SELECT ID, Number, Type "
                          "FROM LampType WHERE Number = %1"
                          ).arg(lampTypeID);

    QSqlQuery querydata = db->query(cmd);
    while (querydata.next())
    {
        lampTypeNumber = querydata.value(0).toInt();
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
    query.bindValue(":LampTypeID", lampTypeNumber);
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
