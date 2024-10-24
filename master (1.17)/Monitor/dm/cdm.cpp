#include "cdm.h"
#include "cdbsqlite.h"
#include "cproject.h"
#include "ccontroller.h"
#include "cdistribution.h"
#include "cloop.h"
#include "cdevice.h"
#include "clayer.h"
#include "dialog/dlgdevices.h"
#include "cglobal.h"

#include <QFile>
#include <QDateTime>
#include <QDir>
#include <QDebug>

CDM::CDM(QObject *parent) :
    QObject(parent), m_isClearing(false), m_isLoading(false), m_isLoad(false), m_statusIsChanged(false), m_statusChangedUpdate(true),
    m_undefinedIsChanged(false), m_undefinedChangedUpdate(true)
{
    m_objects.reserve(2000);
    m_removedObjects.reserve(1000);
    m_devices.reserve(1000);
//    m_alarmPoints.reserve(1000);
    m_modifyObjects.reserve(2000);
}

CDM::~CDM()
{
    if(isLoad())
        clear();
}

int CDM::save(const QString &dbName)
{
//    CProgress progress(CGlobal::instance()->taProcess());
//    if(progress.isValid())
//        progress->setInfo("保存工程: ");
    CDBSqlite db(dbName);
    if(!db.open())
        return -1;
    clearTable(&db);
    createTable(&db);    
    db.transaction();
    QSet<QString> mapHash;
    for(int i = 0; i < m_projects.size(); ++i)
    {
        if(m_projects.at(i)->save(&db) != 0)
        {
            db.rollback();
            return -1;
        }
        mapHash.unite(m_projects.at(i)->layerMapHash());
    }
    removeNotUsingMap(mapHash);
    db.commit();
    db.setCreateTime(m_createTime);
    db.setUpdateTime(m_updateTime);
    db.setCreateId(m_createId);
    db.setUpdateId(m_updateId);
    db.close();
    setNone();
    return 0;
}

int CDM::load(const QString &dbName)
{
    m_isLoading = true;
    m_statusChangedUpdate = false;
    m_undefinedChangedUpdate = false;
    CDBSqlite db(dbName);
    if(!db.open())
        return -1;
    int n = CProject::loadProject(&db, this);
    m_modifyObjects.clear();
    if(n == 0)
    {
        m_isLoad = true;
        if(projectSize() == 0)
        {
            CProject* project = new CProject(1);
            addProject(project);
        }
        setCreateTime(db.createTime());
        setCreateId(db.createId());
    }
    else
        clear();
    m_isLoading = false;
    m_statusChangedUpdate = true;
    m_undefinedChangedUpdate = true;
    db.close();
    CGlobal::instance()->DlgDevices()->getDeviceInfo();
    return n;
}

int CDM::loadDefault()
{
    clear();
    m_isLoad = true;
    CProject* project = new CProject(1);
    addProject(project);
    return 0;
}

bool CDM::isModify()
{
    return m_modifyObjects.size();
}

void CDM::addProject(CProject* project)
{
    if(project != NULL && !m_projects.contains(project))
    {
        m_projects.append(project);
        addObject(project);
    }
}

void CDM::removeProject(CProject* project)
{
    if(project != NULL && m_projects.contains(project))
    {
        m_projects.removeOne(project);
        removeObject(project);
    }
}

CProject* CDM::projectAt(const int i)
{
    return m_projects.value(i, NULL);
}

void CDM::addObject(CObject *object)
{
    if(object != NULL && !m_objects.contains(object->keyId()))
    {
        insert(object);
        addChild(object);
    }
}

void CDM::removeObject(CObject *object, const bool isDelete)
{
    if(object != NULL && m_objects.contains(object->keyId()))
    {
        remove(object, isDelete);
        removeChild(object, isDelete);
    }
}

void CDM::removeObject(const int keyId, const bool isDelete)
{
    removeObject(object(keyId), isDelete);
}

bool CDM::hasSecondCode(const QString &secondCode)
{
    if(secondCode.isEmpty())
        return false;
    return m_devices.contains(secondCode.rightJustified(8, '0', true));
}

//bool CDM::hasAlarmPoint(const QString &address)
//{
//    if(address.isEmpty())
//        return false;
//    return m_alarmPoints.contains(address.rightJustified(10, '0', true));
//}

void CDM::clearStatusObjects(bool reserveLogin)
{
    if(m_statusObjects.isEmpty())
        return;
    m_statusChangedUpdate = false;
    QSet<int>::iterator iter = m_statusObjects.begin();
    while(iter != m_statusObjects.end())
    {
        CObject* object = this->object(*iter);
        bool b = true;
        if(object)
        {
            object->clearStatus(reserveLogin);
            if(object->hasStatus())
                b = false;
        }
        if(b)
            iter = m_statusObjects.erase(iter);
        else
            ++iter;
    }
    if(!reserveLogin){
        for(int i = 0; i < m_projects.size(); ++i)
            m_projects.at(i)->resetOnline();
    }
//    m_firstFireAlarmPoint = NULL;
    m_statusChangedUpdate = true;
    m_statusIsChanged = true;
    emit statusChanged(0, CDM_Clr);
}

