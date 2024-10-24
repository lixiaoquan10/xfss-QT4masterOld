#include "ccontroller.h"
#include "cproject.h"
#include "cdistribution.h"
#include "cloop.h"
#include "cdbsqlite.h"
#include "cdm.h"
#include "cglobal.h"
#include "dialog/dlgtimeinterval.h"
#include "widget/cprogramtreeview.h"

#include <QSqlRecord>
#include <QDebug>

const QString CController::type_s = "CController";

CController::CController(const int keyId) :
    CObject(keyId), m_onlineDistributionCount(0), m_onlinePowerCount(0), m_deviceOnlineTotal(0), m_onlineTotalCount(0)
{
    initController();
}

CController::CController(CProject *project, const int keyId) :
    CObject(keyId), m_onlineDistributionCount(0), m_onlinePowerCount(0), m_deviceOnlineTotal(0), m_onlineTotalCount(0)
{
    initController();
    int n = project->maxControllerId() + 1;
    setControllerId(n);
    project->addController(this);
    setControllerValue(CONTROLLER_VALUE_NAME, controllerValue(CONTROLLER_VALUE_NAME).toString().append(QString::number(n)));
}

CController::~CController()
{
    if(CGlobal::instance()->dm()->isClearing())
        return;
    if(m_project)
        m_project->removeController(this);
    for(int i = 0; i < m_distributions.size(); ++i)
        m_distributions.at(i)->setController(NULL);
}

