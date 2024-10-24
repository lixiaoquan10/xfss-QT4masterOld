#include "ui_dlgdevices.h"
#include "dlgdevices.h"
#include "cglobal.h"


dlgDevices::dlgDevices(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgDevices)
{
    ui->setupUi(this);
    CGlobal::instance()->setDlgDevices(this);
    m_model = new CDeviceListModel();
    ui->tableView->setModel(m_model);
    ui->tableView->setColumnWidth(0, 70);
    ui->tableView->setColumnWidth(1, 50);
    ui->tableView->setColumnWidth(2, 50);
    ui->tableView->setColumnWidth(3, 250);
    ui->tableView->setColumnWidth(4, 60);
    ui->tableView->setColumnWidth(5, 60);
    ui->tableView->setColumnWidth(6, 60);
    ui->tableView->setColumnWidth(7, 80);
    ui->tableView->setColumnWidth(8, 90);
    ui->tableView->setColumnWidth(9, 280);
    ui->tableView->setColumnWidth(10, 280);

    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setHighlightSections(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setWordWrap(true);
    ui->tableView->setStyleSheet("selection-color: rgb(0, 0, 0);");
    ui->ReadBatteryStatus_Button->setStyleSheet("color: rgb(0,0,255);font: bold;");
//    CCanport* canport = CGlobal::instance()->controller()->canportById(CGlobal::instance()->panelAddress());
//    ui->lineEdit->setText(QString::number(canport->getDeviceOnlineTotal()));
    //初始化显示第一页
    ui->m_page->setText(QString::number(1));
    ui->pageScrollBar->setValue(1);
    m_maxPage = 1;
    m_deviceNumber = 0;

    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    m_menuTable = NULL;
    m_style = new CMyIconStyle();
    m_strSheet = "QMenu {background-color: rgb(255, 246, 143); margin: 3px; font-size: 14px;}\
            QMenu::item {padding: 3px 15px 3px 15px; border: 1px solid transparent; min-height: 25px; min-width: 30px; border-radius: 4px;}\
            QMenu::separator {height: 2px; background: #CCCCFF; margin-left: 5px; margin-right: 5px;}\
            QMenu::right-arrow { image: url(:/res/right-arrow.png); }\
            QMenu::indicator:exclusive:checked {image: url(:/res/symbol-check.png);}\
            QMenu::item:selected {background-color: qlineargradient(x1: 0.5, y1: 0, x2: 0.5, y2: 1, stop: 0 #99CCFF, stop: 0.5 #CCFFFF, stop: 1 #99CCFF); border-color: #6699CC;}";
    m_strSheetCheck = "QMenu {background-color: rgb(205, 198, 115); margin: 3px; font-size: 14px;}\
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

void dlgDevices::setDevicePage(int page)
{
    qDebug() << "dlgDevices::setDevicePage"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    ui->m_page->setText(QString::number(page));
    ui->pageScrollBar->setValue(page);
}

void dlgDevices::getDeviceInfo()
{
    qDebug() << "dlgDevices::getDeviceInfo"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int count = 0;
    int deviceTotal = CGlobal::instance()->controller()->getDeviceTotal();
    for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
        if(!canport)
            continue;
        ui->m_deviceNumber->setText(QString::number(deviceTotal));
        //状态显示页面存在时每页显示13条，不存在时显示19条
        if(CGlobal::instance()->DataProcessing()->isStatusTab())
            m_deviceNumber = 13;
        else
            m_deviceNumber = 19;
        m_maxPage = deviceTotal%m_deviceNumber > 0 ? (deviceTotal/m_deviceNumber+1) : (deviceTotal/m_deviceNumber);
        ui->pageScrollBar->setRange(1, m_maxPage);
        //灯具计数
        int deviceCount = 0;
        //当前需要显示的灯具计数范围
        int currentDeviceMinNumber = ui->pageScrollBar->value()*m_deviceNumber - m_deviceNumber;
        int currentDeviceMaxNumber = ui->pageScrollBar->value()*m_deviceNumber;
        QList<DeviceStatusInfo> tempList;  // 创建QList对象
        for(int i=1; i<63; i++)
        {
            CDistribution* distribution = canport->distributionByAddress(i);
            if(distribution)
            {
                for(int i=1; i<9; i++){
                    CLoop* loop = distribution->loopByAdd(i);
                    if(loop)
                    {
                        for(int m=1; m<256; m++){
                            CDevice* device = loop->deviceByAdd(m);
                            if(device)
                            {
                                deviceCount++;
                                if((deviceCount > currentDeviceMinNumber) && (deviceCount <= currentDeviceMaxNumber))
                                {
                                    DeviceStatusInfo tempInfo;
                                    tempInfo.keyId = count++;
                                    tempInfo.statusId = tempInfo.keyId;
                                    tempInfo.distributionId = "CAN" + QString::number(canport->canportAdd()-2) + "-" + QString::number(distribution->distributionAddress());
                                    tempInfo.loopId = loop->loopAdd();
                                    tempInfo.deviceId = device->deviceAdd();
                                    CDeviceType* devType;
                                    devType = CGlobal::instance()->deviceType(device->deviceValue(DEVICE_VALUE_TYPE_ID).toInt());
                                    if(!devType)
                                        devType = CGlobal::instance()->deviceType(0);
                                    tempInfo.type = devType->name();
                                    if(device->isDeviceForbid())
                                        tempInfo.communicationStatus = "故障";
                                    else
                                    {
                                        if(device->getDeviceCommunicationFault() == false)
                                            tempInfo.communicationStatus = "正常";
                                        else
                                            tempInfo.communicationStatus = "故障";
                                    }
                                    if(!device->getDeviceCommunicationFault() &&
                                            (distribution->getemergencyStatus() || device->deviceValue(DEVICE_VALUE_EMERGENCY) == "应急"))
                                        tempInfo.emergencyStatus = "应急";
                                    else if(device->deviceValue(DEVICE_VALUE_TEST) == "测试")
                                        tempInfo.emergencyStatus = "测试";
                                    else
                                        tempInfo.emergencyStatus = "正常";
                                    if(device->deviceValue(DEVICE_VALUE_LIGHT) == "光源故障")
                                        tempInfo.lightStatus = "故障";
                                    else
                                        tempInfo.lightStatus = "正常";
                                    tempInfo.batteryLevel = device->deviceValue(DEVICE_VALUE_BATTERYPOWER).toString();
                                    tempInfo.batteryStatus = device->deviceValue(DEVICE_VALUE_BATTERYSTATUS).toString();
                                    tempInfo.area = device->deviceValue(DEVICE_VALUE_AREA).toString();
                                    tempInfo.location = device->deviceValue(DEVICE_VALUE_LOCATION).toString();
                                    tempList.append(tempInfo);
    //                                m_model->addData(tempInfo);
                                    if(deviceCount == currentDeviceMaxNumber || deviceCount == deviceTotal)
                                    {
                                        m_model->clear();
                                        // 将链表中的数据逐个添加到m_model中
                                        for (int i=count-1; i>=0; i--)
                                        {
                                            m_model->addData(tempList.at(i));
                                        }
                                        return;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    ui->tableView->setCurrentIndex(ui->tableView->model()->index(-1, 0));

}

void dlgDevices::on_tableView_clicked(const QModelIndex &index)
{
    qDebug() << "dlgDevices::on_tableView_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(CGlobal::instance()->m_UserType == CGlobal::OnDuty)
        return;
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
    qDebug() << "dlgDevices::createTableMenu"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString canData = m_model->index(m_index->row(),0).data().toString();
    int dashIndex = canData.indexOf('-');
    int canAddress = canData.mid(3, dashIndex - 3).toInt() + 2;
    int distributionAddress = canData.mid(dashIndex + 1).toInt();
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canAddress);
    if(!canport)
        return NULL;
    CDistribution* distribution = canport->distributionByAddress(distributionAddress);
    if(!distribution)
        return NULL;
    CLoop* loop = distribution->loopByAdd(m_model->index(m_index->row(),1).data().toInt());
    if(!loop)
        return NULL;
    CDevice* device = loop->deviceByAdd(m_model->index(m_index->row(),2).data().toInt());
    if(!device)
        return NULL;
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
    qDebug() << "dlgDevices::slot_StartTest"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString canData = m_model->index(m_index->row(),0).data().toString();
    int dashIndex = canData.indexOf('-');
    int canAddress = canData.mid(3, dashIndex - 3).toInt() + 2;
    int distributionAddress = canData.mid(dashIndex + 1).toInt();
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canAddress);
    if(!canport)
        return;
    CDistribution* distribution = canport->distributionByAddress(distributionAddress);
    if(!distribution)
        return;
    CLoop* loop = distribution->loopByAdd(m_model->index(m_index->row(),1).data().toInt());
    if(!loop)
        return;
    CDevice* device = loop->deviceByAdd(m_model->index(m_index->row(),2).data().toInt());
    if(!device)
        return;
    if(device->getDeviceCommunicationFault())
        return;

    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = canAddress;
    msgObjectStatus.nDisID = distributionAddress;
    msgObjectStatus.nLoopID = loop->loopAdd();
    msgObjectStatus.nDeviceID = device->deviceAdd();

    device->setDeviceValue(DEVICE_VALUE_TEST,"测试");
    device->setDeviceControl(true);
    deviceControl devicecontrol;
    devicecontrol.saveDeviceControlFile();//保存灯具控制表
    CGlobal::instance()->DlgDevices()->getDeviceInfo();

    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceTestStart, &msgObjectStatus);

}

void dlgDevices::slot_StopTest()
{
    qDebug() << "dlgDevices::slot_StopTest"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString canData = m_model->index(m_index->row(),0).data().toString();
    int dashIndex = canData.indexOf('-');
    int canAddress = canData.mid(3, dashIndex - 3).toInt() + 2;
    int distributionAddress = canData.mid(dashIndex + 1).toInt();
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canAddress);
    if(!canport)
        return;
    CDistribution* distribution = canport->distributionByAddress(distributionAddress);
    if(!distribution)
        return;
    CLoop* loop = distribution->loopByAdd(m_model->index(m_index->row(),1).data().toInt());
    if(!loop)
        return;
    CDevice* device = loop->deviceByAdd(m_model->index(m_index->row(),2).data().toInt());
    if(!device)
        return;
    if(device->getDeviceCommunicationFault())
        return;

    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = canAddress;
    msgObjectStatus.nDisID = distributionAddress;
    msgObjectStatus.nLoopID = loop->loopAdd();
    msgObjectStatus.nDeviceID = device->deviceAdd();

    device->setDeviceValue(DEVICE_VALUE_TEST,"正常");
    device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"正常");
    device->setDeviceControl(false);
    deviceControl devicecontrol;
    devicecontrol.saveDeviceControlFile();//保存灯具控制表
    CGlobal::instance()->DlgDevices()->getDeviceInfo();

    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceReset, &msgObjectStatus);
}

void dlgDevices::slot_StartEmergency()
{
    qDebug() << "dlgDevices::slot_StartEmergency"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString canData = m_model->index(m_index->row(),0).data().toString();
    int dashIndex = canData.indexOf('-');
    int canAddress = canData.mid(3, dashIndex - 3).toInt() + 2;
    int distributionAddress = canData.mid(dashIndex + 1).toInt();
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canAddress);
    if(!canport)
        return;
    CDistribution* distribution = canport->distributionByAddress(distributionAddress);
    if(!distribution)
        return;
    CLoop* loop = distribution->loopByAdd(m_model->index(m_index->row(),1).data().toInt());
    if(!loop)
        return;
    CDevice* device = loop->deviceByAdd(m_model->index(m_index->row(),2).data().toInt());
    if(!device)
        return;
    if(device->getDeviceCommunicationFault())
        return;

    device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"应急");
    device->setDeviceControl(true);
    deviceControl devicecontrol;
    devicecontrol.saveDeviceControlFile();//保存灯具控制表
    CGlobal::instance()->DlgDevices()->getDeviceInfo();

    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = canAddress;
    msgObjectStatus.nDisID = distributionAddress;
    msgObjectStatus.nLoopID = loop->loopAdd();
    msgObjectStatus.nDeviceID = device->deviceAdd();

    //标志灯开始闪烁
    if(CGlobal::instance()->programDeviceView()->type(device->deviceTypeId()) != NCT_LightType)
    {
        int typeID = device->deviceTypeId();
        if( typeID == 109 || typeID == 11 || typeID == 46 || typeID == 146)
        {
            msgObjectStatus.nValue = true;
            CGlobal::instance()->ClientBusiness()->exeCommand(NCT_ChangeControl, &msgObjectStatus);
        }
        msgObjectStatus.nValue = CGlobal::instance()->m_TwinkleControl;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_FlashControl, &msgObjectStatus);
    }
//    //上传灯具状态到云平台/客户端
//    CGlobal::instance()->ClientBusiness()->uploadLampStatus(device);
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceEmergencyStart, &msgObjectStatus);
}

