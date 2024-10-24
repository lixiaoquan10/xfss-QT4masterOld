#ifndef CLOOP_H
#define CLOOP_H

#include "cobject.h"

#define LOOP_DEVICE_NUM 64
#define LOOP_VALUE_ID "loop_id"
#define LOOP_VALUE_NUMBER "loop_number"
#define LOOP_VALUE_NAME "loop_name"
#define LOOP_VALUE_ADDR "loop_addr"
#define LOOP_VALUE_REMARK "loop_remark"
#define LOOP_VALUE_DESCRIPTION "loop_description"

#define LOOP_VALUE_COMMUNICATION "loop_communication"

#define LOOP_VALUE_V "loop_v"
#define LOOP_VALUE_A "loop_a"

#define LOOP_VALUE_DISTRIBUTIONADDR "loop_distributionaddress"

class CDistribution;
class CController;
class CDevice;
class CLoop : public CObject
{
    Q_OBJECT
public:
    explicit CLoop(const int keyId = 0);
    CLoop(CController* controller, const int keyId = 0);
    ~CLoop();
    void initDevices();
    int saveLoopdevice(CDBSqlite *db);

    static void createTable(CDBSqlite* db);
    static void clearTable(CDBSqlite* db);
    static int loadLoop(CDBSqlite *db, CDistribution* distribution);

    bool setloopValue(QString name, QVariant value);
    QVariant loopValue(const QString name) const;

    int save(CDBSqlite *db) { return saveLoop(db); }
    CObject* parent();
    CObject* child(const int index) { return m_childs.value(index, NULL); }
    QList<CObject*> childs() { return m_childs; }
    QString codeStr() const;
    QString typeStr() const { return "回路"; }
    QString descriptionStr() const { return loopValue(LOOP_VALUE_DESCRIPTION).toString(); }

    bool setLoopId(int id);
    int loopId() const { return loopValue(LOOP_VALUE_ID).toInt(); }
    int loopAdd() const { return loopValue(LOOP_VALUE_ADDR).toInt(); }
    void setDistribution(CDistribution* distribution) { m_distribution = distribution; }
    CDistribution* distribution() const { return m_distribution; }



    bool addDevice(CDevice* device);
    void removeDevice(CDevice* device);
    bool hasDevice(const int id) const;
    int deviceCount() const { return m_devices.size(); }
    CDevice* device(const int index) const { return m_devices.value(index, NULL); }
    CDevice* deviceById(const int id) const;
    CDevice* deviceByAdd(const int add) const;
    CDevice* deviceByProductionId(const QString productionId) const;
    bool deviceByKeyId(const int keyid) const;
    QList<CDevice*> devices() const { return m_devices; }
    int maxDeviceId() const { return m_maxDeviceId; }
    void calcMaxDeviceId(const int n) { m_maxDeviceId = m_maxDeviceId > n ? m_maxDeviceId : n; }

    QHash<int, int> getDeviceTypeNumber() const { return m_deviceTypeNumber; }
    void setDeviceTypeNumber(int type, int n);
    void adjustedDeviceTypeNumber(int type, int adj);
    QHash<int, int> getDeviceOnlineNumber() const { return m_deviceOnlineNumber; }
    void setDeviceOnlineNumber(int type, int n);
    void adjustedDeviceOnlineNumber(int type, int adj);
    int getDeviceOnlineTotal() { return m_deviceOnlineTotal; }
    void setDeviceOnlineTotal(int n);
    void adjustedDeviceOnlineTotal(int adj);
    void resetDeviceOnlineTotal();
protected:
    void afterSetValue(QString& name, QVariant& oldValue, QVariant &newValue);
    QVariant afterGetValue(QString name, QVariant value) const;
private:
    void initLoop();
    int saveLoop(CDBSqlite* db);
    int saveDevice(CDBSqlite* db);
public:
    const static QString type_s;
private:
    CDistribution* m_distribution;
    CController* m_controller;
    QList<CDevice*> m_devices;
    QList<CObject*> m_childs;
    int m_maxDeviceId;
    QHash<int, int> m_deviceTypeNumber;
    QHash<int, int> m_deviceOnlineNumber;
    int m_deviceOnlineTotal;
    QHash<QString, QVariant> m_loopValue;
};

#endif // CLOOP_H
