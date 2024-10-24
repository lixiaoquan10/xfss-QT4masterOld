#ifndef CMODEL_H
#define CMODEL_H

#include "dm/cobject.h"

#include <QAbstractItemModel>
#include <QList>
#include <QHash>
#include <QPointer>
#include <QColor>

#define MODEL_ONLINE_COLOR QColor(0x00, 0xe0, 0x00)
#define MODEL_OFFLINE_COLOR Qt::blue

class CTreeItem;
class CModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit CModel(QObject *parent = 0);
    virtual ~CModel();
    /*返回指定项状态，可选、可编辑，必须实现*/
    Qt::ItemFlags flags(const QModelIndex& index) const;
    /*返回指定项的子项索引，树必须实现*/
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    /*返回指定项的父项索引，树必须实现*/
    QModelIndex parent(const QModelIndex& index) const;

    bool update(CObject* object);
    bool update(const QModelIndex& indexLeft, const QModelIndex& indexRight);
    /*返回指定项为父项的行数，表格、树必须实现*/
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    /*返回指定项为父项的列数，表格、树必须实现*/
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    /*指定项为父项的子树中在指定行之前插入指定数量的行*/
    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());
    /*指定项为父项的子树中删除指定行开始指定数量的行*/
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());
    /*返回指定角色的数据，必须实现*/
    QVariant data(const QModelIndex &index, int role) const;
    /*设置指定项指定角色的值*/
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    /*返回指定位置指定类型的值*/
    QVariant headerData(int section, Qt::Orientation orientation, int role) const {
        Q_UNUSED(section); Q_UNUSED(orientation); Q_UNUSED(role); return QVariant();}

    bool appendObject(CObject* object, CObject* parent);
    bool insertObject(CObject* object, int pos, CObject* parent);
    bool removeObject(CObject *object);

    void addItem(CObject* object, CTreeItem* item) { m_items.insert(object, item); }
    void removeItem(CObject* object) { m_items.remove(object); }

    CTreeItem* getItem(const QModelIndex& index = QModelIndex()) const;
    CTreeItem *getItem(CObject *object) { return m_items.value(object, NULL); }
    CObject* getObject(const QModelIndex& index = QModelIndex()) const;
    QModelIndex getIndex(CObject* object) { return getIndex(getItem(object)); }
    QModelIndex getIndex(CTreeItem* item);
    void setTrack(const bool track);
    bool track() { return m_track; }

    virtual void reloadData() { loadRootObject(); }
protected slots:
    virtual void valueChanged(QString& name, QVariant& oldValue, QVariant& newValue);
    virtual void objectChanged(CObject::ObjectType type, CObject* object);
    virtual void objectStatusChanged(short status, unsigned char oldValue, unsigned char newValue, unsigned int time);
    virtual void editStatusChanged(CObject::EditStatus status);
    virtual void objectDestroy(CObject* object);
protected:

    int addObject(CObject* object, const QModelIndex& parent = QModelIndex());
    int insObject(CObject* object, int row, const QModelIndex& parent = QModelIndex());
    bool delObject(CObject* object) { return delItem(getItem(object)); }
    bool delItem(const QModelIndex& index);
    bool delItem(CTreeItem *item);
    void clean();

    void setRootObject(CObject *object);
    CObject* getRootObject(){ return m_rootObject; }
    void loadRootObject();

    void loadChildren(CObject* object, QModelIndex parent = QModelIndex(), int pos = -1);
    void removeChildren(CObject* object);
    void connectObjects();
    void disconnectObjects();
    void connectObject(CObject* object);
    void disconnectObject(CObject* object);

    virtual bool beforeLoadChildren(CObject *object, QModelIndex &parent) { Q_UNUSED(object); Q_UNUSED(parent); return true; }
    virtual void afterLoadChildren(CObject *object, QModelIndex &parent) { Q_UNUSED(object); Q_UNUSED(parent); }
    virtual bool beforeRemoveChildren(CObject *object) { Q_UNUSED(object); return true; }
    virtual void afterRemoveChildren(CObject *object) { Q_UNUSED(object); }

    virtual QVariant getDisplayData(const QModelIndex& index) const { Q_UNUSED(index); return QVariant(); }
    virtual QVariant getDecorationData(const QModelIndex& index) const { Q_UNUSED(index); return QVariant(); }
    virtual QVariant getEditData(const QModelIndex& index) const { Q_UNUSED(index); return QVariant(); }
    virtual QVariant getTextColorData(const QModelIndex& index) const { Q_UNUSED(index); return QVariant(); }
    virtual QVariant getToolTipData(const QModelIndex& index) const { Q_UNUSED(index); return QVariant(); }
    virtual QVariant getTextFontData(const QModelIndex& index) const { Q_UNUSED(index); return QVariant(); }
    virtual bool setEditData(const QModelIndex& index, const QVariant& value) { Q_UNUSED(index); Q_UNUSED(value); return true; }
protected:
    CTreeItem* m_root;
    CObject* m_rootObject;
    QHash<CObject*, CTreeItem*> m_items;
    bool m_track;
};

#endif // CMODEL_H
