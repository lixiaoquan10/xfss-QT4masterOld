#include "cglobal.h"

CModel::CModel(QObject *parent) :
    QAbstractItemModel(parent), m_rootObject(NULL), m_track(false)
{
    m_root = new CTreeItem();
}

CModel::~CModel()
{
    delete m_root;
}

int CModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
}

int CModel::rowCount(const QModelIndex &parent) const
{
    CTreeItem* item = NULL;
    if (!parent.isValid()) {
        item =  m_root;
    } else {
        item = getItem(parent);
    }
    if(!item)
        return 0;
    return item->childCount();
}

Qt::ItemFlags CModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return QAbstractItemModel::flags(index);
}

QModelIndex CModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    CTreeItem* item = NULL;
    if (!parent.isValid()) {
        item = m_root;
    } else {
        item = getItem(parent);
    }
    if(!item)
        return QModelIndex();
    CTreeItem* childrenItem = item->childAt(row);
    if (childrenItem) {
        return createIndex(row, column, childrenItem);
    }
    return QModelIndex();
}

QModelIndex CModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    CTreeItem* item = getItem(index);
    if(!item)
        return QModelIndex();
    CTreeItem* parentItem = item->parent();
    if (!parentItem || parentItem == m_root) {
        return QModelIndex();
    }
    return createIndex(parentItem->row(), 0, parentItem);
}

bool CModel::update(CObject *object)
{
    QModelIndex leftIndex = getIndex(object);
    QModelIndex rightIndex = this->index(leftIndex.row(), this->columnCount(leftIndex.parent()) - 1, leftIndex.parent());
    return update(leftIndex, rightIndex);
}

bool CModel::update(const QModelIndex& indexLeft, const QModelIndex& indexRight)
{
    if (!indexLeft.isValid() || !indexRight.isValid()) return false;
    emit dataChanged(indexLeft, indexRight);
    return true;
}

bool CModel::insertRows(int row, int count, const QModelIndex& parent)
{
    CTreeItem *parentItem= getItem(parent);
    if(!parentItem)
        return false;

    bool nRet;
    beginInsertRows(parent, row, row + count - 1);
    nRet = parentItem->insertChilds(row, count);
    endInsertRows();
    return nRet;
}

bool CModel::removeRows(int row, int count, const QModelIndex& parent)
{
    if(!count)
        return true;
    CTreeItem *parentItem= getItem(parent);
    if(!parentItem)
        return false;

    bool nRet;
    beginRemoveRows(parent, row, row + count - 1);
    nRet = parentItem->removeChilds(row, count);
    endRemoveRows();
    return nRet;
}

QVariant CModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::DisplayRole)
        return getDisplayData(index);
    if(role == Qt::DecorationRole)
        return getDecorationData(index);
    if(role == Qt::EditRole)
        return getEditData(index);
    if(role == Qt::TextColorRole)
        return getTextColorData(index);
    if(role == Qt::ToolTipRole)
        return getToolTipData(index);
    if(role == Qt::FontRole)
        return getTextFontData(index);
    return QVariant();
}

bool CModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;
    if(role == Qt::EditRole)
        return setEditData(index, value);
    return true;
}

bool CModel::appendObject(CObject *object, CObject *parent)
{
    if(!parent)
        return false;

    CTreeItem* item = getItem(parent);
    if(!item)
        return false;
    QModelIndex index = getIndex(item);
    int childCount = item->childCount();
    beginInsertRows(index, childCount, childCount);
    loadChildren(object, index);
    endInsertRows();
    return true;
}

bool CModel::insertObject(CObject *object, int pos, CObject *parent)
{
    if(!parent)
        return false;

    CTreeItem* item = getItem(parent);
    if(!item)
        return false;
    int childCount = item->childCount();
    if(pos < 0 || pos > childCount)
        return false;
    QModelIndex index = getIndex(item);
    beginInsertRows(index, pos, pos);
    loadChildren(object, index, pos);
    endInsertRows();
    return true;
}

bool CModel::removeObject(CObject *object)
{
    CTreeItem* item = getItem(object);
    if(!item)
        return false;
    int nPos = item->row();
    if(nPos < 0)
        return false;
    QModelIndex index = getIndex(item);
    beginRemoveRows(index.parent(), nPos, nPos);
    removeChildren(object);
    endRemoveRows();
    return true;
}

CTreeItem* CModel::getItem(const QModelIndex &index) const
{
    if(!index.isValid())
        return m_root;
    CTreeItem* item = static_cast<CTreeItem*>(index.internalPointer());
    return item;
}

CObject* CModel::getObject(const QModelIndex& index) const
{
    CTreeItem* item = getItem(index);
    if(item)
        return item->data();
    return 0;
}

QModelIndex CModel::getIndex(CTreeItem *item)
{
    if(!item)
        return QModelIndex();
    if(item == m_root)
        return QModelIndex();
    CTreeItem* parentItem = item->parent();
    QModelIndex temp;
    if(parentItem == m_root)
        temp = index(item->row(), 0);
    else
        temp = getIndex(parentItem).child(item->row(), 0);
    return temp;
}

void CModel::setTrack(const bool track)
{
    if(m_track == track)
        return;
    m_track = track;
    if(m_track)
        connectObjects();
    else
        disconnectObjects();
}

void CModel::valueChanged(QString &name, QVariant &oldValue, QVariant &newValue)
{
    Q_UNUSED(name);
    Q_UNUSED(oldValue);
    Q_UNUSED(newValue);
    CObject* object = static_cast<CObject*>(sender());
    update(object);
}

