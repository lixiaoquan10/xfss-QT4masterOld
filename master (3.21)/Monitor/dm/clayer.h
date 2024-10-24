#ifndef CLAYER_H
#define CLAYER_H

#include "cobject.h"

#include <QSet>
#include <QUndoStack>//ll 2014.10.20

#define LAYER_VALUE_ID "layer_id"
#define LAYER_VALUE_NAME "layer_name"
#define LAYER_VALUE_MAP "layer_map"
#define LAYER_VALUE_MAPHASH "layer_maphash"
#define LAYER_VALUE_MAPSUFFIX "layer_mapsuffix"
#define LAYER_VALUE_BACKGROUNDCOLOR "layer_backgroundcolor"
#define LAYER_VALUE_BACKGROUNDPICTURE "layer_backgroundpicture"
#define LAYER_VALUE_PAGEWIDTH "layer_pagewidth"
#define LAYER_VALUE_PAGEHEIGHT "layer_pageheight"


#define LAYER_VALUE_SCALE "layer_scale"
#define LAYER_VALUE_POSX "layer_posx"
#define LAYER_VALUE_POSY "layer_posy"
#define LAYER_VALUE_CENTERX "layer_centerx"
#define LAYER_VALUE_CENTERY "layer_centery"
#define LAYER_VALUE_LINEWIDTH "layer_linewidth"

class CDM;
class CController;
class CLayer : public CObject
{
    Q_OBJECT
public:
    explicit CLayer(const int keyId = 0);
    CLayer(CController *controller, CLayer* parent = 0, const int keyId = 0);
    ~CLayer();

    static int loadLayer(CDBSqlite *db, CController* controller);

    CObject* parent();
    CObject* child(const int index) { return m_childs.value(index, NULL); }
    QList<CObject*> childs() { return m_childs; }
    QString codeStr() const { return tr("图层%1").arg(layerId()); }
    QString typeStr() const { return "图层"; }
    QString descriptionStr() const;

    bool setLayerId(int id) { return setLayerValue(LAYER_VALUE_ID, id); }
    int layerId() const { return layerValue(LAYER_VALUE_ID).toInt(); }
    void setController(CController* controller);
    CController* controller() const { return m_controller; }
    void setParentLayer(CLayer* parent) { m_parentLayer = parent; }
    CLayer* parentLayer() const { return m_parentLayer; }

    bool setLayerValue(QString name, QVariant value);
    QVariant layerValue(const QString name) const;


    bool addChildLayer(CLayer* layer);
    void removeChildLayer(CLayer* layer);
    CLayer* childLayer(const int index) const { return m_childLayers.value(index, NULL); }
    QList<CLayer*> childLayers() const { return m_childLayers; }


protected:
    bool beforeSetValue(QString& name, QVariant& value);
    void afterSetValue(QString& name, QVariant& oldValue, QVariant &newValue);
    QVariant afterGetValue(QString name, QVariant value) const;
    void afterSetStatus(short status, unsigned char value);

private:
    void initLayer();
    int saveLayer(CDBSqlite* db);

public:
    const static QString type_s;
private:
    CController* m_controller;
    CLayer* m_parentLayer;
    QList<CLayer*> m_childLayers;

    QList<CObject*> m_childs;
    QHash<QString, QVariant> m_layerValue;
};


#endif // CLAYER_H