void CDM::clearUndefinedObjects(const QSet<int> &reserveKeyIds)
{
    if(m_undefinedObjects.isEmpty())
        return;
    m_undefinedChangedUpdate = false;
    for(int i = m_undefinedObjects.size() - 1; i >= 0; --i)
    {
        int keyId = m_undefinedObjects.at(i);
        if(reserveKeyIds.contains(keyId))
            continue;
        CObject* object = this->object(keyId);
        delete object;
        m_undefinedObjects.removeAt(i);
    }
    m_undefinedChangedUpdate = true;
    m_undefinedIsChanged = true;
    emit undefinedChanged(0, CDM_Clr);
}


void CDM::clear()
{
//    CProgress progress(CGlobal::instance()->taProcess());
//    if(progress.isValid())
//    {
//        progress->setInfo("关闭工程: ");
//        progress->setCount(m_objects.size());
//    }
    m_isClearing = true;
    m_statusChangedUpdate = false;
    m_undefinedChangedUpdate = false;
    m_projects.clear();
    qDebug()<<"Clear objects:"<<m_objects.size();
    qDebug()<<"Clear removed objects:"<<m_removedObjects.size();
    QHashIterator<int, CObject*> iter(m_objects);
    while(iter.hasNext())
    {
        iter.next();
        delete iter.value();
//        if(progress.isValid())
//            progress->addPos();
    }
    m_objects.clear();
//    if(progress.isValid())
//        progress->setCount(m_removedObjects.size());
    QHashIterator<int, CObject*> iter2(m_removedObjects);
    while(iter2.hasNext())
    {
        iter2.next();
        delete iter2.value();
//        if(progress.isValid())
//            progress->addPos();
    }
    m_removedObjects.clear();
    m_devices.clear();
//    m_alarmPoints.clear();
    m_modifyObjects.clear();
    m_statusObjects.clear();
    m_undefinedObjects.clear();
    m_needUpdateEvacuationLayer.clear();
//    m_firstFireAlarmPoint = NULL;
    m_isClearing = false;
    m_isLoad = false;
    m_statusIsChanged = true;
    m_statusChangedUpdate = true;
    m_undefinedIsChanged = true;
    m_undefinedChangedUpdate = true;
    CGlobal::initLineWidth();
    qDebug()<<"CDM::clear end";
}

void CDM::editStatusChanged(CObject::EditStatus status)
{
    CObject* object = static_cast<CObject*>(sender());
    if(!object)
        return;
    if(status & CObject::ES_Modify)
        addModifyObject(object);
}

void CDM::objectStatusChanged(short status, unsigned char oldValue, unsigned char newValue, unsigned int time)
{
    if(m_statusChangedUpdate)
    {
        CObject* object = static_cast<CObject*>(sender());
        if(!object)
            return;
//        int keyId = object->keyId();
//        if(object->hasStatus())
//            addStatusObject(keyId);
//        else
//            removeStatusObject(keyId);
        emit sigObjectStatusChanged(object, status, oldValue, newValue, time);
    }
}

void CDM::objectChanged(CObject::ObjectType type, CObject *object)
{
    if(!object || m_isClearing)
        return;
    if(type == CObject::OT_AddChildren)
        addObject(object);
    if(type == CObject::OT_RemoveChildren)
        removeObject(object);
}

void CDM::secondCodeChanged(QString &oldSecondCode, QString &newSecondCode)
{
    CDevice* device = static_cast<CDevice*>(sender());
    if(!device)
        return;
    if(!m_devices.contains(oldSecondCode))
        return;
    m_devices.remove(oldSecondCode);
    m_devices.insert(newSecondCode, device);
}

void CDM::addressChanged(QString &oldAddress, QString &newAddress)
{
//    CAlarmPoint* alarmPoint = static_cast<CAlarmPoint*>(sender());
//    if(!alarmPoint)
//        return;
//    if(!m_alarmPoints.contains(oldAddress))
//        return;
//    m_alarmPoints.remove(oldAddress);
//    m_alarmPoints.insert(newAddress, alarmPoint);
}

void CDM::objectDestrory(CObject *object)
{
    removeObject(object, true);
    m_removedObjects.remove(object->keyId());
}

void CDM::insert(CObject *object)
{
    int keyId = object->keyId();
    QString type = object->type();
    m_objects.insert(keyId, object);
    if(type == CDevice::type_s)
    {
        CDevice* device = static_cast<CDevice*>(object);
        m_devices.insert(device->secondCode(), device);
        connect(device, SIGNAL(secondCodeChanged(QString&,QString&)), this, SLOT(secondCodeChanged(QString&,QString&)));
    }
    if(object->isModify())
        addModifyObject(object);
    if(m_statusChangedUpdate && object->hasStatus())
        addStatusObject(keyId);
    connect(object, SIGNAL(objectStatusChanged(short,unsigned char,unsigned char,unsigned int)), this, SLOT(objectStatusChanged(short,unsigned char,unsigned char,unsigned int)));
    if(m_undefinedChangedUpdate && object->isUndefined())
        addUndefinedObject(keyId);
    connect(object, SIGNAL(editStatusChanged(CObject::EditStatus)), this, SLOT(editStatusChanged(CObject::EditStatus)));
    connect(object, SIGNAL(objectChanged(CObject::ObjectType,CObject*)), this, SLOT(objectChanged(CObject::ObjectType,CObject*)));
    connect(object, SIGNAL(destrory(CObject*)), this, SLOT(objectDestrory(CObject*)));
    m_removedObjects.remove(keyId);
    emit addedObject(object);
}

