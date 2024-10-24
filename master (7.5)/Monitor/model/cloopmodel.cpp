#include "cglobal.h"


CLoopModel::CLoopModel(QObject *parent) :
    CModel(parent), m_distribution(0),m_canport(0)
{
    setTrack(true);
}

CLoopModel::CLoopModel(CDistribution *distribution, bool track, QObject *parent) :
    CModel(parent), m_distribution(0)
{
    setTrack(track);
    setDistribution(distribution);
}


Qt::ItemFlags CLoopModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return CModel::flags(index);
}

int CLoopModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant CLoopModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == LOOP_COLUMN_ADDRESS)
            return tr("address");
        else if(section == LOOP_COLUMN_NAME)
            return tr("loop name");
        else if(section == LOOP_COLUMN_DESCRIPTION)
            return tr("description");
    }
    else if(orientation == Qt::Vertical && role == Qt::DisplayRole)
    {
        return QString::number(section + 1);
    }
    return QVariant();
}

void CLoopModel::setDistribution(CDistribution *distribution)
{
    if(m_distribution != distribution)
    {
        clean();
        m_distribution = distribution;
        if(m_distribution)
        {
            connectObject(m_distribution);
            m_rootObject = m_distribution;
            addItem(m_distribution, m_root);
            QList<CLoop*> loops = m_distribution->loops();
            for(int i = 0; i < loops.size(); ++i)
            {
                loadChildren(loops.at(i));
            }
        }
    }
}



void CLoopModel::objectChanged(CObject::ObjectType type, CObject *object)
{
    if(object->type() != CLoop::type_s)
        return;
    if(type == CObject::OT_AddChildren)
    {
        appendObject(object, m_distribution);
    }
    if(type == CObject::OT_RemoveChildren)
    {
        removeObject(object);
    }
}

void CLoopModel::objectDestroy(CObject *object)
{
    if(object == m_distribution)
    {
        m_distribution = NULL;
        CModel::setRootObject(m_distribution);
        return;
    }
    CModel::objectDestroy(object);
}

bool CLoopModel::beforeLoadChildren(CObject *object, QModelIndex &parent)
{
    Q_UNUSED(parent);
    QString type = object->type();
    if(type != CLoop::type_s)
        return false;
    return true;
}

QVariant CLoopModel::getDisplayData(const QModelIndex &index) const
{
    CLoop* loop = static_cast<CLoop*>(getObject(index));
    if(!loop)
        return QVariant();
    if(index.column() == LOOP_COLUMN_ADDRESS)
        return loop->loopValue(LOOP_VALUE_ADDR);
    else if(index.column() == LOOP_COLUMN_NAME)
        return loop->loopValue(OBJECT_VALUE_DISPLAY_NAME);
    else if(index.column() == LOOP_COLUMN_DESCRIPTION)
        return loop->loopValue(LOOP_VALUE_DESCRIPTION);
    return QVariant();
}

QVariant CLoopModel::getTextColorData(const QModelIndex &index) const
{
    CLoop* loop = static_cast<CLoop*>(getObject(index));
    if(!loop)
        return QVariant();
    if(loop->isUndefined())
        return QColor(0x66, 0x8e, 0x3c);
    return loop->isLoopOnline() ? MODEL_ONLINE_COLOR : MODEL_OFFLINE_COLOR;
}

