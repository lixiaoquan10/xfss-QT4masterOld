#include "cglobal.h"

const QString CController::type_s = "CController";

CController::CController(const int keyId) :
    CObject(keyId), m_onlineCount(0)
{
    initController();
    setControllerId(1);
    setControllerValue(CONTROLLER_VALUE_NAME, controllerValue(CONTROLLER_VALUE_NAME).toString().append(QString::number(1)));
}

CController::CController(const int controllerId, const int keyId) :
    CObject(keyId), m_onlineCount(0)
{
    initController();
    setControllerId(controllerId);
    setControllerValue(CONTROLLER_VALUE_NAME, controllerValue(CONTROLLER_VALUE_NAME).toString().append(QString::number(controllerId)));
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
bool CController::setControllerValue(QString name, QVariant value)
{
    qDebug() << "CController::setControllerValue"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QVariant old;
    if(m_controllerValue.contains(name))
    {
        old = m_controllerValue.value(name);
        if(value.isNull())
        {
            m_controllerValue.remove(name);
            return true;
        }
        if(old.type() != value.type() || old == value)
            return false;
        m_controllerValue[name] = value;
        return true;
    }
    else
    {
        m_controllerValue.insert(name, value);
        return true;
    }
}

QVariant CController::controllerValue(const QString name) const
{
    return m_controllerValue.value(name);
}

//参数初始化
void CController::paramInit(CDBSqlite *db)
{
    qDebug() << "CController::paramInit"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QSqlQuery query = db->query(QString("SELECT ParamName,ParamValue FROM ParamInfo"));
    QSqlRecord record = query.record();
    int fieldParamName = record.indexOf("ParamName");
    int fieldParamValue = record.indexOf("ParamValue");
    while(query.next())
    {
        if(query.value(fieldParamName).toString() == "LampEmergencyType")
        {
            CGlobal::instance()->m_lampEmergencyType = query.value(fieldParamValue).toInt();
        }
        else if(query.value(fieldParamName).toString() == "EmergencyOnlyDistribution")
        {
            CGlobal::instance()->m_emergencyOnlyDistribution = query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "ShieldFaultSound")
        {
            CGlobal::instance()->m_forbidFaultSound = !query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "ShieldWarningSound")
        {
            CGlobal::instance()->m_forbidEmergencySound = !query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "MonthCheckSpanSecond")
        {
            CGlobal::instance()->m_MonthCheckSpanSecond = query.value(fieldParamValue).toInt();
        }
        else if(query.value(fieldParamName).toString() == "YearCheckSpanSecond")
        {
            CGlobal::instance()->m_YearCheckSpanSecond = query.value(fieldParamValue).toInt();
        }
        else if(query.value(fieldParamName).toString() == "TwinkleControl")
        {
            CGlobal::instance()->m_TwinkleControl = query.value(fieldParamValue).toInt();
        }
        else if(query.value(fieldParamName).toString() == "DataServerIP")
        {
            if(query.value(fieldParamValue).toString() != "")
            {
                CGlobal::instance()->m_DataServerIP = query.value(fieldParamValue).toString();
//                CGlobal::instance()->TimeInterval()->setHostIpPort(CGlobal::instance()->m_DataServerIP,
//                                                           QString::number(CGlobal::instance()->m_DataServerPort));
            }
        }
        else if(query.value(fieldParamName).toString() == "DataServerPort")
        {
            if(query.value(fieldParamValue).toInt())
            {
                CGlobal::instance()->m_DataServerPort = query.value(fieldParamValue).toInt();
//                CGlobal::instance()->TimeInterval()->setHostIpPort(CGlobal::instance()->m_DataServerIP,
//                                                           QString::number(CGlobal::instance()->m_DataServerPort));
            }

        }
        else if(query.value(fieldParamName).toString() == "项目名称")
        {
            CGlobal::instance()->setControllerName(query.value(fieldParamValue).toString());
//            CGlobal::instance()->m_controllerName = query.value(fieldParamValue).toString();
        }
        else if(query.value(fieldParamName).toString() == "省份")
        {
            CGlobal::instance()->m_province = query.value(fieldParamValue).toString();
        }
        else if(query.value(fieldParamName).toString() == "城市")
        {
            CGlobal::instance()->m_city = query.value(fieldParamValue).toString();
        }
        else if(query.value(fieldParamName).toString() == "区县")
        {
            CGlobal::instance()->m_country = query.value(fieldParamValue).toString();
        }
        else if(query.value(fieldParamName).toString() == "详细地址")
        {
            CGlobal::instance()->m_addressDetail = query.value(fieldParamValue).toString();
        }
        else if(query.value(fieldParamName).toString() == "controllerID")
        {
            CGlobal::instance()->m_ControllerID = query.value(fieldParamValue).toString();
        }
        else if(query.value(fieldParamName).toString() == "LiftTime")
        {
            CGlobal::instance()->m_LiftTime = query.value(fieldParamValue).toInt();
        }
        else if(query.value(fieldParamName).toString() == "CanNumber")
        {
            CGlobal::instance()->m_nCanNumber = query.value(fieldParamValue).toInt();
        }
        else if(query.value(fieldParamName).toString() == "DoubleWLAN")
        {
            CGlobal::instance()->m_isDoubleWLAN = query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "AdminParamTime")
        {
            QString dateString = query.value(fieldParamValue).toString();
            CGlobal::instance()->m_adminParamTime = QDateTime::fromString(dateString, Qt::ISODate);
        }
        else if(query.value(fieldParamName).toString() == "LinkmanPhoneNumer")
        {
            CGlobal::instance()->m_LinkmanPhoneNumer = query.value(fieldParamValue).toString();
        }
        else if(query.value(fieldParamName).toString() == "StartFirepointNumber")
        {
            CGlobal::instance()->m_startFirepointNumber = query.value(fieldParamValue).toInt();
        }
        else if(query.value(fieldParamName).toString() == "HideLinkLine")
        {
            CGlobal::instance()->m_isHideLinkLine = query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "HideFirePoint")
        {
            CGlobal::instance()->m_isHideFirePoint = query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "ShieldUnknownFirePoint")
        {
            CGlobal::instance()->m_isShieldUnknownFirePoint = query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "ShieldArrowLampLightFault")
        {
            CGlobal::instance()->m_isShieldArrowLampLightFault = query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "ShieldLightLampLightFault")
        {
            CGlobal::instance()->m_isShieldLightLampLightFault = query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "ShieldUndergroundLampLightFault")
        {
            CGlobal::instance()->m_isShieldUndergroundLampLightFault = query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "ShieldLampComFault")
        {
            CGlobal::instance()->m_isShieldLampComFault = query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "ShieldCanDeviceComFault")
        {
            CGlobal::instance()->m_isShieldCanDeviceComFault = query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "ShieldLoginComFault")
        {
            CGlobal::instance()->m_isShieldLoginComFault = query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "ShieldMainPowerFaultEmergency")
        {
            CGlobal::instance()->m_isShieldMainPowerFaultEmergency = query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "HideBatPowerShow")
        {
            CGlobal::instance()->m_isHideBatPowerFaultShow = query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "OldLampEmergencyParam")
        {
            CGlobal::instance()->m_isOldLampEmergencyParam = query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "BaudRate")
        {
            CGlobal::instance()->m_baudRate = query.value(fieldParamValue).toString();
            QTimer::singleShot(5000,this,SLOT(slot_updateLinkageBaudrate()));
        }
    }
    CGlobal::instance()->TimeInterval()->updateSqliteData();
    CGlobal::instance()->AdminParamPage()->updateSqliteData();
    CMsgObjectStatus m_MsgObjectStatus;
    QByteArray data;
    data.append(CGlobal::instance()->m_forbidFaultSound);
    data.append(CGlobal::instance()->m_forbidEmergencySound);
    data.append(CGlobal::instance()->m_forbidMonthcheck);
    data.append(CGlobal::instance()->m_forbidYearcheck);
    data.append(CGlobal::instance()->m_nCanNumber);
    data.append(CGlobal::instance()->m_MonthCheckSpanSecond);
    data.append(CGlobal::instance()->m_YearCheckSpanSecond);
    m_MsgObjectStatus.ndata = data;
    CGlobal::instance()->processSocket()->setip(CGlobal::instance()->m_DataServerIP);
    CGlobal::instance()->processSocket()->setport(CGlobal::instance()->m_DataServerPort);

    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_SystemSet, &m_MsgObjectStatus);
}

