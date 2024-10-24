#include "ccontrollermodel.h"
#include "ctreeitem.h"
#include "dm/ccontroller.h"
#include "dm/ccanport.h"
#include "cglobal.h"
#include <QMessageBox>

CControllerModel::CControllerModel(QObject *parent) :
    CModel(parent), m_controller(0)
{
    setTrack(true);
}

CControllerModel::CControllerModel(CController *controller, bool track, QObject *parent) :
    CModel(parent), m_controller(0)
{
    setTrack(track);
    setController(controller);
}

Qt::ItemFlags CControllerModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return CModel::flags(index);
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
        if(section == CONTROLLER_COLUMN_ADDRESS)
            return "地址";
        else if(section == CONTROLLER_COLUMN_NAME)
            return "名称";
        else if(section == CONTROLLER_COLUMN_DESCRIPTION)
            return "描述";
    }
    else if(orientation == Qt::Vertical && role == Qt::DisplayRole)
    {
        return QString::number(section + 1);
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
            QList<CCanport*> canports = m_controller->canports();
            for(int i = 0; i < canports.size(); ++i)
            {
                loadChildren(canports.at(i));
            }
        }
    }
}

void CControllerModel::objectChanged(CObject::ObjectType type, CObject *object)
{
    if(object->type() != CCanport::type_s)
        return;
    if(type == CObject::OT_AddChildren)
    {
        appendObject(object, m_controller);
    }
    if(type == CObject::OT_RemoveChildren)
    {
        removeObject(object);
    }
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

bool CControllerModel::beforeLoadChildren(CObject *object, QModelIndex &parent)
{
    Q_UNUSED(parent);
    QString type = object->type();
    if(type != CCanport::type_s)
        return false;
    return true;
}

QVariant CControllerModel::getDisplayData(const QModelIndex &index) const
{
    CObject* data = getObject(index);
    CCanport* canport = static_cast<CCanport*>(getObject(index));
    if(!data)
        return QVariant();
    if(index.column() == CONTROLLER_COLUMN_ADDRESS)
        return canport->canportValue(CANPORT_VALUE_ID);
    else if(index.column() == CONTROLLER_COLUMN_NAME)
        return data->value(OBJECT_VALUE_DISPLAY_NAME);
    else if(index.column() == CONTROLLER_COLUMN_DESCRIPTION)
        return canport->canportValue(CANPORT_VALUE_DESCRIPTION);
    return QVariant();
}

//QVariant CControllerModel::getTextColorData(const QModelIndex &index) const
//{
//    CObject* data = getObject(index);
//    if(!data)
//        return QVariant();
////    if(data->isFaultStatus())
////        return QColor(0xff, 0x99, 0x00);
//    if(data->isUndefined())
//        return QColor(0x66, 0x8e, 0x3c);
//    return data->getStatus(OBJS_Online) ? MODEL_ONLINE_COLOR : MODEL_OFFLINE_COLOR;
//}

//QVariant CControllerModel::getEditData(const QModelIndex &index) const
//{
//    CObject* data = getObject(index);
//    if(!data)
//        return QVariant();
//    if(index.column() == CONTROLLER_COLUMN_ADDRESS)
//        return data->value(CANPORT_VALUE_ID);
//    else if(index.column() == CONTROLLER_COLUMN_NAME)
//        return data->value(OBJECT_VALUE_DISPLAY_NAME);
//    else if(index.column() == CONTROLLER_COLUMN_DESCRIPTION)
//        return data->value(CANPORT_VALUE_DESCRIPTION);
//    return QVariant();
//}

//bool CControllerModel::setEditData(const QModelIndex &index, const QVariant &value)
//{
//    CCanport* canport = static_cast<CCanport*>(getObject(index));
//    if(!canport)
//        return false;
//    CController* controller = canport->controller();
//    if(!controller)
//        return false;
//    switch(index.column())
//    {
//    case CONTROLLER_COLUMN_ADDRESS:
//    {
//        int id = value.toInt();
//        if(id <= 0 || id > CONTROLLER_MAX_Canport_NUM)
//            return false;
//        if(controller->hasCanport(id))
//        {
////                QMessageBox::information(NULL, tr("Information"), tr("The exists canport id"), QMessageBox::Ok);
//            return false;
//        }
//        canport->setCanportId(id);
//        break;
//    }
//    case CONTROLLER_COLUMN_NAME:
//    {
//        QString name = value.toString();
//        if(name.isEmpty())
//            return false;
//        canport->setValue(CANPORT_VALUE_NAME, name);
//        break;
//    }
//    case CONTROLLER_COLUMN_DESCRIPTION:
//    {
//        canport->setValue(CANPORT_VALUE_DESCRIPTION, value);
//        break;
//    }
//    default:
//        return false;
//    }
//    return true;
//}
