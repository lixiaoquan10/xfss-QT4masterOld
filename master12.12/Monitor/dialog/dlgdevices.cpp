#include "dlgdevices.h"
#include "ui_dlgdevices.h"
#include "model/cdevicelistmodel.h"
#include "delegate/cdevicetypedelegate.h"
#include "view/ctableview.h"
#include "widget/cprogramdeviceview.h"

#include "cglobal.h"
#include "dm/cproject.h"
#include "dm/ccontroller.h"
#include "dm/cloop.h"
#include "dm/cdevice.h"
#include "dm/cobject.h"
#include "dm/cdistribution.h"
#include "model/ctreeitem.h"
#include "cdevicetype.h"

#include "struct/cmsgdeviceinfo.h"
#include "dialog/dlgchangeadd.h"
#include "dialog/dlgchangetype.h"
#include "cmyiconstyle.h"
#include "graphics/deviceForbidFault.h"
#include "QDebug"
#include "QTimer"
#include "QMenu"
#include <QMessageBox>


dlgDevices::dlgDevices(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgDevices)
{
    ui->setupUi(this);
    CGlobal::instance()->setDlgDevices(this);
    m_model = new CDeviceListModel();
    ui->tableView->setModel(m_model);
    ui->tableView->setColumnWidth(0, 110);
    ui->tableView->setColumnWidth(1, 70);
    ui->tableView->setColumnWidth(2, 70);
    ui->tableView->setColumnWidth(3, 250);
    ui->tableView->setColumnWidth(4, 80);
    ui->tableView->setColumnWidth(5, 80);
    ui->tableView->setColumnWidth(6, 80);
    ui->tableView->setColumnWidth(7, 100);
    ui->tableView->setColumnWidth(8, 90);
    ui->tableView->setColumnWidth(9, 200);
    ui->tableView->setColumnWidth(10, 200);

    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setHighlightSections(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setWordWrap(true);
    ui->tableView->setStyleSheet("selection-color: rgb(0, 0, 0);");
    ui->ReadBatteryStatus_Button->setStyleSheet("color: rgb(0,0,255);font: bold;");
//    CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
//    ui->lineEdit->setText(QString::number(controller->getDeviceOnlineTotal()));

    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    m_menuTable = NULL;
    m_style = new CMyIconStyle();
    m_strSheet = "QMenu {background-color: rgb(255, 246, 143); margin: 3px; font-size: 12px;}\
            QMenu::item {padding: 3px 15px 3px 15px; border: 1px solid transparent; min-height: 25px; min-width: 30px; border-radius: 4px;}\
            QMenu::separator {height: 2px; background: #CCCCFF; margin-left: 5px; margin-right: 5px;}\
            QMenu::right-arrow { image: url(:/res/right-arrow.png); }\
            QMenu::indicator:exclusive:checked {image: url(:/res/symbol-check.png);}\
            QMenu::item:selected {background-color: qlineargradient(x1: 0.5, y1: 0, x2: 0.5, y2: 1, stop: 0 #99CCFF, stop: 0.5 #CCFFFF, stop: 1 #99CCFF); border-color: #6699CC;}";
    m_strSheetCheck = "QMenu {background-color: rgb(205, 198, 115); margin: 3px; font-size: 12px;}\
            QMenu::item {padding: 3px 10px 3px 30px; border: 1px solid transparent; min-height: 25px; min-width: 30px; border-radius: 4px;}\
            QMenu::separator {height: 2px; background: #CCCCFF; margin-left: 5px; margin-right: 5px;}\
            QMenu::right-arrow { image: url(:/res/right-arrow.png); }\
            QMenu::indicator:exclusive:checked {image: url(:/res/symbol-check.png);}\
            QMenu::item:selected {background-color: qlineargradient(x1: 0.5, y1: 0, x2: 0.5, y2: 1, stop: 0 #99CCFF, stop: 0.5 #CCFFFF, stop: 1 #99CCFF); border-color: #6699CC;}";

}

dlgDevices::~dlgDevices()
{
    CGlobal::instance()->setDlgDevices(0);
    ui->tableView->setModel(0);
    delete m_menuTable;
    delete m_style;
    delete ui;
}


void dlgDevices::getDeviceInfo()
{
    int count = 0;
    CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
    ui->lineEdit->setText(QString::number(controller->getDeviceOnlineTotal()));
    QList<CDistribution*> distributions = controller->distributions();
    for(int i=0; i<distributions.count(); i++)
    {
        CDistribution* distribution = distributions.at(i);
        QList<CLoop*> loops = distribution->loops();
        for(int i=0; i<loops.count(); i++){
            CLoop* loop = loops.at(i);
            QList<CDevice*> devices = loop->devices();
            for(int m=0; m<devices.count(); m++){
                CDevice* device = devices.at(m);
                DeviceStatusInfo tempInfo;
                tempInfo.keyId = count++;
                tempInfo.statusId = tempInfo.keyId;
                tempInfo.distributionId = distribution->distributionAddress();
                tempInfo.loopId = loop->loopAdd();
                tempInfo.deviceId = device->deviceAdd();
                CDeviceType* devType;
                devType = CGlobal::instance()->deviceType(device->value(DEVICE_VALUE_TYPE_ID).toInt());
                if(!devType)
                    devType = CGlobal::instance()->deviceType(0);
                tempInfo.type = devType->name();
                if(device->isDeviceForbid())
                    tempInfo.communicationStatus = "通讯故障";
                else
                    tempInfo.communicationStatus = device->getCommunicationStatus();
                if(device->value(DEVICE_VALUE_EMERGENCY) == "应急")
                    tempInfo.emergencyStatus = "应急";
                else if(device->value(DEVICE_VALUE_TEST) == "测试")
                    tempInfo.emergencyStatus = "测试";
                else
                    tempInfo.emergencyStatus = "正常";
                tempInfo.lightStatus = device->value(DEVICE_VALUE_LIGHT).toString();
                tempInfo.batteryLevel = device->value(DEVICE_VALUE_BATTERYPOWER).toString();
                tempInfo.batteryStatus = device->value(DEVICE_VALUE_BATTERYSTATUS).toString();
                tempInfo.area = device->value(DEVICE_VALUE_AREA).toString();
                tempInfo.location = device->value(DEVICE_VALUE_LOCATION).toString();
                m_model->addData(tempInfo);
            }
            devices.clear();
        }
        loops.clear();
    }
    distributions.clear();
    ui->tableView->setCurrentIndex(ui->tableView->model()->index(-1, 0));

}

void dlgDevices::on_tableView_clicked(const QModelIndex &index)
{
    m_index = &index;;
    if(m_menuTable && !m_menuTable->isHidden())
    {
        delete m_menuTable;
        m_menuTable = NULL;
    }
    else
    {
        if(m_menuTable) delete m_menuTable;
        m_menuTable = createTableMenu();
        if(m_menuTable != NULL)//显示在鼠标点击点的上方
        {
            QCursor cursor;
            QPoint pos;
            pos.setX(cursor.pos().x());
            pos.setY(cursor.pos().y());
            pos = m_menuTable->mapToGlobal(pos);
            m_menuTable->popup(pos);
        }
    }
}

QMenu* dlgDevices::createTableMenu()
{
    CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
    CDistribution* distribution = controller->distributionByAddress(m_model->index(m_index->row(),0).data().toInt());
    CLoop* loop = distribution->loopByAdd(m_model->index(m_index->row(),1).data().toInt());
    CDevice* device = loop->deviceByAdd(m_model->index(m_index->row(),2).data().toInt());
    int DeviceTypeId = device->deviceTypeId();
    QMenu *tableMenu = new QMenu(this);
    tableMenu->setStyleSheet(m_strSheet);
    tableMenu->setStyle(m_style);
    tableMenu->addAction("开始测试", this, SLOT(slot_StartTest()));
    tableMenu->addAction("停止测试", this, SLOT(slot_StopTest()));
    tableMenu->addSeparator();
    tableMenu->addAction("开始应急", this, SLOT(slot_StartEmergency()));
    tableMenu->addAction("停止应急", this, SLOT(slot_StopEmergency()));
    tableMenu->addSeparator();
    if(DeviceTypeId < 100 && DeviceTypeId > 0)
    {
        QMenu* batteryMenu = new QMenu("电池输出设置", this);
        batteryMenu->setStyleSheet(m_strSheet);
        batteryMenu->setStyle(m_style);
        batteryMenu->addAction("关闭电池输出", this, SLOT(slot_CloseBatteryOut()));
        batteryMenu->addSeparator();
        batteryMenu->addAction("电池输出保持", this, SLOT(slot_BatteryOutKeep()));
        tableMenu->addMenu(batteryMenu);
        tableMenu->addSeparator();
    }
    //超级管理员才可以屏蔽灯具
    if(CGlobal::instance()->m_UserType == CGlobal::SuperAdministrator)
    {
        // 创建灯具启用/禁用子菜单
        QMenu* isForbidMenu = new QMenu("启用/禁用", this);
        isForbidMenu->setStyleSheet(m_strSheetCheck);
        isForbidMenu->setStyle(m_style);

        QActionGroup *actionGroup = new QActionGroup(this);
        QAction *m_use = isForbidMenu->addAction("启用");
        connect(m_use, SIGNAL(triggered()), this, SLOT(slot_deviceUse()));
        QAction *m_forbid = isForbidMenu->addAction("禁用");
        connect(m_forbid, SIGNAL(triggered()), this, SLOT(slot_deviceForbid()));
        actionGroup->addAction(m_use);
        actionGroup->addAction(m_forbid);
        if(device->isDeviceForbid() == true)
        {
            m_forbid->setIcon(QIcon(":/images/ok.png"));
        }
        else
        {
            m_use->setIcon(QIcon(":/images/ok.png"));
        }

        tableMenu->addMenu(isForbidMenu);
    }

//    tableMenu->addAction("灯具信息", this, SLOT(slot_DeviceInfo()));
    return tableMenu;
}


void dlgDevices::slot_StartTest()
{
    CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
    CDistribution* distribution = controller->distributionByAddress(m_model->index(m_index->row(),0).data().toInt());
    CLoop* loop = distribution->loopByAdd(m_model->index(m_index->row(),1).data().toInt());
    CDevice* device = loop->deviceByAdd(m_model->index(m_index->row(),2).data().toInt());
    if(device->getStatus(OBJS_LightCommunicationFault))
        return;

    CMsgDeviceInfo msgDeviceInfo;
    msgDeviceInfo.deviceInfo.insert(DI_KeyId, m_index->row());
    msgDeviceInfo.deviceInfo.insert(DI_DeviceId, m_model->index(m_index->row(),2).data().toInt());
    msgDeviceInfo.deviceInfo.insert(DI_LoopId, m_model->index(m_index->row(),1).data().toInt());
    msgDeviceInfo.deviceInfo.insert(DI_DistributionId, m_model->index(m_index->row(),0).data().toInt());

    device->setValue(DEVICE_VALUE_TEST,"测试");
    CGlobal::instance()->DlgDevices()->getDeviceInfo();

    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceTestStart, &msgDeviceInfo);

}