void CController::slot_updateLinkageBaudrate()
{
    qDebug() << "CController::slot_updateLinkageBaudrate"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgObjectStatus m_MsgObjectStatus;
    m_MsgObjectStatus.strdata = CGlobal::instance()->m_baudRate;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_LinkageComBaudRate, &m_MsgObjectStatus);
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
    qDebug() << "CController::loadController"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CController* controller = new CController(1);
    controller->setControllerValue(CONTROLLER_VALUE_ID, 1);
    controller->setControllerValue(CONTROLLER_VALUE_NAME, "控制器");
    controller->setStatus(OBJS_Online,1);
    controller->setNone();
    dm->addController(controller);

    controller->paramInit(db);

    if(CCanport::loadCanport(db, controller) != 0)
        return -1;
    if(CLayer::loadLayer(db, controller) != 0)
        return -1;
    if(CFirePoint::loadFirePoint(db, controller) != 0)
        return -1;
    return 0;
}

bool CController::addCanport(CCanport* canport)
{
    qDebug() << "CController::addCanport"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    qDebug() << "CController::removeCanport"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    qDebug() << "CController::canportById"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_canports.size(); ++i)
    {
        if(m_canports.at(i)->canportId() == id)
            return m_canports.at(i);
    }
    return NULL;
}

