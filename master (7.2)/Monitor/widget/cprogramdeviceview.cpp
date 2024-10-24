#include "ui_cprogramdeviceview.h"
#include "cglobal.h"



CProgramDeviceView::CProgramDeviceView(QWidget *parent) :
    QWidget(parent), ui(new Ui::CProgramDeviceView)
{
    ui->setupUi(this);
    CGlobal::instance()->setProgramDeviceView(this);
    ui->m_pToolBar->hide();
    m_model = new CDeviceModel();
    ui->tableView->setModel(m_model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->addSelectEditColumn(DEVICE_COLUMN_TYPE);
    ui->tableView->addSelectEditColumn(DEVICE_COLUMN_PRODUCTID);
    CDeviceTypeDelegate* delegate = new CDeviceTypeDelegate(this);
    ui->tableView->setItemDelegateForColumn(DEVICE_COLUMN_TYPE, delegate);
    ui->tableView->setColumnWidth(DEVICE_COLUMN_ADDRESS, 70);
    ui->tableView->setColumnWidth(DEVICE_COLUMN_LOGIN_ADDRESS, 70);
    ui->tableView->setColumnWidth(DEVICE_COLUMN_TYPE, 230);
    ui->tableView->setColumnWidth(DEVICE_COLUMN_LOGIN_TYPE, 230);
    ui->tableView->setColumnWidth(DEVICE_COLUMN_PRODUCTID, 170);
    ui->tableView->setColumnWidth(DEVICE_COLUMN_VERSION, 96);
    ui->tableView->setColumnWidth(DEVICE_COLUMN_CONFIG_LOGIN, 70);
//    ui->tableView->setColumnWidth(DEVICE_COLUMN_AREA, 32);
//    ui->tableView->setColumnWidth(DEVICE_COLUMN_LOCATION, 32);
    ui->tableView->setColumnWidth(DEVICE_COLUMN_STATUS, 70);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);   
    ui->tableView->horizontalHeader()->setHighlightSections(false);
    ui->tableView->setStyleSheet("selection-color: rgb(0, 0, 0);");
    installEventFilter(this);
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


CProgramDeviceView::~CProgramDeviceView()
{
    CGlobal::instance()->setProgramDeviceView(0);
    ui->tableView->setModel(0);
    delete m_model;
    delete m_menuTable;
    delete m_style;
    delete ui;
}


bool CProgramDeviceView::eventFilter(QObject *target, QEvent *event)
{
    qDebug() << "CProgramDeviceView::eventFilter" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if (event->type() == QEvent::MouseButtonPress) {
        // 获取鼠标位置
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QPoint point = mouseEvent->pos();

        // 判断是否在 QTableView 区域内
        QRect tableRect = ui->tableView->visualRect(ui->tableView->model()->index(0, 0));
        if (ui->tableView->model()->rowCount() <= ui->tableView->height() / tableRect.height()) {
            tableRect.setHeight(ui->tableView->model()->rowCount() * tableRect.height());
        }
        if (!tableRect.contains(point)) {
            // 取消 QTableView 的选择状态
            ui->tableView->clearSelection();
        }
    }

    return QWidget::eventFilter(target, event);
}

void CProgramDeviceView::setUpdateEnble()
{
    m_model->setupdate();
}

void CProgramDeviceView::setLoop(CLoop *loop)
{
    m_model->setLoop(loop);
}

CLoop *CProgramDeviceView::loop()
{
    return m_model->loop();
}


QMenu* CProgramDeviceView::createTableMenu(int lamptype, CDevice* device)
{
    qDebug() << "CProgramDeviceView::createTableMenu" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int typeID = device->deviceTypeId();
    QMenu *tableMenu = new QMenu(this);
    tableMenu->setStyleSheet(m_strSheet);
    tableMenu->setStyle(m_style);
    switch (lamptype) {
        case NCT_LightType:
        {
            // 创建控制子菜单
            QMenu* directionMenu = new QMenu("控制", this);
            directionMenu->setStyleSheet(m_strSheetCheck);
            directionMenu->setStyle(m_style);
            directionMenu->addAction("点亮", this, SLOT(slot_Lighten()));
            directionMenu->addSeparator();
            directionMenu->addAction("熄灭", this, SLOT(slot_CloseLight()));
            tableMenu->addMenu(directionMenu);
            tableMenu->addSeparator();
            break;
        }
        case NCT_ArrowLeftType:
        {
            // 创建控制子菜单
            QMenu* directionMenu = new QMenu("控制", this);
            directionMenu->setStyleSheet(m_strSheetCheck);
            directionMenu->setStyle(m_style);
            directionMenu->addAction("全灭", this, SLOT(slot_AllClose()));
            directionMenu->addSeparator();
            directionMenu->addAction("左向", this, SLOT(slot_Left()));
            directionMenu->addSeparator();
            directionMenu->addAction("全亮", this, SLOT(slot_StopTest()));
            tableMenu->addMenu(directionMenu);
            tableMenu->addSeparator();
            break;
        }
        case NCT_ArrowRightType:
        {
            // 创建控制子菜单
            QMenu* directionMenu = new QMenu("控制", this);
            directionMenu->setStyleSheet(m_strSheetCheck);
            directionMenu->setStyle(m_style);
            directionMenu->addAction("全灭", this, SLOT(slot_AllClose()));
            directionMenu->addSeparator();
            directionMenu->addAction("右向", this, SLOT(slot_Right()));
            directionMenu->addSeparator();
            directionMenu->addAction("全亮", this, SLOT(slot_StopTest()));
            tableMenu->addMenu(directionMenu);
            tableMenu->addSeparator();
            break;
        }
        case NCT_ArrowDoubleType:
        {
            // 创建控制子菜单
            QMenu* directionMenu = new QMenu("控制", this);
            directionMenu->setStyleSheet(m_strSheetCheck);
            directionMenu->setStyle(m_style);
            directionMenu->addAction("全灭", this, SLOT(slot_AllClose()));
            directionMenu->addSeparator();
            directionMenu->addAction("左向", this, SLOT(slot_Left()));
            directionMenu->addSeparator();
            directionMenu->addAction("右向", this, SLOT(slot_Right()));
            directionMenu->addSeparator();
            directionMenu->addAction("全亮", this, SLOT(slot_StopTest()));
            tableMenu->addMenu(directionMenu);
            tableMenu->addSeparator();
            break;
        }
        default:
            break;
    }
    tableMenu->addAction("修改灯具地址", this, SLOT(slot_ChangeAdd()));
    if((typeID >= 15 && typeID <= 18) || (typeID >= 40 && typeID <= 47) || typeID == 79 ||
            (typeID >= 115 && typeID <= 118) || (typeID >= 140 && typeID <= 147) || typeID == 179 ||
            typeID == 122 || typeID == 123)//非未知类型MA灯具，不可修改设备类型
    {

    }
    else
    {
        tableMenu->addAction("修改设备类型", this, SLOT(slot_ChangeDeviceType()));
    }
    tableMenu->addSeparator();
    if(typeID < 100 && typeID > 0)
    {
        QMenu* batteryMenu = new QMenu("电池输出设置", this);
        batteryMenu->setStyleSheet(m_strSheetCheck);
        batteryMenu->setStyle(m_style);
        batteryMenu->addAction("关闭电池输出", this, SLOT(slot_CloseBatteryOut()));
        batteryMenu->addSeparator();
        batteryMenu->addAction("电池输出保持", this, SLOT(slot_BatteryOutKeep()));
        tableMenu->addMenu(batteryMenu);
        tableMenu->addSeparator();
    }
    tableMenu->addSeparator();
    tableMenu->addAction("开始测试", this, SLOT(slot_StartTest()));
    tableMenu->addAction("停止测试", this, SLOT(slot_StopTest()));
    tableMenu->addSeparator();
    tableMenu->addAction("开始应急", this, SLOT(slot_StartEmergency()));
    tableMenu->addAction("停止应急", this, SLOT(slot_StopEmergency()));
    tableMenu->addSeparator();

    tableMenu->addAction("读软件版本", this, SLOT(slot_ReadLampVersion()));
    tableMenu->addSeparator();

    if(isNewLamp(device))
        tableMenu->addAction("替换旧灯具", this, SLOT(slot_uploadOldLamp()));

    if(!device->isDeviceLogin())
        tableMenu->addAction("下发到集中电源", this, SLOT(slot_nologinSendToDistribution()));

    return tableMenu;
}
//判断是否为新加灯具并且可执行替换旧灯具
bool CProgramDeviceView::isNewLamp(CDevice* device)
{
    qDebug() << "CProgramDeviceView::isNewLamp" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
//    if(device->deviceValue(DEVICE_VALUE_ISUSED) != 2)
    if(device->isDeviceConfig() && (!device->isDeviceLogin()))
        return false;
    CLoop* loop = device->loop();
    if(loop){
        QList<CDevice*> devices = loop->devices();
        for(int i=0; i<devices.count(); i++){
            CDevice* lamp = devices.at(i);
            if(lamp)
            {
                if(((device->deviceTypeId() < 50 && lamp->deviceTypeId() < 50) ||
                        (device->deviceTypeId() >= 50 && device->deviceTypeId() < 100 && lamp->deviceTypeId() >= 50 && lamp->deviceTypeId() < 100) ||
                        (device->deviceTypeId() >= 100 && device->deviceTypeId() < 150 && lamp->deviceTypeId() >= 100 && lamp->deviceTypeId() < 150) ||
                        (device->deviceTypeId() > 150 && lamp->deviceTypeId() > 150))
//                        && (lamp->getDeviceCommunicationFault() ||lamp->deviceValue(DEVICE_VALUE_ISUSED) == 0))
                        && (lamp->getDeviceCommunicationFault() || (!lamp->isDeviceLogin())))
                    return true;
            }
        }
        devices.clear();
    }
    return false;
}

void CProgramDeviceView::on_tableView_clicked(const QModelIndex &index)
{
    qDebug() << "CProgramDeviceView::on_tableView_clicked" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_device = static_cast<CDevice*>(m_model->getObject(index));
    if(m_menuTable && !m_menuTable->isHidden())
    {
        delete m_menuTable;
        m_menuTable = NULL;
    }
    else
    {
        if(m_menuTable) delete m_menuTable;
        m_menuTable = createTableMenu(type(m_device->deviceTypeId()),m_device);
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

int CProgramDeviceView::type(int typeId)
{
    qDebug() << "CProgramDeviceView::type" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
//    int typeId = device->deviceTypeId();
    if((typeId >= 150 && typeId < 200) || (typeId >= 50 && typeId < 100))
        return NCT_LightType;
    else if(typeId == 101 || typeId == 104 || typeId == 2 || typeId == 7 ||
            typeId == 16 || typeId == 42 || typeId == 116 || typeId == 142)
        return NCT_ArrowLeftType;
    else if(typeId == 105 || typeId == 8 || typeId == 43 || typeId == 143)
        return NCT_ArrowRightType;
    else if(typeId == 102 || typeId == 106 || typeId == 3 || typeId == 9 ||
            typeId == 17 || typeId == 44 || typeId == 117 || typeId == 144 ||
            typeId == 121 || typeId == 123)
        return NCT_ArrowDoubleType;
    return 0;
}

void CProgramDeviceView::slot_ChangeAdd()
{
    qDebug() << "CProgramDeviceView::slot_ChangeAdd" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    dlgChangeAdd dlgchangAdd;
    if(QDialog::Accepted == dlgchangAdd.exec())
    {

    }

}

void CProgramDeviceView::slot_ChangeDeviceType()
{
    qDebug() << "CProgramDeviceView::slot_ChangeDeviceType" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    dlgChangeType dlgchangType;
    if(QDialog::Accepted == dlgchangType.exec())
    {

    }
}
//照明灯点亮
void CProgramDeviceView::slot_Lighten()
{
    qDebug() << "CProgramDeviceView::slot_Lighten" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = m_device->canportAdd();
    msgObjectStatus.nDisID = m_device->distributionAdd();
    msgObjectStatus.nLoopID = m_device->loopAdd();
    msgObjectStatus.nDeviceID = m_device->deviceAdd();

    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDefaultOn, &msgObjectStatus);

}

//照明灯灭灯
void CProgramDeviceView::slot_CloseLight()
{
    qDebug() << "CProgramDeviceView::slot_CloseLight" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = m_device->canportAdd();
    msgObjectStatus.nDisID = m_device->distributionAdd();
    msgObjectStatus.nLoopID = m_device->loopAdd();
    msgObjectStatus.nDeviceID = m_device->deviceAdd();

    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDefaultOff, &msgObjectStatus);

}
//标志灯全灭
void CProgramDeviceView::slot_AllClose()
{
    qDebug() << "CProgramDeviceView::slot_AllClose" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int direction;
    direction = 0x04;
    if(!m_device)
        return;
    m_device->setDeviceValue(DEVICE_VALUE_LEFTCLOSE,1);
    m_device->setDeviceValue(DEVICE_VALUE_RIGHTCLOSE,1);

    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nCanportAdd = m_device->canportAdd();
    msgobjectstatus.nDisID = m_device->distributionAdd();
    msgobjectstatus.nLoopID = m_device->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();
    msgobjectstatus.nValue = direction;
//    //上传灯具指向到云平台
//    CGlobal::instance()->ClientBusiness()->XmlUploadLampDirection(
//                CGlobal::instance()->saveKeyId(OBJT_Device, device->keyId()),"全灭");
//    //服务端上传灯具指向
//    CGlobal::instance()->ClientBusiness()->serverLampDirectionUpload(
//                CGlobal::instance()->saveKeyId(OBJT_Device, device->keyId()),"全灭");
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDirection, &msgobjectstatus);

    //布局视图界面灯具状态更新
    CGlobal::instance()->designSplashView()->setPageStateUpdate(true);
}
//标志灯左向
void CProgramDeviceView::slot_Left()
{
    qDebug() << "CProgramDeviceView::slot_Left" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int direction;
    direction = 0x05;
    if(!m_device)
        return;
    m_device->setDeviceValue(DEVICE_VALUE_LEFTCLOSE,0);
    m_device->setDeviceValue(DEVICE_VALUE_RIGHTCLOSE,1);

    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nCanportAdd = m_device->canportAdd();
    msgobjectstatus.nDisID = m_device->distributionAdd();
    msgobjectstatus.nLoopID = m_device->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();
    msgobjectstatus.nValue = direction;
//    //上传灯具指向到云平台
//    CGlobal::instance()->ClientBusiness()->XmlUploadLampDirection(
//                CGlobal::instance()->saveKeyId(OBJT_Device, device->keyId()),"左向");
//    //服务端上传灯具指向
//    CGlobal::instance()->ClientBusiness()->serverLampDirectionUpload(
//                CGlobal::instance()->saveKeyId(OBJT_Device, device->keyId()),"左向");
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDirection, &msgobjectstatus);

    //布局视图界面灯具状态更新
    CGlobal::instance()->designSplashView()->setPageStateUpdate(true);

}

