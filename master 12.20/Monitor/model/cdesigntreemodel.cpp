#include "cdesigntreemodel.h"
#include "dm/cproject.h"
#include "dm/clayer.h"
#include "ctreeitem.h"
#include "cglobal.h"

#include <QPixmap>
#include <QDebug>

CDesignTreeModel::CDesignTreeModel(QObject *parent) :
    CModel(parent), m_project(0)
{
    setTrack(true);
}

CDesignTreeModel::CDesignTreeModel(CProject *project, bool track, QObject *parent) :
    CModel(parent), m_project(0)
{
    setTrack(track);
    setProject(project);
}

Qt::ItemFlags CDesignTreeModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return CModel::flags(index);
    CObject* object = getObject(index);
    if(!object)
        return CModel::flags(index);
    if(object->type() != CLayer::type_s)
        return CModel::flags(index);
    return CModel::flags(index);
}

void CDesignTreeModel::setProject(CProject *project)
{
    if(m_project != project)
    {
        m_project = project;
        CModel::setRootObject(project);
    }
}

void CDesignTreeModel::objectDestroy(CObject *object)
{
    if(object == m_project)
    {
        m_project = NULL;
        CModel::setRootObject(m_project);
        return;
    }
    CModel::objectDestroy(object);
}

bool CDesignTreeModel::beforeLoadChildren(CObject *object, QModelIndex &parent)
{
    QString type = object->type();
    if(type == CProject::type_s)
        return true;
    else if(type == CLayer::type_s)
    {
        CLayer* layer = static_cast<CLayer*>(object);
        CObject* parentObject = layer->parentLayer();
        if(!parentObject)
            return true;
        QModelIndex index = getIndex(parentObject);
        if(!index.isValid())
            return false;
        parent = index;
        return true;
    }
    return false;
}

QVariant CDesignTreeModel::getDisplayData(const QModelIndex &index) const
{
    CObject* data = getObject(index);
    if(!data)
        return QVariant();
    return data->value(OBJECT_VALUE_DISPLAY_NAME);
}

QVariant CDesignTreeModel::getEditData(const QModelIndex &index) const
{
    CObject* data = getObject(index);
    if(!data)
        return QVariant();
    return data->value(OBJECT_VALUE_DISPLAY_NAME);
}

QVariant CDesignTreeModel::getDecorationData(const QModelIndex &index) const
{
    CObject* data = getObject(index);
    if(!data)
        return QVariant();
    if(data->isFireStatus())
        return QPixmap(":/images/fire.png");
    else if(data->isFaultStatus())
        return QPixmap(":/images/fault.png");
    return QPixmap(data->value(OBJECT_VALUE_PIXMAP).toString()).scaled(16,16);
}

QVariant CDesignTreeModel::getTextColorData(const QModelIndex &index) const
{
    CObject* data = getObject(index);
    if(!data)
        return QVariant();
    if(data->isFireStatus())
        return Qt::red;
    else if(data->isFaultStatus())
        return QColor(0xff, 0x99, 0x00);
    return QVariant();
}

bool CDesignTreeModel::setEditData(const QModelIndex &index, const QVariant &value)
{
    QString name = value.toString();
    if(name.isEmpty())
        return false;
    CObject* object = getObject(index);
    if(!object)
        return false;
    if(object->type() != CLayer::type_s)
        return false;
    object->setValue(LAYER_VALUE_NAME, value);
    return true;
}