void dlgDevices::slot_StopTest()
{
    CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
    CDistribution* distribution = controller->distributionByAddress(m_model->index(m_index->row(),0).data().toInt());
    CLoop* loop = distribution->loopByAdd(m_model->index(m_index->row(),1).data().toInt());
    CDevice* device = loop->deviceByAdd(m_model->index(m_index->row(),2).data().toInt());
    if(device->getStatus(OBJS_LightCommunicationFault))
        return;
    CMsgDeviceInfo msgDeviceInfo;
    msgDeviceInfo.deviceInfo.insert(DI_KeyId, m_index->row());
    msgDeviceInfo.deviceInfo.insert(DI_DeviceId, m_model->index(m_index->row(),2).data().toInt());
    msgDeviceInfo.deviceInfo.insert(DI_LoopId, m_model->index(m_index->row(),1).data().toInt());
    msgDeviceInfo.deviceInfo.insert(DI_DistributionId, m_model->index(m_index->row(),0).data().toInt());

    device->setValue(DEVICE_VALUE_TEST,"正常");
    CGlobal::instance()->DlgDevices()->getDeviceInfo();

    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceReset, &msgDeviceInfo);
}

void dlgDevices::slot_StartEmergency()
{
    CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
    CDistribution* distribution = controller->distributionByAddress(m_model->index(m_index->row(),0).data().toInt());
    CLoop* loop = distribution->loopByAdd(m_model->index(m_index->row(),1).data().toInt());
    CDevice* device = loop->deviceByAdd(m_model->index(m_index->row(),2).data().toInt());
    if(device->getStatus(OBJS_LightCommunicationFault))
        return;
    CMsgDeviceInfo msgDeviceInfo;
    msgDeviceInfo.deviceInfo.insert(DI_KeyId, m_index->row());
    msgDeviceInfo.deviceInfo.insert(DI_DeviceId, device->deviceAdd());
    msgDeviceInfo.deviceInfo.insert(DI_LoopId, loop->loopAdd());
    msgDeviceInfo.deviceInfo.insert(DI_DistributionId, distribution->distributionAddress());

    device->setValue(DEVICE_VALUE_EMERGENCY,"应急");
    CGlobal::instance()->DlgDevices()->getDeviceInfo();

    //标志灯开始闪烁
    if(CGlobal::instance()->programDeviceView()->type(device->deviceTypeId()) != NCT_LightType)
    {
        CMsgObjectStatus msgObjectStatus;
        msgObjectStatus.nDisID = distribution->distributionAddress();
        msgObjectStatus.nLoopID = loop->loopAdd();
        msgObjectStatus.nDeviceID = device->deviceAdd();
        int typeID = device->deviceTypeId();
        if( typeID == 109 || typeID == 11 || typeID == 46 || typeID == 146)
        {
            msgObjectStatus.nValue = true;
            CGlobal::instance()->ClientBusiness()->exeCommand(NCT_ChangeControl, &msgObjectStatus);
        }
        msgObjectStatus.nValue = CGlobal::instance()->m_TwinkleControl;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_FlashControl, &msgObjectStatus);
    }
    //上传灯具状态到云平台/客户端
    CGlobal::instance()->ClientBusiness()->uploadLampStatus(device);
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceEmergencyStart, &msgDeviceInfo);
}

