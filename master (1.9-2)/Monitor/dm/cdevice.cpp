#include "cdevice.h"
#include "cdistribution.h"
#include "ccontroller.h"
#include "cloop.h"
#include "cdbsqlite.h"
#include "cdm.h"
#include "cdevicetype.h"
#include "cglobal.h"

#include <QSqlRecord>
#include <QDateTime>
#include <QDebug>

const QString CDevice::type_s = "CDevice";

CDevice::CDevice(const int keyId) :
    CObject(keyId)
{
    initDevice();
    m_isDeviceForbid = false;
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
            if(value == 1 && deviceValue(OBJECT_VALUE_ONLINE) == 0)
                m_loop->adjustedDeviceOnlineNumber(deviceType(), 1);
            else if(value == 0 && deviceValue(OBJECT_VALUE_ONLINE) == 1)
                m_loop->adjustedDeviceOnlineNumber(deviceType(), -1);
        }
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

void CDevice::slot_finished()
{
    QObject::sender()->deleteLater();  // 删除线程对象
}

void CDevice::setDeviceStatus(CDevice* device, short status, unsigned char value)
{
//    DeviceStatusThread* thread = new DeviceStatusThread(device, status, value);
//    connect(thread, SIGNAL(finished()), this, SLOT(slot_finished()));
//    thread->start();
    QString data;
    StatusInfo m_statusInfo;
    CMsgLog m_msgLog;
    data = "setDeviceStatus" +  QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz") + "\n";

    if(!device)
        return;
    if(status == OBJS_LoginDeviceType)
        device->setDeviceValue("device_typeId",value);
    m_statusInfo.keyId = device->keyId();
    m_statusInfo.statusId = status;
    m_statusInfo.type = device->typeStr();
    m_statusInfo.time = QDateTime::currentDateTime();
    m_statusInfo.status = CGlobal::instance()->getObjectStatusStr(status) + CGlobal::instance()->getObjectStatusSuffix(status, value);
    m_statusInfo.layer = "";

    if(status == OBJS_None && value == OBJS_None)
    {
        CGlobal::instance()->DataProcessing()->slotStateInformation(m_statusInfo, SRT_ALL, SRO_DelAll);
        return;
    }
    int type = 0;
    if(status == OBJS_Fire)
        type = SRT_Start;
    else if(CGlobal::instance()->isEmergencyStatus(status))
        type = SRT_Emergency;
    else if(CGlobal::instance()->isFaultStatus(status))
        type = SRT_Fault;
    else
        return;
    QString strtext = CGlobal::instance()->getObjectStatusDescription(device, status, value);
    QString areaAndLocation = device->deviceValue(DEVICE_VALUE_AREA).toString() + device->deviceValue(DEVICE_VALUE_LOCATION).toString();
    if(areaAndLocation != NULL && areaAndLocation != "  ")
        strtext = "(" + areaAndLocation + ")" + strtext;
    m_msgLog.User = "灯具";
    m_msgLog.Type = (type==SRT_Start?"启动":"故障");
    m_msgLog.Time = QDateTime::currentDateTime();
    m_msgLog.Area = device->deviceValue(DEVICE_VALUE_AREA).toString();
    m_msgLog.Location = device->deviceValue(DEVICE_VALUE_LOCATION).toString();
    m_msgLog.Remark = CGlobal::instance()->getObjectStatusStr(status) + CGlobal::instance()->getObjectStatusSuffix(status, value);
    m_msgLog.Canport = 1;
    m_msgLog.CanDeviceAddress = device->deviceValue(DEVICE_VALUE_DISTRIBUTIONADDR).toInt();
    m_msgLog.Loop = device->deviceValue(DEVICE_VALUE_LOOPADDR).toInt();
    m_msgLog.LampDeviceAddress = device->deviceValue(DEVICE_VALUE_ADDR).toInt();
    m_msgLog.title = strtext;
    if(type != SRT_Emergency)
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_AddLog, &m_msgLog);

    m_statusInfo.canID = device->deviceValue(DEVICE_VALUE_DISTRIBUTIONADDR).toInt();
    m_statusInfo.loopID = device->deviceValue(DEVICE_VALUE_LOOPADDR).toInt();
    m_statusInfo.deviceID = device->deviceValue(DEVICE_VALUE_ADDR).toInt();
    m_statusInfo.area = device->deviceValue(DEVICE_VALUE_AREA).toString();
    m_statusInfo.location = device->deviceValue(DEVICE_VALUE_LOCATION).toString();
    int opr = value ? SRO_Add : SRO_Del;
    CGlobal::instance()->DataProcessing()->slotStateInformation(m_statusInfo, type, opr);
    data = data + "end  setDeviceStatus" +  QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz") + "\n";
    QFile file("/home/xfss/root/receive.txt");

    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << data << '\n' << '\n';
        file.close();
    }
    qDebug() << "CClientBusiness::objectStatusChanged" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
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

        device->setDeviceValue(DEVICE_VALUE_NAME, QString("%1_%2").arg(device->deviceValue(DEVICE_VALUE_TYPE).toString()).arg(device->deviceValue(DEVICE_VALUE_ADDR).toInt()));
        device->setDeviceValue(DEVICE_VALUE_EMERGENCY, "正常");
        device->setDeviceValue(DEVICE_VALUE_LIGHT, "正常");
        device->setDeviceValue(DEVICE_VALUE_DEFAULTDIRECTION,query.value(fieldLampDirection).toInt());
        if(query.value(fieldLampDirection).toInt())
        {
            CMsgObjectStatus msgobjectstatus;
            msgobjectstatus.nDisID = loop->distribution()->distributionAddress();
            msgobjectstatus.nLoopID = loop->loopAdd();
            msgobjectstatus.nDeviceID = query.value(fieldAddress).toInt();
            if(query.value(fieldLampDirection).toInt() == 2)
                msgobjectstatus.nValue = 0x05;
            else if(query.value(fieldLampDirection).toInt() == 1)
                msgobjectstatus.nValue = 0x06;
            else if(query.value(fieldLampDirection).toInt() == 3)
                msgobjectstatus.nValue = 0x04;
            CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDefaultDrection, &msgobjectstatus);
        }
//        device->setValue(DEVICE_VALUE_DATABASE, 1);
//        device->setDeviceStatus(device, OBJS_Online,1);
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
            CController* controller = distribution->controller();
            if(controller)
                secondCode.append(QString::number(controller->controllerId()).rightJustified(2, '0', true));
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
                m_loop->adjustedDeviceOnlineNumber(deviceType(), 1);
            else
                m_loop->adjustedDeviceOnlineNumber(deviceType(), -1);
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
    query.bindValue(":isused", 1);
    if(!query.exec())
    {
        qDebug()<<query.lastError().text();
        return -1;
    }
    return 0;
}
