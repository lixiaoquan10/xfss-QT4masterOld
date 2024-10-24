#include "ccontroller.h"
#include "ccanport.h"
#include "clayer.h"
#include "cdbsqlite.h"
#include "cdm.h"
#include "cglobal.h"

#include <QSqlRecord>
#include <QDebug>

const QString CController::type_s = "CController";

CController::CController(const int keyId) :
    CObject(keyId), m_onlineCount(0)
{
    initController();
    setControllerId(1);
    setValue(CONTROLLER_VALUE_NAME, value(CONTROLLER_VALUE_NAME).toString().append(QString::number(1)));
}

CController::CController(const int controllerId, const int keyId) :
    CObject(keyId), m_onlineCount(0)
{
    initController();
    setControllerId(controllerId);
    setValue(CONTROLLER_VALUE_NAME, value(CONTROLLER_VALUE_NAME).toString().append(QString::number(controllerId)));
}

CController::~CController()
{
    if(CGlobal::instance()->dm()->isClearing())
        return;
    for(int i = 0; i < m_canports.size(); ++i)
        m_canports.at(i)->setController(NULL);
    for(int i = 0; i < m_layers.size(); ++i)
        m_layers.at(i)->setController(NULL);
}

void CController::createTable(CDBSqlite *db)
{
    db->query("CREATE TABLE IF NOT EXISTS controller ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "keyid INTEGER,"
                  "controller_id INTEGER,"
                  "controller_name VARCHAR(255),"
                  "controller_address VARCHAR(255),"
                  "controller_maxcalculatePathnumber INTEGER"
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

void CController::clearTable(CDBSqlite *db)
{
    db->query("drop table if exists controller").exec();
    db->query("drop table if exists relateitemgroup").exec();
}

/**
 * @brief CController::loadController
 * @param db
 * @param dm
 * @return
 * 返回表格CanDeviceInfo的数量
 */
int CController::loadController(CDBSqlite *db, CDM *dm)
{
    CController* controller = new CController(1);
    controller->setValue(CONTROLLER_VALUE_ID, 1);
    controller->setValue(CONTROLLER_VALUE_NAME, "控制器");
    controller->setStatus(OBJS_Online,1);
    controller->setNone();
    dm->addController(controller);
    if(CCanport::loadCanport(db, controller) != 0)
        return -1;
    if(CLayer::loadLayer(db, controller) != 0)
        return -1;
    return 0;
}

bool CController::addCanport(CCanport* canport)
{
    if(!canport)
        return false;
    if(m_canports.contains(canport))
        return false;
    if(hasCanport(canport->canportId()))
        return false;
    if(canport->controller())
        canport->controller()->removeCanport(canport);
    m_canports.append(canport);
    canport->setController(this);
    if(this->isUndefined())
        canport->setIsUndefined(true);
    m_childs.append(canport);
    addDeviceTypeAndOnline(canport);
    calcMaxCanportId(canport->canportId());
    emit objectChanged(CObject::OT_AddChildren, canport);
    return true;
}

void CController::removeCanport(CCanport* canport)
{
    if(!canport)
        return;
    if(!m_canports.contains(canport))
        return;
    m_canports.removeOne(canport);
    canport->setController(NULL);
    m_childs.removeOne(canport);
    removeDeviceTypeAndOnline(canport);
    setModify();
    emit objectChanged(CObject::OT_RemoveChildren, canport);
}

CCanport *CController::canportById(const int id) const
{
    for(int i = 0; i < m_canports.size(); ++i)
    {
        if(m_canports.at(i)->canportId() == id)
            return m_canports.at(i);
    }
    return NULL;
}

CCanport *CController::canportByAddress(const int add) const
{
    for(int i = 0; i < m_canports.size(); ++i)
    {
        if(m_canports.at(i)->canportAdd() == add)
            return m_canports.at(i);
    }
    return NULL;
}

CLayer *CController::clayerById(const int id) const
{
    for(int i = 0; i < m_layers.size(); ++i)
    {
        if(m_layers.at(i)->layerId() == id)
            return m_layers.at(i);
    }
    return NULL;
}

bool CController::hasCanport(const int id) const
{
    return canportById(id);
}

bool CController::addLayer(CLayer *layer)
{
    if(!layer)
        return false;
    if(m_layers.contains(layer))
        return false;
    if(hasLayer(layer->layerId()))
        return false;
    if(layer->controller())
        layer->controller()->removeLayer(layer);
    m_layers.append(layer);
    layer->setController(this);
    if(this->isUndefined())
        layer->setIsUndefined(true);
    m_childs.append(layer);
    calcMaxLayerId(layer->layerId());
    emit objectChanged(CObject::OT_AddChildren, layer);
    return true;
}

void CController::removeLayer(CLayer *layer)
{
    if(!layer)
        return;
    if(!m_layers.contains(layer))
        return;
    m_layers.removeOne(layer);
    layer->setController(NULL);
    m_childs.removeOne(layer);
    setModify();
    emit objectChanged(CObject::OT_RemoveChildren, layer);
}

bool CController::moveLayer(CLayer *sourceLayer, CLayer *destinationLayer)
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

bool CController::hasLayer(const int id) const
{
    for(int i = 0; i < m_layers.size(); ++i)
    {
        if(m_layers.at(i)->layerId() == id)
            return true;
    }
    return false;
}

bool CController::hasFireLayer() const
{
    for(int i = 0; i < m_layers.size(); ++i)
    {
        if(m_layers.at(i)->isFireStatus())
            return true;
    }
    return false;
}

QSet<QString> CController::layerMapHash() const
{
    QSet<QString> mapHash;
    for(int i = 0; i < m_layers.size(); ++i)
        mapHash.insert(m_layers.at(i)->value(LAYER_VALUE_MAPHASH).toString());
    return mapHash;
}

void CController::setDeviceTypeNumber(int type, int n)
{
    if(m_deviceTypeNumber[type] == n)
        return;
    int adj = n - m_deviceTypeNumber[type];
    adjustedDeviceTypeNumber(type, adj);
}

void CController::adjustedDeviceTypeNumber(int type, int adj)
{
    m_deviceTypeNumber[type] += adj;
}

void CController::setDeviceOnlineNumber(int type, int n)
{
    if(m_deviceOnlineNumber[type] == n)
        return;
    int adj = n - m_deviceOnlineNumber[type];
    adjustedDeviceOnlineNumber(type, adj);
}

void CController::adjustedDeviceOnlineNumber(int type, int adj)
{
    m_deviceOnlineNumber[type] += adj;
}

void CController::setOnlineCount(int n)
{
    if(m_onlineCount == n)
        return;
    int adj = n - m_onlineCount;
    adjustedOnlineCount(adj);
}

void CController::adjustedOnlineCount(int adj)
{
    m_onlineCount += adj;
    setValue(OBJECT_VALUE_ONLINE_COUNT, m_onlineCount);
}

void CController::resetOnline()
{
    for(int i = 0; i < m_canports.size(); ++i)
        m_canports.at(i)->resetOnline();
    m_deviceOnlineNumber.clear();
    m_onlineCount = 0;
    setValue(OBJECT_VALUE_ONLINE_COUNT, m_onlineCount);
}

void CController::addRelateItemGroup(int itemId, int groupId)
{
    m_itemRelateGroup.insert(itemId, groupId);
    m_groupRelateItem.insert(groupId, itemId);
    setModify();
}

void CController::removeRelateItemGroup(int itemId, int groupId)
{
    m_itemRelateGroup.remove(itemId, groupId);
    m_groupRelateItem.remove(groupId, itemId);
    setModify();
}

void CController::removeRelateItems(int itemId)
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

void CController::removeRelateGroups(int groupId)
{
    QList<int> items = m_groupRelateItem.values(groupId);
    for(int i = 0; i < items.size(); ++i)
        removeRelateItemGroup(items.at(i), groupId);
}

QList<int> CController::getGroups(int itemId) const
{
    return m_itemRelateGroup.values(itemId);
}

QList<int> CController::getItems(int groupId) const
{
    return m_groupRelateItem.values(groupId);
}

QVariant CController::afterGetValue(QString name, QVariant value) const
{
    if(name == OBJECT_VALUE_DISPLAY_NAME)
        return m_value.value(CONTROLLER_VALUE_NAME);
    else if(name == OBJECT_VALUE_ONLINE_COUNT)
        return m_onlineCount;
    else
        return value;
}

void CController::initController()
{
    m_maxCanportId = 0;
    m_maxLayerId = 0;
    setValue(OBJECT_VALUE_PIXMAP, ":/images/controller.png");
    setValue(CONTROLLER_VALUE_NAME, "控制器");
    setValue(CONTROLLER_VALUE_ADDRESS, "");
    setValue(CONTROLLER_VALUE_MAXCALCULATE_PATH_NUMBER, 100);
    setValue(OBJECT_VALUE_ONLINE_COUNT, 0);
}

int CController::loadRelateItemGroup(CDBSqlite *db, CController *controller)
{
    QSqlQuery query = db->query(QString("SELECT item_keyid, group_keyid "
                                        "FROM relateitemgroup "
                                        "WHERE parent_keyid = %1").arg(controller->keyId()));
    QSqlRecord record = query.record();
    int filedItemKeyId = record.indexOf("item_keyid");
    int filedGroupKeyId = record.indexOf("group_keyid");
    while(query.next())
    {
        controller->addRelateItemGroup(query.value(filedItemKeyId).toInt(), query.value(filedGroupKeyId).toInt());
    }
    return 0;
}

int CController::saveController(CDBSqlite *db)
{
    if(isUndefined())
        return 0;
    if(saveCanport(db) != 0)
        return -1;
    return 0;
}

int CController::saveCanport(CDBSqlite *db)
{
    for(int i = 0; i < m_canports.size(); ++i)
    {
        if(m_canports.at(i)->save(db) != 0)
            return -1;
    }
    return 0;
}

int CController::saveLayer(CDBSqlite *db)
{
    for(int i = 0; i < m_layers.size(); ++i)
    {
        if(m_layers.at(i)->save(db) != 0)
            return -1;
    }
    return 0;
}

int CController::saveRelateItemGroup(CDBSqlite *db)
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

void CController::addDeviceTypeAndOnline(CCanport *canport)
{
    QHash<int, int> tmp = canport->getDeviceTypeNumber();
    QHash<int, int>::const_iterator iter = tmp.constBegin();
    while(iter != tmp.constEnd())
    {
        adjustedDeviceTypeNumber(iter.key(), iter.value());
        ++iter;
    }
    tmp = canport->getDeviceOnlineNumber();
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

void CController::removeDeviceTypeAndOnline(CCanport *canport)
{
    QHash<int, int> tmp = canport->getDeviceTypeNumber();
    QHash<int, int>::const_iterator iter = tmp.constBegin();
    while(iter != tmp.constEnd())
    {
        adjustedDeviceTypeNumber(iter.key(), -iter.value());
        ++iter;
    }
    tmp = canport->getDeviceOnlineNumber();
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