CCanport *CController::canportByAddress(const int add) const
{
    qDebug() << "CController::canportByAddress"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_canports.size(); ++i)
    {
        if(m_canports.at(i)->canportAdd() == add)
            return m_canports.at(i);
    }
    return NULL;
}

bool CController::isfirePointByDeviceAddress(int deviceAddress) const
{
    qDebug() << "CController::isfirePointByDeviceAddress"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_firepoints.size(); ++i)
    {
        if(m_firepoints.at(i)->firePointDeviceAddress() == deviceAddress)
            return true;
    }
    return false;
}

bool CController::isfirePointByLoopAddress(int deviceAddress, int loopAddress) const
{
    qDebug() << "CController::isfirePointByLoopAddress"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_firepoints.size(); ++i)
    {
        if((m_firepoints.at(i)->firePointDeviceAddress() == deviceAddress) &&
                (m_firepoints.at(i)->firePointLoopAddress() == loopAddress))
            return true;
    }
    return false;
}

CFirePoint *CController::firePointByAddress(int deviceAddress, int loopAddress, int address) const
{
    qDebug() << "CController::firePointByAddress"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_firepoints.size(); ++i)
    {
        if((m_firepoints.at(i)->firePointDeviceAddress() == deviceAddress) &&
                (m_firepoints.at(i)->firePointLoopAddress() == loopAddress) &&
                (m_firepoints.at(i)->firePointAddress() == address))
            return m_firepoints.at(i);
    }
    return NULL;
}

CFirePoint *CController::firePointByID(int ID) const
{
    qDebug() << "CController::firePointByID"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_firepoints.size(); ++i)
    {
        if(m_firepoints.at(i)->firePointValue(FIREPOINT_VALUE_ID).toInt() == ID)
            return m_firepoints.at(i);
    }
    return NULL;
}

void CController::setMaxFirePointDeviceAddress(int n)
{
    m_maxFirePointDeviceAddress = m_maxFirePointDeviceAddress > n ? m_maxFirePointDeviceAddress : n;
}

void CController::setMaxFirePointLoopAddress(int n)
{
    m_maxFirePointLoopAddress = m_maxFirePointLoopAddress > n ? m_maxFirePointLoopAddress : n;
}

void CController::setMaxFirePointAddress(int n)
{
    m_maxFirePointAddress = m_maxFirePointAddress > n ? m_maxFirePointAddress : n;
}

CLayer *CController::clayerById(const int id) const
{
    qDebug() << "CController::clayerById"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    qDebug() << "CController::addLayer"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    qDebug() << "CController::removeLayer"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    qDebug() << "CController::moveLayer"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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

bool CController::addFirePoint(CFirePoint* firePoint)
{
    qDebug() << "CController::addFirePoint"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!firePoint)
        return false;
    if(m_firepoints.contains(firePoint))
        return false;
    if(hasFirePoint(firePoint->firePointId()))
        return false;
    if(firePoint->controller())
        firePoint->controller()->removeFirePoint(firePoint);
    m_firepoints.append(firePoint);
    int id = firePoint->firePointId();
    m_hashFirePoints.insert(id, firePoint);
    firePoint->setController(this);
    if(this->isUndefined())
        firePoint->setIsUndefined(true);
    m_childs.append(firePoint);
    calcMaxFirePointId(firePoint->firePointId());
    emit objectChanged(CObject::OT_AddChildren, firePoint);
    return true;
}