void dlgDevices::slot_StopEmergency()
{
    qDebug() << "dlgDevices::slot_StopEmergency"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString canData = m_model->index(m_index->row(),0).data().toString();
    int dashIndex = canData.indexOf('-');
    int canAddress = canData.mid(3, dashIndex - 3).toInt() + 2;
    int distributionAddress = canData.mid(dashIndex + 1).toInt();
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canAddress);
    if(!canport)
        return;
    CDistribution* distribution = canport->distributionByAddress(distributionAddress);
    if(!distribution)
        return;
    CLoop* loop = distribution->loopByAdd(m_model->index(m_index->row(),1).data().toInt());
    if(!loop)
        return;
    CDevice* device = loop->deviceByAdd(m_model->index(m_index->row(),2).data().toInt());
    if(!device)
        return;
    if(device->getDeviceCommunicationFault())
        return;

    device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"正常");
    if(device->deviceValue(DEVICE_VALUE_TEST) == "正常")
    {
        device->setDeviceControl(false);
        deviceControl devicecontrol;
        devicecontrol.saveDeviceControlFile();//保存灯具控制表
    }
    CGlobal::instance()->DlgDevices()->getDeviceInfo();

    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = canAddress;
    msgObjectStatus.nDisID = distributionAddress;
    msgObjectStatus.nLoopID = loop->loopAdd();
    msgObjectStatus.nDeviceID = device->deviceAdd();
    //标志灯停止闪烁
    if(CGlobal::instance()->programDeviceView()->type(device->deviceTypeId()) != NCT_LightType)
    {    
        int typeID = device->deviceTypeId();
        if( typeID == 109 || typeID == 11 || typeID == 46 || typeID == 146)
        {
            msgObjectStatus.nValue = false;
            CGlobal::instance()->ClientBusiness()->exeCommand(NCT_ChangeControl, &msgObjectStatus);
        }
        msgObjectStatus.nValue = 0;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_FlashControl, &msgObjectStatus);
    }