bool CController::setControllerValue(QString name, QVariant value)
{
    QVariant old;
    if(name == OBJECT_VALUE_ONLINE)
    {
        CGlobal::instance()->programTreeView()->resetProjectTreeView();
    }
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

void CController::createTable(CDBSqlite *db)
{
    db->query("CREATE TABLE IF NOT EXISTS controller ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "keyid INTEGER,"
                    "parent_keyid INTEGER,"
                    "controller_id INTEGER,"
                    "controller_name VARCHAR(255),"
                    "controller_description VARCHAR(255)"
                ");").exec();
    db->query("CREATE UNIQUE INDEX IF NOT EXISTS idx_controller ON controller ("
                    "parent_keyid ASC,"
                    "controller_id ASC"
                ");").exec();
}

void CController::clearTable(CDBSqlite *db)
{
    db->query("drop table if exists controller").exec();
}

int CController::loadController(CDBSqlite *db, CProject *project)
{
    CController* controller = new CController(63);
    controller->setControllerValue(CONTROLLER_VALUE_ID,63);
    controller->setControllerValue(CONTROLLER_VALUE_PORT, 1);
    controller->setControllerValue(CONTROLLER_VALUE_ADDRESS, 63);
    controller->setControllerValue(CONTROLLER_VALUE_AREA, "");
    controller->setControllerValue(CONTROLLER_VALUE_LOCATION, "");
    controller->setControllerValue(CONTROLLER_VALUE_NAME, "控制器");
    controller->setNone();
    project->addController(controller);
//    controller->setStatus(OBJS_Online,1);
    controller->setControllerValue(OBJECT_VALUE_ONLINE,1);
    QSqlQuery query = db->query(QString("SELECT ParamName,ParamValue FROM ParamInfo"));
    QSqlRecord record = query.record();
    int fieldParamName = record.indexOf("ParamName");
    int fieldParamValue = record.indexOf("ParamValue");
    while(query.next())
    {
        if(query.value(fieldParamName).toString() == "PrintWarning")
        {
            CGlobal::instance()->m_PrintWarning = query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "PrintFault")
        {
            CGlobal::instance()->m_PrintFault = query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "PrintOperation")
        {
            CGlobal::instance()->m_PrintOperation = query.value(fieldParamValue).toBool();
        }
        else if(query.value(fieldParamName).toString() == "SendDirectionAndTwinkleByOne")
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
            CGlobal::instance()->setProjectName(query.value(fieldParamValue).toString());
//            CGlobal::instance()->m_projectName = query.value(fieldParamValue).toString();
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
        else if(query.value(fieldParamName).toString() == "ProjectID")
        {
            CGlobal::instance()->m_ProjectID = query.value(fieldParamValue).toString();
        }
        else if(query.value(fieldParamName).toString() == "LiftTime")
        {
            CGlobal::instance()->m_LiftTime = query.value(fieldParamValue).toInt();
        }
    }
    CGlobal::instance()->TimeInterval()->updateSqliteData();
    CMsgObjectStatus m_MsgObjectStatus;
    QByteArray data;
    data.append(CGlobal::instance()->m_PrintOperation);
    data.append(CGlobal::instance()->m_PrintFault);
    data.append(CGlobal::instance()->m_PrintWarning);
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
    if(CDistribution::loadDistribution(db, controller))
        return -1;
    return 0;
}

CObject* CController::parent()
{
    return m_project;
}

bool CController::addDistribution(CDistribution *distribution)
{
    if(!distribution)
        return false;
    if(m_distributions.contains(distribution))
        return false;
    if(hasDistribution(distribution->distributionProductId()))
        return false;
    if(distribution->controller())
        distribution->controller()->removeDistribution(distribution);
    m_distributions.append(distribution);
    distribution->setController(this);
    if(this->isUndefined())
        distribution->setIsUndefined(true);
    m_childs.append(distribution);
    addDeviceTypeAndOnline(distribution);
    calcMaxDistributionId(distribution->distributionId());
    emit objectChanged(CObject::OT_AddChildren, distribution);
    return true;
}

void CController::removeDistribution(CDistribution *distribution)
{
    if(!distribution)
        return;
    if(!m_distributions.contains(distribution))
        return;
    m_distributions.removeOne(distribution);
    distribution->setController(NULL);
    m_childs.removeOne(distribution);
    removeDeviceTypeAndOnline(distribution);
    setModify();
    emit objectChanged(CObject::OT_RemoveChildren, distribution);
}

bool CController::hasDistribution(const QString str) const
{
    return distributionByProductId(str);
}

CDistribution *CController::distributionById(const int id) const
{
    for(int i = 0; i < m_distributions.size(); ++i)
    {
        if(m_distributions.at(i)->distributionId() == id)
            return m_distributions.at(i);
    }
    return NULL;
}

CDistribution *CController::distributionByAddress(const int add) const
{
    for(int i = 0; i < m_distributions.size(); ++i)
    {
        if(m_distributions.at(i)->distributionAddress() == add)
            return m_distributions.at(i);
    }
    return NULL;
}

CDistribution *CController::distributionByProductId(const QString str) const
{
    for(int i = 0; i < m_distributions.size(); ++i)
    {
        if(m_distributions.at(i)->descriptionStr() == str && str != "")
            return m_distributions.at(i);
    }
    return NULL;
}

CDistribution *CController::distributionByKeyId(const int keyid) const
{
    for(int i = 0; i < m_distributions.size(); ++i)
    {
        if(m_distributions.at(i)->keyId() == keyid)
            return m_distributions.at(i);
    }
    return NULL;
}

bool CController::isdistributionByKeyId(const int keyid) const
{
    for(int i = 0; i < m_distributions.size(); ++i)
    {
        if(m_distributions.at(i)->keyId() == keyid)
            return false;
    }
    return true;
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
    if(m_project)
        m_project->adjustedDeviceTypeNumber(type, adj);
    emit deviceTypeChanged();
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
    if(m_project)
        m_project->adjustedDeviceOnlineNumber(type, adj);
}

void CController::setDeviceOnlineTotal(int n)
{
    if(m_deviceOnlineTotal == n)
        return;
    int adj = n - m_deviceOnlineTotal;
    adjustedDeviceOnlineTotal(adj);
}

void CController::adjustedDeviceOnlineTotal(int adj)
{
    m_deviceOnlineTotal += adj;
    if(m_project)
        m_project->adjustedOnlineCount(adj);
    updateOnlineTotalCount();
}

void CController::setOnlineDistributionCount(int n)
{
    if(m_onlineDistributionCount == n)
        return;
    int adj = n - m_onlineDistributionCount;
    adjustedOnlineDistributionCount(adj);
}

void CController::adjustedOnlineDistributionCount(int adj)
{
    m_onlineDistributionCount += adj;
    if(m_project)
        m_project->adjustedOnlineCount(adj);
    updateOnlineTotalCount();
}

void CController::setOnlinePowerCount(int n)
{
    if(m_onlinePowerCount == n)
        return;
    int adj = n - m_onlinePowerCount;
    adjustedOnlinePowerCount(adj);
}

void CController::adjustedOnlinePowerCount(int adj)
{
    m_onlinePowerCount += adj;
    if(m_project)
        m_project->adjustedOnlineCount(adj);
    updateOnlineTotalCount();
}

void CController::resetOnline()
{
    for(int i = 0; i < m_distributions.size(); ++i)
        m_distributions.at(i)->resetDeviceOnlineTotal();
    m_deviceOnlineNumber.clear();
    m_onlineDistributionCount = 0;
    m_onlinePowerCount = 0;
    m_deviceOnlineTotal = 0;
    updateOnlineTotalCount();
}

void CController::afterSetValue(QString &name, QVariant& oldValue, QVariant& newValue)
{
    if(CGlobal::instance()->dm()->isLoading())
        return;
    if(name == CONTROLLER_VALUE_ID)
    {
        if(m_project)
            m_project->calcMaxControllerId(newValue.toInt());
    }
    CObject::afterSetValue(name, oldValue, newValue);
}

QVariant CController::afterGetValue(QString name, QVariant value) const
{
    if(name == OBJECT_VALUE_DISPLAY_NAME)
        return m_controllerValue.value(CONTROLLER_VALUE_NAME);
    else if(name == OBJECT_VALUE_ADDRESS)
        return m_controllerValue.value(CONTROLLER_VALUE_ID);
    else if(name == OBJECT_VALUE_DESCRIPTION)
        return m_controllerValue.value(CONTROLLER_VALUE_DESCRIPTION);
    else if(name == OBJECT_VALUE_ONLINE_COUNT)
        return m_onlineTotalCount;
    else
        return value;
}

void CController::initController()
{
    m_project = NULL;
    m_maxDistributionId = 0;
    setControllerValue(OBJECT_VALUE_PIXMAP, ":/images/controller.png");
    setControllerValue(CONTROLLER_VALUE_NAME, "控制器");
    setValue(OBJECT_VALUE_ONLINE_COUNT, 0);
}


int CController::saveController(CDBSqlite *db)
{
    if(isUndefined())
        return 0;
    if(saveDistribution(db) != 0)
        return -1;
    return 0;
}

int CController::saveDistribution(CDBSqlite *db)
{
    for(int i = 0; i < m_distributions.size(); ++i)
    {
        if(m_distributions.at(i)->save(db) != 0)
            return -1;
    }
    return 0;
}



void CController::updateOnlineTotalCount()
{
    m_onlineTotalCount = m_deviceOnlineTotal;
    setValue(OBJECT_VALUE_ONLINE_COUNT, m_onlineTotalCount);
    emit onlineChanged();
}

void CController::addDeviceTypeAndOnline(CDistribution *distribution)
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

void CController::removeDeviceTypeAndOnline(CDistribution *distribution)
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

