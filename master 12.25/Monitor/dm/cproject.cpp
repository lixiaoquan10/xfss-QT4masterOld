#include "cproject.h"
#include "ccontroller.h"
#include "clayer.h"
#include "cdbsqlite.h"
#include "cdm.h"
#include "cglobal.h"

#include <QSqlRecord>
#include <QDebug>

const QString CProject::type_s = "CProject";

CProject::CProject(const int keyId) :
    CObject(keyId), m_onlineCount(0)
{
    initProject();
    setProjectId(1);
    setValue(PROJECT_VALUE_NAME, value(PROJECT_VALUE_NAME).toString().append(QString::number(1)));
}

CProject::CProject(const int projectId, const int keyId) :
    CObject(keyId), m_onlineCount(0)
{
    initProject();
    setProjectId(projectId);
    setValue(PROJECT_VALUE_NAME, value(PROJECT_VALUE_NAME).toString().append(QString::number(projectId)));
}

CProject::~CProject()
{
    if(CGlobal::instance()->dm()->isClearing())
        return;
    for(int i = 0; i < m_controllers.size(); ++i)
        m_controllers.at(i)->setProject(NULL);
    for(int i = 0; i < m_layers.size(); ++i)
        m_layers.at(i)->setProject(NULL);
}

void CProject::createTable(CDBSqlite *db)
{
    db->query("CREATE TABLE IF NOT EXISTS project ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "keyid INTEGER,"
                  "project_id INTEGER,"
                  "project_name VARCHAR(255),"
                  "project_address VARCHAR(255),"
                  "project_maxcalculatePathnumber INTEGER"
              ");").exec();

    db->query("CREATE TABLE IF NOT EXISTS relateitemgroup ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "parent_keyid INTEGER,"
                  "item_keyid INTEGER,"
                  "group_keyid INTEGER"
              ");").exec();
    db->query("CREATE UNIQUE INDEX IF NOT EXISTS idx_relateitemgroup ON relateitemgroup ("
                  "item_keyid ASC,"
                  "group_keyid ASC"
              ");").exec();
}

void CProject::clearTable(CDBSqlite *db)
{
    db->query("drop table if exists project").exec();
    db->query("drop table if exists relateitemgroup").exec();
}

/**
 * @brief CProject::loadProject
 * @param db
 * @param dm
 * @return
 * 返回表格CanDeviceInfo的数量
 */
int CProject::loadProject(CDBSqlite *db, CDM *dm)
{
    CProject* project = new CProject(1);
    project->setValue(PROJECT_VALUE_ID, 1);
    project->setValue(PROJECT_VALUE_NAME, "工程");
    project->setStatus(OBJS_Online,1);
    project->setNone();
    dm->addProject(project);
    if(CController::loadController(db, project) != 0)
        return -1;
    if(CLayer::loadLayer(db, project) != 0)
        return -1;
    return 0;
}

bool CProject::addController(CController* controller)
{
    if(!controller)
        return false;
    if(m_controllers.contains(controller))
        return false;
    if(hasController(controller->controllerId()))
        return false;
    if(controller->project())
        controller->project()->removeController(controller);
    m_controllers.append(controller);
    controller->setProject(this);
    if(this->isUndefined())
        controller->setIsUndefined(true);
    m_childs.append(controller);
    addDeviceTypeAndOnline(controller);
    calcMaxControllerId(controller->controllerId());
    emit objectChanged(CObject::OT_AddChildren, controller);
    return true;
}

void CProject::removeController(CController* controller)
{
    if(!controller)
        return;
    if(!m_controllers.contains(controller))
        return;
    m_controllers.removeOne(controller);
    controller->setProject(NULL);
    m_childs.removeOne(controller);
    removeDeviceTypeAndOnline(controller);
    setModify();
    emit objectChanged(CObject::OT_RemoveChildren, controller);
}

CController *CProject::controllerById(const int id) const
{
    for(int i = 0; i < m_controllers.size(); ++i)
    {
        if(m_controllers.at(i)->controllerId() == id)
            return m_controllers.at(i);
    }
    return NULL;
}

CLayer *CProject::clayerById(const int id) const
{
    for(int i = 0; i < m_layers.size(); ++i)
    {
        if(m_layers.at(i)->layerId() == id)
            return m_layers.at(i);
    }
    return NULL;
}

bool CProject::hasController(const int id) const
{
    return controllerById(id);
}

