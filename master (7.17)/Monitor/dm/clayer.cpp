#include "cglobal.h"


const QString CLayer::type_s = "CLayer";

CLayer::CLayer(const int keyId) :
    CObject(keyId)
{
    initLayer();
}

CLayer::CLayer(CController *controller, CLayer *parent, const int keyId) :
    CObject(keyId)
{
    initLayer();
    if(parent)
        parent->addChildLayer(this);
    int n = controller->maxLayerId() + 1;
    setLayerId(n);
    controller->addLayer(this);
    setLayerValue(LAYER_VALUE_NAME, layerValue(LAYER_VALUE_NAME).toString().append(QString::number(n)));
}

CLayer::~CLayer()
{
    if(CGlobal::instance()->dm()->isClearing())
        return;
    if(m_controller)
        m_controller->removeLayer(this);
    m_controller = NULL;
    m_parentLayer = NULL;
}

bool CLayer::setLayerValue(QString name, QVariant value)
{
    qDebug() << "CLayer::setLayerValue" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QVariant old;
    if(m_layerValue.contains(name))
    {
        old = m_layerValue.value(name);
        if(value.isNull())
        {
            m_layerValue.remove(name);
            return true;
        }
        if(old.type() != value.type() || old == value)
            return false;
        m_layerValue[name] = value;
        return true;
    }
    else
    {
        m_layerValue.insert(name, value);
        return true;
    }
}

QVariant CLayer::layerValue(const QString name) const
{
    return m_layerValue.value(name);
}


int CLayer::loadLayer(CDBSqlite *db, CController* controller)
{
    qDebug() << "CLayer::loadLayer" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString sql = QString("SELECT ID, BackgroundColor, BackgroundPicture, PageWidth, PageHeight, Remark FROM LayoutPage ");
    QSqlQuery query = db->query(sql);
    QSqlRecord record = query.record();
    int filedLayerId = record.indexOf("ID");
    int filedLayerName = record.indexOf("Remark");
    int filedLayerBackgroundColor = record.indexOf("BackgroundColor");
    int filedLayerBackgroundPicture = record.indexOf("BackgroundPicture");
    int filedLayerPageWidth = record.indexOf("PageWidth");
    int filedLayerPageHeight = record.indexOf("PageHeight");
    while(query.next())
    {
        CLayer* layer = new CLayer(query.value(filedLayerId).toInt());
        layer->setLayerValue(LAYER_VALUE_ID, query.value(filedLayerId).toInt());
        layer->setLayerValue(LAYER_VALUE_NAME, query.value(filedLayerName).toString());
        layer->setLayerValue(LAYER_VALUE_BACKGROUNDCOLOR, query.value(filedLayerBackgroundColor).toInt());
        layer->setLayerValue(LAYER_VALUE_BACKGROUNDPICTURE, query.value(filedLayerBackgroundPicture).toString());
        layer->setLayerValue(LAYER_VALUE_PAGEWIDTH, query.value(filedLayerPageWidth).toDouble());
        layer->setLayerValue(LAYER_VALUE_PAGEHEIGHT, query.value(filedLayerPageHeight).toDouble());

        layer->setNone();
        controller->addLayer(layer);
    }
    return 0;
}



CObject* CLayer::parent()
{
    return m_controller;
}

QString CLayer::descriptionStr() const
{
    qDebug() << "CLayer::descriptionStr" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_parentLayer)
    {
        return m_parentLayer->descriptionStr().append("/").append(layerValue(LAYER_VALUE_NAME).toString());
    }
    else
    {
        return layerValue(LAYER_VALUE_NAME).toString();
    }
}

void CLayer::setController(CController *controller)
{
    qDebug() << "CLayer::setController" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_controller = controller;
    for(int i = 0; i < m_childLayers.size(); ++i)
    {
        CLayer* layer = m_childLayers.at(i);
        if(!controller)
        {
            if(layer->controller())
                layer->controller()->removeLayer(layer);
        }
        else
            controller->addLayer(layer);
    }
}

bool CLayer::addChildLayer(CLayer *layer)
{
    qDebug() << "CLayer::addChildLayer" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!layer)
        return false;
    if(m_childLayers.contains(layer))
        return false;
    if(layer->parentLayer())
        layer->parentLayer()->removeChildLayer(layer);
    m_childLayers.append(layer);
    layer->setParentLayer(this);
    return true;
}

void CLayer::removeChildLayer(CLayer *layer)
{
    qDebug() << "CLayer::removeChildLayer" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!layer)
        return;
    if(!m_childLayers.contains(layer))
        return;
    m_childLayers.removeOne(layer);
    layer->setParentLayer(NULL);
}


bool CLayer::beforeSetValue(QString &name, QVariant &value)
{
    if(name == LAYER_VALUE_SCALE)
    {
        if(value.toReal() <= 0)
            value = 1.0;
    }
    return true;
}

void CLayer::afterSetValue(QString &name, QVariant& oldValue, QVariant& newValue)
{
    if(CGlobal::instance()->dm()->isLoading())
        return;
    if(name == LAYER_VALUE_ID)
    {
        if(m_controller)
            m_controller->calcMaxLayerId(newValue.toInt());
    }
    CObject::afterSetValue(name, oldValue, newValue);
}

QVariant CLayer::afterGetValue(QString name, QVariant value) const
{
    if(name == OBJECT_VALUE_DISPLAY_NAME)
        return m_layerValue.value(LAYER_VALUE_NAME);
    return value;
}

void CLayer::afterSetStatus(short status, unsigned char value)
{
    Q_UNUSED(value);
    if(status == OBJS_Fire || status == OBJS_Fault || (status == OBJS_None && value == OBJS_None))
    {
        CGlobal::instance()->designTreeView()->update(this);
    }
}


void CLayer::initLayer()
{
    qDebug() << "CLayer::initLayer" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_controller = NULL;
    m_parentLayer = NULL;
    setLayerValue(OBJECT_VALUE_PIXMAP, ":/images/layer.png");
    setLayerValue(LAYER_VALUE_NAME, "图层");
    setLayerValue(LAYER_VALUE_SCALE, 1.0);
    setLayerValue(LAYER_VALUE_BACKGROUNDCOLOR, CGlobal::colorToInt(Qt::black));
}