void CModel::objectChanged(CObject::ObjectType type, CObject *object)
{
    CObject* senderObject = static_cast<CObject*>(sender());
    if(type == CObject::OT_AddChildren)
    {
        appendObject(object, senderObject);
    }
    if(type == CObject::OT_RemoveChildren)
    {
        removeObject(object);
    }
}

void CModel::objectStatusChanged(short status, unsigned char oldValue, unsigned char newValue, unsigned int time)
{
    Q_UNUSED(status);
    Q_UNUSED(oldValue);
    Q_UNUSED(newValue);
    Q_UNUSED(time);
    CObject* object = static_cast<CObject*>(sender());
    update(object);
}

void CModel::editStatusChanged(CObject::EditStatus status)
{
    Q_UNUSED(status);
    CObject* object = static_cast<CObject*>(sender());
    update(object);
}

void CModel::objectDestroy(CObject *object)
{
    if(!object)
        return;
    removeObject(object);
}

int CModel::addObject(CObject *object, const QModelIndex& parent)
{
    CTreeItem *item= getItem(parent);
    if(!item)
        return -1;

    int childCount = item->childCount();
    return insObject(object, childCount, parent);
}

int CModel::insObject(CObject* object, int row, const QModelIndex &parent)
{
    CTreeItem *item= getItem(parent);
    if(!item)
        return -1;

    CTreeItem* child = new CTreeItem(object, this);
    int n = item->insertChild(row, child);
    addItem(object, child);
    if(m_track)
        connectObject(object);
    return n;
}

bool CModel::delItem(const QModelIndex& index)
{
    if(!index.isValid())
        return false;

    CTreeItem *item= getItem(index);
    return delItem(item);
}

bool CModel::delItem(CTreeItem *item)
{
    if(!item)
        return false;

    bool nRet;
    nRet = item->parent()->removeChild(item);
    CObject* object = item->data();
    removeItem(object);
    if(m_track)
        disconnectObject(object);
    delete item;
    return nRet;
}

void CModel::clean()
{
    if(m_track)
        disconnectObjects();
    m_items.clear();
    while(m_root->childCount())
        m_root->deleteChild(0);
    reset();
}

void CModel::setRootObject(CObject *object)
{
    if(m_rootObject != object)
    {
        m_rootObject = object;
        loadRootObject();
    }
}

void CModel::loadRootObject()
{
    clean();
    if(m_rootObject)
        loadChildren(m_rootObject);
}

void CModel::loadChildren(CObject *object, QModelIndex parent, int pos)
{
    if(!object)
        return;
    if(!beforeLoadChildren(object, parent))
        return;
    int n = 0;
    if(pos < 0)
        n = addObject(object, parent);
    else
        n = insObject(object, pos, parent);
    QModelIndex parentIndex = index(n, 0, parent);
    afterLoadChildren(object, parent);
    QList<CObject*> childrens = object->childs();
    for(int i = 0; i < childrens.size(); ++i)
    {
        loadChildren(childrens.at(i), parentIndex);
    }
}

void CModel::removeChildren(CObject *object)
{
    if(!object)
        return;
    QList<CObject*> childrens = object->childs();
    for(int i = 0; i < childrens.size(); ++i)
    {
        removeChildren(childrens.at(i));
    }
    delObject(object);
}

void CModel::connectObjects()
{
    QList<CObject*> objects = m_items.keys();
    for(int i = 0; i < objects.size(); ++i)
        connectObject(objects.at(i));
}

void CModel::disconnectObjects()
{
    QList<CObject*> objects = m_items.keys();
    for(int i = 0; i < objects.size(); ++i)
        disconnectObject(objects.at(i));
}

void CModel::connectObject(CObject *object)
{
    if(object)
    {
        connect(object, SIGNAL(valueChanged(QString&,QVariant&,QVariant&)), this, SLOT(valueChanged(QString&,QVariant&,QVariant&)));
        connect(object, SIGNAL(objectChanged(CObject::ObjectType,CObject*)), this, SLOT(objectChanged(CObject::ObjectType,CObject*)));
        connect(object, SIGNAL(objectStatusChanged(short,unsigned char,unsigned char,unsigned int)), this, SLOT(objectStatusChanged(short,unsigned char,unsigned char,unsigned int)));
//        connect(object, SIGNAL(editStatusChanged(CObject::EditStatus)), this, SLOT(editStatusChanged(CObject::EditStatus)));
        connect(object, SIGNAL(destrory(CObject*)), this, SLOT(objectDestroy(CObject*)));
    }
}

void CModel::disconnectObject(CObject *object)
{
    if(object)
    {
        disconnect(object, SIGNAL(valueChanged(QString&,QVariant&,QVariant&)), this, SLOT(valueChanged(QString&,QVariant&,QVariant&)));
        disconnect(object, SIGNAL(objectChanged(CObject::ObjectType,CObject*)), this, SLOT(objectChanged(CObject::ObjectType,CObject*)));
        disconnect(object, SIGNAL(objectStatusChanged(short,unsigned char,unsigned char,unsigned int)), this, SLOT(objectStatusChanged(short,unsigned char,unsigned char,unsigned int)));
//        disconnect(object, SIGNAL(editStatusChanged(CObject::EditStatus)), this, SLOT(editStatusChanged(CObject::EditStatus)));
        disconnect(object, SIGNAL(destrory(CObject*)), this, SLOT(objectDestroy(CObject*)));
    }
}