bool CProject::addLayer(CLayer *layer)
{
    if(!layer)
        return false;
    if(m_layers.contains(layer))
        return false;
    if(hasLayer(layer->layerId()))
        return false;
    if(layer->project())
        layer->project()->removeLayer(layer);
    m_layers.append(layer);
    layer->setProject(this);
    if(this->isUndefined())
        layer->setIsUndefined(true);
    m_childs.append(layer);
    calcMaxLayerId(layer->layerId());
    emit objectChanged(CObject::OT_AddChildren, layer);
    return true;
}

void CProject::removeLayer(CLayer *layer)
{
    if(!layer)
        return;
    if(!m_layers.contains(layer))
        return;
    m_layers.removeOne(layer);
    layer->setProject(NULL);
    m_childs.removeOne(layer);
    setModify();
    emit objectChanged(CObject::OT_RemoveChildren, layer);
}

bool CProject::moveLayer(CLayer *sourceLayer, CLayer *destinationLayer)
{
    if(sourceLayer == destinationLayer)
        return false;
    int sourceIndex = m_layers.indexOf(sourceLayer);
    if(sourceIndex < 0)
        return false;
    if(!m_layers.contains(destinationLayer))
        return false;
    m_layers.removeAt(sourceIndex);
    int destinationIndex = m_layers.indexOf(destinationLayer);
    m_layers.insert(destinationIndex, sourceLayer);
    sourceLayer->setParentLayer(destinationLayer->parentLayer());
    int index1 = m_childs.indexOf(sourceLayer);
    m_childs.removeAt(index1);
    int index2 = m_childs.indexOf(destinationLayer);
    m_childs.insert(index2, sourceLayer);
    setModify();
    return true;
}

bool CProject::hasLayer(const int id) const
{
    for(int i = 0; i < m_layers.size(); ++i)
    {
        if(m_layers.at(i)->layerId() == id)
            return true;
    }
    return false;
}

bool CProject::hasFireLayer() const
{
    for(int i = 0; i < m_layers.size(); ++i)
    {
        if(m_layers.at(i)->isFireStatus())
            return true;
    }
    return false;
}

QSet<QString> CProject::layerMapHash() const
{
    QSet<QString> mapHash;
    for(int i = 0; i < m_layers.size(); ++i)
        mapHash.insert(m_layers.at(i)->value(LAYER_VALUE_MAPHASH).toString());
    return mapHash;
}

void CProject::setDeviceTypeNumber(int type, int n)
{
    if(m_deviceTypeNumber[type] == n)
        return;
    int adj = n - m_deviceTypeNumber[type];
    adjustedDeviceTypeNumber(type, adj);
}

void CProject::adjustedDeviceTypeNumber(int type, int adj)
{
    m_deviceTypeNumber[type] += adj;
}

void CProject::setDeviceOnlineNumber(int type, int n)
{
    if(m_deviceOnlineNumber[type] == n)
        return;
    int adj = n - m_deviceOnlineNumber[type];
    adjustedDeviceOnlineNumber(type, adj);
}

void CProject::adjustedDeviceOnlineNumber(int type, int adj)
{
    m_deviceOnlineNumber[type] += adj;
}

void CProject::setOnlineCount(int n)
{
    if(m_onlineCount == n)
        return;
    int adj = n - m_onlineCount;
    adjustedOnlineCount(adj);
}

void CProject::adjustedOnlineCount(int adj)
{
    m_onlineCount += adj;
    setValue(OBJECT_VALUE_ONLINE_COUNT, m_onlineCount);
}

void CProject::resetOnline()
{
    for(int i = 0; i < m_controllers.size(); ++i)
        m_controllers.at(i)->resetOnline();
    m_deviceOnlineNumber.clear();
    m_onlineCount = 0;
    setValue(OBJECT_VALUE_ONLINE_COUNT, m_onlineCount);
}

void CProject::addRelateItemGroup(int itemId, int groupId)
{
    m_itemRelateGroup.insert(itemId, groupId);
    m_groupRelateItem.insert(groupId, itemId);
    setModify();
}

void CProject::removeRelateItemGroup(int itemId, int groupId)
{
    m_itemRelateGroup.remove(itemId, groupId);
    m_groupRelateItem.remove(groupId, itemId);
    setModify();
}

void CProject::removeRelateItems(int itemId)
{
    QList<int> groups = m_itemRelateGroup.values(itemId);
    for(int i = 0; i < groups.size(); ++i)
        removeRelateItemGroup(itemId, groups.at(i));
    for(int i = 0; i < groups.size(); ++i)
    {
        if(!m_groupRelateItem.contains(groups.at(i)))
        {
//            CEvacuationGroup* group = static_cast<CEvacuationGroup*>(CGlobal::instance()->dm()->object(groups.at(i)));
//            if(group && group->layer())
//                group->layer()->removeEvacuationGroup(group);
        }
    }
}

