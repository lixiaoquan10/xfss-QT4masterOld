#ifndef CDEVICE_H
#define CDEVICE_H

#include "cobject.h"
#include "cglobal.h"
#include "business/cdataprocessing.h"
#include "struct/cmsglog.h"
#include <QFile>
#include <QTextStream>

#define DEVICE_VALUE_ID "device_id"
#define DEVICE_VALUE_SECONDCODE "device_secondcode"
#define DEVICE_VALUE_TYPE_ID "device_typeId"
#define DEVICE_VALUE_TYPE "device_type"
#define DEVICE_VALUE_DESCRIPTION "device_description"
#define DEVICE_VALUE_ITEM_POINTER "deviceItem_pointer"
#define DEVICE_VALUE_ISREVERSE  "device_reverse"
#define DEVICE_VALUE_ISUSED     "device_isused"   //0:仅配置;  1:配置且注册;  2:仅注册;  3:未定义
#define DEVICE_VALUE_ADDR "device_address"
#define DEVICE_VALUE_NAME "device_name"
#define DEVICE_VALUE_AREA "device_area"
#define DEVICE_VALUE_LOCATION "device_location"

#define DEVICE_VALUE_DEFAULTDIRECTION "device_defaultdirection"

#define DEVICE_VALUE_COMMUNICATION "device_communication"
#define DEVICE_VALUE_EMERGENCY "device_emergency"
#define DEVICE_VALUE_LIGHT "device_light"
#define DEVICE_VALUE_BATTERYSTATUS "device_batterystatus"
#define DEVICE_VALUE_BATTERYPOWER "device_batterypower"
#define DEVICE_VALUE_DATABASE  "device_database"
#define DEVICE_VALUE_CHANGETYPE  "device_changetype"
#define DEVICE_VALUE_CHANGEADD  "device_changeadd"
#define DEVICE_VALUE_TEST "device_test"

#define DEVICE_VALUE_LEFTCLOSE "device_leftclose"
#define DEVICE_VALUE_RIGHTCLOSE "device_rightclose"

#define DEVICE_VALUE_CX "device_cx"
#define DEVICE_VALUE_CY "device_cy"
#define DEVICE_VALUE_HEIGHT "device_height"
#define DEVICE_VALUE_ANGEL "device_angel"
#define DEVICE_VALUE_LAYOUTPAGEID "device_layoutpageid"

#define DEVICE_VALUE_SOFTWARENUMBER "device_softwarenumber"
#define DEVICE_VALUE_SOFTWAREVERSION "device_softwareversion"

#define DEVICE_VALUE_LOOPADDR "device_loopaddress"
#define DEVICE_VALUE_DISTRIBUTIONADDR "device_distributionaddress"

class CLoop;
class CDevice : public CObject
{
    Q_OBJECT
public:
    explicit CDevice(const int keyId = 0);
    CDevice(CLoop *loop, const int keyId = 0);
    ~CDevice();

    static void createTable(CDBSqlite* db);
    static void clearTable(CDBSqlite* db);
    static int loadDevice(CDBSqlite *db, CLoop* loop);
    int save(CDBSqlite *db) { return saveDevice(db); }
    CObject* parent();
    QString codeStr() const;
    QString typeStr() const;
    QString descriptionStr() const { return deviceValue(DEVICE_VALUE_DESCRIPTION).toString(); }
    bool isValueContain(QString value);

    void calcSecondCode();

    bool setDeviceId(int id);
    bool setDeviceType(int type);

    int deviceId() const { return deviceValue(DEVICE_VALUE_ID).toInt(); }
    int deviceAdd() const { return deviceValue(DEVICE_VALUE_ADDR).toInt(); }
    int deviceTypeId() const { return deviceValue(DEVICE_VALUE_TYPE_ID).toInt(); }
    QString deviceProductionId() const { return deviceValue(DEVICE_VALUE_DESCRIPTION).toString(); }
    bool setSecondCode(QString& secondCode);
    QString secondCode() const { return deviceValue(DEVICE_VALUE_SECONDCODE).toString(); }

    int deviceType() const { return deviceValue(DEVICE_VALUE_TYPE).toInt(); }
    void setLoop(CLoop* loop);
    CLoop* loop() const { return m_loop; }
    bool setDeviceValue(QString name, QVariant value);
    QVariant deviceValue(const QString name) const;
    bool getDeviceCommunicationFault();


    QString DeviceEmergency() const { return deviceValue(DEVICE_VALUE_EMERGENCY).toString(); }
    QString DeviceLight() const { return deviceValue(DEVICE_VALUE_LIGHT).toString(); }