//    //上传灯具状态到云平台/客户端
//    CGlobal::instance()->ClientBusiness()->uploadLampStatus(device);
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceEmergencyStop, &msgObjectStatus);
}

void dlgDevices::slot_CloseBatteryOut()
{
    qDebug() << "dlgDevices::slot_CloseBatteryOut"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString canData = m_model->index(m_index->row(),0).data().toString();
    int dashIndex = canData.indexOf('-');
    int canAddress = canData.mid(3, dashIndex - 3).toInt() + 2;
    int distributionAddress = canData.mid(dashIndex + 1).toInt();
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nCanportAdd = canAddress;
    msgobjectstatus.nDisID = distributionAddress;
    msgobjectstatus.nLoopID = m_model->index(m_index->row(),1).data().toInt();
    msgobjectstatus.nDeviceID = m_model->index(m_index->row(),2).data().toInt();
    msgobjectstatus.nValue = true;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_CloseBatteryOut, &msgobjectstatus);
}

void dlgDevices::slot_BatteryOutKeep()
{
    qDebug() << "dlgDevices::slot_BatteryOutKeep"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString canData = m_model->index(m_index->row(),0).data().toString();
    int dashIndex = canData.indexOf('-');
    int canAddress = canData.mid(3, dashIndex - 3).toInt() + 2;
    int distributionAddress = canData.mid(dashIndex + 1).toInt();
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nCanportAdd = canAddress;
    msgobjectstatus.nDisID = distributionAddress;
    msgobjectstatus.nLoopID = m_model->index(m_index->row(),1).data().toInt();
    msgobjectstatus.nDeviceID = m_model->index(m_index->row(),2).data().toInt();
    msgobjectstatus.nValue = false;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_CloseBatteryOut, &msgobjectstatus);
}

