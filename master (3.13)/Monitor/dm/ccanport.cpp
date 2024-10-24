#include "cglobal.h"


const QString CCanport::type_s = "CCanport";

CCanport::CCanport(const int keyId) :
    CObject(keyId), m_onlineDistributionCount(0), m_onlinePowerCount(0), m_deviceOnlineTotal(0), m_onlineTotalCount(0)
{
    initCanport();
}

CCanport::CCanport(CController *controller, const int keyId) :
    CObject(keyId), m_onlineDistributionCount(0), m_onlinePowerCount(0), m_deviceOnlineTotal(0), m_onlineTotalCount(0)
{
    initCanport();
    int n = controller->maxCanportId() + 1;
    setCanportId(n);
    controller->addCanport(this);
    setCanportValue(CANPORT_VALUE_NAME, canportValue(CANPORT_VALUE_NAME).toString().append(QString::number(n)));
}

CCanport::~CCanport()
{
    if(CGlobal::instance()->dm()->isClearing())
        return;
    if(m_controller)
        m_controller->removeCanport(this);
    for(int i = 0; i < m_distributions.size(); ++i)
        m_distributions.at(i)->setCanport(NULL);
}

bool CCanport::setCanportValue(QString name, QVariant value)
{
    QVariant old;
    if(name == OBJECT_VALUE_ONLINE)
    {
        CGlobal::instance()->programTreeView()->resetControllerTreeView();
    }
    if(m_canportValue.contains(name))
    {
        old = m_canportValue.value(name);
        if(value.isNull())
        {
            m_canportValue.remove(name);
            return true;
        }
        if(old.type() != value.type() || old == value)
            return false;
        m_canportValue[name] = value;
        return true;
    }
    else
    {
        m_canportValue.insert(name, value);
        return true;
    }
}

QVariant CCanport::canportValue(const QString name) const
{
    return m_canportValue.value(name);
}

void CCanport::createTable(CDBSqlite *db)
{
    db->query("CREATE TABLE IF NOT EXISTS canport ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "keyid INTEGER,"
                    "parent_keyid INTEGER,"
                    "canport_id INTEGER,"
                    "canport_name VARCHAR(255),"
                    "canport_description VARCHAR(255)"
                ");").exec();
    db->query("CREATE UNIQUE INDEX IF NOT EXISTS idx_canport ON canport ("
                    "parent_keyid ASC,"
                    "canport_id ASC"
                ");").exec();
}

void CCanport::clearTable(CDBSqlite *db)
{
    db->query("drop table if exists canport").exec();
}

int CCanport::loadCanport(CDBSqlite *db, CController *controller)
{
    QSqlQuery query = db->query(QString("SELECT ParamName,ParamValue FROM ParamInfo"));
    QSqlRecord record = query.record();
    int fieldParamName = record.indexOf("ParamName");
    int fieldParamValue = record.indexOf("ParamValue");
    while(query.next())
    {
        if(query.value(fieldParamName).toString() == "SendDirectionAndTwinkleByOne")
        {
            CGlobal::instance()->m_SendDirectionAndTwinkleByOne = query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "UCSendDirectionAndTwinkleByOne")
        {
            CGlobal::instance()->m_UCSendDirectionAndTwinkleByOne = query.value(fieldParamValue).toBool();
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
                CGlobal::instance()->TimeInterval()->setHostIpPort(CGlobal::instance()->m_DataServerIP,
                                                           QString::number(CGlobal::instance()->m_DataServerPort));
            }
        }
        else if(query.value(fieldParamName).toString() == "DataServerPort")
        {
            if(query.value(fieldParamValue).toInt())
            {
                CGlobal::instance()->m_DataServerPort = query.value(fieldParamValue).toInt();
                CGlobal::instance()->TimeInterval()->setHostIpPort(CGlobal::instance()->m_DataServerIP,
                                                           QString::number(CGlobal::instance()->m_DataServerPort));
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
    }
    CGlobal::instance()->TimeInterval()->updateSqliteData();
    CMsgObjectStatus m_MsgObjectStatus;
    QByteArray data;
    data.append(CGlobal::instance()->m_forbidFaultSound);
    data.append(CGlobal::instance()->m_forbidEmergencySound);
    data.append(CGlobal::instance()->m_forbidMonthcheck);
    data.append(CGlobal::instance()->m_forbidYearcheck);
    data.append(CGlobal::instance()->m_SendDirectionAndTwinkleByOne);
    data.append(CGlobal::instance()->m_UCSendDirectionAndTwinkleByOne);
    data.append(30);
    m_MsgObjectStatus.ndata = data;
    m_MsgObjectStatus.nDataServerIP = CGlobal::instance()->m_DataServerIP;
    m_MsgObjectStatus.nDataServerPort = CGlobal::instance()->m_DataServerPort;

    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_SystemSet, &m_MsgObjectStatus);
    for(int i=0; i<4; i++)
    {
        CCanport* canport = new CCanport(63+i);
        canport->setCanportValue(CANPORT_VALUE_ID,63+i);
        canport->setCanportValue(CANPORT_VALUE_PORT, i+1);     //can1-can4
        canport->setCanportValue(CANPORT_VALUE_ADDRESS, i+3);  //can1-can4对应3-6
        canport->setCanportValue(CANPORT_VALUE_AREA, "");
        canport->setCanportValue(CANPORT_VALUE_LOCATION, "");
        canport->setCanportValue(CANPORT_VALUE_NAME, "CAN" + QString::number(i+1));
        canport->setNone();
        controller->addCanport(canport);
        canport->setCanportValue(OBJECT_VALUE_ONLINE,1);

        if(CDistribution::loadDistribution(db, canport))
            return -1;
    }
    return 0;
}

