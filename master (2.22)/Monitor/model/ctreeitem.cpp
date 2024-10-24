#include "ctreeitem.h"
#include "dm/cobject.h"
#include "cmodel.h"

CTreeItem::CTreeItem(CObject *data, CTreeItem *parent) :
    m_itemData(data), m_parentItem(parent), m_model(0)
{
    if(parent)
        parent->m_childItems.append(this);
}

CTreeItem::CTreeItem(CObject *data, CModel *model) :
    m_itemData(data), m_parentItem(0), m_model(model)
{
}

CTreeItem::~CTreeItem()
{
    while(m_childItems.size())
        delete m_childItems.at(0);
    if(m_parentItem)
        m_parentItem->m_childItems.removeAll(this);
    if(m_model)
        m_model->removeItem(m_itemData);
}

int CTreeItem::prependChild(CTreeItem *child)
{
    return insertChild(0, child);
}

int CTreeItem::appendChild(CTreeItem *child)
{
    return insertChild(m_childItems.size(), child);
}

int CTreeItem::insertChild(int pos, CTreeItem *child)
{
    if(!child || (pos < 0 || pos > m_childItems.size()))
        return -1;
    if(!checkItem(child))
        return -1;
    if(m_childItems.contains(child))
        return -1;
    if(child->parent())
        child->parent()->removeChild(child);
    m_childItems.insert(pos, child);
    child->setParent(this);
    return pos;
}

bool CTreeItem::removeChild(int pos)
{
    if(pos < 0 || pos >= m_childItems.size())
        return false;
    CTreeItem* child = m_childItems.at(pos);
    return removeChild(child);
}

bool CTreeItem::removeChild(CTreeItem *child)
{
    if(!child)
        return false;
    if(!m_childItems.contains(child))
        return false;
    m_childItems.removeOne(child);
    child->setParent(NULL);
    return true;
}

bool CTreeItem::deleteChild(int pos)
{
    if(pos < 0 || pos >= m_childItems.size())
        return false;
    CTreeItem* child = m_childItems.at(pos);
    return deleteChild(child);
}

bool CTreeItem::deleteChild(CTreeItem *child)
{
    if(!removeChild(child))
        return false;
    delete child;
    return true;
}

bool CTreeItem::insertChilds(int pos, int count)
{
    if(pos < 0 || pos > m_childItems.size())
        return false;
    CTreeItem* item = 0;
    for(int i = pos; i < pos + count; ++i)
    {
        item = new CTreeItem();
        insertChild(pos, item);
    }
    return true;
}

bool CTreeItem::removeChilds(int pos, int count)
{
    if(pos < 0 || pos + count > m_childItems.size())
        return false;
    for(int i = 0; i < count; ++i)
    {
        removeChild(pos);
    }
    return true;
}

int CTreeItem::row()
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(this);

    return -1;
}

bool CTreeItem::checkItem(CTreeItem *item)
{
    if(!item || item == this)
        return false;
    CTreeItem* p = m_parentItem;
    while(p)
    {
        if(item == p)
            return false;
        p = p->parent();
    }
    return true;
}
