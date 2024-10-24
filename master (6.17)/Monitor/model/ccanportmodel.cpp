#include "cglobal.h"


#define CONTROLLER_MODEL_PIXMAP "canport_model_pixmap"

CCanportModel::CCanportModel(QObject *parent) :
    CModel(parent), m_canport(0)
{
    initObject();
    setTrack(true);
}

CCanportModel::~CCanportModel()
{
    delete m_distributionCount;
    delete m_loopCount;
}

CCanportModel::CCanportModel(CCanport *canport, bool track, QObject *parent) :
    CModel(parent), m_canport(0)
{
    initObject();
    setTrack(track);
    setCanport(canport);
}

int CCanportModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant CCanportModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == CANPORT_COLUMN_NAME)
            return "名称";
        else if(section == CANPORT_COLUMN_NUMBER)
            return "数量";
        else if(section == CANPORT_COLUMN_ONLINE)
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

void CCanportModel::setCanport(CCanport *canport)
{
    if(m_canport != canport)
    {
        clean();
        m_canport = canport;
        if(m_canport)
        {
            connectObject(m_canport);
            m_rootObject = m_canport;
            addItem(m_canport, m_root);
            appendObject(m_distributionCount, m_canport);

//            QVector<CDeviceType*> deviceTypes = CGlobal::instance()->deviceTypes();
//            for(int i = 0; i < deviceTypes.size(); ++i){
//                CDeviceType* deviceType = deviceTypes.value(i, NULL);
//                if(!deviceType) continue;
//                appendObject(m_deviceCount.value(deviceType->type()), m_canport);
//            }

            connect(m_canport, SIGNAL(deviceTypeChanged()), this, SLOT(updateDeviceType()));
            connect(m_canport, SIGNAL(onlineChanged()), this, SLOT(updateOnline()));
        }
        calcObject();
        updateDeviceType();
        updateOnline();
    }
}

void CCanportModel::objectChanged(CObject::ObjectType type, CObject *object)
{
    Q_UNUSED(type);
    Q_UNUSED(object);
    calcObject();
}

void CCanportModel::objectDestroy(CObject *object)
{
    if(object == m_canport)
    {
        m_canport = NULL;
        CModel::setRootObject(m_canport);
        return;
    }
    CModel::objectDestroy(object);
}

QVariant CCanportModel::data(const QModelIndex &index, int role) const
{
    CObject* data = getObject(index);
    if(!data)
        return QVariant();
    if(role == Qt::DisplayRole){
        if(index.column() == CANPORT_COLUMN_NAME)
            return data->value(OBJECT_VALUE_DISPLAY_NAME);
        else if(index.column() == CANPORT_COLUMN_NUMBER)
            return data->value("number");
        else if(index.column() == CANPORT_COLUMN_ONLINE)
            return data->value("online_number");
    }
    if(role == Qt::DecorationRole){
        if(index.column() == CANPORT_COLUMN_NAME)
            return data->value(CONTROLLER_MODEL_PIXMAP);
    }
    return QVariant();
}

void CCanportModel::initObject()
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

void CCanportModel::calcObject()
{
    if(m_canport)
    {
        int distributionCount = m_canport->distributionCount();
        m_distributionCount->setValue("number", distributionCount);
    }
    else
    {
        m_distributionCount->setValue("number", 0);
    }
}

void CCanportModel::updateDeviceType()
{
    QVector<CDeviceType*> deviceTypes = CGlobal::instance()->deviceTypes();
    QHash<int, int> deviceType;
    if(m_canport)
        deviceType = m_canport->getDeviceTypeNumber();
    for(int i = 0; i < deviceTypes.size(); ++i)
    {
        CDeviceType* Type = deviceTypes.value(i, NULL);
        if(!Type) continue;
        int ntype = Type->type();
        m_deviceCount.value(ntype)->setValue("number", deviceType.value(ntype, 0));
    }
}

void CCanportModel::updateOnline()
{
    QVector<CDeviceType*> deviceTypes = CGlobal::instance()->deviceTypes();
    QHash<int, int> deviceOnline;
    if(m_canport)
    {
        deviceOnline = m_canport->getDeviceOnlineNumber();
        m_distributionCount->setValue("online_number", m_canport->getOnlineDistributionCount());
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
