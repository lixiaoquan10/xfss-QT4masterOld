#ifndef CCONTROLLER_H
#define CCONTROLLER_H

#include "cobject.h"

#define CONTROLLER_MAX_CONTROLLER_NUM 64
#define CONTROLLER_VALUE_ID "controller_id"
#define CONTROLLER_VALUE_NAME "controller_name"
#define CONTROLLER_VALUE_ADDRESS "controller_address"
#define CONTROLLER_VALUE_MAINPOWER "controller_mainpower"
#define CONTROLLER_VALUE_BATPOWER "controller_batpower"
#define CONTROLLER_VALUE_MAXCALCULATE_PATH_NUMBER "controller_maxcalculatePathnumber"

class CCanport;
class CLayer;
class CDM;
class CFirePoint;
class CController : public CObject
{
    Q_OBJECT
public:
    explicit CController(const int keyId = 0);
    CController(const int controllerId, const int keyId);
    ~CController();

    static int loadController(CDBSqlite *db, CDM* dm);
    int save(CDBSqlite *db) { return saveController(db); }
    CObject* parent() { return 0; }
    CObject* child(const int index) { return m_childs.value(index, NULL); }
    QList<CObject*> childs() { return m_childs; }

    QString codeStr() const { return tr("控制器").arg(controllerId()); }
    QString typeStr() const { return "控制器"; }
    QString descriptionStr() const { return QString(); }

    void paramInit(CDBSqlite *db);

    bool setControllerId(int id) { return setValue(CONTROLLER_VALUE_ID, id); }
    int controllerId() const { return value(CONTROLLER_VALUE_ID).toInt(); }

    bool addCanport(CCanport* canport);
    void removeCanport(CCanport* canport);
    CCanport* canport(const int index) const { return m_canports.value(index, NULL); }
    CCanport* canportById(const int id) const;
    CCanport* canportByAddress(const int add) const;
    bool isfirePointByDeviceAddress(int loopAddress) const;
    bool isfirePointByLoopAddress(int deviceAddress, int loopAddress) const;
    CFirePoint* firePointByAddress(int deviceAddress, int loopAddress, int address) const;
    CFirePoint* firePointByID(int ID) const;
    int canportCount() const { return m_canports.size(); }
    QList<CCanport*> canports() const { return m_canports; }
    bool hasCanport(const int id) const;
    int maxCanportId() const { return m_maxCanportId; }
    void calcMaxCanportId(const int n) { m_maxCanportId = m_maxCanportId > n ? m_maxCanportId : n; }

    QList<CFirePoint*> firepoints() const { return m_firepoints; }
    int maxFirePointId() const { return m_maxFirePointId; }
    void calcMaxFirePointId(const int n) { m_maxFirePointId = m_maxFirePointId > n ? m_maxFirePointId : n; }

    bool addLayer(CLayer* layer);
    void removeLayer(CLayer* layer);
    bool moveLayer(CLayer* sourceLayer, CLayer* destinationLayer);
    bool hasLayer(const int id) const;
    CLayer* layer(const int index) const { return m_layers.value(index, NULL); }
    CLayer* clayerById(const int id) const;
    int layerCount() const { return m_layers.size(); }
    QList<CLayer*> layers() const { return m_layers; }
    int maxLayerId() const { return m_maxLayerId; }
    void calcMaxLayerId(const int n) { m_maxLayerId = m_maxLayerId > n ? m_maxLayerId : n; }
    bool hasFireLayer() const;
    QSet<QString> layerMapHash() const;

    QHash<int, int> getDeviceTypeNumber() const { return m_deviceTypeNumber; }
    void setDeviceTypeNumber(int type, int n);
    void adjustedDeviceTypeNumber(int type, int adj);
    QHash<int, int> getDeviceOnlineNumber() const { return m_deviceOnlineNumber; }
    void setDeviceOnlineNumber(int type, int n);
    void adjustedDeviceOnlineNumber(int type, int adj);

    void setOnlineCount(int n);
    void adjustedOnlineCount(int adj);
    int onlineCount() const { return m_onlineCount; }
    void resetOnline();

    void setDeviceTotal(int deviceTotal) { m_deviceTotal = deviceTotal;}
    int getDeviceTotal();

    bool addFirePoint(CFirePoint* firePoint);
    void removeFirePoint(CFirePoint* firePoint);
    bool hasFirePoint(const int id) const { return m_hashFirePoints.contains(id); }

    void setMaxFirePointDeviceAddress(int n);
    void setMaxFirePointLoopAddress(int n);
    void setMaxFirePointAddress(int n);

    int maxFirePointDeviceAddress() const { return m_maxFirePointDeviceAddress; }
    int maxFirePointLoopAddress() const { return m_maxFirePointLoopAddress; }
    int maxFirePointAddress() const { return m_maxFirePointAddress; }

    bool setControllerValue(QString name, QVariant value);
    QVariant controllerValue(const QString name) const;
public slots:
    void slot_updateLinkageBaudrate();
protected:
    QVariant afterGetValue(QString name, QVariant value) const;
private:
    void initController();
    int saveController(CDBSqlite *db);
    int saveCanport(CDBSqlite *db);
    int saveLayer(CDBSqlite *db);
    void addDeviceTypeAndOnline(CCanport* canport);
    void removeDeviceTypeAndOnline(CCanport* canport);
public:
    const static QString type_s;
private:
    QList<CCanport*> m_canports;
    QList<CLayer*> m_layers;
    QList<CObject*> m_childs;
    QList<CFirePoint*> m_firepoints;
    int m_maxCanportId;
    int m_maxLayerId;
    int m_maxFirePointId;
    QHash<int, CFirePoint*> m_hashFirePoints;
    QHash<int, int> m_deviceTypeNumber;
    QHash<int, int> m_deviceOnlineNumber;
    QHash<QString, QVariant> m_controllerValue;
    int m_onlineCount;
    int m_deviceTotal;

    int m_maxFirePointDeviceAddress;
    int m_maxFirePointLoopAddress;
    int m_maxFirePointAddress;
};

#endif // CCONTROLLER_H
