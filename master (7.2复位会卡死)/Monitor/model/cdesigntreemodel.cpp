#include "cglobal.h"


CDesignTreeModel::CDesignTreeModel(QObject *parent) :
    CModel(parent), m_controller(0)
{
    setTrack(true);
}

CDesignTreeModel::CDesignTreeModel(CController *controller, bool track, QObject *parent) :
    CModel(parent), m_controller(0)
{
    setTrack(track);
    setController(controller);
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

void CDesignTreeModel::setController(CController *controller)
{
    if(m_controller != controller)
    {
        m_controller = controller;
        CModel::setRootObject(controller);
    }
}

void CDesignTreeModel::objectDestroy(CObject *object)
{
    if(object == m_controller)
    {
        m_controller = NULL;
        CModel::setRootObject(m_controller);
        return;
    }
    CModel::objectDestroy(object);
}

bool CDesignTreeModel::beforeLoadChildren(CObject *object, QModelIndex &parent)
{
    QString type = object->type();
    if(type == CController::type_s)
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

//QVariant CDesignTreeModel::getEditData(const QModelIndex &index) const
//{
//    CObject* data = getObject(index);
//    if(!data)
//        return QVariant();
//    return data->value(OBJECT_VALUE_DISPLAY_NAME);
//}

QVariant CDesignTreeModel::getDecorationData(const QModelIndex &index) const
{
    CObject* data = getObject(index);
    QString objectType = data->type();
    if(!data)
        return QVariant();
//    if(data->isFireStatus())
//        return QPixmap(":/images/fire.png");
//    else if(data->isFaultStatus())
//        return QPixmap(":/images/fault.png");
    if(objectType == CLayer::type_s)
    {
        CLayer* layer = static_cast<CLayer*>(data);
        return QPixmap(layer->layerValue(OBJECT_VALUE_PIXMAP).toString()).scaled(16,16);
    }
    else
        return QPixmap(data->value(OBJECT_VALUE_PIXMAP).toString()).scaled(16,16);
}

QVariant CDesignTreeModel::getTextColorData(const QModelIndex &index) const
{
    CObject* data = getObject(index);
    if(!data)
        return QVariant();
//    if(data->isFireStatus())
//        return Qt::red;
//    else if(data->isFaultStatus())
//        return QColor(0xff, 0x99, 0x00);
    return QVariant();
}

QVariant CDesignTreeModel::getTextFontData(const QModelIndex &index) const
{
    QFont font; // 创建一个QFont对象
    font.setPointSize(12); // 设置字体大小为12
    CObject* data = getObject(index);
    if(!data)
        return QVariant();
    return font;
}

//bool CDesignTreeModel::setEditData(const QModelIndex &index, const QVariant &value)
//{
//    QString name = value.toString();
//    if(name.isEmpty())
//        return false;
//    CObject* object = getObject(index);
//    if(!object)
//        return false;
//    if(object->type() != CLayer::type_s)
//        return false;
//    object->setValue(LAYER_VALUE_NAME, value);
//    return true;
//}