//标志灯右向
void CProgramDeviceView::slot_Right()
{
    qDebug() << "CProgramDeviceView::slot_Right" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int direction;
    direction = 0x06;
    if(!m_device)
        return;
    m_device->setDeviceValue(DEVICE_VALUE_LEFTCLOSE,1);
    m_device->setDeviceValue(DEVICE_VALUE_RIGHTCLOSE,0);
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nCanportAdd = m_device->canportAdd();
    msgobjectstatus.nDisID = m_device->distributionAdd();
    msgobjectstatus.nLoopID = m_device->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();
    msgobjectstatus.nValue = direction;
//    //上传灯具指向到云平台
//    CGlobal::instance()->ClientBusiness()->XmlUploadLampDirection(
//                CGlobal::instance()->saveKeyId(OBJT_Device, device->keyId()),"右向");
//    //服务端上传灯具指向
//    CGlobal::instance()->ClientBusiness()->serverLampDirectionUpload(
//                CGlobal::instance()->saveKeyId(OBJT_Device, device->keyId()),"右向");
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDirection, &msgobjectstatus);

    //布局视图界面灯具状态更新
    CGlobal::instance()->designSplashView()->setPageStateUpdate(true);

}
//灯具测试开始
void CProgramDeviceView::slot_StartTest()
{
    qDebug() << "CProgramDeviceView::slot_StartTest" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = m_device->canportAdd();
    msgObjectStatus.nDisID = m_device->distributionAdd();
    msgObjectStatus.nLoopID = m_device->loopAdd();
    msgObjectStatus.nDeviceID = m_device->deviceAdd();

    m_device->setDeviceValue(DEVICE_VALUE_TEST,"测试");
    m_device->setDeviceControl(true);
    deviceControl devicecontrol;
    devicecontrol.saveDeviceControlFile();//保存灯具控制表
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceTestStart, &msgObjectStatus);

}
//灯具测试停止
void CProgramDeviceView::slot_StopTest()
{
    qDebug() << "CProgramDeviceView::slot_StopTest" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = m_device->canportAdd();
    msgObjectStatus.nDisID = m_device->distributionAdd();
    msgObjectStatus.nLoopID = m_device->loopAdd();
    msgObjectStatus.nDeviceID = m_device->deviceAdd();

    m_device->setDeviceValue(DEVICE_VALUE_TEST,"正常");
    m_device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"正常");
    m_device->setDeviceValue(DEVICE_VALUE_LEFTCLOSE,0);
    m_device->setDeviceValue(DEVICE_VALUE_RIGHTCLOSE,0);
    m_device->setDeviceControl(false);
    deviceControl devicecontrol;
    devicecontrol.saveDeviceControlFile();//保存灯具控制表
    CGlobal::instance()->DlgDevices()->getDeviceInfo();

    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceReset, &msgObjectStatus);

    //布局视图界面灯具状态更新
    CGlobal::instance()->designSplashView()->setPageStateUpdate(true);
}
//灯具应急开始
void CProgramDeviceView::slot_StartEmergency()
{
    qDebug() << "CProgramDeviceView::slot_StartEmergency" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    if(m_device->getDeviceCommunicationFault())
        return;

    m_device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"应急");
    m_device->setDeviceControl(true);
    deviceControl devicecontrol;
    devicecontrol.saveDeviceControlFile();//保存灯具控制表

    CGlobal::instance()->DlgDevices()->getDeviceInfo();

    //布局视图界面灯具状态更新
    CGlobal::instance()->designSplashView()->setPageStateUpdate(true);
    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = m_device->canportAdd();
    msgObjectStatus.nDisID = m_device->distributionAdd();
    msgObjectStatus.nLoopID = m_device->loopAdd();
    msgObjectStatus.nDeviceID = m_device->deviceAdd();

    //标志灯开始闪烁
    if(type(m_device->deviceTypeId()) != NCT_LightType)
    {
        int typeID = m_device->deviceTypeId();
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
//灯具应急停止
void CProgramDeviceView::slot_StopEmergency()
{
    qDebug() << "CProgramDeviceView::slot_StopEmergency" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    if(m_device->getDeviceCommunicationFault())
        return;
    m_device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"正常");

    if(m_device->deviceValue(DEVICE_VALUE_TEST) == "正常")
    {
        m_device->setDeviceControl(false);
        deviceControl devicecontrol;
        devicecontrol.saveDeviceControlFile();//保存灯具控制表
    }

    CGlobal::instance()->DlgDevices()->getDeviceInfo();
    //布局视图界面灯具状态更新
    CGlobal::instance()->designSplashView()->setPageStateUpdate(true);

    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = m_device->canportAdd();
    msgObjectStatus.nDisID = m_device->distributionAdd();
    msgObjectStatus.nLoopID = m_device->loopAdd();
    msgObjectStatus.nDeviceID = m_device->deviceAdd();
    //标志灯停止闪烁
    if(type(m_device->deviceTypeId()) != NCT_LightType)
    {
        int typeID = m_device->deviceTypeId();
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


//读软件版本
void CProgramDeviceView::slot_ReadLampVersion()
{
    qDebug() << "CProgramDeviceView::slot_ReadLampVersion" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;

    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = m_device->canportAdd();
    msgObjectStatus.nDisID = m_device->distributionAdd();
    msgObjectStatus.nLoopID = m_device->loopAdd();
    msgObjectStatus.nDeviceID = m_device->deviceAdd();

    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_ReadDeviceInfo, &msgObjectStatus);
}

//关闭电池输出
void CProgramDeviceView::slot_CloseBatteryOut()
{
    qDebug() << "CProgramDeviceView::slot_CloseBatteryOut" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = m_device->canportAdd();
    msgObjectStatus.nDisID = m_device->distributionAdd();
    msgObjectStatus.nLoopID = m_device->loopAdd();
    msgObjectStatus.nDeviceID = m_device->deviceAdd();
    msgObjectStatus.nValue = true;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_CloseBatteryOut, &msgObjectStatus);
}
//电池输出保持
void CProgramDeviceView::slot_BatteryOutKeep()
{
    qDebug() << "CProgramDeviceView::slot_BatteryOutKeep" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = m_device->canportAdd();
    msgObjectStatus.nDisID = m_device->distributionAdd();
    msgObjectStatus.nLoopID = m_device->loopAdd();
    msgObjectStatus.nDeviceID = m_device->deviceAdd();
    msgObjectStatus.nValue = false;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_CloseBatteryOut, &msgObjectStatus);
}

//替换旧灯具
void CProgramDeviceView::slot_uploadOldLamp()
{
    qDebug() << "CProgramDeviceView::slot_uploadOldLamp" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    dlgUploadLamp dlguploadlamp;
    if(QDialog::Accepted == dlguploadlamp.exec())
    {

    }
}

//未注册灯具下发到集中电源指定回路
void CProgramDeviceView::slot_nologinSendToDistribution()
{
    qDebug() << "CProgramDeviceView::slot_nologinSendToDistribution" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = m_device->canportAdd();
    msgObjectStatus.nDisID = m_device->distributionAdd();
    msgObjectStatus.nLoopID = m_device->loopAdd();
    msgObjectStatus.nDeviceID = m_device->deviceAdd();

    msgObjectStatus.nType = m_device->deviceTypeId();
    msgObjectStatus.strText = m_device->deviceProductionId();

    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_NologinSendToDistribution, &msgObjectStatus);
}
