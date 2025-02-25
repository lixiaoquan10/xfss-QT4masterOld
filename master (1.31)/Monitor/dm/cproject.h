﻿#ifndef CCONTROLLER_H
#define CCONTROLLER_H

#include "cobject.h"

#define CONTROLLER_MAX_CONTROLLER_NUM 64
#define CONTROLLER_VALUE_ID "controller_id"
#define CONTROLLER_VALUE_NAME "controller_name"
#define CONTROLLER_VALUE_ADDRESS "controller_address"
#define CONTROLLER_VALUE_MAXCALCULATE_PATH_NUMBER "controller_maxcalculatePathnumber"

class CCanport;
class CLayer;
class CDM;
class CController : public CObject
{
    Q_OBJECT
public:
    explicit CController(const int keyId = 0);
    CController(const int controllerId, const int keyId);
    ~CController();

    static void createTable(CDBSqlite* db);
    static void clearTable(CDBSqlite* db);
    static int loadController(CDBSqlite *db, CDM* dm);
    int save(CDBSqlite *db) { return saveController(db); }
    CObject* parent() { return 0; }
    CObject* child(const int index) { return m_childs.value(index, NULL); }
    QList<CObject*> childs() { return m_childs; }

    QString codeStr() const { return tr("工程%1").arg(controllerId()); }
    QString typeStr() const { return "工程"; }
    QString descriptionStr() const { return QString(); }

    bool setControllerId(int id) { return setValue(CONTROLLER_VALUE_ID, id); }
    int controllerId() const { return value(CONTROLLER_VALUE_ID).toInt(); }

    bool addCanport(CCanport* canport);
    void removeCanport(CCanport* canport);
    CCanport* canport(const int index) const { return m_canports.value(index, NULL); }
    CCanport* canportById(const int id) const;
    int canportCount() const { return m_canports.size(); }
    QList<CCanport*> canports() const { return m_canports; }
    bool hasCanport(const int id) const;
    int maxCanportId() const { return m_maxCanportId; }
    void calcMaxCanportId(const int n) { m_maxCanportId = m_maxCanportId > n ? m_maxCanportId : n; }

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

    void addRelateItemGroup(int itemId, int groupId);
    void removeRelateItemGroup(int itemId, int groupId);
    void removeRelateItems(int itemId);
    void removeRelateGroups(int groupId);
    QList<int> getGroups(int itemId) const;
    QList<int> getItems(int groupId) const;
protected:
    QVariant afterGetValue(QString name, QVariant value) const;
private:
    void initController();
    static int loadRelateItemGroup(CDBSqlite *db, CController* controller);
    int saveController(CDBSqlite *db);
    int saveCanport(CDBSqlite *db);
    int saveLayer(CDBSqlite *db);
    int saveRelateItemGroup(CDBSqlite *db);
    void addDeviceTypeAndOnline(CCanport* canport);
    void removeDeviceTypeAndOnline(CCanport* canport);
public:
    const static QString type_s;
private:
    QList<CCanport*> m_canports;
    QList<CLayer*> m_layers;
    QList<CObject*> m_childs;
    int m_maxCanportId;
    int m_maxLayerId;
    QHash<int, int> m_deviceTypeNumber;
    QHash<int, int> m_deviceOnlineNumber;
    int m_onlineCount;
    QMultiHash<int, int> m_itemRelateGroup;
    QMultiHash<int, int> m_groupRelateItem;
};

#endif // CCONTROLLER_H