void dlgDevices::slot_StopEmergency()
{
    CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
    CDistribution* distribution = controller->distributionByAddress(m_model->index(m_index->row(),0).data().toInt());
    CLoop* loop = distribution->loopByAdd(m_model->index(m_index->row(),1).data().toInt());
    CDevice* device = loop->deviceByAdd(m_model->index(m_index->row(),2).data().toInt());
    if(device->getStatus(OBJS_LightCommunicationFault))
        return;
    CMsgDeviceInfo msgDeviceInfo;
    msgDeviceInfo.deviceInfo.insert(DI_KeyId, m_index->row());
    msgDeviceInfo.deviceInfo.insert(DI_DeviceId, device->deviceAdd());
    msgDeviceInfo.deviceInfo.insert(DI_LoopId, loop->loopAdd());
    msgDeviceInfo.deviceInfo.insert(DI_DistributionId, distribution->distributionAddress());

    device->setValue(DEVICE_VALUE_EMERGENCY,"正常");
    CGlobal::instance()->DlgDevices()->getDeviceInfo();

    //标志灯停止闪烁
    if(CGlobal::instance()->programDeviceView()->type(device->deviceTypeId()) != NCT_LightType)
    {
        CMsgObjectStatus msgObjectStatus;
        msgObjectStatus.nDisID = distribution->distributionAddress();
        msgObjectStatus.nLoopID = loop->loopAdd();
        msgObjectStatus.nDeviceID = device->deviceAdd();
        int typeID = device->deviceTypeId();
        if( typeID == 109 || typeID == 11 || typeID == 46 || typeID == 146)
        {
            msgObjectStatus.nValue = false;
            CGlobal::instance()->ClientBusiness()->exeCommand(NCT_ChangeControl, &msgObjectStatus);
        }
        msgObjectStatus.nValue = 0;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_FlashControl, &msgObjectStatus);
    }
    //上传灯具状态到云平台/客户端
    CGlobal::instance()->ClientBusiness()->uploadLampStatus(device);
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceEmergencyStop, &msgDeviceInfo);
}