    bool isDeviceForbid() const { return m_isDeviceForbid; }
    void setDeviceForbid(bool isforbid) { m_isDeviceForbid = isforbid ;}
signals:
    void secondCodeChanged(QString& oldSecondCode, QString& newSecondCode);
public slots:
    void slot_finished();
protected:
    void afterSetValue(QString& name, QVariant& oldValue, QVariant &newValue);
    QVariant afterGetValue(QString name, QVariant value) const;
    void afterSetStatus(short status, unsigned char value);
private:
    void initDevice();
    int saveDevice(CDBSqlite* db);
public:
    const static QString type_s;
private:
    CLoop* m_loop;
    QHash<QString, QVariant> m_deviceValue;
    bool m_isDeviceForbid;
};

#include <QThread>

class DeviceStatusThread : public QThread
{
    Q_OBJECT
public:
    explicit DeviceStatusThread(CDevice* device, short status, unsigned char value)
        : m_device(device), m_status(status), m_value(value)
    {
    }
signals:
    void finished();

protected:
    void run()
    {
        QString data;
        StatusInfo m_statusInfo;
        CMsgLog m_msgLog;
        data = "run()" +  QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz") + "\n";

        if(!m_device)
            return;
        if(m_status == OBJS_LoginDeviceType)
            m_device->setDeviceValue("device_typeId",m_value);
        m_statusInfo.keyId = m_device->keyId();
        m_statusInfo.statusId = m_status;
        m_statusInfo.type = m_device->typeStr();
        m_statusInfo.time = QDateTime::currentDateTime();
        m_statusInfo.status = CGlobal::instance()->getObjectStatusStr(m_status) + CGlobal::instance()->getObjectStatusSuffix(m_status, m_value);
        m_statusInfo.layer = "";

        if(m_status == OBJS_None && m_value == OBJS_None)
        {
            CGlobal::instance()->DataProcessing()->slotStateInformation(m_statusInfo, SRT_ALL, SRO_DelAll);
            return;
        }
        int type = 0;
        if(m_status == OBJS_Fire)
            type = SRT_Start;
        else if(CGlobal::instance()->isEmergencyStatus(m_status))
            type = SRT_Emergency;
        else if(CGlobal::instance()->isFaultStatus(m_status))
            type = SRT_Fault;
        else
            return;
        QString strtext = CGlobal::instance()->getObjectStatusDescription(m_device, m_status, m_value);
        QString areaAndLocation = m_device->deviceValue(DEVICE_VALUE_AREA).toString() + m_device->deviceValue(DEVICE_VALUE_LOCATION).toString();
        if(areaAndLocation != NULL && areaAndLocation != "  ")
            strtext = "(" + areaAndLocation + ")" + strtext;
        m_msgLog.User = "灯具";
        m_msgLog.Type = (type==SRT_Start?"启动":"故障");
        m_msgLog.Time = QDateTime::currentDateTime();
        m_msgLog.Area = m_device->deviceValue(DEVICE_VALUE_AREA).toString();
        m_msgLog.Location = m_device->deviceValue(DEVICE_VALUE_LOCATION).toString();
        m_msgLog.Remark = CGlobal::instance()->getObjectStatusStr(m_status) + CGlobal::instance()->getObjectStatusSuffix(m_status, m_value);
        m_msgLog.Canport = 1;
        m_msgLog.CanDeviceAddress = m_device->deviceValue(DEVICE_VALUE_DISTRIBUTIONADDR).toInt();
        m_msgLog.Loop = m_device->deviceValue(DEVICE_VALUE_LOOPADDR).toInt();
        m_msgLog.LampDeviceAddress = m_device->deviceValue(DEVICE_VALUE_ADDR).toInt();
        m_msgLog.title = strtext;
        if(type != SRT_Emergency)
            CGlobal::instance()->ClientBusiness()->exeCommand(NCT_AddLog, &m_msgLog);

        m_statusInfo.canID = m_device->deviceValue(DEVICE_VALUE_DISTRIBUTIONADDR).toInt();
        m_statusInfo.loopID = m_device->deviceValue(DEVICE_VALUE_LOOPADDR).toInt();
        m_statusInfo.deviceID = m_device->deviceValue(DEVICE_VALUE_ADDR).toInt();
        m_statusInfo.area = m_device->deviceValue(DEVICE_VALUE_AREA).toString();
        m_statusInfo.location = m_device->deviceValue(DEVICE_VALUE_LOCATION).toString();
        int opr = m_value ? SRO_Add : SRO_Del;
        CGlobal::instance()->DataProcessing()->slotStateInformation(m_statusInfo, type, opr);
        data = data + "end  run()" +  QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz") + "\n";
        QFile file("/home/xfss/root/receive.txt");

        if (file.open(QIODevice::Append | QIODevice::Text))
        {
            QTextStream stream(&file);
            stream << data << '\n' << '\n';
            file.close();
        }
    //    qDebug() << "CClientBusiness::objectStatusChanged" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
        emit finished();  // 发送结束信号
    }

private:
    CDevice* m_device;
    short m_status;
    unsigned char m_value;
};

#endif // CDEVICE_H
