#include "cdevicemodel.h"
#include "ctreeitem.h"
#include "dm/cproject.h"
#include "dm/ccontroller.h"
#include "dm/cdistribution.h"
#include "dm/cloop.h"
#include "dm/cdevice.h"
#include "dm/cdm.h"
#include "cglobal.h"
#include "cdevicetype.h"

#include <QMessageBox>
#include <QPixmap>
#include <QRegExp>
#include <QDebug>

CDeviceModel::CDeviceModel(QObject *parent) :
    CModel(parent), m_loop(0)
{
    setTrack(true);
}

CDeviceModel::CDeviceModel(CLoop* loop, bool track, QObject *parent) :
    CModel(parent), m_loop(0)
{
    setTrack(track);
    setLoop(loop);
}

Qt::ItemFlags CDeviceModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    if(index.column() == DEVICE_COLUMN_ADDRESS || index.column() == DEVICE_COLUMN_LOGIN_TYPE)
        return CModel::flags(index);
    return CModel::flags(index);
}

int CDeviceModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 7;
}

QVariant CDeviceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == DEVICE_COLUMN_ADDRESS)
            return "配置地址";
        else if(section == DEVICE_COLUMN_TYPE)
            return "配置型号";
        else if(section == DEVICE_COLUMN_PRODUCTID)
            return "产品编号";
        else if(section == DEVICE_COLUMN_LOGIN_TYPE)
            return "注册型号";
        else if(section == DEVICE_COLUMN_STATUS)
            return "状态";
        else if(section == DEVICE_COLUMN_CONFIG_LOGIN)
            return "配置/注册";
        else if(section == DEVICE_COLUMN_LOGIN_ADDRESS)
            return "注册地址";
    }
    else if (role == Qt::FontRole && orientation == Qt::Horizontal)
    {
        QFont font;
        font.setPointSize(12);  // 设置字号为12
        return font;
    }
    return QVariant();
}

void CDeviceModel::setLoop(CLoop* loop)
{
    if(m_loop != loop || isupdate)
    {
        isupdate = false;
        clean();
        m_loop = loop;
        if(m_loop)
        {
            connectObject(m_loop);
            m_rootObject = m_loop;
            addItem(m_loop, m_root);
            for(int i = 1; i < 256; i++)
            {
                CDevice* device = m_loop->deviceByAdd(i);
                if(device)
                {
                    loadChildren(device);
                }
            }
        }
    }
}

void CDeviceModel::objectChanged(CObject::ObjectType type, CObject *object)
{
    if(object->type() != CDevice::type_s)
        return;
    if(type == CObject::OT_AddChildren)
    {
        appendObject(object, m_loop);
    }
    if(type == CObject::OT_RemoveChildren)
    {
        removeObject(object);
    }
}

void CDeviceModel::objectDestroy(CObject *object)
{
    if(object == m_loop)
    {
        m_loop = NULL;
        CModel::setRootObject(m_loop);
        return;
    }
    CModel::objectDestroy(object);
}

bool CDeviceModel::beforeLoadChildren(CObject *object, QModelIndex &parent)
{
    Q_UNUSED(parent);
    QString type = object->type();
    if(type != CDevice::type_s)
        return false;
    return true;
}