CObject* CCanport::parent()
{
    return m_controller;
}

bool CCanport::addDistribution(CDistribution *distribution)
{
    if(!distribution)
        return false;
    if(m_distributions.contains(distribution))
        return false;
    if(hasDistribution(distribution->distributionProductId()))
        return false;
    if(distribution->canport())
        distribution->canport()->removeDistribution(distribution);
    m_distributions.append(distribution);
    distribution->setCanport(this);
    if(this->isUndefined())
        distribution->setIsUndefined(true);
    m_childs.append(distribution);
    addDeviceTypeAndOnline(distribution);
    calcMaxDistributionId(distribution->distributionId());
    emit objectChanged(CObject::OT_AddChildren, distribution);
    return true;
}

void CCanport::removeDistribution(CDistribution *distribution)
{
    if(!distribution)
        return;
    if(!m_distributions.contains(distribution))
        return;
    m_distributions.removeOne(distribution);
    distribution->setCanport(NULL);
    m_childs.removeOne(distribution);
    removeDeviceTypeAndOnline(distribution);
    setModify();
    emit objectChanged(CObject::OT_RemoveChildren, distribution);
}

bool CCanport::hasDistribution(const QString str) const
{
    return distributionByProductId(str);
}

CDistribution *CCanport::distributionById(const int id) const
{
    for(int i = 0; i < m_distributions.size(); ++i)
    {
        if(m_distributions.at(i)->distributionId() == id)
            return m_distributions.at(i);
    }
    return NULL;
}

CDistribution *CCanport::distributionByAddress(const int add) const
{
    for(int i = 0; i < m_distributions.size(); ++i)
    {
        if(m_distributions.at(i)->distributionAddress() == add)
            return m_distributions.at(i);
    }
    return NULL;
}

CDistribution *CCanport::distributionByProductId(const QString str) const
{
    for(int i = 0; i < m_distributions.size(); ++i)
    {
        if(m_distributions.at(i)->descriptionStr() == str && str != "")
            return m_distributions.at(i);
    }
    return NULL;
}

CDistribution *CCanport::distributionByKeyId(const int keyid) const
{
    for(int i = 0; i < m_distributions.size(); ++i)
    {
        if(m_distributions.at(i)->keyId() == keyid)
            return m_distributions.at(i);
    }
    return NULL;
}

bool CCanport::isdistributionByKeyId(const int keyid) const
{
    for(int i = 0; i < m_distributions.size(); ++i)
    {
        if(m_distributions.at(i)->keyId() == keyid)
            return false;
    }
    return true;
}




void CCanport::setDeviceTypeNumber(int type, int n)
{
    if(m_deviceTypeNumber[type] == n)
        return;
    int adj = n - m_deviceTypeNumber[type];
    adjustedDeviceTypeNumber(type, adj);
}

void CCanport::adjustedDeviceTypeNumber(int type, int adj)
{
    m_deviceTypeNumber[type] += adj;
    if(m_controller)
        m_controller->adjustedDeviceTypeNumber(type, adj);
    emit deviceTypeChanged();
}

void CCanport::setDeviceOnlineNumber(int type, int n)
{
    if(m_deviceOnlineNumber[type] == n)
        return;
    int adj = n - m_deviceOnlineNumber[type];
    adjustedDeviceOnlineNumber(type, adj);
}

void CCanport::adjustedDeviceOnlineNumber(int type, int adj)
{
    m_deviceOnlineNumber[type] += adj;
    if(m_controller)
        m_controller->adjustedDeviceOnlineNumber(type, adj);
}

void CCanport::setDeviceOnlineTotal(int n)
{
    if(m_deviceOnlineTotal == n)
        return;
    int adj = n - m_deviceOnlineTotal;
    adjustedDeviceOnlineTotal(adj);
}

void CCanport::adjustedDeviceOnlineTotal(int adj)
{
    m_deviceOnlineTotal += adj;
    if(m_controller)
        m_controller->adjustedOnlineCount(adj);
    updateOnlineTotalCount();
}