void CController::removeFirePoint(CFirePoint* firePoint)
{
    qDebug() << "CController::removeFirePoint"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!firePoint)
        return;
    if(!m_firepoints.contains(firePoint))
        return;
    m_firepoints.removeOne(firePoint);
    m_hashFirePoints.remove(firePoint->firePointId());
    firePoint->setController(NULL);
    m_childs.removeOne(firePoint);
    setModify();
    emit objectChanged(CObject::OT_RemoveChildren, firePoint);
}


bool CController::hasLayer(const int id) const
{
    qDebug() << "CController::hasLayer"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_layers.size(); ++i)
    {
        if(m_layers.at(i)->layerId() == id)
            return true;
    }
    return false;
}

bool CController::hasFireLayer() const
{
    qDebug() << "CController::hasFireLayer"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_layers.size(); ++i)
    {
        if(m_layers.at(i)->isFireStatus())
            return true;
    }
    return false;
}

QSet<QString> CController::layerMapHash() const
{
    qDebug() << "CController::layerMapHash"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QSet<QString> mapHash;
    for(int i = 0; i < m_layers.size(); ++i)
        mapHash.insert(m_layers.at(i)->value(LAYER_VALUE_MAPHASH).toString());
    return mapHash;
}

void CController::setDeviceTypeNumber(int type, int n)
{
    qDebug() << "CController::setDeviceTypeNumber"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    qDebug() << "CController::setDeviceOnlineNumber"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    qDebug() << "CController::setOnlineCount"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_onlineCount == n)
        return;
    int adj = n - m_onlineCount;
    adjustedOnlineCount(adj);
}

void CController::adjustedOnlineCount(int adj)
{
    qDebug() << "CController::adjustedOnlineCount"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_onlineCount += adj;
    setValue(OBJECT_VALUE_ONLINE_COUNT, m_onlineCount);
}

void CController::resetOnline()
{
    qDebug() << "CController::resetOnline"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_canports.size(); ++i)
        m_canports.at(i)->resetOnline();
    m_deviceOnlineNumber.clear();
    m_onlineCount = 0;
    setValue(OBJECT_VALUE_ONLINE_COUNT, m_onlineCount);
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
    qDebug() << "CController::initController"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_maxCanportId = 0;
    m_maxLayerId = 0;
    m_maxFirePointDeviceAddress = 0;
    m_maxFirePointLoopAddress = 0;
    m_maxFirePointAddress = 0;
    setValue(OBJECT_VALUE_PIXMAP, ":/images/controller.png");
    setValue(CONTROLLER_VALUE_NAME, "控制器");
    setValue(CONTROLLER_VALUE_ADDRESS, "");
    setValue(CONTROLLER_VALUE_MAXCALCULATE_PATH_NUMBER, 100);
    setValue(OBJECT_VALUE_ONLINE_COUNT, 0);
}


int CController::saveController(CDBSqlite *db)
{
    qDebug() << "CController::saveController"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(isUndefined())
        return 0;
    if(saveCanport(db) != 0)
        return -1;
    return 0;
}

int CController::saveCanport(CDBSqlite *db)
{
    qDebug() << "CController::saveCanport"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_canports.size(); ++i)
    {
        if(m_canports.at(i)->save(db) != 0)
            return -1;
    }
    return 0;
}

int CController::saveLayer(CDBSqlite *db)
{
    qDebug() << "CController::saveLayer"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i = 0; i < m_layers.size(); ++i)
    {
        if(m_layers.at(i)->save(db) != 0)
            return -1;
    }
    return 0;
}


void CController::addDeviceTypeAndOnline(CCanport *canport)
{
    qDebug() << "CController::addDeviceTypeAndOnline"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    qDebug() << "CController::removeDeviceTypeAndOnline"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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

int CController::getDeviceTotal()
{
    qDebug() << "CController::getDeviceTotal"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int deviceTotel = 0;
    QHash<int, int>::const_iterator it;
    for (it = m_deviceTypeNumber.constBegin(); it != m_deviceTypeNumber.constEnd(); ++it)
    {
        deviceTotel += it.value(); // 将哈希表中的第二个int值累加起来
    }
    return deviceTotel;
}
