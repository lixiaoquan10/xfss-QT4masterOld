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
#define DEVICE_VALUE_LOGINTYPE_ID "device_logintypeId"
#define DEVICE_VALUE_LOGINTYPE "device_logintype"
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
#define DEVICE_VALUE_CANPORTADDR "device_canportaddress"

#define DEVICE_VALUE_NOLOGIN "device_nologin"

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

    int returnLampPower();


    int deviceId() const { return deviceValue(DEVICE_VALUE_ID).toInt(); }
    int deviceAdd() const { return deviceValue(DEVICE_VALUE_ADDR).toInt(); }
    int deviceTypeId() const { return deviceValue(DEVICE_VALUE_TYPE_ID).toInt(); }
    int distributionAdd() const { return deviceValue(DEVICE_VALUE_DISTRIBUTIONADDR).toInt(); }
    int loopAdd() const { return deviceValue(DEVICE_VALUE_LOOPADDR).toInt(); }
    int canportAdd() const { return deviceValue(DEVICE_VALUE_CANPORTADDR).toInt(); }
    QString deviceProductionId() const { return deviceValue(DEVICE_VALUE_DESCRIPTION).toString(); }
    bool setSecondCode(QString& secondCode);
    QString secondCode() const { return deviceValue(DEVICE_VALUE_SECONDCODE).toString(); }
    void setLoop(CLoop* loop);
    CLoop* loop() const { return m_loop; }
    bool setDeviceValue(QString name, QVariant value);
    QVariant deviceValue(const QString name) const;
    bool getDeviceCommunicationFault();


    QString DeviceEmergency() const { return deviceValue(DEVICE_VALUE_EMERGENCY).toString(); }
    QString DeviceLight() const { return deviceValue(DEVICE_VALUE_LIGHT).toString(); }

    bool isDeviceForbid() const { return m_isDeviceForbid; }
    void setDeviceForbid(bool isForbid) { m_isDeviceForbid = isForbid; }
    bool isDeviceControl() const { return m_isDeviceControl; }
    void setDeviceControl(bool isControl) { m_isDeviceControl = isControl; }
    bool isDeviceTypeChanged() const { return m_isDeviceTypeChanged; }
    void setDeviceTypeChanged(bool isChanged) { m_isDeviceTypeChanged = isChanged; }

    bool isDeviceLogin() const { return m_islogin; }
    void setDeviceLogin(bool islogin) { m_islogin = islogin; }

    bool isDatabaseLogin() const { return m_isDatabaseLogin; }
    void setDatabaseLogin(bool isdatabaselogin) { m_isDatabaseLogin = isdatabaselogin; }

    bool isDeviceConfig() const { return m_isconfig; }
    void setDeviceConfig(bool isconfig) { m_isconfig = isconfig; }

    bool isDeviceOnline() const { return m_isOnline; }
    void setDeviceOnline(bool isOnline);

    bool isMAtype();

    void setEnableNumberChange(bool enable) { m_enableNumberChange = enable; }
signals:
    void secondCodeChanged(QString& oldSecondCode, QString& newSecondCode);
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
    bool m_isDeviceControl;
    bool m_isDeviceTypeChanged;
    //注册状态
    bool m_islogin;
    bool m_isDatabaseLogin;
    //配置状态
    bool m_isconfig;
    //在线状态
    bool m_isOnline;
    //使能更新灯具数量标志位
    bool m_enableNumberChange;
};

#endif // CDEVICE_H