void dlgDevices::slot_deviceUse()
{
    qDebug() << "dlgDevices::slot_deviceUse"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString canData = m_model->index(m_index->row(),0).data().toString();
    int dashIndex = canData.indexOf('-');
    int canAddress = canData.mid(3, dashIndex - 3).toInt() + 2;
    int distributionAddress = canData.mid(dashIndex + 1).toInt();
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canAddress);
    if(!canport)
        return;
    CDistribution* distribution = canport->distributionByAddress(distributionAddress);
    if(!distribution)
        return;
    CLoop* loop = distribution->loopByAdd(m_model->index(m_index->row(),1).data().toInt());
    if(!loop)
        return;
    CDevice* device = loop->deviceByAdd(m_model->index(m_index->row(),2).data().toInt());
    if(!device)
        return;
    device->setDeviceForbid(false);
    getDeviceInfo();
    deviceForbidFault deviceforbid;
    deviceforbid.saveDeviceForbidFile();//保存灯具禁用表
}

void dlgDevices::slot_deviceForbid()
{
    qDebug() << "dlgDevices::slot_deviceForbid"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString canData = m_model->index(m_index->row(),0).data().toString();
    int dashIndex = canData.indexOf('-');
    int canAddress = canData.mid(3, dashIndex - 3).toInt() + 2;
    int distributionAddress = canData.mid(dashIndex + 1).toInt();
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canAddress);
    if(!canport)
        return;
    CDistribution* distribution = canport->distributionByAddress(distributionAddress);
    if(!distribution)
        return;
    CLoop* loop = distribution->loopByAdd(m_model->index(m_index->row(),1).data().toInt());
    if(!loop)
        return;
    CDevice* device = loop->deviceByAdd(m_model->index(m_index->row(),2).data().toInt());
    if(!device)
        return;
    device->setDeviceForbid(true);
    device->setDeviceOnline(true);
    device->setDeviceValue(DEVICE_VALUE_LIGHT,"正常");
    device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"正常");
    if(device->deviceTypeId() < 100)
    {
        device->setDeviceValue(DEVICE_VALUE_BATTERYPOWER,"正常");
        device->setDeviceValue(DEVICE_VALUE_BATTERYSTATUS,"正常");
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
//    CCanport* canport = CGlobal::instance()->controller()->canportById(CGlobal::instance()->panelAddress());
//    if(canport)
//    {
//        CDistribution* distribution = canport->distributionByAddress(distributionAddress);
//        if(distribution)
//        {
//            CLoop* loop = distribution->loopByAdd(loopAddress);
//            if(loop)
//            {
//                CDevice* device = loop->deviceByAdd(deviceAddress);
//                CGlobal::instance()->m_HaveDialog = true;
//                QMessageBox::information(this,"灯具信息",
//                                         "灯具区域:" + device->deviceValue(DEVICE_VALUE_AREA).toString() + tr("\n") +
//                                         "灯具位置:" + device->deviceValue(DEVICE_VALUE_LOCATION).toString());
//                CGlobal::instance()->m_HaveDialog = false;
//            }
//        }
//    }
//}
//灯具电池状态复位并更新
void dlgDevices::lampBatStatusUpload()
{
    qDebug() << "dlgDevices::lampBatStatusUpload"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgObjectStatus msgObjectStatus;
    for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
        if(canport)
        {
            QList<CDistribution*> list = canport->distributions();
            for(int i=0; i<list.count(); i++){
                CDistribution* distribution = list.at(i);
                if(distribution && distribution->distributionTypeId() >= 240 && distribution->distributionTypeId() <= 250){
                    QList<CLoop*> loops = distribution->loops();
                    for(int i=0; i<loops.count(); i++){
                        CLoop* loop = loops.at(i);
                        if(!loop)
                            continue;
                        QList<CDevice*> devices = loop->devices();
                        for(int i = 0; i<devices.count(); i++){
                            CDevice* device = devices.at(i);
                            if(!device)
                                continue;
                            device->setDeviceValue(DEVICE_VALUE_BATTERYSTATUS,"正常");
                            device->setDeviceValue(DEVICE_VALUE_BATTERYPOWER,"正常");
                            msgObjectStatus.nCanportAdd = distribution->canportAddress();
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
}

void dlgDevices::on_ReadBatteryStatus_Button_clicked()
{
    qDebug() << "dlgDevices::on_ReadBatteryStatus_Button_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgObjectStatus msgObjectStatus;
    for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
        if(canport)
        {
            QList<CDistribution*> list = canport->distributions();
            for(int i=0; i<list.count(); i++){
                CDistribution* distribution = list.at(i);
                if(!distribution)
                    continue;
                QList<CLoop*> loops = distribution->loops();
                for(int i=0; i<loops.count(); i++){
                    CLoop* loop = loops.at(i);
                    if(!loop)
                        continue;
                    QList<CDevice*> devices = loop->devices();
                    for(int i = 0; i<devices.count(); i++){
                        CDevice* device = devices.at(i);
                        if(!device)
                            continue;
                        if(device->deviceTypeId() > 0 && device->deviceTypeId() < 100)
                        {
                            msgObjectStatus.nCanportAdd = device->canportAdd();
                            msgObjectStatus.nDisID = distribution->canportAddress();
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
}

void dlgDevices::on_pageScrollBar_valueChanged(int value)
{
    qDebug() << "dlgDevices::on_pageScrollBar_valueChanged"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    ui->m_page->setText(QString::number(value));
    getDeviceInfo();
}

void dlgDevices::on_nextPage_Button_clicked()
{
    qDebug() << "dlgDevices::on_nextPage_Button_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int currentpage = ui->m_page->text().toInt();
    int page = currentpage + 1;
    if(currentpage == m_maxPage)
    {
        page = 1;
    }
    ui->pageScrollBar->setValue(page);
    ui->m_page->setText(QString::number(page));
    getDeviceInfo();
}

void dlgDevices::on_previousPage_Button_clicked()
{
    qDebug() << "dlgDevices::on_previousPage_Button_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int currentpage = ui->m_page->text().toInt();
    int page = currentpage - 1;
    if(currentpage == 1)
    {
        page = m_maxPage;
    }
    ui->pageScrollBar->setValue(page);
    ui->m_page->setText(QString::number(page));
    getDeviceInfo();
}