QVariant CDeviceModel::data(const QModelIndex &index, int role) const
{
    CDevice* device = static_cast<CDevice*>(getObject(index));
    CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
    CDistribution* distribution = controller->distributionByAddress(device->deviceValue(DEVICE_VALUE_DISTRIBUTIONADDR).toInt());
    if(!device)
        return QVariant();
    if(role== Qt::DisplayRole){
        if(index.column() == DEVICE_COLUMN_ADDRESS){
            //仅配置或者配且注
            if(device->deviceValue(DEVICE_VALUE_ISUSED) == 1 || device->deviceValue(DEVICE_VALUE_ISUSED) == 0)
                return device->deviceValue(DEVICE_VALUE_ADDR);
        }
        else if(index.column() == DEVICE_COLUMN_LOGIN_ADDRESS)
        {
            //仅注册或者配且注
            if(device->deviceValue(DEVICE_VALUE_ISUSED) == 1 || device->deviceValue(DEVICE_VALUE_ISUSED) == 2)
                return device->deviceValue(DEVICE_VALUE_ADDR);
        }

        else if(index.column() == DEVICE_COLUMN_TYPE)
        {
            //仅注册
            if(device->deviceValue(DEVICE_VALUE_ISUSED) == 2)
                return CGlobal::instance()->deviceType(0)->name();
            CDeviceType* devType = CGlobal::instance()->deviceType(device->deviceValue(DEVICE_VALUE_TYPE_ID).toInt());
            if(!devType)
                devType = CGlobal::instance()->deviceType(0);
            return devType->name();
        }
        else if(index.column() == DEVICE_COLUMN_PRODUCTID)
            return device->deviceValue(DEVICE_VALUE_DESCRIPTION);
        else if(index.column() == DEVICE_COLUMN_LOGIN_TYPE)
        {
            CDeviceType* devType;
            //仅注册
            if(device->deviceValue(DEVICE_VALUE_ISUSED) == 2){
                devType = CGlobal::instance()->deviceType(device->getStatus(OBJS_LoginDeviceType));
                device->setDeviceValue(DEVICE_VALUE_TYPE_ID, devType->type());
            }
            //配且注
            else if(device->deviceValue(DEVICE_VALUE_ISUSED) == 1){
                devType = CGlobal::instance()->deviceType(device->deviceValue(DEVICE_VALUE_TYPE_ID).toInt());
            }
            //仅配置//未定义
            else if(device->deviceValue(DEVICE_VALUE_ISUSED) == 0 || device->deviceValue(DEVICE_VALUE_ISUSED) == 3){
                devType = CGlobal::instance()->deviceType(0);
            }
            if(!devType)
                devType = CGlobal::instance()->deviceType(0);
            return devType->name();
        }
        else if(index.column() == DEVICE_COLUMN_CONFIG_LOGIN)
        {
            if(device->deviceValue(DEVICE_VALUE_ISUSED) == 2)
                return "仅注册";
            else if(device->deviceValue(DEVICE_VALUE_ISUSED) == 1)
                return "配且注";
            else if(device->deviceValue(DEVICE_VALUE_ISUSED) == 0)
                return "仅配置";
            else if(device->deviceValue(DEVICE_VALUE_ISUSED) == 3)
                return "未定义";

        }
        else if(index.column() == DEVICE_COLUMN_STATUS)
        {
            if(device->getDeviceCommunicationFault() == false)
            {
                if(distribution->getemergencyStatus() || device->deviceValue(DEVICE_VALUE_EMERGENCY) == "应急")
                    return "应急";
                else if(device->deviceValue(DEVICE_VALUE_TEST) == "测试")
                    return "测试";
                else
                    return "正常";
            }
            else
                return "通讯故障";
        }
    }
    if(role == Qt::BackgroundColorRole){
        if(index.column() == DEVICE_COLUMN_STATUS)
        {
            if(device->getStatus(OBJS_LightFault) == 1 || device->getDeviceCommunicationFault())
                return QColor(245,222,179); //黄色
            else if(device->deviceValue(DEVICE_VALUE_EMERGENCY) == "应急" ||
                    device->deviceValue(DEVICE_VALUE_TEST) == "测试" || distribution->getemergencyStatus())
                return QColor(255,99,71);  //红色
            else
                return QColor(127,255,0);   //绿色
        }
        if(device->deviceValue(DEVICE_VALUE_CHANGETYPE) == 1)
        {
            if(index.column() == DEVICE_COLUMN_LOGIN_TYPE || index.column() == DEVICE_COLUMN_TYPE)
                return QColor(227,168,105);   //瓜色
        }
        if(device->deviceValue(DEVICE_VALUE_CHANGEADD) == 1)
        {
            if(index.column() == DEVICE_COLUMN_ADDRESS || index.column() == DEVICE_COLUMN_LOGIN_ADDRESS)
                return QColor(227,168,105);  //瓜色
        }
    }
    if(role == Qt::TextColorRole){
        if(device->getDeviceCommunicationFault() == false)
            return QColor(0,0,0);
        else
            return QColor(128,138,135);    //灰色
    }
    if(role == Qt::TextAlignmentRole){
        if(index.column() == DEVICE_COLUMN_ADDRESS ||
           index.column() == DEVICE_COLUMN_LOGIN_ADDRESS ||
           index.column() == DEVICE_COLUMN_CONFIG_LOGIN ||
           index.column() == DEVICE_COLUMN_STATUS)
        {
            return Qt::AlignCenter;
        }
    }
//    if(role == Qt::DecorationRole){
//        if(index.column() == DEVICE_COLUMN_TYPE)
//        {
//            CDeviceType* devType = CGlobal::instance()->deviceType(data->value(DEVICE_VALUE_TYPE_ID).toInt());
//            if(!devType)
//                devType = CGlobal::instance()->deviceType(0);
//            return devType->imageReal(0.5);
//        }
//        else if(index.column() == DEVICE_COLUMN_LOGIN_TYPE)
//        {
//            CDeviceType* devType = CGlobal::instance()->deviceType(data->getStatus(OBJS_LoginDeviceType));
//            if(!devType)
//                devType = CGlobal::instance()->deviceType(0);
//            return devType->imageReal(0.5);
//        }
//    }
    if(role == Qt::TextColorRole){
//        return data->getStatus(OBJS_Online) ? QColor(Qt::green) : QColor(Qt::black);
    }
    return QVariant();
}


