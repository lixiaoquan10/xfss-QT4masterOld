#ifndef CCONTROL_H
#define CCONTROL_H

#include "cobject.h"

#define CONTROLLER_MAX_DISTRIBUTION_NUM 128
#define CONTROLLER_MAX_POWER_NUM 125
#define CONTROLLER_VALUE_ID "controller_id"
#define CONTROLLER_VALUE_NAME "controller_name"
#define CONTROLLER_VALUE_PORT "controller_port"
#define CONTROLLER_VALUE_ADDRESS "controller_addr"
#define CONTROLLER_VALUE_AREA "controller_area"
#define CONTROLLER_VALUE_LOCATION "controller_location"
#define CONTROLLER_VALUE_DESCRIPTION "controller_description"
#define CONTROLLER_VALUE_COMMUNICATION "controller_communication"
#define OBJECT_VALUE_ONLINE "object_online"

class CProject;
class CDistribution;
class CLoop;

class CController : public CObject
{
    Q_OBJECT
public:
    explicit CController(const int keyId = 0);
    CController(CProject *project, const int keyId = 0);
    ~CController();

    bool setControllerValue(QString name, QVariant value);
    QVariant controllerValue(const QString name) const;

    static void createTable(CDBSqlite* db);
    static void clearTable(CDBSqlite* db);
    static int loadController(CDBSqlite *db, CProject* project);
    int save(CDBSqlite *db) { return saveController(db); }
    CObject* parent();
    CObject* child(const int index) { return m_childs.value(index, NULL); }
    QList<CObject*> childs() { return m_childs; }
    QString codeStr() const { return "控制器"; }
    QString typeStr() const { return "控制器"; }
    QString descriptionStr() const { return controllerValue(CONTROLLER_VALUE_DESCRIPTION).toString(); }

    bool setControllerId(int id) { return setControllerValue(CONTROLLER_VALUE_ID, id); }
    int controllerId() const { return controllerValue(CONTROLLER_VALUE_ID).toInt(); }
    void setProject(CProject* project) { m_project = project; }
    CProject* project() const { return m_project; }

    bool addDistribution(CDistribution* distribution);
    void removeDistribution(CDistribution* distribution);
    bool hasDistribution(const QString str) const;
    CDistribution* distribution(const int index) const { return m_distributions.value(index, NULL); }
    CDistribution* distributionById(const int id) const;
    CDistribution* distributionByAddress(const int add) const;
    CDistribution* distributionByProductId(const QString str) const;
    CDistribution* distributionByKeyId(const int keyid) const;
    bool isdistributionByKeyId(const int keyid) const;
    int distributionCount() const { return m_distributions.size(); }
    QList<CDistribution*> distributions() const { return m_distributions; }
    int maxDistributionId() const { return m_maxDistributionId; }
    void calcMaxDistributionId(const int n) { m_maxDistributionId = m_maxDistributionId > n ? m_maxDistributionId : n; }

    QHash<int, int> getDeviceTypeNumber() const { return m_deviceTypeNumber; }
    void setDeviceTypeNumber(int type, int n);
    void adjustedDeviceTypeNumber(int type, int adj);
    QHash<int, int> getDeviceOnlineNumber() const { return m_deviceOnlineNumber; }
    void setDeviceOnlineNumber(int type, int n);
    void adjustedDeviceOnlineNumber(int type, int adj);
    int getDeviceOnlineTotal() const { return m_deviceOnlineTotal; }
    void setDeviceOnlineTotal(int n);
    void adjustedDeviceOnlineTotal(int adj);
    int onlineCount() const { return m_onlineTotalCount; }
    void resetOnline();

    int getOnlineDistributionCount() const { return m_onlineDistributionCount; }
    void setOnlineDistributionCount(int n);
    void adjustedOnlineDistributionCount(int adj);
    int getOnlinePowerCount() const { return m_onlinePowerCount; }
    void setOnlinePowerCount(int n);
    void adjustedOnlinePowerCount(int adj);
protected:
    void afterSetValue(QString& name, QVariant& oldValue, QVariant &newValue);
    QVariant afterGetValue(QString name, QVariant value) const;
private:
    void initController();
    int saveController(CDBSqlite *db);
    int saveDistribution(CDBSqlite* db);
    void updateOnlineTotalCount();
    void addDeviceTypeAndOnline(CDistribution *distribution);
    void removeDeviceTypeAndOnline(CDistribution *distribution);
    void addDeviceTypeAndOnline(CLoop* loop);
    void removeDeviceTypeAndOnline(CLoop* loop);
signals:
    void deviceTypeChanged();
    void onlineChanged();
public:
    const static QString type_s;
private:
    CProject *m_project;
    QList<CLoop*> m_loops;
    QList<CDistribution*> m_distributions;
    QList<CObject*> m_childs;
    CObject* m_virtualDistribution;
    int m_maxDistributionId;
    int m_maxLoopId;
    QHash<int, int> m_deviceTypeNumber;
    QHash<int, int> m_deviceOnlineNumber;
    QHash<QString, QVariant> m_controllerValue;
    int m_onlineDistributionCount;
    int m_onlinePowerCount;
    int m_deviceOnlineTotal;
    int m_onlineTotalCount;
};

#endif // CCONTROL_H
