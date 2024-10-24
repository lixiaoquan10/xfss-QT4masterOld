#include "cglobal.h"


CDistributionModel::CDistributionModel(QObject *parent) :
    CModel(parent), m_controller(0)
{
    setTrack(true);
}


Qt::ItemFlags CDistributionModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return CModel::flags(index);
}

int CDistributionModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 5;
}

QVariant CDistributionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == DISTRIBUTION_COLUMN_ADDRESS)
            return "地址";   /*Address*/
        else if(section == DISTRIBUTION_COLUMN_NAME)
            return "设备名称";
        else if(section == DISTRIBUTION_COLUMN_DESCRIPTION)
            return "产品编号";
        else if(section == DISTRIBUTION_COLUMN_AREA)
            return "区域";
        else if(section == DISTRIBUTION_COLUMN_LOCATION)
            return "位置";
    }
    else if (role == Qt::FontRole && orientation == Qt::Horizontal)
    {
        QFont font;
        font.setPointSize(12);  // 设置字号为12
        return font;
    }
    return QVariant();
}

void CDistributionModel::setController(CController *controller)
{
    if(m_controller != controller)
    {
        clean();
        m_controller = controller;
        if(m_controller)
        {
            connectObject(m_controller);
            m_rootObject = m_controller;
            addItem(m_controller, m_root);
            QList<CCanport*> canports = m_controller->canports();
            for(int i = 0; i < canports.size(); ++i)
            {
                for(int j = 1; j < 63; ++j)
                {
                    CDistribution* distribution = canports.at(i)->distributionByAddress(j);
                    if(distribution)
                    {
                        loadChildren(distribution);
                    }
                }
            }
        }
    }
}

void CDistributionModel::objectChanged(CObject::ObjectType type, CObject *object)
{
    if(object->type() != CDistribution::type_s)
        return;
    if(type == CObject::OT_AddChildren)
    {
        appendObject(object, m_controller);
    }
    if(type == CObject::OT_RemoveChildren)
    {
        removeObject(object);
    }
}

void CDistributionModel::objectDestroy(CObject *object)
{
    if(object == m_controller)
    {
        m_controller = NULL;
        CModel::setRootObject(m_controller);
        return;
    }
    CModel::objectDestroy(object);
}

bool CDistributionModel::beforeLoadChildren(CObject *object, QModelIndex &parent)
{
    Q_UNUSED(parent);
    QString type = object->type();
    if(type != CDistribution::type_s)
        return false;
    return true;
}


QVariant CDistributionModel::data(const QModelIndex &index, int role) const
{
    CDistribution* distribution = static_cast<CDistribution*>(getObject(index));
    QString canport = QString::number(distribution->distributionValue(DISTRIBUTION_VALUE_CANPORTADDR).toInt()-2);
    if(!distribution)
        return QVariant();
    if(role== Qt::DisplayRole)
    {
        if(index.column() == DISTRIBUTION_COLUMN_ADDRESS)
            return "CAN" + canport + "-" + distribution->distributionValue(DISTRIBUTION_VALUE_ADDR).toString();
        else if(index.column() == DISTRIBUTION_COLUMN_NAME)
            return distribution->distributionValue(DISTRIBUTION_VALUE_NAME);
        else if(index.column() == DISTRIBUTION_COLUMN_DESCRIPTION)
            return distribution->distributionValue(DISTRIBUTION_VALUE_DESCRIPTION);
        else if(index.column() == DISTRIBUTION_COLUMN_AREA)
            return distribution->distributionValue(DISTRIBUTION_VALUE_AREA);
        else if(index.column() == DISTRIBUTION_COLUMN_LOCATION)
            return distribution->distributionValue(DISTRIBUTION_VALUE_LOCATION);
        return QVariant();
    }
    if(role == Qt::TextAlignmentRole){
        if(index.column() == DISTRIBUTION_COLUMN_ADDRESS
                || index.column() == DISTRIBUTION_COLUMN_NAME
                || index.column() == DISTRIBUTION_COLUMN_DESCRIPTION)
        {
            return Qt::AlignCenter;
        }
    }
    return QVariant();
}

//QVariant CDistributionModel::getTextColorData(const QModelIndex &index) const
//{
//    CObject* data = getObject(index);
//    if(!data)
//        return QVariant();
////    if(data->isFaultStatus())
////        return QColor(0xff, 0x99, 0x00);
//    if(data->isUndefined())
//        return QColor(0x66, 0x8e, 0x3c);
//    return data->getStatus(OBJS_Online) ? MODEL_ONLINE_COLOR : MODEL_OFFLINE_COLOR;
//}

//QVariant CDistributionModel::getEditData(const QModelIndex &index) const
//{
//    CObject* data = getObject(index);
//    if(!data)
//        return QVariant();
//    if(index.column() == DISTRIBUTION_COLUMN_ADDRESS)
//        return data->value(DISTRIBUTION_VALUE_ID);
//    else if(index.column() == DISTRIBUTION_COLUMN_NAME)
//        return data->value(OBJECT_VALUE_DISPLAY_NAME);
//    else if(index.column() == DISTRIBUTION_COLUMN_DESCRIPTION)
//        return data->value(DISTRIBUTION_VALUE_DESCRIPTION);
//    else if(index.column() == DISTRIBUTION_COLUMN_STATUS)
//        return (data->isFaultStatus()?"故障":"正常");
//    return QVariant();
//}

//bool CDistributionModel::setEditData(const QModelIndex &index, const QVariant &value)
//{
//    CDistribution* distribution = static_cast<CDistribution*>(getObject(index));
//    if(!distribution)
//        return false;
//    CCanport* canport = distribution->canport();
//    if(!canport)
//        return false;
//    switch(index.column())
//    {
//    case DISTRIBUTION_COLUMN_ADDRESS:
//    {
//        int id = value.toInt();
//        if(id <= 0 || id > CONTROLLER_MAX_DISTRIBUTION_NUM)
//            return false;
////        if(canport->hasDistribution(id))
////        {
//////                QMessageBox::information(NULL, tr("Information"), tr("The exists distribution id"), QMessageBox::Ok);
////            return false;
////        }
//        distribution->setDistributionId(id);
//        break;
//    }
//    case DISTRIBUTION_COLUMN_NAME:
//    {
//        QString name = value.toString();
//        if(name.isEmpty())
//            return false;
//        distribution->setDistributionValue(DISTRIBUTION_VALUE_NAME, name);
//        break;
//    }
//    case DISTRIBUTION_COLUMN_DESCRIPTION:
//    {
//        distribution->setDistributionValue(DISTRIBUTION_VALUE_DESCRIPTION, value);
//        break;
//    }
//    case DISTRIBUTION_COLUMN_STATUS:
//        return false;
//    default:
//        return false;
//    }
//    return true;
//}
