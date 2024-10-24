#include "cprojecttreemodel.h"
#include "dm/cproject.h"
#include "dm/ccontroller.h"
#include "dm/cdistribution.h"
#include "dm/cloop.h"
#include "dm/cdevice.h"
#include "dm/clayer.h"
#include "cglobal.h"
#include "ctreeitem.h"

#include <QPixmap>


CProjectTreeModel::CProjectTreeModel(QObject *parent) :
    CModel(parent), m_project(0)
{
    setTrack(true);
}

CProjectTreeModel::CProjectTreeModel(CProject *project, bool track, QObject *parent) :
    CModel(parent), m_project(0)
{
    setTrack(track);
    setProject(project);
}

void CProjectTreeModel::setProject(CProject *project)
{
    if(m_project != project)
    {
        m_project = project;
        CModel::setRootObject(project);
    }
}

void CProjectTreeModel::objectDestroy(CObject *object)
{
    if(object == m_project)
    {
        m_project = NULL;
        CModel::setRootObject(m_project);
        return;
    }
    CModel::objectDestroy(object);
}

bool CProjectTreeModel::beforeLoadChildren(CObject *object, QModelIndex &parent)
{
    QString type = object->type();
    if(type == CDistribution::type_s)
    {
        CController* controller = static_cast<CController*>(object->parent());
        if(!controller)
            return false;
         QModelIndex index = getIndex(controller);
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
    else if(type == CDevice::type_s || type == CLayer::type_s)
        return false;
    return true;
}

void CProjectTreeModel::afterLoadChildren(CObject *object, QModelIndex &parent)
{
    Q_UNUSED(parent);
    QString type = object->type();
    if(type == CController::type_s)
    {
        CController* controller = static_cast<CController*>(object);
        if(!controller)
            return;
        /* 注释此处避免在数中出现分配电和集中电源节点 */
//        if(!getIndex(controller->virtualDistribution()).isValid())
//            appendObject(controller->virtualDistribution(), controller);
//        if(!getIndex(controller->virtualPower()).isValid())
//            appendObject(controller->virtualPower(), controller);
    }
}

QVariant CProjectTreeModel::getDisplayData(const QModelIndex &index) const
{
    CObject* data = getObject(index);
    if(!data)
        return QVariant();
    return data->value(OBJECT_VALUE_DISPLAY_NAME).toString() + QString(" ( %1 )").arg(data->value(OBJECT_VALUE_ONLINE_COUNT).toInt());
}

QVariant CProjectTreeModel::getDecorationData(const QModelIndex &index) const
{
    CObject* data = getObject(index);
    if(!data)
        return QVariant();
    return QPixmap(data->value(OBJECT_VALUE_PIXMAP).toString()).scaled(16,16);
}

QVariant CProjectTreeModel::getTextColorData(const QModelIndex &index) const
{
    CObject* data = getObject(index);
    if(!data)
        return QVariant();
    if(data->isUndefined())
        return QColor(0x66, 0x8e, 0x3c);
    if(data->type() == CLoop::type_s)
    {
        return ((data->getStatus(OBJS_Online)==1) && (data->value(OBJECT_VALUE_ONLINE_COUNT).toInt() > 0)) ? QColor(0,139,0) : QColor(139,125,107);
    }
    if(data->type() == CDistribution::type_s)
    {
        return ((data->getStatus(OBJS_DistributionCommunicationFault) == 1) || (data->getStatus(OBJS_Online) == 0)) ? QColor(139,125,107) : QColor(0,139,0);
    }
    if(data->type() == CController::type_s)
    {
        return ((data->getStatus(OBJS_Online)==1) || (data->value(OBJECT_VALUE_ONLINE_COUNT).toInt() > 0)) ? QColor(0,139,0) : QColor(139,125,107);
    }
    return (data->getStatus(OBJS_Online)==1) ? QColor(0,139,0) : QColor(139,125,107);
}

QVariant CProjectTreeModel::getTextFontData(const QModelIndex &index) const
{
    QFont font; // 创建一个QFont对象
    font.setPointSize(12); // 设置字体大小为12
    CObject* data = getObject(index);
    if(!data)
        return QVariant();
    return font;
}

//QVariant CProjectTreeModel::getToolTipData(const QModelIndex &index) const
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