void CProject::removeRelateGroups(int groupId)
{
    QList<int> items = m_groupRelateItem.values(groupId);
    for(int i = 0; i < items.size(); ++i)
        removeRelateItemGroup(items.at(i), groupId);
}

QList<int> CProject::getGroups(int itemId) const
{
    return m_itemRelateGroup.values(itemId);
}

QList<int> CProject::getItems(int groupId) const
{
    return m_groupRelateItem.values(groupId);
}

QVariant CProject::afterGetValue(QString name, QVariant value) const
{
    if(name == OBJECT_VALUE_DISPLAY_NAME)
        return m_value.value(PROJECT_VALUE_NAME);
    else if(name == OBJECT_VALUE_ONLINE_COUNT)
        return m_onlineCount;
    else
        return value;
}

void CProject::initProject()
{
    m_maxControllerId = 0;
    m_maxLayerId = 0;
    setValue(OBJECT_VALUE_PIXMAP, ":/images/project.png");
    setValue(PROJECT_VALUE_NAME, "工程");
    setValue(PROJECT_VALUE_ADDRESS, "");
    setValue(PROJECT_VALUE_MAXCALCULATE_PATH_NUMBER, 100);
    setValue(OBJECT_VALUE_ONLINE_COUNT, 0);
}

int CProject::loadRelateItemGroup(CDBSqlite *db, CProject *project)
{
    QSqlQuery query = db->query(QString("SELECT item_keyid, group_keyid "
                                        "FROM relateitemgroup "
                                        "WHERE parent_keyid = %1").arg(project->keyId()));
    QSqlRecord record = query.record();
    int filedItemKeyId = record.indexOf("item_keyid");
    int filedGroupKeyId = record.indexOf("group_keyid");
    while(query.next())
    {
        project->addRelateItemGroup(query.value(filedItemKeyId).toInt(), query.value(filedGroupKeyId).toInt());
    }
    return 0;
}

int CProject::saveProject(CDBSqlite *db)
{
    if(isUndefined())
        return 0;
    if(saveController(db) != 0)
        return -1;
    return 0;
}

int CProject::saveController(CDBSqlite *db)
{
    for(int i = 0; i < m_controllers.size(); ++i)
    {
        if(m_controllers.at(i)->save(db) != 0)
            return -1;
    }
    return 0;
}

int CProject::saveLayer(CDBSqlite *db)
{
    for(int i = 0; i < m_layers.size(); ++i)
    {
        if(m_layers.at(i)->save(db) != 0)
            return -1;
    }
    return 0;
}

int CProject::saveRelateItemGroup(CDBSqlite *db)
{
    QSqlQuery query = db->query("REPLACE INTO relateitemgroup (parent_keyid, item_keyid, group_keyid) "
                                "VALUES (:parent_keyid, :item_keyid, :group_keyid)");
    query.bindValue(":parent_keyid", this->keyId());
    QMultiHash<int, int>::const_iterator iter = m_itemRelateGroup.constBegin();
    while(iter != m_itemRelateGroup.constEnd())
    {
        query.bindValue(":item_keyid", iter.key());
        query.bindValue(":group_keyid", iter.value());
        if(!query.exec())
            return -1;
        ++iter;
    }
    return 0;
}

void CProject::addDeviceTypeAndOnline(CController *controller)
{
    QHash<int, int> tmp = controller->getDeviceTypeNumber();
    QHash<int, int>::const_iterator iter = tmp.constBegin();
    while(iter != tmp.constEnd())
    {
        adjustedDeviceTypeNumber(iter.key(), iter.value());
        ++iter;
    }
    tmp = controller->getDeviceOnlineNumber();
    iter = tmp.constBegin();
    int n = 0;
    while(iter != tmp.constEnd())
    {
        n += iter.value();
        adjustedDeviceOnlineNumber(iter.key(), iter.value());
        ++iter;
    }
    adjustedOnlineCount(n);
}

void CProject::removeDeviceTypeAndOnline(CController *controller)
{
    QHash<int, int> tmp = controller->getDeviceTypeNumber();
    QHash<int, int>::const_iterator iter = tmp.constBegin();
    while(iter != tmp.constEnd())
    {
        adjustedDeviceTypeNumber(iter.key(), -iter.value());
        ++iter;
    }
    tmp = controller->getDeviceOnlineNumber();
    iter = tmp.constBegin();
    int n = 0;
    while(iter != tmp.constEnd())
    {
        n += iter.value();
        adjustedDeviceOnlineNumber(iter.key(), -iter.value());
        ++iter;
    }
    adjustedOnlineCount(-n);
}
