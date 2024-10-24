#ifndef CTREEITEM_H
#define CTREEITEM_H

#include "dm/cobject.h"

#include <QObject>
#include <QList>

class CObject;
class CModel;
class CTreeItem
{
public:
    CTreeItem(CObject* data = 0, CTreeItem *parent = 0);
    CTreeItem(CObject* data, CModel *model);
    virtual ~CTreeItem();

    void setData(CObject* data) { m_itemData = data; }
    CObject* data() const { return m_itemData; }
    void setModel(CModel* model) { m_model = model; }
    CModel* model() const { return m_model; }
    void setParent(CTreeItem* parent) { m_parentItem = parent; }
    CTreeItem *parent() const { return m_parentItem; }

    int prependChild(CTreeItem *child = 0);
    int appendChild(CTreeItem *child = 0);
    int insertChild(int pos, CTreeItem *child = 0);

    bool removeChild(int pos);
    bool removeChild(CTreeItem* child);
    bool deleteChild(int pos);
    bool deleteChild(CTreeItem* child);

    bool insertChilds(int pos, int count);
    bool removeChilds(int pos, int count);

    int row();
    int childCount() { return m_childItems.size(); }
    CTreeItem* childAt(int pos) const { return m_childItems.value(pos, NULL); }
    QList<CTreeItem*> childs() { return m_childItems; }

    bool checkItem(CTreeItem* item);
private:
    CObject* m_itemData;
    CTreeItem* m_parentItem;
    CModel* m_model;
    QList<CTreeItem*> m_childItems;
};

#endif // CTREEITEM_H