void CCanport::setOnlineDistributionCount(int n)
{
    if(m_onlineDistributionCount == n)
        return;
    int adj = n - m_onlineDistributionCount;
    adjustedOnlineDistributionCount(adj);
}

void CCanport::adjustedOnlineDistributionCount(int adj)
{
    m_onlineDistributionCount += adj;
    if(m_controller)
        m_controller->adjustedOnlineCount(adj);
    updateOnlineTotalCount();
}

void CCanport::setOnlinePowerCount(int n)
{
    if(m_onlinePowerCount == n)
        return;
    int adj = n - m_onlinePowerCount;
    adjustedOnlinePowerCount(adj);
}

void CCanport::adjustedOnlinePowerCount(int adj)
{
    m_onlinePowerCount += adj;
    if(m_controller)
        m_controller->adjustedOnlineCount(adj);
    updateOnlineTotalCount();
}

void CCanport::resetOnline()
{
    for(int i = 0; i < m_distributions.size(); ++i)
        m_distributions.at(i)->resetDeviceOnlineTotal();
    m_deviceOnlineNumber.clear();
    m_onlineDistributionCount = 0;
    m_onlinePowerCount = 0;
    m_deviceOnlineTotal = 0;
    updateOnlineTotalCount();
}

void CCanport::afterSetValue(QString &name, QVariant& oldValue, QVariant& newValue)
{
    if(CGlobal::instance()->dm()->isLoading())
        return;
    if(name == CANPORT_VALUE_ID)
    {
        if(m_controller)
            m_controller->calcMaxCanportId(newValue.toInt());
    }
    CObject::afterSetValue(name, oldValue, newValue);
}

QVariant CCanport::afterGetValue(QString name, QVariant value) const
{
    if(name == OBJECT_VALUE_DISPLAY_NAME)
        return m_canportValue.value(CANPORT_VALUE_NAME);
    else if(name == OBJECT_VALUE_ADDRESS)
        return m_canportValue.value(CANPORT_VALUE_ID);
    else if(name == OBJECT_VALUE_DESCRIPTION)
        return m_canportValue.value(CANPORT_VALUE_DESCRIPTION);
    else if(name == OBJECT_VALUE_ONLINE_COUNT)
        return m_onlineTotalCount;
    else
        return value;
}

void CCanport::initCanport()
{
    m_controller = NULL;
    m_maxDistributionId = 0;
    setCanportValue(OBJECT_VALUE_PIXMAP, ":/images/CanDeviceConfig.png");
    setCanportValue(CANPORT_VALUE_NAME, "CAN");
    setValue(OBJECT_VALUE_ONLINE_COUNT, 0);
}


int CCanport::saveCanport(CDBSqlite *db)
{
    if(isUndefined())
        return 0;
    if(saveDistribution(db) != 0)
        return -1;
    return 0;
}

int CCanport::saveDistribution(CDBSqlite *db)
{
    for(int i = 0; i < m_distributions.size(); ++i)
    {
        if(m_distributions.at(i)->save(db) != 0)
            return -1;
    }
    return 0;
}



void CCanport::updateOnlineTotalCount()
{
    m_onlineTotalCount = m_deviceOnlineTotal;
    setValue(OBJECT_VALUE_ONLINE_COUNT, m_onlineTotalCount);
    emit onlineChanged();
}

void CCanport::addDeviceTypeAndOnline(CDistribution *distribution)
{
    QHash<int, int> tmp = distribution->getDeviceTypeNumber();
    QHash<int, int>::const_iterator iter = tmp.constBegin();
    while(iter != tmp.constEnd())
    {
        adjustedDeviceTypeNumber(iter.key(), iter.value());
        ++iter;
    }
    tmp = distribution->getDeviceOnlineNumber();
    iter = tmp.constBegin();
    int n = 0;
    while(iter != tmp.constEnd())
    {
        n += iter.value();
        adjustedDeviceOnlineNumber(iter.key(), iter.value());
        ++iter;
    }
    adjustedOnlineDistributionCount(n);
}

void CCanport::removeDeviceTypeAndOnline(CDistribution *distribution)
{
    QHash<int, int> tmp = distribution->getDeviceTypeNumber();
    QHash<int, int>::const_iterator iter = tmp.constBegin();
    while(iter != tmp.constEnd())
    {
        adjustedDeviceTypeNumber(iter.key(), -iter.value());
        ++iter;
    }
    tmp = distribution->getDeviceOnlineNumber();
    iter = tmp.constBegin();
    int n = 0;
    while(iter != tmp.constEnd())
    {
        n += iter.value();
        adjustedDeviceOnlineNumber(iter.key(), -iter.value());
        ++iter;
    }
    adjustedOnlineDistributionCount(-n);
}

