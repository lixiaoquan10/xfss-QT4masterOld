#include "cglobal.h"



CControllerTreeModel::CControllerTreeModel(QObject *parent) :
    CModel(parent), m_controller(0)
{
    setTrack(true);
}

CControllerTreeModel::CControllerTreeModel(CController *controller, bool track, QObject *parent) :
    CModel(parent), m_controller(0)
{
    setTrack(track);
    setController(controller);
}

void CControllerTreeModel::setController(CController *controller)
{
    if(m_controller != controller)
    {
        m_controller = controller;
        CModel::setRootObject(controller);
    }
}

void CControllerTreeModel::objectDestroy(CObject *object)
{
    if(object == m_controller)
    {
        m_controller = NULL;
        CModel::setRootObject(m_controller);
        return;
    }
    CModel::objectDestroy(object);
}

bool CControllerTreeModel::beforeLoadChildren(CObject *object, QModelIndex &parent)
{
    QString type = object->type();
    if(type == CDistribution::type_s)
    {
        CCanport* canport = static_cast<CCanport*>(object->parent());
        if(!canport)
            return false;
         QModelIndex index = getIndex(canport);
        if(!index.isValid())
            return false;
        parent = index;
        return true;
    }
    else if(type == CLoop::type_s)
    {
        CLoop* loop = static_cast<CLoop*>(object);

        CDistribution* distribution = loop->distribution();
        if(!distribution)
            return false;

        QModelIndex index = getIndex(distribution);
        if(!index.isValid())
            return false;
        parent = index;
        return true;
    }
    else if(type == CDevice::type_s || type == CLayer::type_s || type == CFirePoint::type_s)
        return false;
    return true;
}

void CControllerTreeModel::afterLoadChildren(CObject *object, QModelIndex &parent)
{
    Q_UNUSED(parent);
    QString type = object->type();
    if(type == CCanport::type_s)
    {
        CCanport* canport = static_cast<CCanport*>(object);
        if(!canport)
            return;
        /* 注释此处避免在数中出现分配电和集中电源节点 */
//        if(!getIndex(canport->virtualDistribution()).isValid())
//            appendObject(canport->virtualDistribution(), canport);
//        if(!getIndex(canport->virtualPower()).isValid())
//            appendObject(canport->virtualPower(), canport);
    }
}

QVariant CControllerTreeModel::getDisplayData(const QModelIndex &index) const
{
    CObject* data = getObject(index);
    QString objectType = data->type();
    if(!data)
        return QVariant();
    QString text = data->value(OBJECT_VALUE_DISPLAY_NAME).toString();
    if(objectType == CController::type_s)
    {
        int distributionSum = 0;
        CController* controller = static_cast<CController*>(data);
        for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
        {
            CCanport* canport = controller->canportByAddress(i);
            distributionSum += canport->getOnlineDistributionCount();
        }
        text = text + QString(" ( %1 )").arg(distributionSum);
    }
    if(objectType == CLoop::type_s)
    {
        CLoop* loop = static_cast<CLoop*>(data);
        text = text + QString(" ( %1 )").arg(data->value(OBJECT_VALUE_ONLINE_COUNT).toInt());
        if((loop->isLoopOnline() && (!loop->isLoopHasMALamp()) && (data->value(OBJECT_VALUE_ONLINE_COUNT).toInt() > 0)))
        {
            text = text + QString(" ( %1 W )").arg(loop->loopPower());
        }
    }
    return text;
}

QVariant CControllerTreeModel::getDecorationData(const QModelIndex &index) const
{
    CObject* data = getObject(index);
    QString objectType = data->type();
    if(!data)
        return QVariant();
    if(objectType == CCanport::type_s)
    {
        CCanport* canport = static_cast<CCanport*>(data);
        return QPixmap(canport->canportValue(OBJECT_VALUE_PIXMAP).toString()).scaled(16,16);
    }
    else if(objectType == CDistribution::type_s)
    {
        CDistribution* distribution = static_cast<CDistribution*>(data);
        return QPixmap(distribution->distributionValue(OBJECT_VALUE_PIXMAP).toString()).scaled(16,16);
    }
    else if(objectType == CLoop::type_s)
    {
        CLoop* loop = static_cast<CLoop*>(data);
        return QPixmap(loop->loopValue(OBJECT_VALUE_PIXMAP).toString()).scaled(16,16);
    }
    return QPixmap(data->value(OBJECT_VALUE_PIXMAP).toString()).scaled(16,16);
}

QVariant CControllerTreeModel::getTextColorData(const QModelIndex &index) const
{
    CObject* data = getObject(index);
    if(!data)
        return QVariant();
    if(data->isUndefined())
        return QColor(0x66, 0x8e, 0x3c);
    if(data->type() == CLoop::type_s)
    {
        CLoop* loop = static_cast<CLoop*>(data);
        if((loop->isLoopOnline() && (data->value(OBJECT_VALUE_ONLINE_COUNT).toInt() > 0)))
        {
            if(loop->loopPower() > 100)
                return QColor(255,99,71);
            else
                return QColor(0,139,0);
        }
        else
            return QColor(139,125,107);
    }
    if(data->type() == CDistribution::type_s)
    {
        CDistribution* distribution = static_cast<CDistribution*>(data);
        return ((data->getStatus(OBJS_DistributionCommunicationFault) == 1) || (!distribution->isDistributionOnline())) ? QColor(139,125,107) : QColor(0,139,0);
    }
    if(data->type() == CCanport::type_s)
    {
        CCanport* canport = static_cast<CCanport*>(data);
        return ((canport->canportValue(OBJECT_VALUE_ONLINE)==1) || (data->value(OBJECT_VALUE_ONLINE_COUNT).toInt() > 0)) ? QColor(0,139,0) : QColor(139,125,107);
    }
    return (data->getStatus(OBJS_Online)==1) ? QColor(0,139,0) : QColor(139,125,107);
}

QVariant CControllerTreeModel::getTextFontData(const QModelIndex &index) const
{
    QFont font; // 创建一个QFont对象
    font.setPointSize(12); // 设置字体大小为12
    CObject* data = getObject(index);
    if(!data)
        return QVariant();
    return font;
}

//QVariant CControllerTreeModel::getToolTipData(const QModelIndex &index) const
//{
//    CObject* data = getObject(index);
//    QString str;
//    if(!data)
//        return str;
//    int address = data->value(OBJECT_VALUE_ADDRESS).toInt();
//    if(address > 0)
//        str.append(tr("address:%1").arg(address));
//    QString description = data->value(OBJECT_VALUE_DESCRIPTION).toString();
//    if(!description.isEmpty())
//        str.append(tr("\ndescription:%1").arg(description));
//    return str;
//}