void dlgDevices::slot_CloseBatteryOut()
{
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nDisID = m_model->index(m_index->row(),0).data().toInt();
    msgobjectstatus.nLoopID = m_model->index(m_index->row(),1).data().toInt();
    msgobjectstatus.nDeviceID = m_model->index(m_index->row(),2).data().toInt();
    msgobjectstatus.nValue = true;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_CloseBatteryOut, &msgobjectstatus);
}

void dlgDevices::slot_BatteryOutKeep()
{
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nDisID = m_model->index(m_index->row(),0).data().toInt();
    msgobjectstatus.nLoopID = m_model->index(m_index->row(),1).data().toInt();
    msgobjectstatus.nDeviceID = m_model->index(m_index->row(),2).data().toInt();
    msgobjectstatus.nValue = false;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_CloseBatteryOut, &msgobjectstatus);
}

void dlgDevices::slot_deviceUse()
{
    CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
    CDistribution* distribution = controller->distributionByAddress(m_model->index(m_index->row(),0).data().toInt());
    CLoop* loop = distribution->loopByAdd(m_model->index(m_index->row(),1).data().toInt());
    CDevice* device = loop->deviceByAdd(m_model->index(m_index->row(),2).data().toInt());
    device->setDeviceForbid(false);
    getDeviceInfo();
    deviceForbidFault deviceforbid;
    deviceforbid.saveDeviceForbidFile();//保存灯具禁用表
}

