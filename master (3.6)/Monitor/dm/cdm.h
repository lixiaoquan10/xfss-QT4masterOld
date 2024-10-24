#ifndef CDM_H
#define CDM_H

#include "cobject.h"

#include <QObject>
#include <QList>
#include <QHash>
#include <QVector>
#include <QSet>
#include <QTimer>
#include <QDateTime>

#define DEVICE_TYPE_NUM 256

class CDBSqlite;
class CController;
class CDevice;
class CFirePoint;
class CLayer;
class CDM : public QObject
{
    Q_OBJECT
public:
    enum { CDM_Add = 1, CDM_Del, CDM_Clr };
    explicit CDM(QObject *parent = 0);
    ~CDM();

    int save(const QString& dbName);
    int load(const QString& dbName);    
    int loadDefault();
    bool isModify();
    bool isLoad()const { return m_isLoad; }
    bool isClearing() const { return m_isClearing; }
    bool isLoading() const { return m_isLoading; }

    void addController(CController* controller);
    void removeController(CController* controller);
    CController* controllerAt(const int i);
    int controllerSize() { return m_controllers.size(); }
    QList<CController*> controllers() { return m_controllers; }

    void addObject(CObject* object);
    void removeObject(CObject* object, const bool isDelete = false);
    void removeObject(const int keyId, const bool isDelete = false);
    int objectCount() { return m_objects.size(); }
    CObject* object(const int keyId) { return m_objects.value(keyId, NULL); }
    CObject* removedObject(const int keyId) { return m_removedObjects.value(keyId, NULL); }
    QHash<int, CObject*> objects() { return m_objects; }
    bool hasKeyId(const int keyId) { return m_objects.contains(keyId); }

    bool hasSecondCode(const QString& secondCode);
    int deviceCount() { return m_devices.size(); }
    CDevice* device(const QString& secondCode) { return m_devices.value(secondCode, NULL); }
    QHash<QString, CDevice*> devices() { return m_devices; }

    bool hasFirePoint(const QString& address);
//    int alarmPointCount() { return m_alarmPoints.size(); }
//    CAlarmPoint* alarmPoint(const QString& address) { return m_alarmPoints.value(address, NULL); }
//    QHash<QString, CAlarmPoint*> alarmPoints() { return m_alarmPoints; }
//    void setFirstFireAlarmPoint(CAlarmPoint* firstFire) { m_firstFireAlarmPoint = firstFire; }
//    CAlarmPoint* firstFireAlarmPoint() const { return m_firstFireAlarmPoint; }

    QSet<int> statusObjects() const { return m_statusObjects; }
    void setStatusIsChanged(const bool b) { m_statusIsChanged = b; }
    bool statusIsChanged() const { return m_statusIsChanged; }
    void clearStatusObjects(bool reserveLogin = false);

    QList<int> undefinedObjects() const { return m_undefinedObjects; }
    void setUndefinedIsChanged(const bool b) { m_undefinedIsChanged = b; }
    bool undefinedIsChanged() const { return m_undefinedIsChanged; }
    void clearUndefinedObjects(const QSet<int> &reserveKeyIds = QSet<int>());

    void setCreateTime(const QDateTime& time) { m_createTime = time; }
    QDateTime createTime() const { return m_createTime; }
    void setUpdateTime(const QDateTime& time) { m_updateTime = time; }
    QDateTime updateTime() const { return m_updateTime; }
    void setCreateId(const QString& id) { m_createId = id; }
    QString createId() const { return m_createId; }
    void setUpdateId(const QString& id) { m_updateId = id; }
    QString updateId() const { return m_updateId; }


    void clear();
signals:
    void addedObject(CObject* object);
    void removedObject(CObject* object);
    void statusChanged(int keyId, int type);
    void undefinedChanged(int keyId, int type);
    void sigObjectStatusChanged(CObject* object, short status, unsigned char oldValue, unsigned char newValue, unsigned int time);
protected slots:
    void editStatusChanged(CObject::EditStatus status);
    void objectStatusChanged(short status, unsigned char oldValue, unsigned char newValue, unsigned int time);
    void objectChanged(CObject::ObjectType type, CObject* object);
    void secondCodeChanged(QString& oldSecondCode, QString& newSecondCode);
    void addressChanged(QString& oldAddress, QString& newAddress);
    void objectDestrory(CObject* object);
    void insert(CObject* object);
    void remove(CObject* object, const bool isDelete = false);
    void addChild(CObject* parent);
    void removeChild(CObject* parent, const bool isDelete = false);
    void addNeedUpdateEvacuationLayer(CLayer* layer);
    void removeNeedUpdateEvacuationLayer(CLayer* layer);
private:
    void createTable(CDBSqlite* db);
    void clearTable(CDBSqlite *db);
    void addModifyObject(CObject* object);
    void removeModifyObject(CObject* object);
    void addStatusObject(int keyId);
    void removeStatusObject(int keyId);
    void addUndefinedObject(int keyId);
    void removeUndefinedObject(int keyId);
    void setNone();
    void removeNotUsingMap(const QSet<QString>& mapHash);
private:
    bool m_isClearing;
    bool m_isLoading;
    bool m_isLoad;
    QList<CController*> m_controllers;
    QHash<int, CObject*> m_objects;
    QHash<int, CObject*> m_removedObjects;
    QHash<QString, CDevice*> m_devices;
    QHash<QString, CFirePoint*> m_firePoints;
//    CFirePoint* m_firstFirePoint;
    QSet<CObject*> m_modifyObjects;
    QSet<int> m_statusObjects;
    bool m_statusIsChanged;
    bool m_statusChangedUpdate;
    QList<int> m_undefinedObjects;
    bool m_undefinedIsChanged;
    bool m_undefinedChangedUpdate;
    QSet<CLayer*> m_needUpdateEvacuationLayer;
    QTimer m_timer;
    QDateTime m_createTime;
    QDateTime m_updateTime;
    QString m_createId;
    QString m_updateId;
};

#endif // CDM_H
