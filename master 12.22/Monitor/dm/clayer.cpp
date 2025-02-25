﻿#include "clayer.h"
#include "cproject.h"
#include "cdbsqlite.h"
#include "cdm.h"
#include "cglobal.h"
#include "graphics/cmath.h"
#include "widget/cdesigntreeview.h"
#include <QFileInfo>
#include <QSqlRecord>
#include <QDebug>

const QString CLayer::type_s = "CLayer";

CLayer::CLayer(const int keyId) :
    CObject(keyId)
{
    initLayer();
}

CLayer::CLayer(CProject *project, CLayer *parent, const int keyId) :
    CObject(keyId)
{
    initLayer();
    if(parent)
        parent->addChildLayer(this);
    int n = project->maxLayerId() + 1;
    setLayerId(n);
    project->addLayer(this);
    setValue(LAYER_VALUE_NAME, value(LAYER_VALUE_NAME).toString().append(QString::number(n)));
}

CLayer::~CLayer()
{
    if(CGlobal::instance()->dm()->isClearing())
        return;
    if(m_project)
        m_project->removeLayer(this);
    m_project = NULL;
    m_parentLayer = NULL;
}


int CLayer::loadLayer(CDBSqlite *db, CProject* project)
{
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
        layer->setValue(LAYER_VALUE_ID, query.value(filedLayerId).toInt());
        layer->setValue(LAYER_VALUE_NAME, query.value(filedLayerName).toString());
        layer->setValue(LAYER_VALUE_BACKGROUNDCOLOR, query.value(filedLayerBackgroundColor).toInt());
        layer->setValue(LAYER_VALUE_BACKGROUNDPICTURE, query.value(filedLayerBackgroundPicture).toString());
        layer->setValue(LAYER_VALUE_PAGEWIDTH, query.value(filedLayerPageWidth).toDouble());
        layer->setValue(LAYER_VALUE_PAGEHEIGHT, query.value(filedLayerPageHeight).toDouble());

        layer->setNone();
        project->addLayer(layer);
    }
    return 0;
}



CObject* CLayer::parent()
{
    return m_project;
}

QString CLayer::descriptionStr() const
{
    if(m_parentLayer)
    {
        return m_parentLayer->descriptionStr().append("/").append(value(LAYER_VALUE_NAME).toString());
    }
    else
    {
        return value(LAYER_VALUE_NAME).toString();
    }
}

void CLayer::setProject(CProject *project)
{
    m_project = project;
    for(int i = 0; i < m_childLayers.size(); ++i)
    {
        CLayer* layer = m_childLayers.at(i);
        if(!project)
        {
            if(layer->project())
                layer->project()->removeLayer(layer);
        }
        else
            project->addLayer(layer);
    }
}

bool CLayer::addChildLayer(CLayer *layer)
{
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
        if(m_project)
            m_project->calcMaxLayerId(newValue.toInt());
    }
    CObject::afterSetValue(name, oldValue, newValue);
}

QVariant CLayer::afterGetValue(QString name, QVariant value) const
{
    if(name == OBJECT_VALUE_DISPLAY_NAME)
        return m_value.value(LAYER_VALUE_NAME);
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
    m_project = NULL;
    m_parentLayer = NULL;
    setValue(OBJECT_VALUE_PIXMAP, ":/images/layer.png");
    setValue(LAYER_VALUE_NAME, "图层");
    setValue(LAYER_VALUE_SCALE, 1.0);
    setValue(LAYER_VALUE_BACKGROUNDCOLOR, CGlobal::colorToInt(Qt::black));
}

