#ifndef CCONTROL_H
#define CCONTROL_H

#include "cobject.h"

#define CANPORT_MAX_DISTRIBUTION_NUM 128
#define CANPORT_MAX_POWER_NUM 125
#define CANPORT_VALUE_ID "canport_id"
#define CANPORT_VALUE_NAME "canport_name"
#define CANPORT_VALUE_PORT "canport_port"
#define CANPORT_VALUE_ADDRESS "canport_addr"
#define CANPORT_VALUE_AREA "canport_area"
#define CANPORT_VALUE_LOCATION "canport_location"
#define CANPORT_VALUE_DESCRIPTION "canport_description"
#define CANPORT_VALUE_COMMUNICATION "canport_communication"
#define OBJECT_VALUE_ONLINE "object_online"

class CProject;
class CDistribution;
class CLoop;

class CCanport : public CObject
{
    Q_OBJECT
public:
    explicit CCanport(const int keyId = 0);
    CCanport(CProject *project, const int keyId = 0);
    ~CCanport();

    bool setCanportValue(QString name, QVariant value);
    QVariant canportValue(const QString name) const;

    static void createTable(CDBSqlite* db);
    static void clearTable(CDBSqlite* db);
    static int loadCanport(CDBSqlite *db, CProject* project);
    int save(CDBSqlite *db) { return saveCanport(db); }
    CObject* parent();
    CObject* child(const int index) { return m_childs.value(index, NULL); }
    QList<CObject*> childs() { return m_childs; }
    QString codeStr() const { return "控制器"; }
    QString typeStr() const { return "控制器"; }
    QString descriptionStr() const { return canportValue(CANPORT_VALUE_DESCRIPTION).toString(); }

    bool setCanportId(int id) { return setCanportValue(CANPORT_VALUE_ID, id); }
    int canportId() const { return canportValue(CANPORT_VALUE_ID).toInt(); }
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
    void setDeviceTotal() { m_deviceTotal = m_deviceOnlineTotal;}
    int getDeviceTotal() const { return m_deviceTotal; }
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
    void initCanport();
    int saveCanport(CDBSqlite *db);
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
    QHash<QString, QVariant> m_canportValue;
    int m_onlineDistributionCount;
    int m_onlinePowerCount;
    int m_deviceOnlineTotal;
    int m_onlineTotalCount;
    int m_deviceTotal;
};

#endif // CCONTROL_H