void dlgDevices::slot_deviceForbid()
{
    CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
    CDistribution* distribution = controller->distributionByAddress(m_model->index(m_index->row(),0).data().toInt());
    CLoop* loop = distribution->loopByAdd(m_model->index(m_index->row(),1).data().toInt());
    CDevice* device = loop->deviceByAdd(m_model->index(m_index->row(),2).data().toInt());
    device->setDeviceForbid(true);
    device->setStatus(OBJS_Online,1);
    device->setStatus(OBJS_LightCommunicationFault,0);
    device->setValue(DEVICE_VALUE_LIGHT,"正常");
    device->setValue(DEVICE_VALUE_EMERGENCY,"正常");
    if(device->deviceTypeId() < 100)
    {
        device->setValue(DEVICE_VALUE_BATTERYPOWER,"正常");
        device->setValue(DEVICE_VALUE_BATTERYSTATUS,"正常");
    }
    getDeviceInfo();
    deviceForbidFault deviceforbid;
    deviceforbid.saveDeviceForbidFile();//保存灯具禁用表
}

//void dlgDevices::slot_DeviceInfo()
//{
//    int distributionAddress = m_model->index(m_index->row(),0).data().toInt();
//    int loopAddress = m_model->index(m_index->row(),1).data().toInt();
//    int deviceAddress = m_model->index(m_index->row(),2).data().toInt();
//    CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
//    if(controller)
//    {
//        CDistribution* distribution = controller->distributionByAddress(distributionAddress);
//        if(distribution)
//        {
//            CLoop* loop = distribution->loopByAdd(loopAddress);
//            if(loop)
//            {
//                CDevice* device = loop->deviceByAdd(deviceAddress);
//                CGlobal::instance()->m_HaveDialog = true;
//                QMessageBox::information(this,"灯具信息",
//                                         "灯具区域:" + device->value(DEVICE_VALUE_AREA).toString() + tr("\n") +
//                                         "灯具位置:" + device->value(DEVICE_VALUE_LOCATION).toString());
//                CGlobal::instance()->m_HaveDialog = false;
//            }
//        }
//    }
//}
//灯具电池状态复位并更新
void dlgDevices::lampStatusUpload()
{
    CMsgObjectStatus msgObjectStatus;
    CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
    if(controller)
    {
        QList<CDistribution*> list = controller->distributions();
        for(int i=0; i<list.count(); i++){
            CDistribution* distribution = list.at(i);
            if(distribution->distributionTypeId() >= 240 && distribution->distributionTypeId() <= 250){
                QList<CLoop*> loops = distribution->loops();
                for(int i=0; i<loops.count(); i++){
                    CLoop* loop = loops.at(i);
                    QList<CDevice*> devices = loop->devices();
                    for(int i = 0; i<devices.count(); i++){
                        CDevice* device = devices.at(i);
                        device->setValue(DEVICE_VALUE_BATTERYSTATUS,"正常");
                        device->setValue(DEVICE_VALUE_BATTERYPOWER,"正常");
                        msgObjectStatus.nDisID = distribution->distributionAddress();
                        msgObjectStatus.nLoopID = loop->loopAdd();
                        msgObjectStatus.nDeviceID = device->deviceAdd();
                        //查询电池电量
                        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_PowerInfo,&msgObjectStatus);
                    }
                    devices.clear();
                }
                loops.clear();
            }
        }
        list.clear();
    }
}

void dlgDevices::on_ReadBatteryStatus_Button_clicked()
{
    CMsgObjectStatus msgObjectStatus;
    CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
    if(controller)
    {
        QList<CDistribution*> list = controller->distributions();
        for(int i=0; i<list.count(); i++){
            CDistribution* distribution = list.at(i);
            QList<CLoop*> loops = distribution->loops();
            for(int i=0; i<loops.count(); i++){
                CLoop* loop = loops.at(i);
                QList<CDevice*> devices = loop->devices();
                for(int i = 0; i<devices.count(); i++){
                    CDevice* device = devices.at(i);
                    if(device->deviceTypeId() > 0 && device->deviceTypeId() < 100)
                    {
                        msgObjectStatus.nDisID = distribution->distributionAddress();
                        msgObjectStatus.nLoopID = loop->loopAdd();
                        msgObjectStatus.nDeviceID = device->deviceAdd();
                        //查询电池电量
                        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_PowerInfo,&msgObjectStatus);
                    }
                }
                devices.clear();
            }
            loops.clear();
        }
        list.clear();
    }
}
