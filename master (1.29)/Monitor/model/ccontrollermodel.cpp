#include "ccontrollermodel.h"
#include "ctreeitem.h"
#include "dm/ccontroller.h"
#include "dm/cdistribution.h"
#include "dm/cdevice.h"
#include "cdevicetype.h"
#include "cglobal.h"

#include <QPixmap>
#include <QDebug>

#define CONTROLLER_MODEL_PIXMAP "controller_model_pixmap"

CControllerModel::CControllerModel(QObject *parent) :
    CModel(parent), m_controller(0)
{
    initObject();
    setTrack(true);
}

CControllerModel::~CControllerModel()
{
    delete m_distributionCount;
    delete m_loopCount;
}

CControllerModel::CControllerModel(CController *project, bool track, QObject *parent) :
    CModel(parent), m_controller(0)
{
    initObject();
    setTrack(track);
    setController(project);
}

int CControllerModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant CControllerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == CONTROLLER_COLUMN_NAME)
            return "名称";
        else if(section == CONTROLLER_COLUMN_NUMBER)
            return "数量";
        else if(section == CONTROLLER_COLUMN_ONLINE)
            return "在线数量";
    }
    else if (role == Qt::FontRole && orientation == Qt::Horizontal)
    {
        QFont font;
        font.setPointSize(12);  // 设置字号为12
        return font;
    }
    return QVariant();
}

void CControllerModel::setController(CController *controller)
{
    if(m_controller != controller)
    {
        clean();
        m_controller = controller;
        if(m_controller)
        {
            connectObject(m_controller);
            m_rootObject = m_controller;
            addItem(m_controller, m_root);
            appendObject(m_distributionCount, m_controller);

//            QVector<CDeviceType*> deviceTypes = CGlobal::instance()->deviceTypes();
//            for(int i = 0; i < deviceTypes.size(); ++i){
//                CDeviceType* deviceType = deviceTypes.value(i, NULL);
//                if(!deviceType) continue;
//                appendObject(m_deviceCount.value(deviceType->type()), m_controller);
//            }

            connect(m_controller, SIGNAL(deviceTypeChanged()), this, SLOT(updateDeviceType()));
            connect(m_controller, SIGNAL(onlineChanged()), this, SLOT(updateOnline()));
        }
        calcObject();
        updateDeviceType();
        updateOnline();
    }
}

void CControllerModel::objectChanged(CObject::ObjectType type, CObject *object)
{
    Q_UNUSED(type);
    Q_UNUSED(object);
    calcObject();
}

void CControllerModel::objectDestroy(CObject *object)
{
    if(object == m_controller)
    {
        m_controller = NULL;
        CModel::setRootObject(m_controller);
        return;
    }
    CModel::objectDestroy(object);
}

QVariant CControllerModel::data(const QModelIndex &index, int role) const
{
    CObject* data = getObject(index);
    if(!data)
        return QVariant();
    if(role == Qt::DisplayRole){
        if(index.column() == CONTROLLER_COLUMN_NAME)
            return data->value(OBJECT_VALUE_DISPLAY_NAME);
        else if(index.column() == CONTROLLER_COLUMN_NUMBER)
            return data->value("number");
        else if(index.column() == CONTROLLER_COLUMN_ONLINE)
            return data->value("online_number");
    }
    if(role == Qt::DecorationRole){
        if(index.column() == CONTROLLER_COLUMN_NAME)
            return data->value(CONTROLLER_MODEL_PIXMAP);
    }
    return QVariant();
}

void CControllerModel::initObject()
{
    m_distributionCount = new CObject();
    m_distributionCount->setValue(OBJECT_VALUE_DISPLAY_NAME, "集中电源");
    m_distributionCount->setValue("number", 0);
    m_distributionCount->setValue("online_number", 0);
    m_distributionCount->setValue(CONTROLLER_MODEL_PIXMAP, QPixmap(":/images/distribution.png"));
    m_loopCount = NULL;
    QVector<CDeviceType*> deviceTypes = CGlobal::instance()->deviceTypes();
    for(int i = 0; i < deviceTypes.size(); ++i)
    {
        CDeviceType* deviceType = deviceTypes.value(i, NULL);
        if(!deviceType) continue;
        CObject* object = new CObject();
        object->setValue(OBJECT_VALUE_DISPLAY_NAME, deviceType->name());
        object->setValue("number", 0);
        object->setValue("online_number", 0);
        object->setValue(CONTROLLER_MODEL_PIXMAP, deviceType->imageReal(0.5));
        m_deviceCount.insert(deviceType->type(), object);
    }
}

void CControllerModel::calcObject()
{
    if(m_controller)
    {
        int distributionCount = m_controller->distributionCount();
        m_distributionCount->setValue("number", distributionCount);
    }
    else
    {
        m_distributionCount->setValue("number", 0);
    }
}

void CControllerModel::updateDeviceType()
{
    QVector<CDeviceType*> deviceTypes = CGlobal::instance()->deviceTypes();
    QHash<int, int> deviceType;
    if(m_controller)
        deviceType = m_controller->getDeviceTypeNumber();
    for(int i = 0; i < deviceTypes.size(); ++i)
    {
        CDeviceType* Type = deviceTypes.value(i, NULL);
        if(!Type) continue;
        int ntype = Type->type();
        m_deviceCount.value(ntype)->setValue("number", deviceType.value(ntype, 0));
    }
}

void CControllerModel::updateOnline()
{
    QVector<CDeviceType*> deviceTypes = CGlobal::instance()->deviceTypes();
    QHash<int, int> deviceOnline;
    if(m_controller)
    {
        deviceOnline = m_controller->getDeviceOnlineNumber();
        m_distributionCount->setValue("online_number", m_controller->getOnlineDistributionCount());
    }
    else
    {
        m_distributionCount->setValue("online_number", 0);
    }
    for(int i = 0; i < deviceTypes.size(); ++i)
    {
        CDeviceType *pType = deviceTypes.value(i, NULL);
        if(!pType) continue;
        int type = pType->type();
        m_deviceCount.value(type)->setValue("online_number", deviceOnline.value(type, 0));
        qDebug() << "online number" << type << deviceOnline.value(type,0);
    }
}
