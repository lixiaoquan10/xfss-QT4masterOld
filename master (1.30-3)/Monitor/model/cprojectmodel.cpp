#include "cprojectmodel.h"
#include "ctreeitem.h"
#include "dm/cproject.h"
#include "dm/ccanport.h"
#include "cglobal.h"
#include <QMessageBox>

CProjectModel::CProjectModel(QObject *parent) :
    CModel(parent), m_project(0)
{
    setTrack(true);
}

CProjectModel::CProjectModel(CProject *project, bool track, QObject *parent) :
    CModel(parent), m_project(0)
{
    setTrack(track);
    setProject(project);
}

Qt::ItemFlags CProjectModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return CModel::flags(index);
}

int CProjectModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant CProjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == PROJECT_COLUMN_ADDRESS)
            return "地址";
        else if(section == PROJECT_COLUMN_NAME)
            return "名称";
        else if(section == PROJECT_COLUMN_DESCRIPTION)
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

void CProjectModel::setProject(CProject *project)
{
    if(m_project != project)
    {
        clean();
        m_project = project;
        if(m_project)
        {
            connectObject(m_project);
            m_rootObject = m_project;
            addItem(m_project, m_root);
            QList<CCanport*> canports = m_project->canports();
            for(int i = 0; i < canports.size(); ++i)
            {
                loadChildren(canports.at(i));
            }
        }
    }
}

void CProjectModel::objectChanged(CObject::ObjectType type, CObject *object)
{
    if(object->type() != CCanport::type_s)
        return;
    if(type == CObject::OT_AddChildren)
    {
        appendObject(object, m_project);
    }
    if(type == CObject::OT_RemoveChildren)
    {
        removeObject(object);
    }
}

void CProjectModel::objectDestroy(CObject *object)
{
    if(object == m_project)
    {
        m_project = NULL;
        CModel::setRootObject(m_project);
        return;
    }
    CModel::objectDestroy(object);
}

bool CProjectModel::beforeLoadChildren(CObject *object, QModelIndex &parent)
{
    Q_UNUSED(parent);
    QString type = object->type();
    if(type != CCanport::type_s)
        return false;
    return true;
}

QVariant CProjectModel::getDisplayData(const QModelIndex &index) const
{
    CObject* data = getObject(index);
    CCanport* canport = static_cast<CCanport*>(getObject(index));
    if(!data)
        return QVariant();
    if(index.column() == PROJECT_COLUMN_ADDRESS)
        return canport->canportValue(CANPORT_VALUE_ID);
    else if(index.column() == PROJECT_COLUMN_NAME)
        return data->value(OBJECT_VALUE_DISPLAY_NAME);
    else if(index.column() == PROJECT_COLUMN_DESCRIPTION)
        return canport->canportValue(CANPORT_VALUE_DESCRIPTION);
    return QVariant();
}

//QVariant CProjectModel::getTextColorData(const QModelIndex &index) const
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

//QVariant CProjectModel::getEditData(const QModelIndex &index) const
//{
//    CObject* data = getObject(index);
//    if(!data)
//        return QVariant();
//    if(index.column() == PROJECT_COLUMN_ADDRESS)
//        return data->value(CANPORT_VALUE_ID);
//    else if(index.column() == PROJECT_COLUMN_NAME)
//        return data->value(OBJECT_VALUE_DISPLAY_NAME);
//    else if(index.column() == PROJECT_COLUMN_DESCRIPTION)
//        return data->value(CANPORT_VALUE_DESCRIPTION);
//    return QVariant();
//}

//bool CProjectModel::setEditData(const QModelIndex &index, const QVariant &value)
//{
//    CCanport* canport = static_cast<CCanport*>(getObject(index));
//    if(!canport)
//        return false;
//    CProject* project = canport->project();
//    if(!project)
//        return false;
//    switch(index.column())
//    {
//    case PROJECT_COLUMN_ADDRESS:
//    {
//        int id = value.toInt();
//        if(id <= 0 || id > PROJECT_MAX_Canport_NUM)
//            return false;
//        if(project->hasCanport(id))
//        {
////                QMessageBox::information(NULL, tr("Information"), tr("The exists canport id"), QMessageBox::Ok);
//            return false;
//        }
//        canport->setCanportId(id);
//        break;
//    }
//    case PROJECT_COLUMN_NAME:
//    {
//        QString name = value.toString();
//        if(name.isEmpty())
//            return false;
//        canport->setValue(CANPORT_VALUE_NAME, name);
//        break;
//    }
//    case PROJECT_COLUMN_DESCRIPTION:
//    {
//        canport->setValue(CANPORT_VALUE_DESCRIPTION, value);
//        break;
//    }
//    default:
//        return false;
//    }
//    return true;
//}