void CDM::remove(CObject *object, const bool isDelete)
{
    int keyId = object->keyId();
    QString type = object->type();
    m_objects.remove(keyId);
    if(type == CDevice::type_s)
    {
        CDevice* device = static_cast<CDevice*>(object);
        m_devices.remove(device->secondCode());
        disconnect(device, SIGNAL(secondCodeChanged(QString&,QString&)), this, SLOT(secondCodeChanged(QString&,QString&)));
    }
    if(object->isModify())
        removeModifyObject(object);
    if(m_statusChangedUpdate)
        removeStatusObject(keyId);
    disconnect(object, SIGNAL(objectStatusChanged(short,unsigned char,unsigned char,unsigned int)), this, SLOT(objectStatusChanged(short,unsigned char,unsigned char,unsigned int)));
    if(m_undefinedChangedUpdate)
        removeUndefinedObject(keyId);
    disconnect(object, SIGNAL(editStatusChanged(CObject::EditStatus)), this, SLOT(editStatusChanged(CObject::EditStatus)));
    disconnect(object, SIGNAL(objectChanged(CObject::ObjectType,CObject*)), this, SLOT(objectChanged(CObject::ObjectType,CObject*)));
    if(!isDelete)
        m_removedObjects.insert(keyId, object);
    else
    {
        m_removedObjects.remove(keyId);
        disconnect(object, SIGNAL(destrory(CObject*)), this, SLOT(objectDestrory(CObject*)));
    }
    emit removedObject(object);
}

void CDM::addChild(CObject *parent)
{
    QList<CObject*> childs = parent->childs();
    for(int i = 0; i < childs.size(); ++i)
    {
        insert(childs.at(i));
        addChild(childs.at(i));
    }
    childs.clear();
}

void CDM::removeChild(CObject *parent, const bool isDelete)
{
    QList<CObject*> childs = parent->childs();
    for(int i = 0; i < childs.size(); ++i)
    {
        remove(childs.at(i), isDelete);
        removeChild(childs.at(i), isDelete);
    }
    childs.clear();
}

void CDM::addNeedUpdateEvacuationLayer(CLayer *layer)
{
    if(layer)
        m_needUpdateEvacuationLayer.insert(layer);
}

void CDM::removeNeedUpdateEvacuationLayer(CLayer *layer)
{
    if(layer)
        m_needUpdateEvacuationLayer.remove(layer);
}


void CDM::createTable(CDBSqlite *db)
{
    CController::createTable(db);
    CDistribution::createTable(db);
    CLoop::createTable(db);
    CDevice::createTable(db);
}

void CDM::clearTable(CDBSqlite *db)
{
    CController::clearTable(db);
    CDistribution::clearTable(db);
    CLoop::clearTable(db);
    CDevice::clearTable(db);
}

void CDM::addModifyObject(CObject *object)
{
    m_modifyObjects.insert(object);
}

void CDM::removeModifyObject(CObject *object)
{
    m_modifyObjects.remove(object);
}

void CDM::addStatusObject(int keyId)
{
    m_statusObjects.insert(keyId);
    m_statusIsChanged = true;
    emit statusChanged(keyId, CDM_Add);
}

void CDM::removeStatusObject(int keyId)
{
    m_statusObjects.remove(keyId);
    m_statusIsChanged = true;
    emit statusChanged(keyId, CDM_Del);
}

void CDM::addUndefinedObject(int keyId)
{
    m_undefinedObjects.append(keyId);
    m_undefinedIsChanged = true;
    emit undefinedChanged(keyId, CDM_Add);
}

void CDM::removeUndefinedObject(int keyId)
{
    m_undefinedObjects.removeOne(keyId);
    m_undefinedIsChanged = true;
    emit undefinedChanged(keyId, CDM_Del);
}

void CDM::setNone()
{
    m_modifyObjects.clear();
}

void CDM::removeNotUsingMap(const QSet<QString> &mapHash)
{
    QString path = CGlobal::instance()->workspaces() + "/" + CGlobal::instance()->projectName() + "/" + LAYER_VALUE_MAP;
    QDir finder(path);
    finder.setFilter(QDir::Files);
    QStringList files = finder.entryList();
    for(int i = 0; i < files.size(); ++i)
    {
        QString file = files.at(i);
        if(!mapHash.contains(file))
        {
            QFile tmp(path + "/" + file);
            tmp.remove();
        }
    }
}
