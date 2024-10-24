#include "cclientbusiness.h"
#include "cdataprocessing.h"
#include "communication/ccommunication.h"
#include "widget/cprogramtreeview.h"
#include "widget/cdesigntreeview.h"
#include "widget/cbottomtoolbar.h"
#include "widget/ctoptoolbar.h"
#include "dm/ccontroller.h"
#include "dm/cdm.h"
#include "dm/ccanport.h"
#include "dm/cdistribution.h"
#include "dm/cloop.h"
#include "dm/cdevice.h"
#include "dm/clayer.h"
#include "dialog/dlgadministrator.h"
#include "dialog/dlgdevices.h"
#include "dialog/dlgtimeinterval.h"
#include "dialog/dlgsystemrecord.h"
#include "widget/cprogramdistributionview.h"
#include "widget/cprogramcandeviceview.h"
#include "widget/cprogramdeviceview.h"
#include "widget/cdesignsplashview.h"
#include "graphics/configfilemanager.h"
#include "graphics/deviceForbidFault.h"
#include "cglobal.h"
#include "dm/cdbsqlite.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QApplication>
#include <QTextCodec>
#include <QUuid>

CClientBusiness::CClientBusiness(QObject *parent) :
    QObject(parent), m_strNormal("<span style='color:#60D978;'>正常</span>"),
    m_strFault("<span style='color:#E53333;'>故障</span>"),
    m_strCharging("<span style='color:#00D5FF;'>充电</span>"),
    m_isInitController(false), m_isRecvStatus(false),
    m_currentStatusId(0), m_currentRecvStatusId(0), m_currentRecvStatusNumber(0),
    m_advPassword("6006")
{
    m_projectPath = QCoreApplication::applicationDirPath(); /*应用程序目录 /opt*/
//    m_communication = new CCommunication(m_projectPath + "/BService.dll");
    m_communication = new CCommunication(m_projectPath + "/libBService.so.1.0.0");
    connect(m_communication, SIGNAL(recvData(int,QByteArray&)), this, SLOT(recvData(int,QByteArray&)));
    m_projectPath = QCoreApplication::applicationDirPath();
    createPath(m_projectPath);        
    setCommunicationConfig();

    m_projectFile = m_projectPath + "/ESSQLiteCE100.db";
    m_checkSet.yearCycle = 365;
    m_checkSet.yearDuration = 30;
    m_checkSet.monthCycle = 30;
    m_checkSet.monthDuration = 30;
    m_strPassword = "0000";
    m_bForceEmergency = true;
    m_xmlheader = "version=\"1.0\" encoding=\"UTF-8\"";
    connect(CGlobal::instance()->dm(), SIGNAL(sigObjectStatusChanged(CObject*,short,unsigned char,unsigned char,uint)), this, SLOT(objectStatusChanged(CObject*,short,unsigned char,unsigned char,uint)));
    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeOut()));
    m_timer->start(5000);
    m_masterStateUploadFlag = false;
    m_cloudStateUploadFlag = false;
    m_isQueryAllState = false;
    m_isCANconfig = false;

    m_msgLog = new CMsgLog();

    m_mainPowerStoptimer = new QTimer();
    connect(m_mainPowerStoptimer, SIGNAL(timeout()), this, SLOT(mainPowerStop()));

    m_saveLogintimer = new QTimer();
    connect(m_saveLogintimer, SIGNAL(timeout()), this, SLOT(slot_saveLoginData()));

    m_flashTimer = new QTimer();
    connect(m_flashTimer, SIGNAL(timeout()), this, SLOT(slot_flashControl()));
    // 创建 QProcess 对象并连接信号与槽
//    m_process_light = new QProcess(this);
//    QString program = "/home/xfss/root/USBprogramUpdate/lightScreem.sh";  // 脚本的路径
//    m_process_light->start(program);
}

CClientBusiness::~CClientBusiness()
{
    delete m_timer;
    delete m_communication;
    delete m_msgLog;
    delete m_mainPowerStoptimer;
    delete m_saveLogintimer;
    delete m_flashTimer;
    m_msg.clear();
}

void CClientBusiness::setCommunicationConfig()
{
    qDebug() << "CClientBusiness::setCommunicationConfig" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    m_communication->CommunicationType(m_projectPath);
    m_communication->InitCommunication();
    QByteArray byte;
    CMsgRouteConfig msg;
    foreach(RouteConfig* card, CGlobal::instance()->m_hashCard)
    {
        if(!card) continue;
        msg.routeConfigs.append(*card);
    }
    byte = msg.data();
    m_communication->ParsePacket(NCT_SetRouteConfig, byte.data(), byte.size());
}

void CClientBusiness::destroyCommunication()
{
    qDebug() << "CClientBusiness::destroyCommunication"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if(m_communication) m_communication->DestroyedCommunication();
}

void CClientBusiness::startCommunication()
{
    qDebug() << "CClientBusiness::startCommunication"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if(m_communication->StartCommunication(CGlobal::instance()->panelAddress())){
        CGlobal::instance()->dm()->clearStatusObjects(true);
        CGlobal::instance()->m_bStartComm = true;
        CGlobal::instance()->InformationWindow()->hide();
        CGlobal::instance()->setMSgColor(QColor(12, 180, 3));
    }
    m_communication->ShowTest(true);        
}

void CClientBusiness::stopCommunication()
{
    qDebug() << "CClientBusiness::stopCommunication"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if(m_communication->StopCommunication()){
        CGlobal::instance()->m_bStartComm = false;
        CGlobal::instance()->m_bInLogin = false;
    }
}

bool CClientBusiness::verifyAdminUser(const int commandType, const QString &password)
{
    qDebug() << "CClientBusiness::verifyAdminUser"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    bool status = false;
    if(commandType == 3){
        if(password == m_advPassword)
            status = true;
    }
    else{
        if(password == m_strPassword)
            status = true;
    }
    if(CGlobal::instance()->m_loginStatus == false){
        if(status)
        {
            QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");;
            db.setDatabaseName(fileName);
            if(!db.open())
                return false;
            QSqlQuery query(db);
            if(commandType == 0)
            {
                CGlobal::instance()->m_UserType = CGlobal::OnDuty;
                query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('LastLoginName', '%1');").arg("值班员"));
            }
            else if(commandType == 1)
            {
                CGlobal::instance()->m_UserType = CGlobal::Operator;
                query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('LastLoginName', '%1');").arg("操作员"));
            }
            else if(commandType == 2)
            {
                CGlobal::instance()->m_UserType = CGlobal::Administrator;
                query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('LastLoginName', '%1');").arg("管理员"));
            }
            else if(commandType == 3)
            {
                CGlobal::instance()->m_UserType = CGlobal::SuperAdministrator;
                query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('LastLoginName', '%1');").arg("SuperAdmin"));
            }

            db.commit();
            db.close();
            CGlobal::instance()->BottomToolBar()->setAdministrator();
        }

    }
    else
    {
        if(commandType != CGlobal::instance()->m_UserType)
            status = false;
    }
    if((CGlobal::instance()->m_loginStatus == false) && status){
        CGlobal::instance()->m_loginStatus = true;
    }
    return status;
}

void CClientBusiness::setNewPwd(const QString strpwd)
{
    qDebug() << "CClientBusiness::setNewPwd"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    m_strPassword = strpwd;
    CGlobal::instance()->m_strPassWord = strpwd;
}

///用户操作按键时要求验证用户权限
bool CClientBusiness::verifyAdminUser(const int commandType)
{
    qDebug() << "CClientBusiness::verifyAdminUser"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    static bool flag = true;
    if(CGlobal::instance()->m_UserType == CGlobal::User)
    {
        if(flag)
        {
            flag = false;
            dlgAdministrator m_Administrator;
            if(QDialog::Accepted == m_Administrator.exec()) {
                CGlobal::instance()->BottomToolBar()->setAdministrator();
            } else {
                flag = true;
                return false;
            }
            flag = true;
        }
    }
    else if(CGlobal::instance()->m_UserType == CGlobal::Administrator)
    {
        if (commandType == NCT_ForceEmergency)
        {
            if(flag)
            {
                flag = false;
                dlgAdministrator m_Administrator;
                if(QDialog::Accepted == m_Administrator.exec()) {
                    CGlobal::instance()->BottomToolBar()->setAdministrator();
                } else {
                    flag = true;
                    return false;
                }
                flag = true;
            }
        }
    }
    return true;
}

void CClientBusiness::addLog(const QString strUser, const QString strTitle, const QString strArea, const QString strLocation, const QString strRemark,
                             const int CanPort, const int CanDeviceAddress, const int Loop, const int LampDeviceAddress, const QString strType)
{
    qDebug() << "CClientBusiness::addLog"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    CMsgLog msgLog;
    msgLog.user = strUser;
    msgLog.time = QDateTime::currentDateTime();
    msgLog.title = strTitle;
    msgLog.type = strType;
    msgLog.User = strUser;
    msgLog.Type = strType;
    msgLog.Time = QDateTime::currentDateTime();
    msgLog.Area = strArea;
    msgLog.Location = strLocation;
    msgLog.Remark = strRemark;
    msgLog.Canport = CanPort;
    msgLog.CanDeviceAddress = CanDeviceAddress;
    msgLog.Loop = Loop;
    msgLog.LampDeviceAddress = LampDeviceAddress;
    exeCommand(NCT_AddLog, &msgLog);
}

void CClientBusiness::showTest(bool bwrite)
{
    qDebug() << "CClientBusiness::showTest"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if(m_communication)
        m_communication->ShowTest(bwrite);
}

bool CClientBusiness::exeCommand(int commandType, CMsgStructBase *msgData)
{
    qDebug() << "CClientBusiness::exeCommand"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if(!msgData)
        return false;
    QByteArray data = msgData->data(commandType);
    m_communication->ParsePacket(commandType, data.data(), data.size());
    return true;
}

CCanport *CClientBusiness::currentPanel() const
{
    qDebug() << "CClientBusiness::currentPanel"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    CCanport* canport = NULL;
    if(CGlobal::instance()->dm()->isLoad())
        canport = CGlobal::instance()->dm()->controllerAt(0)->canportById(CGlobal::instance()->panelAddress());
    return canport;
}

//主电故障应急启动
void CClientBusiness::mainPowerStart()
{
    qDebug() << "CClientBusiness::mainPowerStart"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    CMsgNull m_MsgNUll;
    exeCommand(NCT_CancelMute, &m_MsgNUll);
    CGlobal::instance()->DataProcessing()->setSoundIcon(true);
    CGlobal::instance()->m_isEmergency = true;
    if(CGlobal::instance()->m_EmergencyOperation == CGlobal::Start)
        return;
    CGlobal::instance()->m_EmergencyOperation = CGlobal::Start;
    exeCommand(NCT_EmergencyStart, &m_MsgNUll);
    Emergency();
    if(CGlobal::instance()->m_tcptype & 0X01)
    {
        //上传手动应急到云平台
        XmlManualLaunchUpload();
    }
//    else if(CGlobal::instance()->m_tcptype & 0X02)
//    {
//        //服务端上传手动应急
//        serverManualLaunchUpload();
//    }
}

//主电故障应急停止
void CClientBusiness::mainPowerStop()
{
    qDebug() << "CClientBusiness::mainPowerStop"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    CMsgNull m_MsgNUll;
    if(exeCommand(NCT_EmergencyStop, &m_MsgNUll))
    {
        CGlobal::instance()->m_EmergencyOperation = CGlobal::Stop;
    }
    else
    {
        CGlobal::instance()->m_HaveDialog = true;
        QFont font;
        font.setPointSize(12); // 设置字体大小为12像素
        // 在QMessageBox中设置字体
        QMessageBox messageBox;
        messageBox.setFont(font);
        QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
        messageBox.setStyleSheet(styleSheet);
        messageBox.setIcon(QMessageBox::NoIcon);
        messageBox.setWindowTitle("信息");
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.button(QMessageBox::Ok)->setText("确定");
        QString log;
        log = "系统应急停止失败！";
        messageBox.setText(log);
        messageBox.exec();
        CGlobal::instance()->m_HaveDialog = false;
    }
}

//对比检查数据库数据是否在线
void CClientBusiness::slotCheckLoginInfo()
{
    qDebug() << "CClientBusiness::slotCheckLoginInfo"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    deviceForbidFault df;
    df.deviceForbidFile();//读取灯具禁用表
    CGlobal::instance()->DlgDevices()->getDeviceInfo();
    ConfigFileManager cfg;
    cfg.ParseConfigFile();//读取注册文件
    CGlobal::instance()->DataProcessing()->slotcheckLoginInfo();
}

//带电池灯具状态
void CClientBusiness::DeviceBatteryStatus(CDevice* device, int status, int power)
{
    qDebug() << "CClientBusiness::DeviceBatteryStatus"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if(device && device->deviceTypeId() < 100){
        QString batteryStatus;
        QString batteryPower;
        if(device->isDeviceForbid())
        {
            batteryStatus = "正常";
        }
        else
        {
            if(status == 0x01)
            {
                batteryStatus = "电池断线";
                device->setStatus(OBJS_DeviceBatteryOffFault,1);
                if(device->getStatus(OBJS_DeviceBatteryShortFault))
                    device->setStatus(OBJS_DeviceBatteryShortFault,0);
            }
            else if(status == 0x02)
            {
                batteryStatus = "电池短路";
                device->setStatus(OBJS_DeviceBatteryShortFault,1);
                if(device->getStatus(OBJS_DeviceBatteryOffFault))
                    device->setStatus(OBJS_DeviceBatteryOffFault,0);
            }
            else
            {
                if(device->getStatus(OBJS_DeviceBatteryOffFault))
                    device->setStatus(OBJS_DeviceBatteryOffFault,0);
                if(device->getStatus(OBJS_DeviceBatteryShortFault))
                    device->setStatus(OBJS_DeviceBatteryShortFault,0);
                if(status == 0)
                    batteryStatus = "正常";
                else if(status == 0x04)
                    batteryStatus = "电池放电";
                else if(status == 0x08)
                    batteryStatus = "电池充电";
            }
        }



        if(power == 0x01)
        {
            if(!device->isDeviceForbid())
            {
                batteryPower = "电池过放电";
                device->setStatus(OBJS_DeviceBatteryOverdischargeFault,1);
            }
            else
            {
                batteryPower = "正常";
            }
        }
        else
        {
            if(device->getStatus(OBJS_DeviceBatteryOverdischargeFault))
                device->setStatus(OBJS_DeviceBatteryOverdischargeFault,0);
            if(power == 0)
                batteryPower = "正常";
            else if(power == 0x02)
                batteryPower = "%25";
            else if(power == 0x04)
                batteryPower = "%50";
            else if(power == 0x08)
                batteryPower = "%75";
        }

        device->setDeviceValue(DEVICE_VALUE_BATTERYSTATUS,batteryStatus);
        device->setDeviceValue(DEVICE_VALUE_BATTERYPOWER,batteryPower);
        CGlobal::instance()->DlgDevices()->getDeviceInfo();
    }
}

//上传灯具状态到云平台
void CClientBusiness::uploadLampStatus(CDevice* device)
{
    qDebug() << "CClientBusiness::uploadLampStatus"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((CGlobal::instance()->m_tcptype & 0x03) == 0)
        return;
    bool isCommunicationOK = true,isLightFault = false,isWarning = false;
    if(!device->isDeviceOnline())
        isCommunicationOK = false;
    if(device->deviceValue(DEVICE_VALUE_LIGHT) == "光源故障")
        isLightFault = true;
    if(device->deviceValue(DEVICE_VALUE_EMERGENCY) == "应急" || CGlobal::instance()->m_isEmergency)
        isWarning = true;
    XmlLampStateUpload(CGlobal::instance()->saveKeyId(OBJT_Device,device->keyId()),isCommunicationOK,isLightFault,isWarning);
    serverLampStateUpload(CGlobal::instance()->saveKeyId(OBJT_Device,device->keyId()),isCommunicationOK,isLightFault,isWarning);
}

//生成设备唯一码
QString CClientBusiness::generateUniqueID()
{
    qDebug() << "CClientBusiness::generateUniqueID"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    bool isUniqueID = true;
    QString filePath = "/root/uniquecode.txt"; // 将文件路径替换为实际的文件路径
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        file.close();

        // 检查文件中的唯一码是否为空
        QString uniqueCode = QString::fromUtf8(data);
        if(uniqueCode.isEmpty())
            isUniqueID = false;
        else
            return uniqueCode;
    }
    else
    {
        // 文件不存在或无法打开
        isUniqueID = false;
    }
    if(!isUniqueID)
    {
        // 执行QUuid::createUuid()方法生成唯一码
        QUuid uuid = QUuid::createUuid();
        QString uniqueCode = uuid.toString();
        // 将唯一码保存到文件中
        QString filePath = "/root/uniquecode.txt"; // 将文件路径替换为实际的文件路径
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out << uniqueCode;
            file.close();
            qDebug() << "Data saved to file: " << filePath;
            isUniqueID = true;
            return uniqueCode;
        }
        else
        {
            qDebug() << "Failed to open file for writing: " << filePath;
        }
    }
    return NULL;
}


//////////wait for deal, deal msg from libBService
void CClientBusiness::recvData(int nMsgType, QByteArray &data)
{
    Q_UNUSED(nMsgType);
    QDataStream stream(&data, QIODevice::ReadOnly);
    CMsgStructBase* msgStruct = NULL;
    int type = NCT_None;
    stream>>type>>&msgStruct;
    QList<QVariant> param;
    qDebug() << "CClientBusiness::recvData, msg type is " << type << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if(!msgStruct)
        return;
    switch(type)
    {
        case NCT_ObjectStatus:
        {
            //////////////
            /////////2:distribution login
            /////////3: data
            /////////4:device login
            /////////5: status
            CMsgObjectStatus* msgObjectStatus = static_cast<CMsgObjectStatus*>(msgStruct);
            qDebug() << "CClientBusiness::recvData, NCT_ObjectStatus " << msgObjectStatus->nType  << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
            if(!CGlobal::instance()->dm()->isLoad()) return;
            CController* controller = CGlobal::instance()->controller();
            if(!controller)
                return;
            if(msgObjectStatus->nType == 0x01)
            {
                controller->setStatus(msgObjectStatus->nStatusID, msgObjectStatus->nValue);
            }
            else if(msgObjectStatus->nType == 0x02)
            {
                CCanport* canport = controller->canportByAddress(msgObjectStatus->nCanportAdd);
                if(!canport)
                    return;
                if(msgObjectStatus->strText != "")
                {
                    canport->setCanportValue(OBJECT_VALUE_ONLINE,1);
//                    CDistribution* distribution = canport->distributionByProductId(msgObjectStatus->strText);
                    CDistribution* distribution = canport->distributionByAddress(msgObjectStatus->nDisID);
                    if(!distribution)
                        distribution = createUndefinedDistribution(canport, msgObjectStatus->nDisID, msgObjectStatus->strText);
                    if(distribution)
                    {
                        if(distribution->getStatus(OBJS_DistributionCommunicationFault) == 1)
                        {
                            distribution->setloopCommunication(0);
                            distribution->setStatus(OBJS_DistributionCommunicationFault, 0);
                            XmlCanDeviceStateUpload(distribution);
                        }
                        distribution->setDistributionOnline(true);
                        distribution->setStatus(OBJS_Online, 1);
                        distribution->setDistributionValue(DISTRIBUTION_VALUE_ADDR,msgObjectStatus->nDisID);
                        if(msgObjectStatus->nValue >= 252 && msgObjectStatus->nValue < 255)
                            distribution->setDistributionValue(DISTRIBUTION_VALUE_NAME,"B型集中电源");
                        else if(msgObjectStatus->nValue >= 240 && msgObjectStatus->nValue < 250)
                            distribution->setDistributionValue(DISTRIBUTION_VALUE_NAME,"配电箱");
                        else if(msgObjectStatus->nValue >= 200 && msgObjectStatus->nValue < 240)
                            distribution->setDistributionValue(DISTRIBUTION_VALUE_NAME,"A型集中电源");
                        else if(msgObjectStatus->nValue >= 110 && msgObjectStatus->nValue < 130)
                            distribution->setDistributionValue(DISTRIBUTION_VALUE_NAME,"集中电源(MA)");
                        distribution->setDistributionValue(DISTRIBUTION_VALUE_CANPORTADDR, canport->canportAdd());
                        distribution->setDistributionValue(DISTRIBUTION_VALUE_TYPEID,msgObjectStatus->nValue);
                        distribution->setDistributionValue(DISTRIBUTION_VALUE_DESCRIPTION,msgObjectStatus->strText);
                        distribution->setDistributionValue(DISTRIBUTION_VALUE_NAMEDETAIL,tr("地址:%1 %2").arg(distribution->distributionValue(DISTRIBUTION_VALUE_ADDR).toInt())
                                               .arg(distribution->distributionValue(DISTRIBUTION_VALUE_NAME).toString()));
//                        distribution->setDistributionValue(DISTRIBUTION_VALUE_NOLOGIN, 0);
                        distribution->setDistributionLogin(true);
                        CLoop* loop;
                        for(int i=1; i<9; i++){
                            loop = distribution->loopByAdd(i);
                            if(!loop)
                            {
                                loop = createUndefinedLoop(distribution, i);
                                if(!loop)
                                    continue;
                                loop->setloopValue(LOOP_VALUE_ADDR,i);
                                loop->setloopValue(LOOP_VALUE_CANPORTADDR,canport->canportAdd());
                                loop->setLoopOnline(true);
//                                loop->setDeviceOnlineTotal(loop->getDeviceOnlineTotalBak());
                                loop->setLoopLogin(true);
                            }
                        }
                        //注册到的集中电源加入到注册表中,2s后保存
                        if(!CGlobal::instance()->m_bInLogin)
                        {
                            m_saveLogintimer->start(2000);
                        }
                    }
                }
            }
            else if(msgObjectStatus->nType == 0x03)
            {
                QByteArray byte;
                bool ok;
                CCanport* canport = controller->canportByAddress(msgObjectStatus->nCanportAdd);
                if(!canport)
                    return;
                if(msgObjectStatus->strdata != ""){
                    for (int i = 0; i < msgObjectStatus->strdata.size(); i += 2)
                    {
                        byte.append((char)msgObjectStatus->strdata.mid(i, 2).toInt(&ok, 16));
                    }
                    if(byte.count() < 8)
                        return;
                    int type = msgObjectStatus->strdata.mid(12, 2).toInt(&ok, 16);
                    int distributionAddress = msgObjectStatus->strdata.mid(2, 2).toInt(&ok, 16);
                    int loopAddress = msgObjectStatus->strdata.mid(4, 2).toInt(&ok, 16);
                    int deviceAddress = msgObjectStatus->strdata.mid(6, 2).toInt(&ok, 16);
                    if(type == 0x01){//读运行参数
                        CDistribution* distribution = canport->distributionByAddress(distributionAddress);
                        if(distribution){
                            if(loopAddress == 0 && deviceAddress == 0)
                            {
                                if(msgObjectStatus->strdata.mid(8, 2).toInt(&ok, 16) == 0x01 && byte.count() == 13)
                                {
                                    distribution->setDistributionValue(DISTRIBUTION_VALUE_MAINV,
                                                           (double(msgObjectStatus->strdata.mid(16, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16)))/10);
                                    distribution->setDistributionValue(DISTRIBUTION_VALUE_OUTV,
                                                           (double(msgObjectStatus->strdata.mid(20, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(18, 2).toInt(&ok, 16)))/10);
                                    distribution->setDistributionValue(DISTRIBUTION_VALUE_OUTA,
                                                           (double(msgObjectStatus->strdata.mid(24, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(22, 2).toInt(&ok, 16)))/100);
                                }
                                else if(msgObjectStatus->strdata.mid(8, 2).toInt(&ok, 16) == 0x02 && byte.count() == 13)
                                {
                                    distribution->setDistributionValue(DISTRIBUTION_VALUE_EXTERNALV,(double(msgObjectStatus->strdata.mid(16, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16)))/10);
                                    distribution->setDistributionValue(DISTRIBUTION_VALUE_BAT1V,(double(msgObjectStatus->strdata.mid(20, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(18, 2).toInt(&ok, 16)))/10);
                                    distribution->setDistributionValue(DISTRIBUTION_VALUE_BAT2V,(double(msgObjectStatus->strdata.mid(24, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(22, 2).toInt(&ok, 16)))/10);
                                }
                                else if(msgObjectStatus->strdata.mid(8, 2).toInt(&ok, 16) == 0x03 && byte.count() == 13)
                                {
                                    distribution->setDistributionValue(DISTRIBUTION_VALUE_BAT3V,(double(msgObjectStatus->strdata.mid(16, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16)))/10);
                                    distribution->setDistributionValue(DISTRIBUTION_VALUE_BAT1T,double(msgObjectStatus->strdata.mid(20, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(18, 2).toInt(&ok, 16)));
                                    distribution->setDistributionValue(DISTRIBUTION_VALUE_BAT2T,double(msgObjectStatus->strdata.mid(24, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(22, 2).toInt(&ok, 16)));
                                }
                                else if(msgObjectStatus->strdata.mid(8, 2).toInt(&ok, 16) == 0x04 && byte.count() == 13)
                                {
                                    distribution->setDistributionValue(DISTRIBUTION_VALUE_BAT3T,double(msgObjectStatus->strdata.mid(16, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16)));
                                    CLoop* loop = distribution->loopByAdd(1);
                                    if(loop){
                                        loop->setloopValue(LOOP_VALUE_V,(double(msgObjectStatus->strdata.mid(20, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(18, 2).toInt(&ok, 16)))/10);
                                        loop->setloopValue(LOOP_VALUE_A,(double(msgObjectStatus->strdata.mid(24, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(22, 2).toInt(&ok, 16)))/100);
                                    }
                                }
                                else if(msgObjectStatus->strdata.mid(8, 2).toInt(&ok, 16) == 0x05 && byte.count() == 13)
                                {
                                    CLoop* loop = distribution->loopByAdd(2);
                                    if(loop){
                                        loop->setloopValue(LOOP_VALUE_V,(double(msgObjectStatus->strdata.mid(16, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16)))/10);
                                        loop->setloopValue(LOOP_VALUE_A,(double(msgObjectStatus->strdata.mid(20, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(18, 2).toInt(&ok, 16)))/100);
                                    }
                                    loop = distribution->loopByAdd(3);
                                    if(loop)
                                        loop->setloopValue(LOOP_VALUE_V,(double(msgObjectStatus->strdata.mid(24, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(22, 2).toInt(&ok, 16)))/10);
                                }
                                else if(msgObjectStatus->strdata.mid(8, 2).toInt(&ok, 16) == 0x06 && byte.count() == 13)
                                {
                                    CLoop* loop = distribution->loopByAdd(3);
                                    if(loop){
                                        loop->setloopValue(LOOP_VALUE_A,(double(msgObjectStatus->strdata.mid(16, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16)))/100);
                                    }
                                    loop = distribution->loopByAdd(4);
                                    if(loop)
                                    {
                                        loop->setloopValue(LOOP_VALUE_V,(double(msgObjectStatus->strdata.mid(20, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(18, 2).toInt(&ok, 16)))/10);
                                        loop->setloopValue(LOOP_VALUE_A,(double(msgObjectStatus->strdata.mid(24, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(22, 2).toInt(&ok, 16)))/100);
                                    }
                                }
                                else if(msgObjectStatus->strdata.mid(8, 2).toInt(&ok, 16) == 0x07 && byte.count() == 13)
                                {
                                    CLoop* loop = distribution->loopByAdd(5);
                                    if(loop){
                                        loop->setloopValue(LOOP_VALUE_V,(double(msgObjectStatus->strdata.mid(16, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16)))/10);
                                        loop->setloopValue(LOOP_VALUE_A,(double(msgObjectStatus->strdata.mid(20, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(18, 2).toInt(&ok, 16)))/100);
                                    }
                                    loop = distribution->loopByAdd(6);
                                    if(loop)
                                        loop->setloopValue(LOOP_VALUE_V,(double(msgObjectStatus->strdata.mid(24, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(22, 2).toInt(&ok, 16)))/10);
                                }
                                else if(msgObjectStatus->strdata.mid(8, 2).toInt(&ok, 16) == 0x08 && byte.count() == 13)
                                {
                                    CLoop* loop = distribution->loopByAdd(6);
                                    if(loop){
                                        loop->setloopValue(LOOP_VALUE_A,(double(msgObjectStatus->strdata.mid(16, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16)))/100);
                                    }
                                    loop = distribution->loopByAdd(7);
                                    if(loop)
                                    {
                                        loop->setloopValue(LOOP_VALUE_V,(double(msgObjectStatus->strdata.mid(20, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(18, 2).toInt(&ok, 16)))/10);
                                        loop->setloopValue(LOOP_VALUE_A,(double(msgObjectStatus->strdata.mid(24, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(22, 2).toInt(&ok, 16)))/100);
                                    }
                                }
                                else if(msgObjectStatus->strdata.mid(8, 2).toInt(&ok, 16) == 0x09 && byte.count() == 13)
                                {
                                    CLoop* loop = distribution->loopByAdd(8);
                                    if(loop){
                                        loop->setloopValue(LOOP_VALUE_V,(double(msgObjectStatus->strdata.mid(16, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16)))/10);
                                        loop->setloopValue(LOOP_VALUE_A,(double(msgObjectStatus->strdata.mid(20, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(18, 2).toInt(&ok, 16)))/100);
                                    }
                                    loop = distribution->loopByAdd(9);
                                    if(loop)
                                        loop->setloopValue(LOOP_VALUE_V,(double(msgObjectStatus->strdata.mid(24, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(22, 2).toInt(&ok, 16)))/10);
                                }
                                else if(msgObjectStatus->strdata.mid(8, 2).toInt(&ok, 16) == 0x0a && byte.count() == 13)
                                {
                                    CLoop* loop = distribution->loopByAdd(9);
                                    if(loop){
                                        loop->setloopValue(LOOP_VALUE_A,(double(msgObjectStatus->strdata.mid(16, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16)))/100);
                                    }
                                    loop = distribution->loopByAdd(10);
                                    if(loop)
                                    {
                                        loop->setloopValue(LOOP_VALUE_V,(double(msgObjectStatus->strdata.mid(20, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(18, 2).toInt(&ok, 16)))/10);
                                        loop->setloopValue(LOOP_VALUE_A,(double(msgObjectStatus->strdata.mid(24, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(22, 2).toInt(&ok, 16)))/100);
                                    }
                                }
                                else if(msgObjectStatus->strdata.mid(8, 2).toInt(&ok, 16) == 0x0b && byte.count() == 13)
                                {
                                    CLoop* loop = distribution->loopByAdd(11);
                                    if(loop){
                                        loop->setloopValue(LOOP_VALUE_V,(double(msgObjectStatus->strdata.mid(16, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16)))/10);
                                        loop->setloopValue(LOOP_VALUE_A,(double(msgObjectStatus->strdata.mid(20, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(18, 2).toInt(&ok, 16)))/100);
                                    }
                                    loop = distribution->loopByAdd(12);
                                    if(loop)
                                        loop->setloopValue(LOOP_VALUE_V,(double(msgObjectStatus->strdata.mid(24, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(22, 2).toInt(&ok, 16)))/10);
                                }
                                else if(msgObjectStatus->strdata.mid(8, 2).toInt(&ok, 16) == 0x0c && byte.count() == 11)
                                {
                                    CLoop* loop = distribution->loopByAdd(12);
                                    if(loop){
                                        loop->setloopValue(LOOP_VALUE_A,(double(msgObjectStatus->strdata.mid(16, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16)))/100);
                                    }
                                    distribution->setDistributionValue(DISTRIBUTION_VALUE_BATV,(double(msgObjectStatus->strdata.mid(20, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(18, 2).toInt(&ok, 16)))/10);
                                }
                                else if(msgObjectStatus->strdata.mid(8, 2).toInt(&ok, 16) == 0x3f && byte.count() == 8)
                                {
                                    //判断客户端是否连接
//                                    if(CGlobal::instance()->TimeInterval()->isMasterConnect())
//                                    {
                                        //服务端回复集中电源电参量信息
//                                        serverDeviceRealtimeData(distribution);
//                                    }

                                }
                            }
                            else if(loopAddress != 0 && deviceAddress != 0)
                            {
                                if(byte.count() < 13)
                                    return;
                                    CLoop* loop = distribution->loopByAdd(loopAddress);
                                    if(loop){
                                        CDevice* device = loop->deviceByAdd(deviceAddress);
                                        if(device)
                                            DeviceBatteryStatus(device,msgObjectStatus->strdata.mid(22, 2).toInt(&ok, 16),msgObjectStatus->strdata.mid(24, 2).toInt(&ok, 16));
                                    }
                            }
                        }
                    }else if(type == 0x02){//读保护参数
                        CDistribution* distribution = canport->distributionByAddress(distributionAddress);
                        if(distribution){
                            if(byte.count() < 14)
                                return;
                            distribution->setoutA(msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16));
                            distribution->setlightswitch(msgObjectStatus->strdata.mid(16, 2).toInt(&ok, 16));
                            distribution->setCANfaultswitch(msgObjectStatus->strdata.mid(18, 2).toInt(&ok, 16));
                            distribution->setlinkageswitch(msgObjectStatus->strdata.mid(20, 2).toInt(&ok, 16));
                            distribution->setpowermode(msgObjectStatus->strdata.mid(22, 2).toInt(&ok, 16));
                            distribution->setfiretime(msgObjectStatus->strdata.mid(24, 2).toInt(&ok, 16));
                            distribution->setunfiretime(msgObjectStatus->strdata.mid(26, 2).toInt(&ok, 16));
                        }
                    }else if(type == 0x08){//读设备信息
                        if(distributionAddress == 0x3f)
                        {
                            if(byte.count() < 10)
                                return;
                            int softwareNumber = msgObjectStatus->strdata.mid(16, 2).toInt(&ok, 16)*256 + msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16);
                            double softwareVersion = msgObjectStatus->strdata.mid(18, 2).toInt(&ok, 16)/100.0;
                            if(msgObjectStatus->nCanportAdd == 3 || msgObjectStatus->nCanportAdd == 4)
                            {
                                CGlobal::instance()->m_version_can1can2 = QString::number(softwareNumber) + " V" + QString::number(softwareVersion, 'f', 2);;
                            }
                            else if(msgObjectStatus->nCanportAdd == 5 || msgObjectStatus->nCanportAdd == 6)
                            {
                                CGlobal::instance()->m_version_can3can4 = QString::number(softwareNumber) + " V" + QString::number(softwareVersion, 'f', 2);;
                            }
                            else if(msgObjectStatus->nCanportAdd == 7 || msgObjectStatus->nCanportAdd == 8)
                            {
                                CGlobal::instance()->m_version_can5can6 = QString::number(softwareNumber) + " V" + QString::number(softwareVersion, 'f', 2);;
                            }
                            else if(msgObjectStatus->nCanportAdd == 9 || msgObjectStatus->nCanportAdd == 10)
                            {
                                CGlobal::instance()->m_version_can7can8 = QString::number(softwareNumber) + " V" + QString::number(softwareVersion, 'f', 2);;
                            }
                            CGlobal::instance()->TimeInterval()->updateVersion();
                        }
                        else if(loopAddress == 0 && deviceAddress == 0)
                        {
                            CDistribution* distribution = canport->distributionByAddress(distributionAddress);
                            if(!distribution)
                                return;
                            if(msgObjectStatus->strdata.mid(8, 2).toInt(&ok, 16) == 0x01)
                            {
                                if(byte.count() < 14)
                                    return;
                                distribution->setDistributionValue(DISTRIBUTION_VALUE_CPUVERSION,msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16));
                                distribution->setDistributionValue(DISTRIBUTION_VALUE_POWVERSION,msgObjectStatus->strdata.mid(16, 2).toInt(&ok, 16));
                                distribution->setDistributionValue(DISTRIBUTION_VALUE_LCDVERSION,msgObjectStatus->strdata.mid(18, 2).toInt(&ok, 16));
                                distribution->setDistributionValue(DISTRIBUTION_VALUE_ABUS1VERSION,msgObjectStatus->strdata.mid(20, 2).toInt(&ok, 16));
                                distribution->setDistributionValue(DISTRIBUTION_VALUE_ABUS2VERSION,msgObjectStatus->strdata.mid(22, 2).toInt(&ok, 16));
                                distribution->setDistributionValue(DISTRIBUTION_VALUE_ABUS3VERSION,msgObjectStatus->strdata.mid(24, 2).toInt(&ok, 16));
                                distribution->setDistributionValue(DISTRIBUTION_VALUE_ABUS4VERSION,msgObjectStatus->strdata.mid(26, 2).toInt(&ok, 16));
                            }
                            else if(msgObjectStatus->strdata.mid(8, 2).toInt(&ok, 16) == 0x02)
                            {
                                if(byte.count() < 14)
                                    return;
                                distribution->setDistributionValue(DISTRIBUTION_VALUE_ABUS5VERSION,msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16));
                                distribution->setDistributionValue(DISTRIBUTION_VALUE_ABUS6VERSION,msgObjectStatus->strdata.mid(16, 2).toInt(&ok, 16));
                                distribution->setDistributionValue(DISTRIBUTION_VALUE_ABUS7VERSION,msgObjectStatus->strdata.mid(18, 2).toInt(&ok, 16));
                                distribution->setDistributionValue(DISTRIBUTION_VALUE_ABUS8VERSION,msgObjectStatus->strdata.mid(20, 2).toInt(&ok, 16));
                                distribution->setDistributionValue(DISTRIBUTION_VALUE_ABUS9VERSION,msgObjectStatus->strdata.mid(22, 2).toInt(&ok, 16));
                                distribution->setDistributionValue(DISTRIBUTION_VALUE_ABUS10VERSION,msgObjectStatus->strdata.mid(24, 2).toInt(&ok, 16));
                                distribution->setDistributionValue(DISTRIBUTION_VALUE_ABUS11VERSION,msgObjectStatus->strdata.mid(26, 2).toInt(&ok, 16));
                            }
                            else if(msgObjectStatus->strdata.mid(8, 2).toInt(&ok, 16) == 0x3f)
                            {
                                if(byte.count() < 8)
                                    return;
                                distribution->setDistributionValue(DISTRIBUTION_VALUE_ABUS12VERSION,msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16));
//                                //判断客户端是否连接
//                                if(CGlobal::instance()->TimeInterval()->isMasterConnect())
//                                {
//                                    //服务端回复集中电源软件版本信息
//                                    serverDeviceSoftwareInfo(distribution);
//                                }
                            }
                        }
                        else
                        {
                            if(byte.count() < 11)
                                return;
                            CDistribution* distribution = canport->distributionByAddress(distributionAddress);
                            if(!distribution)
                                return;
                            CLoop* loop = distribution->loopByAdd(loopAddress);
                            if(!loop)
                                return;
                            CDevice* device = loop->deviceByAdd(deviceAddress);
                            if(!device)
                                return;
                            device->setDeviceValue(DEVICE_VALUE_SOFTWARENUMBER,msgObjectStatus->strdata.mid(16, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16));
                            device->setDeviceValue(DEVICE_VALUE_SOFTWAREVERSION,msgObjectStatus->strdata.mid(18, 2).toInt(&ok, 16));
                            device->setDeviceValue(DEVICE_VALUE_TYPE_ID,msgObjectStatus->strdata.mid(20, 2).toInt(&ok, 16));
//                            //服务端回复灯具软件版本信息
//                            serverLampSoftwareInfo(CGlobal::instance()->saveKeyId(OBJT_Device,device->keyId()),
//                                                   msgObjectStatus->strdata.mid(16, 2).toInt(&ok, 16)*256+msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16),msgObjectStatus->strdata.mid(18, 2).toInt(&ok, 16));
                        }
                    }else if(type == 0x11){
                        CDistribution* distribution = canport->distributionByAddress(distributionAddress);
                        if(distribution){
                            if(loopAddress == 0 && deviceAddress == 0){

                            }else{
                                CLoop* loop = distribution->loopByAdd(loopAddress);
                                if(loop){
                                    if(byte.count() < 10)
                                        return;
                                    CDevice* device = loop->deviceByAdd(deviceAddress);
                                    if(device)
                                        DeviceBatteryStatus(device,msgObjectStatus->strdata.mid(16, 2).toInt(&ok, 16),msgObjectStatus->strdata.mid(18, 2).toInt(&ok, 16));
                                }
                            }
                        }
                    }
                    else if(type == 0x77){
                        if(byte.count() < 10)
                            return;
                        CMsgObjectStatus msg;
                        msg.nDisID = distributionAddress;
                        msg.nLoopID = 0;
                        msg.nCanportAdd = canport->canportAdd();
                        if(msg.nDisID == 0){
                            return;
                        }
                        if(!exeCommand(NCT_LoginInfoLoop, &msg))
                        {
                            CGlobal::instance()->m_HaveDialog = true;
                            QFont font;
                            font.setPointSize(12); // 设置字体大小为12像素
                            // 在QMessageBox中设置字体
                            QMessageBox messageBox;
                            messageBox.setFont(font);
                            QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
                            messageBox.setStyleSheet(styleSheet);
                            messageBox.setIcon(QMessageBox::NoIcon);
                            messageBox.setWindowTitle("信息");
                            messageBox.setStandardButtons(QMessageBox::Ok);
                            messageBox.button(QMessageBox::Ok)->setText("确定");
                            QString log;
                            log = "注册失败！";
                            messageBox.setText(log);
                            messageBox.exec();
                            CGlobal::instance()->m_HaveDialog = false;
                        }
                        else
                        {
                            CGlobal::instance()->m_bInLogin = true;
                            CGlobal::instance()->InformationWindow()->setMovie(":/images/allregedit.png");
                            CGlobal::instance()->InformationWindow()->setDescription(QString("<h1><font color=black>%1</font></h1>").arg("系统注册......!"));
                            CGlobal::instance()->InformationWindow()->show();
                            CGlobal::instance()->InformationWindow()->hideCloseButton();
                            CGlobal::instance()->setMSgColor(QColor(12, 180, 3));
                            CGlobal::instance()->setMsgText("系统注册......!");
                        }
                    }
                    else if(type == 0x78){
                        if(byte.count() < 8)
                            return;
                        CDistribution* distribution = canport->distributionByAddress(distributionAddress);
                        if(distribution)
                            distribution->setloopOutStatus(msgObjectStatus->strdata.mid(14, 2).toInt(&ok, 16));
                    }
                }
            }
            else if(msgObjectStatus->nType == 0x04)
            {  //注册数据
                CCanport* canport = controller->canportByAddress(msgObjectStatus->nCanportAdd);
                if(!canport)
                    return;
                qDebug() << msgObjectStatus->strText << "=======================";
                CDistribution* distribution = canport->distributionByAddress(msgObjectStatus->nDisID);
                if(!distribution)
                {
                    distribution = createUndefinedDistribution(canport, msgObjectStatus->nDisID);
                    if(distribution)
                        distribution->setDistributionValue(DISTRIBUTION_VALUE_CANPORTADDR, canport->canportAdd());
                }
                if(distribution)
                {
                    if(!distribution->isDistributionOnline())
                    {
                        distribution->setDistributionOnline(true);
                        distribution->setStatus(OBJS_Online,1);
                    }
                    CLoop* loop;
                    for(int i=1; i<9; i++){
                        loop = distribution->loopByAdd(i);
                        if(!loop)
                        {
                            loop = createUndefinedLoop(distribution, i);
                            if(!loop)
                                continue;
                            loop->setloopValue(LOOP_VALUE_ADDR,i);
                            loop->setloopValue(LOOP_VALUE_CANPORTADDR,canport->canportAdd());
                            loop->setLoopOnline(true);
//                            loop->setDeviceOnlineTotal(loop->getDeviceOnlineTotalBak());
                            loop->setLoopLogin(true);
                        }
                    }
                    if(distribution->getStatus(OBJS_DistributionCommunicationFault) == 1)
                    {
                        distribution->setloopCommunication(0);
                        distribution->setStatus(OBJS_DistributionCommunicationFault, 0);
                        XmlCanDeviceStateUpload(distribution);
                    }
                    loop = distribution->loopByAdd(msgObjectStatus->nLoopID);
                    if(!loop)
                        return;

                    CDevice* device;
                    if(loop && msgObjectStatus->strText != ""){
                        device = loop->deviceByProductionId(msgObjectStatus->strText);
                        if(!device)
                        {
                            device = createUndefinedDevice(loop);
                            if(device) {
                                device->setDeviceLogin(true);
                                device->setDatabaseLogin(true);

                                device->setStatus(msgObjectStatus->nStatusID, msgObjectStatus->nValue);
                                device->setDeviceValue(DEVICE_VALUE_LOGINTYPE_ID, msgObjectStatus->nValue);
                                device->setDeviceValue(DEVICE_VALUE_DESCRIPTION, msgObjectStatus->strText);
                                device->setDeviceValue(DEVICE_VALUE_ADDR, msgObjectStatus->nDeviceID);
//                                device->setDeviceValue(DEVICE_VALUE_ISUSED, msgObjectStatus->nLoginStatus);
                                device->setDeviceValue(DEVICE_VALUE_LOOPADDR, loop->loopAdd());
                                device->setDeviceValue(DEVICE_VALUE_DISTRIBUTIONADDR, distribution->distributionAddress());
                                device->setDeviceValue(DEVICE_VALUE_CANPORTADDR, canport->canportAdd());
                                device->setDeviceValue(DEVICE_VALUE_EMERGENCY, "正常");
                                device->setDeviceValue(DEVICE_VALUE_LIGHT, "正常");
                                if(msgObjectStatus->nCommunicationStatus == OBJS_Online)
                                {
                                    device->setEnableNumberChange(true);
                                    device->setDeviceOnline(true);
                                }
//                                device->setDeviceValue(DEVICE_VALUE_NOLOGIN, 0);
                            }
                        }
                        else
                        {
                            device->setDeviceLogin(true);
                            device->setDatabaseLogin(true);
                            if(!device->isDeviceOnline() || msgObjectStatus->nCommunicationStatus == OBJS_Online)
                            {
                                device->setEnableNumberChange(true);
                                device->setDeviceOnline(true);
                            }
                            if(device->getStatus(OBJS_LightCommunicationFault))
                            {
                                device->setStatus(OBJS_LightCommunicationFault,0);
                            }
                            //设备类型改变，数据库中相关参数需要删除
                            if(device->deviceTypeId() != msgObjectStatus->nValue)
                            {
                                device->setDeviceValue(DEVICE_VALUE_LOGINTYPE_ID, msgObjectStatus->nValue);
//                                device->setDeviceValue(DEVICE_VALUE_TYPE_ID, msgObjectStatus->nValue);
//                                QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
//                                QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");;
//                                db.setDatabaseName(fileName);
//                                if(!db.open())
//                                    return;
//                                QSqlQuery query(db);

//                                int id = CGlobal::instance()->saveKeyId(OBJT_Device,device->keyId());
//                                query.exec(QString("DELETE FROM EvacuationPathItem WHERE LampInfoID=%1").arg(id));
//                                query.exec(QString("DELETE FROM LampConnection WHERE StartLampID=%1").arg(id));
//                                query.exec(QString("DELETE FROM LampConnection WHERE EndLampID=%1").arg(id));
//                                query.exec(QString("DELETE FROM LampDefaultDirection WHERE LampInfoID=%1").arg(id));
//                                query.exec(QString("DELETE FROM LampToFirePoint WHERE LampInfoID=%1").arg(id));
//                                query.exec(QString("DELETE FROM LampToLaunchGroup WHERE LampInfoID=%1").arg(id));
//                                if(!query.exec())
//                                {
//                                    qDebug() << query.lastError();
//                                }
//                                db.close();
                            }

                            device->setDeviceValue(DEVICE_VALUE_ADDR, msgObjectStatus->nDeviceID);
                            device->setDeviceValue(DEVICE_VALUE_CHANGETYPE,0);
                        }
                        //注册到的灯具加入到注册表中,2s后保存
                        if(!CGlobal::instance()->m_bInLogin)
                        {
                            m_saveLogintimer->start(2000);
                        }
                    }
                    //该回路注册到灯具
                    loop->setLoopOnline(true);
                    loop->setLoopLogin(true);
                }
            }
            else if(msgObjectStatus->nType == 0x05)
            {   //状态判断数据
                if(msgObjectStatus->nStatusID == OBJS_FireEmergency)
                {
                    bool isFireEmergency = msgObjectStatus->nValue;
                    CFirePoint* firepoint = controller->firePointByAddress(msgObjectStatus->nDisID,
                                                                           msgObjectStatus->nLoopID,msgObjectStatus->nDeviceID);
                    if(firepoint)
                    {
                        controller->setStatus(OBJS_FireEmergency,isFireEmergency);
                        CMsgNull m_MsgNUll;
                        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_CancelMute, &m_MsgNUll);
                        CGlobal::instance()->DataProcessing()->setSoundIcon(true);
                        if(CGlobal::instance()->m_emergencyOnlyDistribution)
                        {
                            CGlobal::instance()->ClientBusiness()->firePointEmergencyByDistribution(firepoint->firePointId());
                        }
                        else
                        {
                            if(CGlobal::instance()->m_nStartRule == RULE_OnlyGroup)
                                CGlobal::instance()->ClientBusiness()->firePointEmergencyByGroup(firepoint->firePointId());
                            else if(CGlobal::instance()->m_nStartRule == RULE_OnlyPage)
                                CGlobal::instance()->ClientBusiness()->firePointEmergencyByLayoutPage(firepoint->firePointId());
                            else
                            {
                                CMsgObjectStatus msgObjectStatus;
                                msgObjectStatus.nValue = isFireEmergency;
                                CGlobal::instance()->ClientBusiness()->exeCommand(NCT_Emergency, &msgObjectStatus);
                            }
                        }
                        firepoint->setStatus(OBJS_FireEmergency,msgObjectStatus->nValue);
                        firepoint->setEmergency(isFireEmergency);
                    }
                    else if(!CGlobal::instance()->m_isShieldUnknownFirePoint)
                    {
                        CMsgObjectStatus msgObjectStatus;
                        msgObjectStatus.nValue = isFireEmergency;
                        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_Emergency, &msgObjectStatus);
                        controller->setStatus(OBJS_UnknownFireEmergency,isFireEmergency);
                    }
                }
                else
                {
                    if(msgObjectStatus->nDisID == 0)
                    {
                        QString batteryStatus;
                        if(CGlobal::instance()->m_isHideBatPowerFaultShow && (msgObjectStatus->nStatusID == OBJS_StandbyPowerOff ||
                           msgObjectStatus->nStatusID == OBJS_StandbyPowerShort || msgObjectStatus->nStatusID == OBJS_StandbyPowerUndervoltage))
                            return;
                        controller->setStatus(msgObjectStatus->nStatusID,msgObjectStatus->nValue);
                        if(msgObjectStatus->nStatusID == OBJS_MainPowerFault )
                        {
                            if(msgObjectStatus->nValue)
                            {
                                if(!CGlobal::instance()->m_isShieldMainPowerFaultEmergency)
                                {
                                    mainPowerStart();
                                    m_mainPowerStoptimer->start(10800000);
                                }
                            }
                            else
                            {
                                mainPowerStop();
                                CGlobal::instance()->TopToolBar()->resetSystem();
                                m_mainPowerStoptimer->stop();
                            }
                        }
                        if(controller->getStatus(OBJS_StandbyPowerOff))
                            batteryStatus = "断线";
                        else if(controller->getStatus(OBJS_StandbyPowerShort))
                            batteryStatus = "短路";
                        else if(controller->getStatus(OBJS_StandbyPowerUndervoltage))
                            batteryStatus = "欠压";
                        else
                            batteryStatus = "正常";
                        //主机状态上传到云平台
                        XmlHostStateUpload(!controller->getStatus(OBJS_MainPowerFault),batteryStatus);
//                        //服务端主机状态上传
//                        serverHostStateUpload(!controller->getStatus(OBJS_MainPowerFault),batteryStatus);
                    }
                    else
                    {
                        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(msgObjectStatus->nCanportAdd);
//                        if(!canport)
//                            canport = createUndefinedCanport(CGlobal::instance()->controller(), msgObjectStatus->nCanportAdd);
                        //状态上传前先读取一下注册表，配置CAN设备名称。只在第一次收到CAN设备时执行（防止刚开机时收到CAN设备状态时，CAN设备名称还未配置）
                        if(m_isCANconfig == false)
                        {
                            m_isCANconfig = true;
//                            slotCheckLoginInfo();
                        }
                        if(!canport)
                            return;
                        CDistribution* distribution = canport->distributionByAddress(msgObjectStatus->nDisID);
                        CLoop* loop;
                        if(distribution)
                        {
                            if(msgObjectStatus->nLoopID == 0 && msgObjectStatus->nDeviceID == 0)
                            {
                                //主电故障
                                if(msgObjectStatus->nStatusID == OBJS_MainPowerFault)
                                {
                                    distribution->setmainPowerFault(msgObjectStatus->nValue);
                                    //集中电源状态上传到云平台
                                    XmlCanDeviceStateUpload(distribution);
                                }
                                //备电断线
                                else if(msgObjectStatus->nStatusID == OBJS_BatteryOffFault)
                                {

                                }
                                //备电短路
                                else if(msgObjectStatus->nStatusID == OBJS_BatteryShortFault)
                                {

                                }
                                //集中电源备电故障
                                else if(msgObjectStatus->nStatusID == OBJS_StandbyPowerFault)
                                {
                                    distribution->setbackupPowerFault(msgObjectStatus->nValue);
                                    //集中电源状态上传到云平台
                                    XmlCanDeviceStateUpload(distribution);
                                }
                                //集中电源输出过载
                                else if(msgObjectStatus->nStatusID == OBJS_LoopOutOverloadFault)
                                {
                                    distribution->setoverOut(msgObjectStatus->nValue);
                                }
                                //集中电源输出开路
                                else if(msgObjectStatus->nStatusID == OBJS_PowerKaiLuFault)
                                {
                                    distribution->setoutOpen(msgObjectStatus->nValue);
                                }
                                //集中电源备电欠压（通用）
                                else if(msgObjectStatus->nStatusID == OBJS_BatteryLowVoltageFault)
                                {
                                    distribution->setbatUndervoltage(msgObjectStatus->nValue);
                                    //集中电源状态上传到云平台
                                    XmlCanDeviceStateUpload(distribution);
                                }
                                //集中电源电池欠压
                                else if(msgObjectStatus->nStatusID == OBJS_BatteryLowVoltageFault_1 ||
                                        msgObjectStatus->nStatusID == OBJS_BatteryLowVoltageFault_2 ||
                                        msgObjectStatus->nStatusID == OBJS_BatteryLowVoltageFault_3)
                                {
                                    if(msgObjectStatus->nStatusID == OBJS_BatteryLowVoltageFault_1)
                                        distribution->setbat1Undervoltage(msgObjectStatus->nValue);
                                    else if(msgObjectStatus->nStatusID == OBJS_BatteryLowVoltageFault_2)
                                        distribution->setbat2Undervoltage(msgObjectStatus->nValue);
                                    else if(msgObjectStatus->nStatusID == OBJS_BatteryLowVoltageFault_3)
                                        distribution->setbat3Undervoltage(msgObjectStatus->nValue);
                                    distribution->setbatUndervoltage(msgObjectStatus->nValue);
                                    //集中电源状态上传到云平台
                                    XmlCanDeviceStateUpload(distribution);
                                }
                                //集中电源24/36V输出故障
                                else if(msgObjectStatus->nStatusID == OBJS_PowerOutputFault)
                                {
                                    distribution->set36vOut(msgObjectStatus->nValue);
                                }
                                //集中电源系统故障
                                else if(msgObjectStatus->nStatusID == OBJS_SystemFault)
                                {
                                    distribution->setsystemFault(msgObjectStatus->nValue);
                                }
                                //集中电源充电器故障
                                else if(msgObjectStatus->nStatusID == OBJS_BatteryOverDischargeFault)
                                {
                                    distribution->setchargeFault(msgObjectStatus->nValue);
                                }
                                //集中电源过放电故障
                                else if(msgObjectStatus->nStatusID == OBJS_BatteryLowQuantityFault)
                                {
                                    distribution->setoverDischargeFault(msgObjectStatus->nValue);
                                    //集中电源状态上传到云平台
                                    XmlCanDeviceStateUpload(distribution);
                                }
                                //集中电源电池高温报警
                                else if(msgObjectStatus->nStatusID == OBJS_BatteryOverheating)
                                {
                                    distribution->setbatOverheating(msgObjectStatus->nValue);
                                }
                                //集中电源运行模式
                                else if(msgObjectStatus->nStatusID == OBJS_RunMode)
                                {
                                    distribution->setrunMode(msgObjectStatus->nValue);
                                }
                                //集中电源充电
                                else if(msgObjectStatus->nStatusID == OBJS_ChargeStatus)
                                {
                                    distribution->setchargeStatus(msgObjectStatus->nValue);
                                }
                                //集中电源应急
                                else if(msgObjectStatus->nStatusID == OBJS_ManualEmergency ||
                                        msgObjectStatus->nStatusID == OBJS_ForceEmergency ||
                                        msgObjectStatus->nStatusID == OBJS_DIEmergency ||
                                        msgObjectStatus->nStatusID == OBJS_TestEmergency ||
                                        msgObjectStatus->nStatusID == OBJS_SystemEmergency ||
                                        msgObjectStatus->nStatusID == OBJS_UndervoltageEmergency ||
                                        msgObjectStatus->nStatusID == OBJS_AutoEmergency)
                                {
                                    distribution->setemergencyStatus(msgObjectStatus->nValue);
//                                    QList<CLoop*> loops = distribution->loops();
//                                    for(int i=0; i<loops.count(); i++){
//                                        CLoop* loop = loops.at(i);
//                                        //回路存在、通讯正常、注册表该回路有灯具
//                                        if(loop && !loop->getStatus(OBJS_LoopCommunicationFault) &&
//                                                loop->isLoopOnline() && loop->isLoopLogin())
//                                        {
//                                            QList<CDevice*> devices = loop->devices();
//                                            for(int m=0; m<devices.count(); m++){
//                                                CDevice* device = devices.at(m);
//                                                if(device && (!device->getStatus(OBJS_DistributionCommunicationFault) || device->deviceTypeId() < 100))
//                                                {
//                                                    device->setStatus(msgObjectStatus->nStatusID,msgObjectStatus->nValue);
//                                                    if(msgObjectStatus->nValue == 1)
//                                                        device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"应急");
//                                                    else
//                                                        device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"正常");
//                                                    //上传灯具状态到云平台/客户端
//                                                    uploadLampStatus(device);
//                                                }
//                                                QApplication::processEvents();
//                                            }
//                                            devices.clear();
//                                        }
//                                    }
//                                    loops.clear();
                                    CGlobal::instance()->DlgDevices()->getDeviceInfo();
                                    //布局视图界面灯具状态更新
                                    CGlobal::instance()->designSplashView()->setPageStateUpdate(true);
                                }
//                                //服务端上报集中电源状态
//                                serverCentralizedPowerStateUpload(distribution);

                                if(msgObjectStatus->nStatusID != OBJS_DistributionCommunicationFault)
                                    distribution->setStatus(msgObjectStatus->nStatusID,msgObjectStatus->nValue);
                            }
                            //集中电源通讯故障
                            if(msgObjectStatus->nStatusID == OBJS_DistributionCommunicationFault
                                    && (distribution->getStatus(OBJS_DistributionCommunicationFault) != msgObjectStatus->nValue))
                            {
                                if(CGlobal::instance()->m_isShieldCanDeviceComFault)
                                    return;
                                if(msgObjectStatus->nValue)
                                {
                                    distribution->setDistributionOnline(false);
                                    distribution->setStatus(OBJS_Online, 0);
                                }
                                else
                                {
                                    distribution->setDistributionOnline(true);
                                    distribution->setStatus(OBJS_Online, 1);
                                }
                                distribution->setStatus(OBJS_DistributionCommunicationFault, msgObjectStatus->nValue);
                                if(!msgObjectStatus->nValue)
                                {
                                    distribution->setloopCommunication(0);
                                }
                                else
                                {
                                    distribution->setloopCommunication(0xFF);
                                }
                                QList<CLoop*> loops = distribution->loops();
                                for(int i=0; i<loops.count(); i++){
                                    CLoop* loop = loops.at(i);
                                    //回路存在、通讯正常、注册表该回路有灯具
                                    if(loop)
                                    {
                                        if(msgObjectStatus->nValue)
                                        {
                                            loop->setLoopOnline(false);
                                            //回路通讯故障时灯具数量备份后清零
                                            loop->setDeviceOnlineTotalBak(loop->getDeviceOnlineTotal());
                                            loop->setDeviceOnlineTotal(0);
                                        }
                                        else
                                        {
                                            loop->setLoopOnline(true);
                                            //回路通讯正常复位灯具数量
                                            loop->setDeviceOnlineTotal(loop->getDeviceOnlineTotalBak());
                                        }
                                    }
                                }
                                //上传集中电源通讯故障到云平台
                                XmlCanDeviceStateUpload(distribution);
//                                //服务端上报集中电源状态
//                                serverCentralizedPowerStateUpload(distribution);
                                CGlobal::instance()->DlgDevices()->getDeviceInfo();
                                //布局视图界面灯具状态更新
                                CGlobal::instance()->designSplashView()->setPageStateUpdate(true);
                            }
                            //回路通讯故障
                            if(msgObjectStatus->nStatusID == OBJS_LoopCommunicationFault)
                            {
                                loop = distribution->loopByAdd(msgObjectStatus->nLoopID);
                                if(!loop)
                                    return;
                                if((loop->getStatus(OBJS_LoopCommunicationFault) != msgObjectStatus->nValue) && (loop->deviceCount() > 0))
                                {
                                    loop->setStatus(OBJS_LoopCommunicationFault, msgObjectStatus->nValue);
                                    if(msgObjectStatus->nValue)
                                    {
                                        loop->setLoopOnline(false);
                                        //回路通讯故障时灯具数量备份后清零
                                        loop->setDeviceOnlineTotalBak(loop->getDeviceOnlineTotal());
                                        loop->setDeviceOnlineTotal(0);
                                    }
                                    else
                                    {
                                        loop->setLoopOnline(true);
                                        //回路通讯正常复位灯具数量
                                        loop->setDeviceOnlineTotal(loop->getDeviceOnlineTotalBak());
                                    }
                                    if(msgObjectStatus->nValue)
                                        distribution->setloopCommunication(distribution->getloopCommunication() | (0x01 << (loop->loopAdd()-1)));
                                    else
                                        distribution->setloopCommunication(distribution->getloopCommunication() & (~(0x01 << (loop->loopAdd()-1))));
                                    //上传回路故障状态到云平台
                                    XmlChannelStateUpload(loop);
//                                    //服务端上报集中电源状态
//                                    serverCentralizedPowerStateUpload(distribution);
                                }
                                CGlobal::instance()->DlgDevices()->getDeviceInfo();
                                //布局视图界面灯具状态更新
                                CGlobal::instance()->designSplashView()->setPageStateUpdate(true);
                            }
                            //集中电源回路输出开路
                            else if(msgObjectStatus->nStatusID == OBJS_OutLoopFault)
                            {
                                QList<CLoop*> loops = distribution->loops();
                                for(int i=0; i<loops.count(); i++){
                                    QApplication::processEvents();
                                    CLoop* loop = loops.at(i);
                                    if(loop)
                                    {
                                        if((loop->loopAdd() == msgObjectStatus->nLoopID) || (msgObjectStatus->nLoopID == 0))
                                        {
                                            loop->setStatus(msgObjectStatus->nStatusID,msgObjectStatus->nValue);
                                            if(msgObjectStatus->nValue)
                                                distribution->setloopOpen(distribution->getloopOpen() | (0x01 << (loop->loopAdd()-1)));
                                            else
                                                distribution->setloopOpen(distribution->getloopOpen() & (~(0x01 << (loop->loopAdd()-1))));
                                        }
                                    }
                                }
                                loops.clear();
                                CGlobal::instance()->DlgDevices()->getDeviceInfo();
                                //布局视图界面灯具状态更新
                                CGlobal::instance()->designSplashView()->setPageStateUpdate(true);
                            }
                            //灯具通讯故障
                            else if(msgObjectStatus->nStatusID == OBJS_LightCommunicationFault)
                            {
                                loop = distribution->loopByAdd(msgObjectStatus->nLoopID);
                                if(loop)
                                {
                                    CDevice* device = loop->deviceByAdd(msgObjectStatus->nDeviceID);
                                    //配置过的灯具才会报故障
                                    if(device && (device->getStatus(OBJS_LightCommunicationFault) != msgObjectStatus->nValue) &&
//                                            (device->deviceValue(DEVICE_VALUE_ISUSED) == 1 || device->deviceValue(DEVICE_VALUE_ISUSED) == 0))
                                            device->isDeviceConfig())
                                    {
                                        if(CGlobal::instance()->m_isShieldLampComFault)
                                            return;
                                        if(!device->isDeviceForbid() && device->isDeviceLogin())
                                        {
                                            if(msgObjectStatus->nValue)
                                            {
                                                device->setEnableNumberChange(true);
                                                device->setDeviceOnline(false);
                                            }
                                            else
                                            {
                                                device->setEnableNumberChange(true);
                                                device->setDeviceOnline(true);
                                            }
                                            device->setStatus(OBJS_LightCommunicationFault, msgObjectStatus->nValue);
                                            if(msgObjectStatus->nValue == true)
                                            {
                                                device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"正常");
                                                device->setDeviceValue(DEVICE_VALUE_LIGHT,"正常");
                                            }
                                            //上传灯具状态到云平台/客户端
                                            uploadLampStatus(device);
                                        }
                                    }
                                }
                                CGlobal::instance()->DlgDevices()->getDeviceInfo();
                                //布局视图界面灯具状态更新
                                CGlobal::instance()->designSplashView()->setPageStateUpdate(true);
                            }
                            //灯具光源故障
                            else if(msgObjectStatus->nStatusID == OBJS_LightFault)
                            {
                                loop = distribution->loopByAdd(msgObjectStatus->nLoopID);
                                if(loop)
                                {
                                    CDevice* device = loop->deviceByAdd(msgObjectStatus->nDeviceID);
                                    //配置过的灯具才会报故障
                                    if(device && (!device->getStatus(OBJS_LightCommunicationFault)) &&
                                            device->isDeviceConfig())
                                    {
                                        if((deviceType(device->deviceTypeId()) == NCT_LightType && CGlobal::instance()->m_isShieldLightLampLightFault)
                                                || (deviceType(device->deviceTypeId()) == NCT_ArrowType && CGlobal::instance()->m_isShieldArrowLampLightFault)
                                                || (deviceType(device->deviceTypeId()) == NCT_UndergroundType && CGlobal::instance()->m_isShieldUndergroundLampLightFault))
                                            return;
                                        if(!device->isDeviceForbid())
                                        {
                                            device->setStatus(OBJS_LightFault, msgObjectStatus->nValue);
                                            if(msgObjectStatus->nValue == 1)
                                                device->setDeviceValue(DEVICE_VALUE_LIGHT,"光源故障");
                                            else
                                                device->setDeviceValue(DEVICE_VALUE_LIGHT,"正常");
                                            //上传灯具状态到云平台/客户端
                                            uploadLampStatus(device);
                                            CGlobal::instance()->DlgDevices()->getDeviceInfo();
                                            //布局视图界面灯具状态更新
                                            CGlobal::instance()->designSplashView()->setPageStateUpdate(true);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

//            CMsgLedInfo msgLed;
//            msgLed.nLedID = 4;
//            msgLed.nLedStatus = m_faultObjects.isEmpty()?0:1;
//            exeCommand(NCT_LedStatus, &msgLed);
            break;
        }
        case NCT_PowerInfo:
        {

            break;
        }
        case NCT_Log:
        {
            CMsgLog* msgLog = static_cast<CMsgLog*>(msgStruct);
            CMsgLog *tempMsgLog = new CMsgLog;
            tempMsgLog->EventTypeID = msgLog->EventTypeID;
            tempMsgLog->User = msgLog->User;
            tempMsgLog->Time = msgLog->Time;
            tempMsgLog->Remark = msgLog->Remark;
            tempMsgLog->Type = msgLog->Type;
            tempMsgLog->Canport = msgLog->Canport;
            tempMsgLog->CanDeviceAddress = msgLog->CanDeviceAddress;
            tempMsgLog->Loop = msgLog->Loop;
            tempMsgLog->LampDeviceAddress = msgLog->LampDeviceAddress;
            tempMsgLog->Area = msgLog->Area;
            tempMsgLog->Location = msgLog->Location;
            emit commandResult(type, tempMsgLog);
            break;
        }
        case NCT_Reset://reset button
        {
            if(verifyAdminUser(type)){
                CGlobal::instance()->dm()->clearStatusObjects(true);
                exeCommand(type, msgStruct);
                m_faultObjects.clear();
                m_startObjects.clear();
//                CMsgLedInfo msgLed;
//                msgLed.nLedID = 6;
//                msgLed.nLedStatus = m_faultObjects.isEmpty()?0:1;
//                exeCommand(NCT_LedStatus, &msgLed);
            }
            break;
        }
        case NCT_Mute://mute button
        {
            param.append(true);
            emit notifyInformation(NIT_Mute, param);
            exeCommand(type, msgStruct);
            break;
        }
        case NCT_CancelMute:
        {
            param.append(false);
            emit notifyInformation(NIT_Mute, param);
            exeCommand(type, msgStruct);
            break;
        }
        case NCT_InputEmergency://开关量应急
        {
            if(CGlobal::instance()->m_EmergencyOperation != CGlobal::Start)
            {
                if(CGlobal::instance()->m_EvacuationMode == CGlobal::Manual)
                {
                    dlgAdministrator m_Administrator;
                    m_Administrator.setWindowTitle("用户验证");
                    m_Administrator.setUserDisabled(true);
                    if(QDialog::Accepted == m_Administrator.exec())
                    {
                        InputEmergency();
                        //上传开关量应急到云平台
                        XmlEmergencyInputUpload();
//                        //服务端上传开关量应急
//                        serverEmergencyInputUpload();
                    }
                }
                else
                {
                    InputEmergency();
                    //上传开关量应急到云平台
                    XmlEmergencyInputUpload();
//                    //服务端上传开关量应急
//                    serverEmergencyInputUpload();
                }
            }
            break;
        }
        case NCT_ActiveInputEmergency://有源输入应急
        {
            if(CGlobal::instance()->m_EmergencyOperation != CGlobal::Start)
            {
                if(CGlobal::instance()->m_EvacuationMode == CGlobal::Manual)
                {
                    dlgAdministrator m_Administrator;
                    m_Administrator.setWindowTitle("用户验证");
                    m_Administrator.setUserDisabled(true);
                    if(QDialog::Accepted == m_Administrator.exec())
                    {
                        ActiveInputEmergency();
                    }
                }
                else
                {
                    ActiveInputEmergency();
                }
            }
            break;
        }
        case NCT_PassiveInputEmergency://无源输入应急
        {
            if(CGlobal::instance()->m_EmergencyOperation != CGlobal::Start)
            {
                if(CGlobal::instance()->m_EvacuationMode == CGlobal::Manual)
                {
                    dlgAdministrator m_Administrator;
                    m_Administrator.setWindowTitle("用户验证");
                    m_Administrator.setUserDisabled(true);
                    if(QDialog::Accepted == m_Administrator.exec())
                    {
                        PassiveInputEmergency();
                    }
                }
                else
                {
                    PassiveInputEmergency();
                }
            }
            break;
        }
        case NCT_ForceEmergency://强启按钮
        {
            ForceEmergency();
            //上传手动应急到云平台
            XmlManualLaunchUpload();
//            //服务端上传手动应急
//            serverManualLaunchUpload();
            break;
        }
        case NCT_ControllerParam:
        {
            QString paramdata = static_cast<CMsgObjectStatus*>(msgStruct)->strdata;
            bool ok;
            if(paramdata.count() < 7)
                return;
            CController* controller = CGlobal::instance()->controller();
            double mainPower = (paramdata.mid(0, 2).toInt(&ok, 16) + paramdata.mid(2, 2).toInt(&ok, 16)*256)/100.0;   //主电电压
            double batPower = (paramdata.mid(4, 2).toInt(&ok, 16) + paramdata.mid(6, 2).toInt(&ok, 16)*256)/100.0;    //备电电压
            controller->setControllerValue(CONTROLLER_VALUE_MAINPOWER, mainPower);
            controller->setControllerValue(CONTROLLER_VALUE_BATPOWER, batPower);
            //灯键板软件版本
            int softwareNumber =  paramdata.mid(8, 2).toInt(&ok, 16) + paramdata.mid(10, 2).toInt(&ok, 16)*256;
            double softwareVersion =  paramdata.mid(12, 2).toInt(&ok, 16)/100.0;
            QString version_ledkey = QString::number(softwareNumber) + " V" + QString::number(softwareVersion, 'f', 2);;
            if(version_ledkey != CGlobal::instance()->m_version_ledkey)
            {
                CGlobal::instance()->m_version_ledkey = version_ledkey;
                CGlobal::instance()->TimeInterval()->updateVersion();
            }
            break;
        }
        case NCT_TestLinkageCom:
        {
            CGlobal::instance()->TimeInterval()->setTestLinkageComResult("成功！");
            break;
        }
        case NCT_AutomaticEvacuation://auto button
            CGlobal::instance()->TopToolBar()->slotAuto();
            break;
        case NCT_ManualEvacuation://manual button
            CGlobal::instance()->TopToolBar()->slotManual();
            break;
        case NCT_CheckTimeSync:
        {
            CMsgCheckTime* checkTime = static_cast<CMsgCheckTime*>(msgStruct);
            param.append(checkTime->monthCheckTime);
            param.append(checkTime->yearCheckTime);
            param.append(checkTime->speedMonthCheckTime);
            param.append(checkTime->speedYearCheckTime);
            param.append(checkTime->systemCurrentTime);
            emit notifyInformation(NIT_CheckTimeUpdate, param);
            break;
        }
        case NCT_PrintSet:
        {
            CMsgPrintSet* printSet = static_cast<CMsgPrintSet*>(msgStruct);
            m_printSet.printLendon = printSet->printLendon;
            m_printSet.printFault = printSet->printFault;
            m_printSet.printOther = printSet->printOther;
            break;
        }
        case NCT_CheckSet:
        {
            CMsgCheckSet* checkSet = static_cast<CMsgCheckSet*>(msgStruct);
            m_checkSet.yearCycle = checkSet->yearCycle;
            m_checkSet.yearDuration = checkSet->yearDuration;
            m_checkSet.monthCycle = checkSet->monthCycle;
            m_checkSet.monthDuration = checkSet->monthDuration;
            break;
        }
        case NCT_FactoryReset:
        {
            if(CGlobal::instance()->dm()->isLoad())
                CGlobal::instance()->programTreeView()->closeController();
            clearProjectFile();
            if(!CGlobal::instance()->dm()->isLoad())
            {
                CGlobal::instance()->dm()->loadDefault();
                CController* controller = CGlobal::instance()->dm()->controllerAt(0);
                CGlobal::instance()->programTreeView()->setController(controller);
                CGlobal::instance()->designTreeView()->setController(controller);
                CGlobal::instance()->designTreeView()->setCurrentView(1);
            }
            break;
        }
        case NCT_CanportInfo:
        {
            CMsgCanportInfo* msgCanportInfo = static_cast<CMsgCanportInfo*>(msgStruct);
            m_canportInfo.address = msgCanportInfo->address;
            m_canportInfo.netType = msgCanportInfo->netType;
            qDebug()<<"CanportInfo:"<<m_canportInfo.address<<m_canportInfo.netType;
            break;
        }
        case NCT_SelfCheckStart://self check button
        {
            emit notifyInformation(NIT_SelfCheckStart, QList<QVariant>());
            break;
        }
        case NCT_SelfCheckStop:
        {
            emit notifyInformation(NIT_SelfCheckStop, QList<QVariant>());
            break;
        }
        case NCT_StartCommunication:
        {
            if(!m_isInitController)
            {
                InitController();
                param.append(CGlobal::instance()->panelAddress());
                emit notifyInformation(NIT_Start, param);
            }
            break;
        }
        case NCT_StopCommunication:
        {
            param.append(CGlobal::instance()->panelAddress());
            emit notifyInformation(NIT_Stop, param);
            break;
        }
        case NCT_MonthCheckBegin:
        {
            qDebug()<<"NCT_MonthCheckBegin";
            CGlobal::instance()->m_isMonthEmergency = true;
            CMsgObjectStatus* objectStatus = static_cast<CMsgObjectStatus*>(msgStruct);
            int time = objectStatus->nValue;
            if(time == CGlobal::instance()->m_MonthCheckSpanSecond)
            {
                emit notifyInformation(NIT_MonthCheckBegin, param);
            }
            else
            {
                int min = time/60;
                int sec = time%60;
                QString str = QString("%1分 %2秒")
                        .arg(min, 2, 10, QChar('0'))  // 格式化分钟数，宽度2，用'0'填充
                        .arg(sec, 2, 10, QChar('0')); // 格式化秒数，宽度2，用'0'填充
                CGlobal::instance()->InformationWindow()->setLiftTime(str);
            }
            break;
        }
        case NCT_MonthCheckEnd:
        {
            qDebug()<<"NCT_MonthCheckEnd";
            CGlobal::instance()->m_isMonthEmergency = false;
            emit notifyInformation(NIT_MonthCheckEnd, param);
            break;
        }
        case NCT_YearCheckBegin:
        {
            CMsgObjectStatus* objectStatus = static_cast<CMsgObjectStatus*>(msgStruct);
            CGlobal::instance()->m_isYearEmergency = true;
            int time = objectStatus->nValue;
            if(time == CGlobal::instance()->m_YearCheckSpanSecond * 60)
            {
                emit notifyInformation(NIT_YearCheckBegin, param);
            }
            else
            {
                int min = time/60;
                int sec = time%60;
                QString str = QString("%1分 %2秒")
                        .arg(min, 2, 10, QChar('0'))  // 格式化分钟数，宽度2，用'0'填充
                        .arg(sec, 2, 10, QChar('0')); // 格式化秒数，宽度2，用'0'填充
                CGlobal::instance()->InformationWindow()->setLiftTime(str);
            }
            break;
        }
        case NCT_YearCheckEnd:
        {
            CGlobal::instance()->m_isYearEmergency = false;
            emit notifyInformation(NIT_YearCheckEnd, param);
            break;
        }
        case NCT_UndefinedObject:
        {
            break;
        }
        case NCT_ClientReset:
        {
            reset();
            break;
        }
        case NCT_HasFASSystem:
        {
            qDebug()<<"NCT_HasFASSystem";
            emit notifyInformation(NIT_HasFASSystem, param);
            break;
        }
        case NCT_ExitProgram:
        {
            qDebug()<<"NCT_ExitProgram";
            emit notifyInformation(NIT_ExitProgram, param);
            break;
        }
        case NCT_StartSpeedUp:
        {
            qDebug()<<"NCT_StartSpeedUp";
            emit notifyInformation(NIT_StartSpeedUp, param);
            break;
        }
        case NCT_StopSpeedUp:
        {
            qDebug()<<"NCT_StopSpeedUp";
            emit notifyInformation(NIT_StopSpeedUp, param);
            break;
        }
        case NCT_Relogin:
        {
            emit notifyInformation(NIT_Relogin, param);
//            CGlobal::instance()->DlgDevices()->getDeviceInfo();
            CGlobal::instance()->m_bInLogin = false;
            CGlobal::instance()->setMSgColor(QColor(12, 180, 3));
            CGlobal::instance()->setMsgText("系统运行！");
            //更新回路是否清除注册状态
            updateIsLoopRemoveLogin();
            break;
        }
        case NCT_RecordPage:
        {
            CGlobal::instance()->SystemRecord()->setPage(1);//返回第一页
            break;
        }
        default:
            break;
    }
    delete msgStruct;
}
//云平台xml格式模板初始化
void CClientBusiness::XmlInit()
{
    qDebug() << "CClientBusiness::XmlInit"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    QString deviceID = generateUniqueID();
    m_xmldoc.clear();
    m_xmldoc.appendChild(m_xmldoc.createProcessingInstruction("xml",m_xmlheader));
    //根元素
    m_xmlroot = m_xmldoc.createElement("R");
    m_xmlroot.setAttribute("DeviceID",deviceID);
    m_xmldoc.appendChild(m_xmlroot);
}
//云平台通讯心跳
void CClientBusiness::XmlHeartbeat()
{
    qDebug() << "CClientBusiness::XmlHeartbeat"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("Heartbeat");
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray());
}
//向云平台申请项目识别号
void CClientBusiness::XmlApplyForProjectID()
{
    qDebug() << "CClientBusiness::XmlApplyForProjectID"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("ApplyForProjectID");
    m_xmlitem_1.setAttribute("ProjectName",CGlobal::instance()->controllerName());
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray());
}
//上传主机信息到云平台
void CClientBusiness::XmlUploadDeviceInfo()
{
    qDebug() << "CClientBusiness::XmlUploadDeviceInfo"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("UploadDeviceInfo");
    m_xmlitem_1.setAttribute("ProjectID",CGlobal::instance()->m_ControllerID);
    m_xmlitem_1.setAttribute("DeviceID",generateUniqueID());
    m_xmlitem_1.setAttribute("DeviceType","应急疏散监控主机");
    m_xmlitem_1.setAttribute("Province",CGlobal::instance()->m_province);
    m_xmlitem_1.setAttribute("City",CGlobal::instance()->m_city);
    m_xmlitem_1.setAttribute("County",CGlobal::instance()->m_country);
    m_xmlitem_1.setAttribute("Address",CGlobal::instance()->m_addressDetail);
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray());
}
//上传CAN设备信息到云平台
void CClientBusiness::XmlCanDeviceInfo()
{
    qDebug() << "CClientBusiness::XmlCanDeviceInfo"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("CanDeviceInfo");
    for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
        if(!canport)
            continue;
        for(int i=0; i<62; i++)
        {
            CDistribution* distribution = canport->distributionByAddress(i+1);
            if(distribution)
            {
                int distributionkeyId = CGlobal::instance()->saveKeyId(OBJT_Distribution,distribution->keyId());
                m_xmlitem_2 = m_xmldoc.createElement("C");
                m_xmlitem_2.setAttribute("ID", distributionkeyId);
                m_xmlitem_2.setAttribute("Can", "1");
                m_xmlitem_2.setAttribute("Address", distribution->distributionAddress());
                m_xmlitem_2.setAttribute("T", QString(distribution->typeStr().toUtf8()));
                m_xmlitem_2.setAttribute("A", distribution->distributionValue(DISTRIBUTION_VALUE_AREA).toString());
                m_xmlitem_2.setAttribute("L", distribution->distributionValue(DISTRIBUTION_VALUE_LOCATION).toString());
                m_xmlitem_2.setAttribute("R", "");
                m_xmlitem_1.appendChild(m_xmlitem_2);
            }
        }
        m_xmlroot.appendChild(m_xmlitem_1);
        CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
    }
}
//上传通道信息到云平台
void CClientBusiness::XmlChannelInfo()
{
    qDebug() << "CClientBusiness::XmlChannelInfo"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("ChannelInfo");

    for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
        if(!canport)
            continue;
        for(int i=0; i<62; i++)
        {
            CDistribution* distribution = canport->distributionByAddress(i+1);
            if(distribution)
            {
                int distributionkeyId = CGlobal::instance()->saveKeyId(OBJT_Distribution,distribution->keyId());
                for(int i=0; i<8; i++)
                {
                    CLoop* loop = distribution->loopByAdd(i+1);
                    if(loop)
                    {
                        int loopkeyId = CGlobal::instance()->saveKeyId(OBJT_Loop,loop->keyId());
                        m_xmlitem_2 = m_xmldoc.createElement("C");
                        m_xmlitem_2.setAttribute("ID", loopkeyId);
                        m_xmlitem_2.setAttribute("CanDeviceID", distributionkeyId);
                        m_xmlitem_2.setAttribute("Channel", loop->loopValue(LOOP_VALUE_NUMBER).toInt());
                        m_xmlitem_2.setAttribute("Loop", loop->loopAdd());
                        m_xmlitem_2.setAttribute("R", tr("通道%1").arg(loop->loopValue(LOOP_VALUE_NUMBER).toInt()));
                        m_xmlitem_1.appendChild(m_xmlitem_2);
                    }
                }
            }
        }
        m_xmlroot.appendChild(m_xmlitem_1);
        CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
    }
}
//上传灯具信息到云平台
void CClientBusiness::XmlLampInfo()
{
    qDebug() << "CClientBusiness::XmlLampInfo"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("LampInfo");

    for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
        if(!canport)
            continue;
        for(int i=0; i<62; i++)
        {
            CDistribution* distribution = canport->distributionByAddress(i+1);
            if(distribution)
            {
                for(int i=0; i<8; i++)
                {
                    CLoop* loop = distribution->loopByAdd(i+1);
                    if(loop)
                    {
                        int loopkeyId = CGlobal::instance()->saveKeyId(OBJT_Loop,loop->keyId());
                        for(int i=0; i<255; i++)
                        {
                            CDevice* device = loop->deviceByAdd(i+1);
                            if(device)
                            {
                                int devicekeyId = CGlobal::instance()->saveKeyId(OBJT_Device,device->keyId());
                                m_xmlitem_2 = m_xmldoc.createElement("D");
                                m_xmlitem_2.setAttribute("ID", devicekeyId);
                                m_xmlitem_2.setAttribute("ChannelID", loopkeyId);
                                m_xmlitem_2.setAttribute("Address", device->deviceAdd());
                                m_xmlitem_2.setAttribute("T", device->deviceValue(DEVICE_VALUE_TYPE).toString());
                                m_xmlitem_2.setAttribute("A", device->deviceValue(DEVICE_VALUE_AREA).toString());
                                m_xmlitem_2.setAttribute("L", device->deviceValue(DEVICE_VALUE_LOCATION).toString());
                                m_xmlitem_2.setAttribute("R", device->deviceProductionId());
                                m_xmlitem_1.appendChild(m_xmlitem_2);
                            }
                        }
                    }
                }
            }
        }
        m_xmlroot.appendChild(m_xmlitem_1);
        CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
    }
}

//上传页面信息到云平台
void CClientBusiness::XmlLayoutPage()
{
    qDebug() << "CClientBusiness::XmlLayoutPage"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("LayoutPage");
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSqlQuery query = db.query(QString("SELECT ID,BackgroundColor,BackgroundPicture,PageWidth,PageHeight,Remark from LayoutPage"));
    QSqlRecord record = query.record();
    int fieldId = record.indexOf("ID");
    int fieldBackgroundColor = record.indexOf("BackgroundColor");
    int fieldBackgroundPicture = record.indexOf("BackgroundPicture");
    int fieldPageWidth = record.indexOf("PageWidth");
    int fieldPageHeight = record.indexOf("PageHeight");
    int fieldRemark = record.indexOf("Remark");
    while(query.next())
    {
        m_xmlitem_2 = m_xmldoc.createElement("P");
        m_xmlitem_2.setAttribute("ID", query.value(fieldId).toInt());
        m_xmlitem_2.setAttribute("Color", query.value(fieldBackgroundColor).toInt());
        m_xmlitem_2.setAttribute("Picture", query.value(fieldBackgroundPicture).toString());

        m_xmlitem_2.setAttribute("PageWidth", query.value(fieldPageWidth).toInt());
        m_xmlitem_2.setAttribute("PageHeight", query.value(fieldPageHeight).toInt());
        m_xmlitem_2.setAttribute("Remark", query.value(fieldRemark).toString());
        m_xmlitem_1.appendChild(m_xmlitem_2);

    }
    db.close();
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}

//上传图片到云平台
void CClientBusiness::XmlPicture(CMsgObjectStatus* xmlObjectStatus)
{
    qDebug() << "CClientBusiness::XmlPicture"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
//    XmlInit();
//    m_xmlitem_1 = m_xmldoc.createElement("Picture");


//    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}
//上传灯具坐标到云平台
void CClientBusiness::XmlLampCoordinate()
{
    qDebug() << "CClientBusiness::XmlLampCoordinate"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("LampCoordinate");

    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSqlQuery query = db.query(QString("SELECT c.id,c.lampinfoid,c.layoutpageid,c.cx,c.cy,c.width,c.height,c.angle from Coordinate c"));
    QSqlRecord record = query.record();
    int fieldKeyId = record.indexOf("id");
    int fieldLampInfoID = record.indexOf("lampinfoid");
    int fieldLayoutPageID = record.indexOf("layoutpageid");
    int fieldCX = record.indexOf("cx");
    int fieldCY = record.indexOf("cy");
    int fieldWidth = record.indexOf("width");
    int fieldHeight = record.indexOf("height");
    int fieldAngle = record.indexOf("angle");
    while(query.next())
    {
        m_xmlitem_2 = m_xmldoc.createElement("LC");
        m_xmlitem_2.setAttribute("ID", query.value(fieldKeyId).toInt());
        m_xmlitem_2.setAttribute("LampID", query.value(fieldLampInfoID).toInt());
        m_xmlitem_2.setAttribute("LayoutPageID", query.value(fieldLayoutPageID).toInt());
        m_xmlitem_2.setAttribute("CX", query.value(fieldCX).toInt());
        m_xmlitem_2.setAttribute("CY", query.value(fieldCY).toInt());
        m_xmlitem_2.setAttribute("Width", query.value(fieldWidth).toInt());
        m_xmlitem_2.setAttribute("Height", query.value(fieldHeight).toInt());
        m_xmlitem_2.setAttribute("Angle", query.value(fieldAngle).toInt());
        m_xmlitem_1.appendChild(m_xmlitem_2);

    }
    db.close();
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}
//上传着火点信息到云平台
void CClientBusiness::XmlFirePoint()
{
    qDebug() << "CClientBusiness::XmlFirePoint"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("FirePoint");

    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSqlQuery query = db.query(QString("SELECT f.id,f.device,f.loop,f.address,f.area,f.location from FirePoint f"));
    QSqlRecord record = query.record();
    int fieldId = record.indexOf("id");
    int fieldDeviceAddress = record.indexOf("device");
    int fieldLoopAddress = record.indexOf("loop");
    int fieldTerminalAddress = record.indexOf("address");
    int fieldArea = record.indexOf("area");
    int fieldLocation = record.indexOf("location");
    while(query.next())
    {
        m_xmlitem_2 = m_xmldoc.createElement("FP");
        m_xmlitem_2.setAttribute("ID", query.value(fieldId).toInt());
        m_xmlitem_2.setAttribute("DeviceAddress", query.value(fieldDeviceAddress).toInt());
        m_xmlitem_2.setAttribute("LoopAddress", query.value(fieldLoopAddress).toInt());
        m_xmlitem_2.setAttribute("TerminalAddress", query.value(fieldTerminalAddress).toInt());
        m_xmlitem_2.setAttribute("Area", query.value(fieldArea).toString());
        m_xmlitem_2.setAttribute("Location", query.value(fieldLocation).toString());
        m_xmlitem_1.appendChild(m_xmlitem_2);

    }
    db.close();
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}
//上传着火点坐标到云平台
void CClientBusiness::XmlFirePointCoordinate()
{
    qDebug() << "CClientBusiness::XmlFirePointCoordinate"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("FirePointCoordinate");

    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSqlQuery query = db.query(QString("SELECT f.id,f.firepointid,f.layoutpageid,f.cx,f.cy,f.width,f.height from FirePointCoordinate f"));
    QSqlRecord record = query.record();
    int fieldId = record.indexOf("id");
    int fieldFirePointID = record.indexOf("firepointid");
    int fieldLayoutPageID = record.indexOf("layoutpageid");
    int fieldCX = record.indexOf("cx");
    int fieldCY = record.indexOf("cy");
    int fieldWidth = record.indexOf("width");
    int fieldHeight = record.indexOf("height");
    while(query.next())
    {
        m_xmlitem_2 = m_xmldoc.createElement("FPC");
        m_xmlitem_2.setAttribute("ID", query.value(fieldId).toInt());
        m_xmlitem_2.setAttribute("FirePointID", query.value(fieldFirePointID).toInt());
        m_xmlitem_2.setAttribute("LayoutPageID", query.value(fieldLayoutPageID).toInt());
        m_xmlitem_2.setAttribute("CX", query.value(fieldCX).toDouble());
        m_xmlitem_2.setAttribute("CY", query.value(fieldCY).toDouble());
        m_xmlitem_2.setAttribute("Width", query.value(fieldWidth).toDouble());
        m_xmlitem_2.setAttribute("Height", query.value(fieldHeight).toDouble());
        m_xmlitem_1.appendChild(m_xmlitem_2);

    }
    db.close();
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}
//上传灯具关联着火点到云平台
void CClientBusiness::XmlLampToFirePoint()
{
    qDebug() << "CClientBusiness::XmlLampToFirePoint"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("LampToFirePoint");

    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSqlQuery query = db.query(QString("SELECT f.id,f.lampid,f.firepointid from LampToFirePoint f"));
    QSqlRecord record = query.record();
    int fieldId = record.indexOf("id");
    int fieldFirePointID = record.indexOf("firepointid");
    int fieldLampID = record.indexOf("lampid");
    while(query.next())
    {
        m_xmlitem_2 = m_xmldoc.createElement("LTF");
        m_xmlitem_2.setAttribute("ID", query.value(fieldId).toInt());
        m_xmlitem_2.setAttribute("FirePointID", query.value(fieldFirePointID).toInt());
        m_xmlitem_2.setAttribute("LampID", query.value(fieldLampID).toInt());
        m_xmlitem_1.appendChild(m_xmlitem_2);
    }
    db.close();
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}
//上传灯具默认方向到云平台
void CClientBusiness::XmlLampDefaultDirection()
{
    qDebug() << "CClientBusiness::XmlLampDefaultDirection"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("LampDefaultDirection");

    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSqlQuery query = db.query(QString("SELECT f.id,f.lampinfoid,f.lampdirection from LampDefaultDirection f"));
    QSqlRecord record = query.record();
    int fieldId = record.indexOf("id");
    int fieldDirectionID = record.indexOf("lampdirection");
    int fieldLampID = record.indexOf("lampinfoid");
    while(query.next())
    {
        m_xmlitem_2 = m_xmldoc.createElement("LDD");
        m_xmlitem_2.setAttribute("ID", query.value(fieldId).toInt());
        m_xmlitem_2.setAttribute("Direction", query.value(fieldDirectionID).toInt());
        m_xmlitem_2.setAttribute("LampID", query.value(fieldLampID).toInt());
        m_xmlitem_1.appendChild(m_xmlitem_2);

    }
    db.close();
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}
void CClientBusiness::XmlHostElectricParameter()
{
    qDebug() << "CClientBusiness::XmlHostElectricParameter"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
}
void CClientBusiness::XmlCanDeviceElectricParameter(int ID)
{
    qDebug() << "CClientBusiness::XmlCanDeviceElectricParameter"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("CanDeviceElectricParameter");
    CCanport* canport = CGlobal::instance()->controller()->canportById(CGlobal::instance()->panelAddress());
    CDistribution* distribution = canport->distributionByKeyId(CGlobal::instance()->loadKeyId(OBJT_Distribution,ID));
    if(distribution)
    {
        m_xmlitem_1.setAttribute("ID", ID);
        m_xmlitem_1.setAttribute("Voltage", distribution->distributionValue(DISTRIBUTION_VALUE_MAINV).toString());
        m_xmlitem_1.setAttribute("OutputVoltage", distribution->distributionValue(DISTRIBUTION_VALUE_OUTV).toString());
        m_xmlitem_1.setAttribute("OutputCurrent", distribution->distributionValue(DISTRIBUTION_VALUE_OUTA).toString());
    }
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}
//云平台使能开关
void CClientBusiness::XmlEnableUpload(QString enable)
{
    qDebug() << "CClientBusiness::XmlEnableUpload"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if(enable == "True" || enable == "true")
        m_cloudStateUploadFlag = true;
    else if(enable == "False" || enable == "false")
        m_cloudStateUploadFlag = false;
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("EnableUploadConfirm");
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}
//上传主机故障到云平台
void CClientBusiness::XmlHostStateUpload(bool isMainPowerOK, QString batteryState)
{
    qDebug() << "CClientBusiness::XmlHostStateUpload"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((CGlobal::instance()->m_tcptype & 0x01) == 0)
        return;
    if(m_cloudStateUploadFlag == false)
        return;
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("HostStateUpload");
    m_xmlitem_1.setAttribute("IsMainPowerOK", statusToString(isMainPowerOK));
    m_xmlitem_1.setAttribute("BatteryState", batteryState);
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}
//上传CAN设备故障到云平台
void CClientBusiness::XmlCanDeviceStateUpload(CDistribution* distribution)
{
    qDebug() << "CClientBusiness::XmlCanDeviceStateUpload"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((CGlobal::instance()->m_tcptype & 0x01) == 0)
        return;
    if(m_cloudStateUploadFlag == false && m_isQueryAllState == false)
        return;
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("CanDeviceStateUpload");
    m_xmlitem_1.setAttribute("ID", CGlobal::instance()->saveKeyId(OBJT_Distribution,distribution->keyId()));
    m_xmlitem_1.setAttribute("IsMainPowerOK", statusToString(!distribution->getmainPowerFault()));
    m_xmlitem_1.setAttribute("BatteryState", distribution->batteryStatus());
    m_xmlitem_1.setAttribute("IsCommunicationOK", statusToString(!distribution->getStatus(OBJS_DistributionCommunicationFault)));
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}

//上传通道故障状态到云平台
void CClientBusiness::XmlChannelStateUpload(CLoop* loop)
{
    qDebug() << "CClientBusiness::XmlChannelStateUpload"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((CGlobal::instance()->m_tcptype & 0x01) == 0)
        return;
    if(m_cloudStateUploadFlag == false && m_isQueryAllState == false)
        return;
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("ChannelStateUpload");
    m_xmlitem_1.setAttribute("ID", CGlobal::instance()->saveKeyId(OBJT_Loop,loop->keyId()));
    m_xmlitem_1.setAttribute("IsCommunicationOK", statusToString(!loop->getLoopCommunicationFault()));
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}
//上传灯具状态到云平台
void CClientBusiness::XmlLampStateUpload(int ID, bool isCommunicationOK, bool isLightFault, bool isWarning)
{
    qDebug() << "CClientBusiness::XmlLampStateUpload"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((CGlobal::instance()->m_tcptype & 0x01) == 0)
        return;
    if(m_cloudStateUploadFlag == false && m_isQueryAllState == false)
        return;
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("LampStateUpload");
    m_xmlitem_1.setAttribute("ID", ID);
    m_xmlitem_1.setAttribute("IsCommunicationOK", statusToString(isCommunicationOK));
    m_xmlitem_1.setAttribute("IsLightSourceFault", statusToString(isLightFault));
    m_xmlitem_1.setAttribute("IsWarning", statusToString(isWarning));
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}
//上传开关量应急到云平台
void CClientBusiness::XmlEmergencyInputUpload()
{
    qDebug() << "CClientBusiness::XmlEmergencyInputUpload"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((CGlobal::instance()->m_tcptype & 0x01) == 0)
        return;
    if(m_cloudStateUploadFlag == false)
        return;
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("EmergencyInputUpload");
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}
//上传手动应急到云平台
void CClientBusiness::XmlManualLaunchUpload()
{
    qDebug() << "CClientBusiness::XmlManualLaunchUpload"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((CGlobal::instance()->m_tcptype & 0x01) == 0)
        return;
    if(m_cloudStateUploadFlag == false)
        return;
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("ManualLaunchUpload");
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}
//上传着火点报警到云平台
void CClientBusiness::XmlUploadFirePointWarning(int deviceAddress, int loopAddress, int terminalAddress)
{
    qDebug() << "CClientBusiness::XmlUploadFirePointWarning"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((CGlobal::instance()->m_tcptype & 0x01) == 0)
        return;
    if(m_cloudStateUploadFlag == false)
        return;
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("UploadFirePointWarning");
    m_xmlitem_1.setAttribute("DeviceAddress", deviceAddress);
    m_xmlitem_1.setAttribute("LoopAddress", loopAddress);
    m_xmlitem_1.setAttribute("TerminalAddress", terminalAddress);
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}
//上传灯具指向到云平台
void CClientBusiness::XmlUploadLampDirection(int lampID, QString direction)
{
    qDebug() << "CClientBusiness::XmlUploadLampDirection"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((CGlobal::instance()->m_tcptype & 0x01) == 0)
        return;
    if(m_cloudStateUploadFlag == false)
        return;
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("UploadLampDirection");
    m_xmlitem_1.setAttribute("LampID", lampID);
    m_xmlitem_1.setAttribute("Direction", direction);
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}
//上传复位申明到云平台
void CClientBusiness::XmlResetDeclare()
{
    qDebug() << "CClientBusiness::XmlResetDeclare"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((CGlobal::instance()->m_tcptype & 0x01) == 0)
        return;
    if(m_cloudStateUploadFlag == false)
        return;
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("ResetDeclare");
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}
//云平台主机控制指令
void CClientBusiness::XmlHostControl(CMsgObjectStatus* xmlObjectStatus)
{
    qDebug() << "CClientBusiness::XmlHostControl"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    CMsgNull m_MsgNUll;
    if(xmlObjectStatus->strdata == "启动")
    {
        CMsgNull m_MsgNUll;
        exeCommand(NCT_CancelMute, &m_MsgNUll);
        CGlobal::instance()->DataProcessing()->setSoundIcon(true);
        if(CGlobal::instance()->m_EmergencyOperation == CGlobal::Start)
            return;
        CGlobal::instance()->m_EmergencyOperation = CGlobal::Start;
        exeCommand(NCT_EmergencyStart, &m_MsgNUll);
        Emergency();
    }
    else if(xmlObjectStatus->strdata == "复位")
    {
//        CGlobal::instance()->m_bInLogin = false;
//        CGlobal::instance()->m_isEmergency = false;
//        clearStatusObject(true);
//        clearExpectObject();
//        //复位声明上传到云平台
//        XmlResetDeclare();
//        //灯具状态复位
//        DeviceResetStatus();
//        CGlobal::instance()->DlgDevices()->getDeviceInfo();
//        CGlobal::instance()->DlgDevices()->lampBatStatusUpload();
//        //布局视图界面灯具状态更新
//        CGlobal::instance()->designSplashView()->setPageStateUpdate(true);
//        //集中电源状态复位
//        CGlobal::instance()->programDistributionView()->distributionResetStatus();

//        //复位后延迟1s对比检查数据库数据是否在线
//        QTimer::singleShot(1000, this, SLOT(slotCheckLoginInfo()));
//        exeCommand(NCT_Reset, &m_MsgNUll);
    }
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("HostControlConfirm");
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}
//云平台灯具控制指令
void CClientBusiness::XmlLampControl(CMsgObjectStatus* xmlObjectStatus)
{
    qDebug() << "CClientBusiness::XmlLampControl"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    QString direction = xmlObjectStatus->strdata;
    int lampInfoID = xmlObjectStatus->nDeviceID;
    int ChannelInfoID = 0,LampAddress = 0,LoopAddress = 0,DistributionAddress = 0,directionValue = 0,typeNumber = 0;
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSqlQuery querydevice = db.query(QString("SELECT l.id,l.channelinfoid,l.address,r.number from LampInfo l "
                                             "left join lamptype r on l.lamptypeid=r.id "
                                             "where l.id=%1"
                                       ).arg(lampInfoID));
    QSqlRecord recorddevice = querydevice.record();
    int fieldLampID = recorddevice.indexOf("id");
    int fieldChannelInfoID = recorddevice.indexOf("channelinfoid");
    int fieldLampAddress = recorddevice.indexOf("address");
    int fieldTypeNumber = recorddevice.indexOf("number");
    while(querydevice.next())
    {
        ChannelInfoID = querydevice.value(fieldChannelInfoID).toInt();
        LampAddress = querydevice.value(fieldLampAddress).toInt();
        typeNumber = recorddevice.value(fieldTypeNumber).toInt();
    }

    QSqlQuery query = db.query(QString("SELECT m.id,m.candeviceinfoid,m.loop,c.address from ChannelInfo m "
                                       "left join CanDeviceInfo c on m.candeviceinfoid=c.id "
                                       "where m.id=%1"
                                       ).arg(ChannelInfoID));
    QSqlRecord record = query.record();
    int fieldLoopAddress = record.indexOf("loop");
    int fieldDistributionAddress = record.indexOf("address");
    while(query.next())
    {
        LoopAddress = query.value(fieldLoopAddress).toInt();
        DistributionAddress = query.value(fieldDistributionAddress).toInt();
    }
    db.close();

    if(direction == "左向")
        directionValue = 0x05;
    else if(direction == "右向")
        directionValue = 0x06;
    else if(direction == "全灭")
        directionValue = 0x04;
    else if(direction == "全亮")
        directionValue = 0x07;


    if((typeNumber >= 150 && typeNumber < 200) || (typeNumber >= 50 && typeNumber < 100))
    {
        CMsgDeviceInfo msgDeviceInfo;
        msgDeviceInfo.deviceInfo.insert(DI_KeyId, lampInfoID);
        msgDeviceInfo.deviceInfo.insert(DI_DeviceId, LampAddress);
        msgDeviceInfo.deviceInfo.insert(DI_LoopId, LoopAddress);
        msgDeviceInfo.deviceInfo.insert(DI_DistributionId, DistributionAddress);
        if(direction == "全灭")
            exeCommand(NCT_DeviceDefaultOff, &msgDeviceInfo);
        else if(direction == "全亮")
            exeCommand(NCT_DeviceDefaultOn, &msgDeviceInfo);
    }
    else
    {
        CMsgObjectStatus msgobjectstatus;
        msgobjectstatus.nDisID = DistributionAddress;
        msgobjectstatus.nLoopID = LoopAddress;
        msgobjectstatus.nDeviceID = LampAddress;
        msgobjectstatus.nValue = directionValue;
        //上传灯具指向到云平台
        XmlUploadLampDirection(querydevice.value(fieldLampID).toInt(),direction);
        //服务端上传灯具指向
        serverLampDirectionUpload(querydevice.value(fieldLampID).toInt(),direction);
        exeCommand(NCT_DeviceDirection, &msgobjectstatus);
    }
    //应答
    XmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("LampControlConfirm");
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processSocket()->sendXmlMsg(m_xmldoc.toByteArray(4));
}
//云平台查询所有状态
void CClientBusiness::XmlQueryAllState()
{
    qDebug() << "CClientBusiness::XmlQueryAllState"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    QString batteryStatus;
    CCanport* canport = CGlobal::instance()->controller()->canportById(CGlobal::instance()->panelAddress());
    if(!canport)
        return;
    if(canport->getStatus(OBJS_StandbyPowerOff))
        batteryStatus = "断线";
    else if(canport->getStatus(OBJS_StandbyPowerShort))
        batteryStatus = "短路";
    else if(canport->getStatus(OBJS_StandbyPowerUndervoltage))
        batteryStatus = "欠压";
    else
        batteryStatus = "正常";
    //主机状态上传到云平台
    XmlHostStateUpload(!canport->getStatus(OBJS_MainPowerFault),batteryStatus);


    for(int i=0; i<62; i++)
    {
        CDistribution* distribution = canport->distributionByAddress(i+1);
        if(distribution)
        {
            QApplication::processEvents();
            //集中电源状态
            XmlCanDeviceStateUpload(distribution);
            for(int i=0; i<8; i++)
            {
                CLoop* loop = distribution->loopByAdd(i+1);
                if(loop)
                {
                    QApplication::processEvents();
                    //回路状态
                    XmlChannelStateUpload(loop);
                    for(int i=0; i<255; i++)
                    {
                        CDevice* device = loop->deviceByAdd(i+1);
                        if(device)
                        {
                            QApplication::processEvents();
                            //灯具状态
                            uploadLampStatus(device);
                        }
                    }
                }
            }
        }
    }
    m_isQueryAllState = false;
}


//服务端xml格式模板初始化
void CClientBusiness::serverXmlInit()
{
    qDebug() << "CClientBusiness::serverXmlInit"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    m_xmldoc.clear();
    m_xmldoc.appendChild(m_xmldoc.createProcessingInstruction("xml",m_xmlheader));
    //根元素
    m_xmlroot = m_xmldoc.createElement("R");
    m_xmldoc.appendChild(m_xmlroot);
}

//服务端通讯心跳
void CClientBusiness::serverHeartbeat()
{
    qDebug() << "CClientBusiness::serverHeartbeat"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("HeartbeatUpload");
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray());
}

//服务端回复CAN设备信息
void CClientBusiness::serverCanDeviceInfo()
{
    qDebug() << "CClientBusiness::serverCanDeviceInfo"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("CanDeviceList");

    CCanport* canport = CGlobal::instance()->controller()->canportById(CGlobal::instance()->panelAddress());
    if(!canport)
        return;
    for(int i=0; i<62; i++)
    {
        CDistribution* distribution = canport->distributionByAddress(i+1);
        if(distribution)
        {
            int distributionkeyId = CGlobal::instance()->saveKeyId(OBJT_Distribution,distribution->keyId());
            m_xmlitem_2 = m_xmldoc.createElement("D");
            m_xmlitem_2.setAttribute("ID", distributionkeyId);
            m_xmlitem_2.setAttribute("CanPort", "1");
            m_xmlitem_2.setAttribute("Address", distribution->distributionAddress());
            m_xmlitem_2.setAttribute("DeviceType", "集中电源");
            m_xmlitem_2.setAttribute("Area", distribution->distributionValue(DISTRIBUTION_VALUE_AREA).toString());
            m_xmlitem_2.setAttribute("Location", distribution->distributionValue(DISTRIBUTION_VALUE_LOCATION).toString());
            m_xmlitem_1.appendChild(m_xmlitem_2);
        }
    }
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

//服务端回复通道信息
void CClientBusiness::serverChannelInfo()
{
    qDebug() << "CClientBusiness::serverChannelInfo"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("ChannelList");

    CCanport* canport = CGlobal::instance()->controller()->canportById(CGlobal::instance()->panelAddress());
    if(!canport)
        return;
    for(int i=0; i<62; i++)
    {
        CDistribution* distribution = canport->distributionByAddress(i+1);
        if(distribution)
        {
            int distributionkeyId = CGlobal::instance()->saveKeyId(OBJT_Distribution,distribution->keyId());
            for(int i=0; i<8; i++)
            {
                CLoop* loop = distribution->loopByAdd(i+1);
                if(loop)
                {
                    int loopkeyId = CGlobal::instance()->saveKeyId(OBJT_Loop,loop->keyId());
                    m_xmlitem_2 = m_xmldoc.createElement("C");
                    m_xmlitem_2.setAttribute("ID", loopkeyId);
                    m_xmlitem_2.setAttribute("CanDeviceID", distributionkeyId);
                    m_xmlitem_2.setAttribute("Number", loop->loopValue(LOOP_VALUE_NUMBER).toInt());
                    m_xmlitem_2.setAttribute("Loop", loop->loopAdd());
                    m_xmlitem_2.setAttribute("Remark", tr("通道%1").arg(loop->loopValue(LOOP_VALUE_NUMBER).toInt()));
                    m_xmlitem_1.appendChild(m_xmlitem_2);
                }
            }
        }
    }
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}
//服务端回复灯具信息
void CClientBusiness::serverLampInfo()
{
    qDebug() << "CClientBusiness::serverLampInfo"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("LampList");

    CCanport* canport = CGlobal::instance()->controller()->canportById(CGlobal::instance()->panelAddress());
    if(!canport)
        return;
    for(int i=0; i<62; i++)
    {
        CDistribution* distribution = canport->distributionByAddress(i+1);
        if(distribution)
        {
            for(int i=0; i<8; i++)
            {
                CLoop* loop = distribution->loopByAdd(i+1);
                if(loop)
                {
                    int loopkeyId = CGlobal::instance()->saveKeyId(OBJT_Loop,loop->keyId());
                    for(int i=0; i<255; i++)
                    {
                        CDevice* device = loop->deviceByAdd(i+1);
                        if(device)
                        {
                            int devicekeyId = CGlobal::instance()->saveKeyId(OBJT_Device,device->keyId());
                            m_xmlitem_2 = m_xmldoc.createElement("L");
                            m_xmlitem_2.setAttribute("ID", devicekeyId);
                            m_xmlitem_2.setAttribute("CID", loopkeyId);
                            m_xmlitem_2.setAttribute("A", device->deviceAdd());
                            m_xmlitem_2.setAttribute("T", device->deviceTypeId());
                            m_xmlitem_2.setAttribute("Ar", device->deviceValue(DEVICE_VALUE_AREA).toString());
                            m_xmlitem_2.setAttribute("Lo", device->deviceValue(DEVICE_VALUE_LOCATION).toString());
                            m_xmlitem_2.setAttribute("R", device->deviceValue(DEVICE_VALUE_DESCRIPTION).toString());
                            m_xmlitem_1.appendChild(m_xmlitem_2);
                        }
                    }
                }
            }
        }
    }
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

//服务端回复着火点信息
void CClientBusiness::serverFirePoint()
{
    qDebug() << "CClientBusiness::serverFirePoint"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("FirePointList");

    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSqlQuery query = db.query(QString("SELECT f.id,f.device,f.loop,f.address,f.area,f.location from FirePoint f"));
    QSqlRecord record = query.record();
    int fieldId = record.indexOf("id");
    int fieldDevice = record.indexOf("device");
    int fieldLoopAddress = record.indexOf("loop");
    int fieldTerminalAddress = record.indexOf("address");
    int fieldArea = record.indexOf("area");
    int fieldLocation = record.indexOf("location");
    while(query.next())
    {
        m_xmlitem_2 = m_xmldoc.createElement("FP");
        m_xmlitem_2.setAttribute("ID", query.value(fieldId).toInt());
        m_xmlitem_2.setAttribute("D", query.value(fieldDevice).toInt());
        m_xmlitem_2.setAttribute("L", query.value(fieldLoopAddress).toInt());
        m_xmlitem_2.setAttribute("A", query.value(fieldTerminalAddress).toInt());
        m_xmlitem_2.setAttribute("Ar", query.value(fieldArea).toString());
        m_xmlitem_2.setAttribute("Lo", query.value(fieldLocation).toString());
        m_xmlitem_1.appendChild(m_xmlitem_2);

    }
    db.close();
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

//服务端回复灯具关联着火点
void CClientBusiness::serverLampToFirePoint()
{
    qDebug() << "CClientBusiness::serverLampToFirePoint"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("LampToFirePointList");

    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSqlQuery query = db.query(QString("SELECT f.id,f.lampid,f.firepointid from LampToFirePoint f"));
    QSqlRecord record = query.record();
    int fieldId = record.indexOf("id");
    int fieldFirePointID = record.indexOf("firepointid");
    int fieldLampID = record.indexOf("lampid");
    while(query.next())
    {
        m_xmlitem_2 = m_xmldoc.createElement("LTFP");
        m_xmlitem_2.setAttribute("ID", query.value(fieldId).toInt());
        m_xmlitem_2.setAttribute("FPID", query.value(fieldFirePointID).toInt());
        m_xmlitem_2.setAttribute("LID", query.value(fieldLampID).toInt());
        m_xmlitem_1.appendChild(m_xmlitem_2);
    }
    db.close();
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

//服务端回复灯具连接信息
void CClientBusiness::serverLampConnection()
{
    qDebug() << "CClientBusiness::serverLampConnection"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("LampConnection");

    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSqlQuery query = db.query(QString("SELECT f.id,f.StartLampID,f.Joint,f.EndLampID from LampConnection f"));
    QSqlRecord record = query.record();
    int fieldId = record.indexOf("id");
    int fieldStartLampID = record.indexOf("StartLampID");
    int fieldEndLampID = record.indexOf("EndLampID");
    int fieldJoint = record.indexOf("Joint");
    while(query.next())
    {
        m_xmlitem_2 = m_xmldoc.createElement("LC");
        m_xmlitem_2.setAttribute("ID", query.value(fieldId).toInt());
        m_xmlitem_2.setAttribute("SID", query.value(fieldStartLampID).toInt());
        m_xmlitem_2.setAttribute("EID", query.value(fieldEndLampID).toInt());
        m_xmlitem_2.setAttribute("J", query.value(fieldJoint).toString());
        m_xmlitem_1.appendChild(m_xmlitem_2);
    }
    db.close();
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

//服务端回复故障记录
void CClientBusiness::serverEventList(CMsgObjectStatus* xmlObjectStatus)
{
    qDebug() << "CClientBusiness::serverEventList"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("EventList");

    QString fileName = CGlobal::instance()->workspaces() + "/" + "log.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QString sql = QString("SELECT EventTypeID,Type,Time,CanPort,CanDeviceAddress,Loop,"
                                       "LampDeviceAddress,Area,Location,Remark from log WHERE Time > '%1' AND Time < '%2' ")
                               .arg(xmlObjectStatus->nStartTime.toString("yyyy-MM-dd")).arg(xmlObjectStatus->nEndTime.toString("yyyy-MM-dd"));
//    QSqlQuery query = db.query(QString("SELECT EventTypeID,Type,Time,CanPort,CanDeviceAddress,Loop,"
//                                       "LampDeviceAddress,Area,Location,Remark from log WHERE Time > '%1' AND Time < '%2' ")
//                               .arg(xmlObjectStatus->nStartTime.toString("yyyy-MM-dd")).arg(xmlObjectStatus->nEndTime.toString("yyyy-MM-dd")));
    if(xmlObjectStatus->strdata != "")
    {
        sql.append(QString(" AND type = '%1'").arg(xmlObjectStatus->strdata));
    }
    QSqlQuery query = db.query(sql);
    QSqlRecord record = query.record();
    int fieldId = record.indexOf("EventTypeID");
    int fieldType = record.indexOf("Type");
    int fieldTime = record.indexOf("Time");
    int fieldCanPort = record.indexOf("CanPort");
    int fieldCanDeviceAddress = record.indexOf("CanDeviceAddress");
    int fieldLoop = record.indexOf("Loop");
    int fieldLampDeviceAddress = record.indexOf("LampDeviceAddress");
    int fieldArea = record.indexOf("Area");
    int fieldLocation = record.indexOf("Location");
    int fieldRemark = record.indexOf("Remark");
    while(query.next())
    {
        m_xmlitem_2 = m_xmldoc.createElement("E");
        m_xmlitem_2.setAttribute("ID", query.value(fieldId).toInt());
        m_xmlitem_2.setAttribute("EventType", query.value(fieldType).toString());
        m_xmlitem_2.setAttribute("Time", query.value(fieldTime).toString());
        m_xmlitem_2.setAttribute("CanPort", query.value(fieldCanPort).toInt());
        m_xmlitem_2.setAttribute("CanDeviceAddress", query.value(fieldCanDeviceAddress).toInt());
        m_xmlitem_2.setAttribute("Loop", query.value(fieldLoop).toInt());
        m_xmlitem_2.setAttribute("LampDeviceAddress", query.value(fieldLampDeviceAddress).toInt());
        m_xmlitem_2.setAttribute("Area", query.value(fieldArea).toString());
        m_xmlitem_2.setAttribute("Location", query.value(fieldLocation).toString());
        m_xmlitem_2.setAttribute("Remark", query.value(fieldRemark).toString());
        m_xmlitem_1.appendChild(m_xmlitem_2);
    }
    db.close();
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

//服务端回复所有状态
void CClientBusiness::serverAllState()
{
    qDebug() << "CClientBusiness::serverAllState"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    QString batteryStatus;
    CCanport* canport = CGlobal::instance()->controller()->canportById(CGlobal::instance()->panelAddress());
    if(!canport)
        return;
    if(canport->getStatus(OBJS_StandbyPowerOff))
        batteryStatus = "断线";
    else if(canport->getStatus(OBJS_StandbyPowerShort))
        batteryStatus = "短路";
    else if(canport->getStatus(OBJS_StandbyPowerUndervoltage))
        batteryStatus = "欠压";
    else
        batteryStatus = "正常";
    //服务端主机状态上传
    serverHostStateUpload(!canport->getStatus(OBJS_MainPowerFault),batteryStatus);

    for(int i=0; i<62; i++)
    {
        CDistribution* distribution = canport->distributionByAddress(i+1);
        if(distribution)
        {
            QApplication::processEvents();
            //集中电源状态
            serverCentralizedPowerStateUpload(distribution);
            for(int i=0; i<8; i++)
            {
                CLoop* loop = distribution->loopByAdd(i+1);
                if(loop)
                {
                    QApplication::processEvents();
                    for(int i=0; i<255; i++)
                    {
                        CDevice* device = loop->deviceByAdd(i+1);
                        if(device)
                        {
                            QApplication::processEvents();
                            //灯具状态
                            uploadLampStatus(device);
                        }
                    }
                }
            }
        }
    }
    m_isQueryAllState = false;
}

//服务端回复页面信息
void CClientBusiness::serverPageInfo()
{
    qDebug() << "CClientBusiness::serverPageInfo"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("PageInfo");

    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSqlQuery query = db.query(QString("SELECT ID,BackgroundColor,BackgroundPicture,PageWidth,PageHeight,Remark from LayoutPage"));
    QSqlRecord record = query.record();
    int fieldId = record.indexOf("ID");
    int fieldBackgroundColor = record.indexOf("BackgroundColor");
    int fieldBackgroundPicture = record.indexOf("BackgroundPicture");
    int fieldPageWidth = record.indexOf("PageWidth");
    int fieldPageHeight = record.indexOf("PageHeight");
    int fieldRemark = record.indexOf("Remark");
    while(query.next())
    {
        m_xmlitem_2 = m_xmldoc.createElement("PI");
        m_xmlitem_2.setAttribute("ID", query.value(fieldId).toInt());
        if(query.value(fieldBackgroundPicture).toString() == "")
            m_xmlitem_2.setAttribute("C", query.value(fieldBackgroundColor).toInt());
        else
        {
            m_xmlitem_2.setAttribute("P", query.value(fieldBackgroundPicture).toString());
            QString filePath = CGlobal::instance()->workspaces() + "/" + "PageBackgroundFolder/" + query.value(fieldBackgroundPicture).toString();
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly)) {
                    qDebug() << "Failed to open file";
                }
            QByteArray imageData = file.readAll();
            m_xmlitem_2.setAttribute("S", imageData.size());
//            QImage image;
//            if (image.loadFromData(imageData)) {
//                int memorySize = image.byteCount();
//                m_xmlitem_2.setAttribute("S", memorySize);
//                qDebug() << "Image Memory Size:" << memorySize << "bytes";
//            }
//            else
//                m_xmlitem_2.setAttribute("S", 0);
            file.close();
        }

        m_xmlitem_2.setAttribute("W", query.value(fieldPageWidth).toInt());
        m_xmlitem_2.setAttribute("H", query.value(fieldPageHeight).toInt());
        m_xmlitem_2.setAttribute("R", query.value(fieldRemark).toString());
        m_xmlitem_1.appendChild(m_xmlitem_2);

    }
    db.close();
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

//服务端回复图片
void CClientBusiness::serverPicture(CMsgObjectStatus* xmlObjectStatus)
{
    qDebug() << "CClientBusiness::serverPicture"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    QString filePath = CGlobal::instance()->workspaces() + "/" + "PageBackgroundFolder/" + xmlObjectStatus->strdata;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open file";
        }
    QByteArray imageData = file.readAll();
    imageData.append("E");
    imageData.append("O");
    imageData.append("F");
    CGlobal::instance()->processServer()->replyXmlMsg(imageData);
}

//服务端回复灯具坐标
void CClientBusiness::serverLampCoordinate()
{
    qDebug() << "CClientBusiness::serverLampCoordinate"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("LampCoordinate");

    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSqlQuery query = db.query(QString("SELECT c.id,c.lampinfoid,c.layoutpageid,c.cx,c.cy,c.width,c.height,c.angle from Coordinate c"));
    QSqlRecord record = query.record();
    int fieldKeyId = record.indexOf("id");
    int fieldLampInfoID = record.indexOf("lampinfoid");
    int fieldLayoutPageID = record.indexOf("layoutpageid");
    int fieldCX = record.indexOf("cx");
    int fieldCY = record.indexOf("cy");
    int fieldWidth = record.indexOf("width");
    int fieldHeight = record.indexOf("height");
    int fieldAngle = record.indexOf("angle");
    while(query.next())
    {
        m_xmlitem_2 = m_xmldoc.createElement("LC");
        m_xmlitem_2.setAttribute("ID", query.value(fieldKeyId).toInt());
        m_xmlitem_2.setAttribute("LID", query.value(fieldLampInfoID).toInt());
        m_xmlitem_2.setAttribute("PID", query.value(fieldLayoutPageID).toInt());
        m_xmlitem_2.setAttribute("X", query.value(fieldCX).toInt());
        m_xmlitem_2.setAttribute("Y", query.value(fieldCY).toInt());
        m_xmlitem_2.setAttribute("W", query.value(fieldWidth).toInt());
        m_xmlitem_2.setAttribute("H", query.value(fieldHeight).toInt());
        m_xmlitem_2.setAttribute("A", query.value(fieldAngle).toInt());
        m_xmlitem_1.appendChild(m_xmlitem_2);

    }
    db.close();
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

//服务端回复着火点坐标
void CClientBusiness::serverFirePointCoordinate()
{
    qDebug() << "CClientBusiness::serverFirePointCoordinate"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("FirePointCoordinate");

    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSqlQuery query = db.query(QString("SELECT f.id,f.firepointid,f.layoutpageid,f.cx,f.cy,f.width,f.height from FirePointCoordinate f"));
    QSqlRecord record = query.record();
    int fieldId = record.indexOf("id");
    int fieldFirePointID = record.indexOf("firepointid");
    int fieldLayoutPageID = record.indexOf("layoutpageid");
    int fieldCX = record.indexOf("cx");
    int fieldCY = record.indexOf("cy");
    int fieldWidth = record.indexOf("width");
    int fieldHeight = record.indexOf("height");
    while(query.next())
    {
        m_xmlitem_2 = m_xmldoc.createElement("FPC");
        m_xmlitem_2.setAttribute("ID", query.value(fieldId).toInt());
        m_xmlitem_2.setAttribute("FPID", query.value(fieldFirePointID).toInt());
        m_xmlitem_2.setAttribute("PID", query.value(fieldLayoutPageID).toInt());
        m_xmlitem_2.setAttribute("X", query.value(fieldCX).toDouble());
        m_xmlitem_2.setAttribute("Y", query.value(fieldCY).toDouble());
        m_xmlitem_2.setAttribute("W", query.value(fieldWidth).toDouble());
        m_xmlitem_2.setAttribute("H", query.value(fieldHeight).toDouble());
        m_xmlitem_1.appendChild(m_xmlitem_2);

    }
    db.close();
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

//服务端回复灯具软件信息
void CClientBusiness::serverLampSoftwareInfo(int ID, int SoftwareNumber, int SoftwareVersion)
{
    qDebug() << "CClientBusiness::serverLampSoftwareInfo"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("LampSoftwareInfo");
    m_xmlitem_1.setAttribute("ID", ID);
    m_xmlitem_1.setAttribute("SoftwareNumber", SoftwareNumber);
    m_xmlitem_1.setAttribute("SoftwareVersion", SoftwareVersion);
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

//服务端回复集中电源软件信息
void CClientBusiness::serverDeviceSoftwareInfo(CDistribution* distribution)
{
    qDebug() << "CClientBusiness::serverDeviceSoftwareInfo"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("DeviceSoftwareInfo");
    m_xmlitem_1.setAttribute("ID", CGlobal::instance()->saveKeyId(OBJT_Distribution,distribution->keyId()));
    m_xmlitem_1.setAttribute("CPUVersion", distribution->distributionValue(DISTRIBUTION_VALUE_CPUVERSION).toString());
    m_xmlitem_1.setAttribute("POWVersion", distribution->distributionValue(DISTRIBUTION_VALUE_POWVERSION).toString());
    m_xmlitem_1.setAttribute("ABUS1Version", distribution->distributionValue(DISTRIBUTION_VALUE_ABUS1VERSION).toString());
    m_xmlitem_1.setAttribute("ABUS2Version", distribution->distributionValue(DISTRIBUTION_VALUE_ABUS2VERSION).toString());
    m_xmlitem_1.setAttribute("ABUS3Version", distribution->distributionValue(DISTRIBUTION_VALUE_ABUS3VERSION).toString());
    m_xmlitem_1.setAttribute("ABUS4Version", distribution->distributionValue(DISTRIBUTION_VALUE_ABUS4VERSION).toString());
    m_xmlitem_1.setAttribute("ABUS5Version", distribution->distributionValue(DISTRIBUTION_VALUE_ABUS5VERSION).toString());
    m_xmlitem_1.setAttribute("ABUS6Version", distribution->distributionValue(DISTRIBUTION_VALUE_ABUS6VERSION).toString());
    m_xmlitem_1.setAttribute("ABUS7Version", distribution->distributionValue(DISTRIBUTION_VALUE_ABUS7VERSION).toString());
    m_xmlitem_1.setAttribute("ABUS8Version", distribution->distributionValue(DISTRIBUTION_VALUE_ABUS8VERSION).toString());
    m_xmlitem_1.setAttribute("ABUS9Version", distribution->distributionValue(DISTRIBUTION_VALUE_ABUS9VERSION).toString());
    m_xmlitem_1.setAttribute("ABUS10Version", distribution->distributionValue(DISTRIBUTION_VALUE_ABUS10VERSION).toString());
    m_xmlitem_1.setAttribute("ABUS11Version", distribution->distributionValue(DISTRIBUTION_VALUE_ABUS11VERSION).toString());
    m_xmlitem_1.setAttribute("ABUS12Version", distribution->distributionValue(DISTRIBUTION_VALUE_ABUS12VERSION).toString());
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

//服务端回复集中电源运行参数
void CClientBusiness::serverDeviceRealtimeData(CDistribution* distribution)
{
    qDebug() << "CClientBusiness::serverDeviceRealtimeData"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    CLoop* loop;
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("DeviceRealtimeData");
    m_xmlitem_1.setAttribute("ID", CGlobal::instance()->saveKeyId(OBJT_Distribution,distribution->keyId()));
    m_xmlitem_1.setAttribute("MainPowerVoltage", distribution->distributionValue(DISTRIBUTION_VALUE_MAINV).toString());
    m_xmlitem_1.setAttribute("OutputVoltage", distribution->distributionValue(DISTRIBUTION_VALUE_OUTV).toString());
    m_xmlitem_1.setAttribute("OutputCurrent", distribution->distributionValue(DISTRIBUTION_VALUE_OUTA).toString());
    m_xmlitem_1.setAttribute("OutsideVoltage", distribution->distributionValue(DISTRIBUTION_VALUE_EXTERNALV).toString());
    m_xmlitem_1.setAttribute("Battery1Voltage", distribution->distributionValue(DISTRIBUTION_VALUE_BAT1V).toString());
    m_xmlitem_1.setAttribute("Battery2Voltage", distribution->distributionValue(DISTRIBUTION_VALUE_BAT2V).toString());
    m_xmlitem_1.setAttribute("Battery3Voltage", distribution->distributionValue(DISTRIBUTION_VALUE_BAT3V).toString());
    m_xmlitem_1.setAttribute("Battery1Temperature", distribution->distributionValue(DISTRIBUTION_VALUE_BAT1T).toString());
    m_xmlitem_1.setAttribute("Battery2Temperature", distribution->distributionValue(DISTRIBUTION_VALUE_BAT2T).toString());
    m_xmlitem_1.setAttribute("Battery3Temperature", distribution->distributionValue(DISTRIBUTION_VALUE_BAT3T).toString());
    m_xmlitem_1.setAttribute("BatteryTotalVoltage", distribution->distributionValue(DISTRIBUTION_VALUE_BATV).toString());
    for(int i=1; i<13; i++)
    {
        loop = distribution->loopByAdd(i);
        if(loop)
        {
            m_xmlitem_1.setAttribute(QString("Loop%1Voltage").arg(i), loop->loopValue(LOOP_VALUE_V).toString());
            m_xmlitem_1.setAttribute(QString("Loop%1Current").arg(i), loop->loopValue(LOOP_VALUE_A).toString());
        }
        else
        {
            m_xmlitem_1.setAttribute(QString("Loop%1Voltage").arg(i), 0);
            m_xmlitem_1.setAttribute(QString("Loop%1Current").arg(i), 0);
        }
    }


    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

//服务端上传主机故障
void CClientBusiness::serverHostStateUpload(bool isMainPowerOK, QString batteryState)
{
    qDebug() << "CClientBusiness::serverHostStateUpload"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((CGlobal::instance()->m_tcptype & 0x02) == 0)
        return;
    if(m_masterStateUploadFlag == false && m_isQueryAllState == false)
        return;
    QString batteryOK = "False";
    if(batteryState == "正常")
        batteryOK = "True";
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("HostFaultUpload");
    m_xmlitem_1.setAttribute("IsMainPowerOK", statusToString(isMainPowerOK));
    m_xmlitem_1.setAttribute("IsBackupBatteryOK", batteryOK);
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

//服务端上传集中电源故障
void CClientBusiness::serverCentralizedPowerStateUpload(CDistribution* distribution)
{
    qDebug() << "CClientBusiness::serverCentralizedPowerStateUpload"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((CGlobal::instance()->m_tcptype & 0x02) == 0)
        return;
    if(m_masterStateUploadFlag == false && m_isQueryAllState == false)
        return;
    CLoop* loop;
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("CanDeviceStateUpload");
    m_xmlitem_1.setAttribute("CanDeviceID", CGlobal::instance()->saveKeyId(OBJT_Distribution, distribution->keyId()));
    m_xmlitem_1.setAttribute("IsCommunicationOK", statusToString(!distribution->getStatus(OBJS_DistributionCommunicationFault)));
    m_xmlitem_1.setAttribute("IsOutputOverload", statusToString(distribution->getoverOut()));
    m_xmlitem_1.setAttribute("IsOutputCut", statusToString(distribution->getoutOpen()));
    m_xmlitem_1.setAttribute("IsBatteryVoltage1Low", statusToString(distribution->getbat1Undervoltage()));
    m_xmlitem_1.setAttribute("IsBatteryVoltage2Low", statusToString(distribution->getbat2Undervoltage()));
    m_xmlitem_1.setAttribute("IsBatteryVoltage3Low", statusToString(distribution->getbat3Undervoltage()));
    m_xmlitem_1.setAttribute("IsBackupBatteryFault", statusToString(distribution->getbackupPowerFault()));
    m_xmlitem_1.setAttribute("IsChargerFault", statusToString(distribution->getchargeFault()));
    m_xmlitem_1.setAttribute("IsOverDischarge", statusToString(distribution->getoverDischargeFault()));
    m_xmlitem_1.setAttribute("Warning", statusToString(distribution->getemergencyStatus()));
    m_xmlitem_1.setAttribute("Is36VInputFault", statusToString(distribution->get36vOut()));
    m_xmlitem_1.setAttribute("IsMainPowerFault", statusToString(distribution->getmainPowerFault()));
    m_xmlitem_1.setAttribute("IsSystemFault", statusToString(distribution->getsystemFault()));
    m_xmlitem_1.setAttribute("IsManualMode", statusToString(distribution->getrunMode()));
    m_xmlitem_1.setAttribute("IsCharging", statusToString(distribution->getchargeStatus()));
    m_xmlitem_1.setAttribute("IsBattery1OverHeating", statusToString(distribution->getbatOverheating()));
    m_xmlitem_1.setAttribute("IsBattery2OverHeating", statusToString(distribution->getbatOverheating()));
    m_xmlitem_1.setAttribute("IsBattery3OverHeating", statusToString(distribution->getbatOverheating()));

    for(int i=1; i<13; i++)
    {
        loop = distribution->loopByAdd(i);
        if(loop)
        {
            m_xmlitem_1.setAttribute(QString("Loop%1State").arg(i), loop->getLoopCommunicationFault() ? "故障":"正常");
            m_xmlitem_1.setAttribute(QString("Loop%1OutputCut").arg(i), (distribution->getloopOpen() & (0x01<<(i-1))) ? "True" : "False");
        }
        else
        {
            m_xmlitem_1.setAttribute(QString("Loop%1State").arg(i), "正常");
            m_xmlitem_1.setAttribute(QString("Loop%1OutputCut").arg(i), "False");
        }
    }
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

//服务端上传灯具状态
void CClientBusiness::serverLampStateUpload(int ID, bool isCommunicationOK, bool isLightFault, bool isWarning)
{
    qDebug() << "CClientBusiness::serverLampStateUpload"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((CGlobal::instance()->m_tcptype & 0x02) == 0)
        return;
    if(m_masterStateUploadFlag == false && m_isQueryAllState == false)
        return;
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("LampStateUpload");
    m_xmlitem_1.setAttribute("LampID", ID);
    m_xmlitem_1.setAttribute("IsCommunicationOK", statusToString(isCommunicationOK));
    m_xmlitem_1.setAttribute("IsLightSourceFault", statusToString(isLightFault));
    m_xmlitem_1.setAttribute("IsWarning", statusToString(isWarning));
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

//服务端上传着火点报警
void CClientBusiness::serverFirePointWarningUpload(int deviceAddress, int loopAddress, int terminalAddress)
{
    qDebug() << "CClientBusiness::serverFirePointWarningUpload"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((CGlobal::instance()->m_tcptype & 0x02) == 0)
        return;
    if(m_masterStateUploadFlag == false)
        return;
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("FirePointWarningUpload");
    m_xmlitem_1.setAttribute("DeviceAddress", deviceAddress);
    m_xmlitem_1.setAttribute("LoopAddress", loopAddress);
    m_xmlitem_1.setAttribute("TerminalAddress", terminalAddress);
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

//服务端上传开关量应急
void CClientBusiness::serverEmergencyInputUpload()
{
    qDebug() << "CClientBusiness::serverEmergencyInputUpload"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((CGlobal::instance()->m_tcptype & 0x02) == 0)
        return;
    if(m_masterStateUploadFlag == false)
        return;
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("EmergencyInputUpload");
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}
//服务端上传手动应急
void CClientBusiness::serverManualLaunchUpload()
{
    qDebug() << "CClientBusiness::serverManualLaunchUpload"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((CGlobal::instance()->m_tcptype & 0x02) == 0)
        return;
    if(m_masterStateUploadFlag == false)
        return;
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("ManualLaunchUpload");
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

//服务端上传灯具指向
void CClientBusiness::serverLampDirectionUpload(int lampID, QString direction)
{
    qDebug() << "CClientBusiness::serverLampDirectionUpload"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((CGlobal::instance()->m_tcptype & 0x02) == 0)
        return;
    if(m_masterStateUploadFlag == false)
        return;
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("LampDirectionUpload");
    m_xmlitem_1.setAttribute("LampID", lampID);
    m_xmlitem_1.setAttribute("Direction", direction);
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}


//服务端接收主机控制指令
void CClientBusiness::serverHostControl(CMsgObjectStatus* xmlObjectStatus)
{
    qDebug() << "CClientBusiness::serverHostControl"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    CMsgNull m_MsgNUll;
    if(xmlObjectStatus->strdata == "启动")
    {
        CMsgNull m_MsgNUll;
        exeCommand(NCT_CancelMute, &m_MsgNUll);
        CGlobal::instance()->DataProcessing()->setSoundIcon(true);
        if(CGlobal::instance()->m_EmergencyOperation == CGlobal::Start)
            return;
        CGlobal::instance()->m_EmergencyOperation = CGlobal::Start;
        exeCommand(NCT_EmergencyStart, &m_MsgNUll);
        Emergency();
    }
    else if(xmlObjectStatus->strdata == "复位")
    {
//        CGlobal::instance()->m_bInLogin = false;
//        CGlobal::instance()->m_isEmergency = false;
//        clearStatusObject(true);
//        clearExpectObject();
//        //复位声明上传到客户端
//        serverResetDeclareUpload();
//        //灯具状态复位
//        DeviceResetStatus();
//        CGlobal::instance()->DlgDevices()->getDeviceInfo();
//        CGlobal::instance()->DlgDevices()->lampBatStatusUpload();
//        //布局视图界面灯具状态更新
//        CGlobal::instance()->designSplashView()->setPageStateUpdate(true);
//        //集中电源状态复位
//        CGlobal::instance()->programDistributionView()->distributionResetStatus();

//        //复位后延迟1s对比检查数据库数据是否在线
//        QTimer::singleShot(1000, this, SLOT(slotCheckLoginInfo()));
//        exeCommand(NCT_Reset, &m_MsgNUll);
    }
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("HostControlConfirm");
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

//服务端自动上传控制开关
void CClientBusiness::serverSetAutoStateUpload(QString enable)
{
    qDebug() << "CClientBusiness::serverSetAutoStateUpload"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if(enable == "True" || enable == "true")
        m_masterStateUploadFlag = true;
    else if(enable == "False" || enable == "false")
        m_masterStateUploadFlag = false;
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("SetAutoStateUploadConfirm");
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

//服务端上传复位声明
void CClientBusiness::serverResetDeclareUpload()
{
    qDebug() << "CClientBusiness::serverResetDeclareUpload"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((CGlobal::instance()->m_tcptype & 0x02) == 0)
        return;
    if(m_masterStateUploadFlag == false)
        return;
    serverXmlInit();
    m_xmlitem_1 = m_xmldoc.createElement("ResetDeclareUpload");
    m_xmlroot.appendChild(m_xmlitem_1);
    CGlobal::instance()->processServer()->replyXmlMsg(m_xmldoc.toByteArray(4));
}

void CClientBusiness::DeviceResetStatus()
{
    qDebug() << "CClientBusiness::DeviceResetStatus"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    //单控灯具复位
    deviceControl devicecontrol;
    devicecontrol.restDeviceControl();
}

void CClientBusiness::FirepointResetStatus()
{
    QList<CFirePoint*> firepoints = CGlobal::instance()->controller()->firepoints();
    for(int i=0; i<firepoints.count(); i++)
    {
        CFirePoint* firepoint = firepoints.at(i);
        if(firepoint)
        {
            if(firepoint->isSimulateEmergency())
            {
                firepoint->setSimulateEmergency(false);
                firepoint->setStatus(OBJS_SimulateFireEmergency,0);
            }
            if(firepoint->isEmergency())
            {
                firepoint->setEmergency(false);
                firepoint->setStatus(OBJS_FireEmergency,0);
            }
        }
    }
}

void CClientBusiness::ForceEmergency()
{
    qDebug() << "CClientBusiness::ForceEmergency"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if(CGlobal::instance()->m_HaveDialog)
        return;
    dlgAdministrator m_Administrator;
    m_Administrator.setWindowTitle("用户验证");
    m_Administrator.setUserDisabled(true);
    if(QDialog::Accepted == m_Administrator.exec())
    {
        CMsgNull m_MsgNUll;
        exeCommand(NCT_CancelMute, &m_MsgNUll);
        CGlobal::instance()->DataProcessing()->setSoundIcon(true);
        if(CGlobal::instance()->m_EmergencyOperation == CGlobal::Start)
            return;
        CGlobal::instance()->m_EmergencyOperation = CGlobal::Start;
        exeCommand(NCT_ForceEmergency, &m_MsgNUll);
        Emergency();
    }
}

void CClientBusiness::InputEmergency()
{
    qDebug() << "CClientBusiness::InputEmergency"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    CMsgNull m_MsgNUll;
    exeCommand(NCT_CancelMute, &m_MsgNUll);
    CGlobal::instance()->DataProcessing()->setSoundIcon(true);
    if(CGlobal::instance()->m_EmergencyOperation == CGlobal::Start)
        return;
    CGlobal::instance()->m_EmergencyOperation = CGlobal::Start;
    exeCommand(NCT_InputEmergency, &m_MsgNUll);
    Emergency();
}

void CClientBusiness::ActiveInputEmergency()
{
    qDebug() << "CClientBusiness::ActiveInputEmergency"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    CMsgNull m_MsgNUll;
    exeCommand(NCT_CancelMute, &m_MsgNUll);
    CGlobal::instance()->DataProcessing()->setSoundIcon(true);
    if(CGlobal::instance()->m_EmergencyOperation == CGlobal::Start)
        return;
    CGlobal::instance()->m_EmergencyOperation = CGlobal::Start;
    exeCommand(NCT_ActiveInputEmergency, &m_MsgNUll);
    Emergency();
}

void CClientBusiness::PassiveInputEmergency()
{
    qDebug() << "CClientBusiness::PassiveInputEmergency"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if(CGlobal::instance()->m_EmergencyOperation == CGlobal::Start)
        return;
    CGlobal::instance()->m_EmergencyOperation = CGlobal::Start;
    CMsgNull m_MsgNUll;
    exeCommand(NCT_PassiveInputEmergency, &m_MsgNUll);
    Emergency();
}

void CClientBusiness::slot_flashControl()
{
    foreach (CMsgObjectStatus* msg, m_msg)
    {
        exeCommand(NCT_FlashControl, msg);
    }
    m_msg.clear();
}

void CClientBusiness::lampControl(CDevice* device, bool isDoubleDirection, bool isLeft)
{
    int direction = 0;
    bool isReverse = false;
    if(device->deviceValue(DEVICE_VALUE_ISREVERSE).toInt() == 0x01)
        isReverse = true;

    CMsgObjectStatus* msgObjectStatus = new CMsgObjectStatus(); // 动态分配对象
    msgObjectStatus->nCanportAdd = device->canportAdd();
    msgObjectStatus->nDisID = device->distributionAdd();
    msgObjectStatus->nLoopID = device->loopAdd();
    msgObjectStatus->nDeviceID = device->deviceAdd();
//    int lamptypeID = device->deviceTypeId();
    if(device->isMAtype())
    {
        if(isDoubleDirection)
        {
            if(isLeft)
            {
                direction |= 0x05;
                DeviceStartStatus(device, "左向", QDateTime::currentDateTime());
            }
            else
            {
                direction |= 0x06;
                DeviceStartStatus(device, "右向", QDateTime::currentDateTime());
            }
        }
        else
        {
            direction |= 0x07;
        }
        direction |= 0x08;
        //发送方向控制指令
        msgObjectStatus->nValue = direction;
        exeCommand(NCT_DeviceDirection, msgObjectStatus);
        return;
    }
    if(CGlobal::instance()->m_TwinkleControl)
    {
        if(isDoubleDirection)
        {
            if(!isLeft || (isLeft && isReverse))
            {
                direction |= 0x06;
                DeviceStartStatus(device, "右向", QDateTime::currentDateTime());
            }
            else
            {
                direction |= 0x05;
                DeviceStartStatus(device, "左向", QDateTime::currentDateTime());
            }
        }
        else
        {
            direction |= 0x07;
        }
        if(CGlobal::instance()->m_lampEmergencyType == 0)
        {
            direction |= 0x08;
            //发送方向控制指令
            msgObjectStatus->nValue = direction;
            exeCommand(NCT_DeviceDirection, msgObjectStatus);
        }
        else if(CGlobal::instance()->m_lampEmergencyType == 0x01)
        {
            if(isDoubleDirection)
            {
                //发送方向控制指令
                msgObjectStatus->nValue = direction;
                exeCommand(NCT_DeviceDirection, msgObjectStatus);
            }
            //发送闪烁指令
            if(CGlobal::instance()->m_isOldLampEmergencyParam)
            {
                msgObjectStatus->nValue = 0x01;
                m_msg.append(msgObjectStatus);
                m_flashTimer->start(6000);
            }
            else
            {
                msgObjectStatus->nValue = 0x01;
                exeCommand(NCT_FlashControl, msgObjectStatus);
            }
        }
        else if(CGlobal::instance()->m_lampEmergencyType == 0x02)
        {
            if(isDoubleDirection)
            {
                direction |= 0x08;
                //发送方向控制指令
                msgObjectStatus->nValue = direction;
                exeCommand(NCT_DeviceDirection, msgObjectStatus);
            }
            else
            {
                //发送闪烁指令
                msgObjectStatus->nValue = 0x01;
                exeCommand(NCT_FlashControl, msgObjectStatus);
                //发送应急指令
                exeCommand(NCT_Emergency, msgObjectStatus);
            }
        }
//        //非智能型灯具
//        if(isUCLamp(lamptypeID))
//        {
//            direction |= 0x07;
//            if(CGlobal::instance()->m_UCSendDirectionAndTwinkleByOne)
//            {
//                //发送方向控制指令
//                msgObjectStatus->nValue = 0x08;
//                exeCommand(NCT_DeviceDirection, msgObjectStatus);
//            }
//            else
//            {
//                //发送闪烁指令
//                msgObjectStatus->nValue = 0x01;
//                exeCommand(NCT_FlashControl, msgObjectStatus);
//                //发送应急指令
//                exeCommand(NCT_Emergency, msgObjectStatus);
//            }
//        }
//        //智能型灯具
//        else
//        {
//            if(isDoubleDirection)
//            {
//                if(!isLeft || (isLeft && isReverse))
//                {
//                    direction |= 0x06;
//                    DeviceStartStatus(device, "右向", QDateTime::currentDateTime());
//                }
//                else
//                {
//                    direction |= 0x05;
//                    DeviceStartStatus(device, "左向", QDateTime::currentDateTime());
//                }
//            }
//            else
//            {
//                direction |= 0x07;
//            }
//            if(CGlobal::instance()->m_SendDirectionAndTwinkleByOne)
//            {
//                direction |= 0x08;
//                //发送方向控制指令
//                msgObjectStatus->nValue = direction;
//                exeCommand(NCT_DeviceDirection, msgObjectStatus);
//            }
//            else
//            {
//                if(isDoubleDirection)
//                {
//                    //发送方向控制指令
//                    msgObjectStatus->nValue = direction;
//                    exeCommand(NCT_DeviceDirection, msgObjectStatus);
//                }
//                //发送闪烁指令
//                msgObjectStatus->nValue = 0x01;
//                if(CGlobal::instance()->m_isOldLampEmergencyParam)
//                {
//                    m_msg.append(msgObjectStatus);
//                    m_flashTimer->start(2000);
//                }
//                else
//                {
//                    exeCommand(NCT_FlashControl, msgObjectStatus);
//                    if(CGlobal::instance()->m_emergencyAfterDirection)
//                    {
//                        //发送应急指令
//                        exeCommand(NCT_Emergency, msgObjectStatus);
//                    }
//                }

//            }
//        }
    }
    else
    {
        if(isDoubleDirection)
        {
            if(!isLeft || (isLeft && isReverse))
                direction |= 0x06;
            else
                direction |= 0x05;
            direction |= 0x08;
            //发送方向控制指令
            msgObjectStatus->nValue = direction;
            exeCommand(NCT_DeviceDirection, msgObjectStatus);
        }
    }
}

void CClientBusiness::Emergency()
{
    qDebug() << "CClientBusiness::Emergency"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    //所有灯
    QSqlQuery query = db.query(QString("SELECT l.Address,c.Loop,a.CanPort,a.Address,r.Number from LampInfo l "
                                       " left join ChannelInfo c on l.ChannelInfoID = c.ID "
                                       " left join CanDeviceInfo a on c.CanDeviceInfoID = a.ID "
                                       " left join LampType r on l.LampTypeID=r.ID "));
    //query.value(0)代表第0列
    //第0列:l.Address        灯具地址
    //第1列:c.Loop           回路地址
    //第2列:a.CanPort        CAN地址
    //第3列:a.Address        集中电源地址
    //第4列:r.Number         灯具型号
    while(query.next())
    {
        int canAddress = query.value(2).toInt()+2;
        int distributionAddress = query.value(3).toInt();
        int loopAddress = query.value(1).toInt();
        int deviceAddress = query.value(0).toInt();
        int lamptypeID = query.value(4).toInt();
        //可变状态灯状态
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canAddress);
        if(!canport)
            continue;
        CDistribution* distribution = canport->distributionByAddress(distributionAddress);
        if(!distribution || distribution->getStatus(OBJS_DistributionCommunicationFault))
            continue;
        CLoop* loop = distribution->loopByAdd(loopAddress);
        if(!loop || loop->getStatus(OBJS_LoopCommunicationFault) || !loop->isLoopLogin())
            continue;
        CDevice* device = loop->deviceByAdd(deviceAddress);
        if(!device || device->getStatus(OBJS_LightCommunicationFault))
            continue;
//        device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"应急");
        //上传灯具状态到云平台/客户端
        uploadLampStatus(device);
        if(CGlobal::instance()->programDeviceView()->type(lamptypeID) == NCT_LightType)
            continue;
        CMsgObjectStatus msgObjectStatus;
        msgObjectStatus.nCanportAdd = canport->canportAdd();
        msgObjectStatus.nDisID = distribution->distributionAddress();
        msgObjectStatus.nLoopID = loop->loopAdd();
        msgObjectStatus.nDeviceID = device->deviceAdd();
        //闪烁(双向灯和地埋灯除外,它们在方向控制时闪烁）
        if(!isDoubleLamp(lamptypeID))
        {
            lampControl(device);
        }
        //可变状态灯
        if(lamptypeID == 109 || lamptypeID == 11 || lamptypeID == 146)   //109/11/146代表可变状态灯
        {
            msgObjectStatus.nValue = true;
            exeCommand(NCT_ChangeControl, &msgObjectStatus);
            DeviceStartStatus(device, "禁止入内", QDateTime::currentDateTime());
        }
    }
    //方向控制(双向灯具或地埋灯)
    QSqlQuery querylampcontrol = db.query(QString("SELECT l.LampDirection,r.Remark,r.Address,c.Loop,a.CanPort,a.Address,r.id,r.LampTypeID,r.IsReverse from EvacuationPathItem l "
                                       " left join LampInfo r on l.LampInfoID=r.ID "
                                       " left join ChannelInfo c on r.ChannelInfoID = c.ID "
                                       " left join CanDeviceInfo a on c.CanDeviceInfoID = a.ID "
                                       " where l.FirePointID is Null "));
    //query.value(0)代表第0列
    //第0列:l.LampDirection  灯具方向
    //第1列:r.Remark         灯具产品编号
    //第2列:r.Address        灯具地址
    //第3列:c.Loop           回路地址
    //第4列:a.CanPort        CAN地址
    //第5列:a.Address        集中电源地址
    //第6列:r.ID             灯具ID
    //第7列:r.LampTypeID     灯具型号ID
    //第8列:r.IsReverse      灯具是否反接
    while(querylampcontrol.next())
    {
        int lampDirection = querylampcontrol.value(0).toInt();
        int canAddress = querylampcontrol.value(4).toInt()+2;
        int distributionAddress = querylampcontrol.value(5).toInt();
        int loopAddress = querylampcontrol.value(3).toInt();
        int deviceAddress = querylampcontrol.value(2).toInt();
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canAddress);
        if(!canport)
            continue;
        CDistribution* distribution = canport->distributionByAddress(distributionAddress);
        if(!distribution || distribution->getStatus(OBJS_DistributionCommunicationFault))
            continue;
        CLoop* loop = distribution->loopByAdd(loopAddress);
        if(!loop || loop->getStatus(OBJS_LoopCommunicationFault) || !loop->isLoopLogin())
            continue;
        CDevice* device = loop->deviceByAdd(deviceAddress);
        if(!device || device->getStatus(OBJS_LightCommunicationFault))
            continue;

        if(!isDoubleLamp(device->deviceTypeId()))
        {
            continue;
        }
        if(lampDirection & 0x01)
        {
            lampControl(device, true, true);
        }
        else if(lampDirection & 0x02)
        {
            lampControl(device, true, false);
        }
        device->setDeviceControl(true);
    }
    deviceControl devicecontrol;
    devicecontrol.saveDeviceControlFile();//保存灯具控制表
    db.close();

//    //方向灯闪烁配置
//    msgObjectStatus.nValue = CGlobal::instance()->m_TwinkleControl;
//    exeCommand(NCT_FlashConfig, &msgObjectStatus);

    //布局视图界面灯具状态更新
    CGlobal::instance()->designSplashView()->setPageStateUpdate(true);

}
//火警点协议联动应急(按分组启动)
void CClientBusiness::firePointEmergencyByGroup(int firePointID)
{
    qDebug() << "CClientBusiness::firePointEmergencyByGroup"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    CFirePoint* firepoint = CGlobal::instance()->controller()->firePointByID(firePointID);
    firepoint->setSimulateEmergency(true);
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSet<int> groups;
    //该火警点对应分组
    QSqlQuery queryGroup = db.query(QString("SELECT g.LaunchGroupID from LampToLaunchGroup g "
                                       " left join LampToFirePoint f on f.LampInfoID=g.LampInfoID "
                                       " where f.FirePointID=%1").arg(firePointID));
    while(queryGroup.next())
    {
        groups.insert(queryGroup.value(0).toInt());
    }
    // 遍历输出结果
    for (QSet<int>::const_iterator it = groups.begin(); it != groups.end(); ++it) {
        //该火警点对应分组的所有灯
        QSqlQuery query = db.query(QString("SELECT l.Address,c.Loop,a.CanPort,a.Address,r.Number from LampInfo l "
                                           " left join ChannelInfo c on l.ChannelInfoID = c.ID "
                                           " left join CanDeviceInfo a on c.CanDeviceInfoID = a.ID "
                                           " left join LampType r on l.LampTypeID=r.ID "
                                           " left join LampToLaunchGroup g on l.ID=g.LampInfoID "
                                           " where g.LaunchGroupID=%1").arg(*it));
        //query.value(0)代表第0列
        //第0列:l.Address        灯具地址
        //第1列:c.Loop           回路地址
        //第2列:a.CanPort        CAN地址
        //第3列:a.Address        集中电源地址
        //第4列:r.Number         灯具型号
        while(query.next())
        {
            int canAddress = query.value(2).toInt()+2;
            int distributionAddress = query.value(3).toInt();
            int loopAddress = query.value(1).toInt();
            int deviceAddress = query.value(0).toInt();
            int lamptypeID = query.value(4).toInt();
            CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canAddress);
            if(!canport)
                continue;
            CDistribution* distribution = canport->distributionByAddress(distributionAddress);
            if(!distribution || distribution->getStatus(OBJS_DistributionCommunicationFault))
                continue;
            CLoop* loop = distribution->loopByAdd(loopAddress);
            if(!loop || loop->getStatus(OBJS_LoopCommunicationFault) || !loop->isLoopLogin())
                continue;
            CDevice* device = loop->deviceByAdd(deviceAddress);
            if(!device || device->getStatus(OBJS_LightCommunicationFault))
                continue;
            device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"应急");
            CMsgObjectStatus msgObjectStatus;
            msgObjectStatus.nCanportAdd = canAddress;
            msgObjectStatus.nDisID = distributionAddress;
            msgObjectStatus.nLoopID = loopAddress;
            msgObjectStatus.nDeviceID = deviceAddress;
            //上传灯具状态到云平台/客户端
            uploadLampStatus(device);
            if(CGlobal::instance()->programDeviceView()->type(lamptypeID) == NCT_LightType)
            {
                msgObjectStatus.nValue = true;
                exeCommand(NCT_Emergency, &msgObjectStatus);
                device->setDeviceControl(true);
                continue;
            }
            //闪烁(双向灯和地埋灯除外,它们在方向控制时闪烁）
            if(!isDoubleLamp(lamptypeID))
            {
                lampControl(device);
            }
            //可变状态灯
            if(lamptypeID == 109 || lamptypeID == 11 || lamptypeID == 146)   //109/11/146代表可变状态灯
            {
                msgObjectStatus.nValue = true;
                exeCommand(NCT_ChangeControl, &msgObjectStatus);
                DeviceStartStatus(device, "禁止入内", QDateTime::currentDateTime());
            }

//            //方向灯闪烁配置
//            msgObjectStatus.nValue = CGlobal::instance()->m_TwinkleControl;
//            exeCommand(NCT_FlashConfig, &msgObjectStatus);
            device->setDeviceControl(true);
        }

        //方向控制(该火警点对应分组的双向灯具或地埋灯)
        QSqlQuery querylampcontrol = db.query(QString("SELECT l.LampDirection,r.Remark,r.Address,c.Loop,a.CanPort,a.Address,r.id,r.LampTypeID,r.IsReverse,l.FirePointID from EvacuationPathItem l "
                                           " left join LampInfo r on l.LampInfoID=r.ID "
                                           " left join ChannelInfo c on r.ChannelInfoID = c.ID "
                                           " left join CanDeviceInfo a on c.CanDeviceInfoID = a.ID "
                                           " left join LampToLaunchGroup g on l.LampInfoID=g.LampInfoID "
                                           " where g.LaunchGroupID=%1 and l.FirePointID=%2").arg(*it).arg(firePointID));
        //query.value(0)代表第0列
        //第0列:l.LampDirection  灯具方向
        //第1列:r.Remark         灯具产品编号
        //第2列:r.Address        灯具地址
        //第3列:c.Loop           回路地址
        //第4列:a.CanPort        CAN地址
        //第5列:a.Address        集中电源地址
        //第6列:r.ID             灯具ID
        //第7列:r.LampTypeID     灯具型号ID
        //第8列:r.IsReverse      灯具是否反接
        while(querylampcontrol.next())
        {
            int lampDirection = querylampcontrol.value(0).toInt();
            int canAddress = querylampcontrol.value(4).toInt()+2;
            int distributionAddress = querylampcontrol.value(5).toInt();
            int loopAddress = querylampcontrol.value(3).toInt();
            int deviceAddress = querylampcontrol.value(2).toInt();
//            int lampID = querylampcontrol.value(6).toInt();
//            int lamptypeID = querylampcontrol.value(7).toInt();
//            int lampIsReverse = querylampcontrol.value(8).toInt();
            CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canAddress);
            if(!canport)
                continue;
            CDistribution* distribution = canport->distributionByAddress(distributionAddress);
            if(!distribution || distribution->getStatus(OBJS_DistributionCommunicationFault))
                continue;
            CLoop* loop = distribution->loopByAdd(loopAddress);
            if(!loop || loop->getStatus(OBJS_LoopCommunicationFault) || !loop->isLoopLogin())
                continue;
            CDevice* device = loop->deviceByAdd(deviceAddress);
            if(!device || device->getStatus(OBJS_LightCommunicationFault))
                continue;
            if(!isDoubleLamp(device->deviceTypeId()))
            {
                continue;
            }
            if(lampDirection & 0x01)
            {
                lampControl(device, true, true);
            }
            else if(lampDirection & 0x02)
            {
                lampControl(device, true, false);
            }
            device->setDeviceControl(true);
        }
    }
    deviceControl devicecontrol;
    devicecontrol.saveDeviceControlFile();//保存灯具控制表
    db.close();

//    //方向灯应急闪烁
//    CMsgObjectStatus msgObjectStatus;
//    msgObjectStatus.nValue = 0x01;
//    exeCommand(NCT_FlashControl, &msgObjectStatus);

//    //方向灯闪烁配置
//    msgObjectStatus.nValue = CGlobal::instance()->m_TwinkleControl;
//    exeCommand(NCT_FlashConfig, &msgObjectStatus);

    //布局视图界面灯具状态更新
    CGlobal::instance()->designSplashView()->setPageStateUpdate(true);

}

//火警点协议联动应急(按页面启动)
void CClientBusiness::firePointEmergencyByLayoutPage(int firePointID)
{
    qDebug() << "CClientBusiness::firePointEmergencyByLayoutPage"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    CFirePoint* firepoint = CGlobal::instance()->controller()->firePointByID(firePointID);
    firepoint->setSimulateEmergency(true);
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSet<int> layoutPages;
    //该火警点对应页面
    QSqlQuery queryLayoutPage = db.query(QString("SELECT c.LayoutPageID from Coordinate c "
                                       " left join LampToFirePoint f on f.LampInfoID=c.LampInfoID "
                                       " where f.FirePointID=%1").arg(firePointID));
    while(queryLayoutPage.next())
    {
        layoutPages.insert(queryLayoutPage.value(0).toInt());
    }
    // 遍历输出结果
    for (QSet<int>::const_iterator it = layoutPages.begin(); it != layoutPages.end(); ++it) {
        //该页面所有灯
        QSqlQuery query = db.query(QString("SELECT l.Address,c.Loop,a.CanPort,a.Address,r.Number,p.layoutPageID from LampInfo l "
                                           " left join ChannelInfo c on l.ChannelInfoID = c.ID "
                                           " left join CanDeviceInfo a on c.CanDeviceInfoID = a.ID "
                                           " left join LampType r on l.LampTypeID=r.ID "
                                           " left join Coordinate p on l.ID=p.LampInfoID "
                                           " where p.layoutPageID=%1").arg(*it));
        //query.value(0)代表第0列
        //第0列:l.Address        灯具地址
        //第1列:c.Loop           回路地址
        //第2列:a.CanPort        CAN地址
        //第3列:a.Address        集中电源地址
        //第4列:r.Number         灯具型号
        while(query.next())
        {
            int canAddress = query.value(2).toInt()+2;
            int distributionAddress = query.value(3).toInt();
            int loopAddress = query.value(1).toInt();
            int deviceAddress = query.value(0).toInt();
            int lamptypeID = query.value(4).toInt();
            CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canAddress);
            if(!canport)
                continue;
            CDistribution* distribution = canport->distributionByAddress(distributionAddress);
            if(!distribution || distribution->getStatus(OBJS_DistributionCommunicationFault))
                continue;
            CLoop* loop = distribution->loopByAdd(loopAddress);
            if(!loop || loop->getStatus(OBJS_LoopCommunicationFault) || !loop->isLoopLogin())
                continue;
            CDevice* device = loop->deviceByAdd(deviceAddress);
            if(!device || device->getStatus(OBJS_LightCommunicationFault))
                continue;
            CMsgObjectStatus msgObjectStatus;
            msgObjectStatus.nCanportAdd = canAddress;
            msgObjectStatus.nDisID = distributionAddress;
            msgObjectStatus.nLoopID = loopAddress;
            msgObjectStatus.nDeviceID = deviceAddress;
            device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"应急");
            if(CGlobal::instance()->programDeviceView()->type(lamptypeID) == NCT_LightType)
            {
                msgObjectStatus.nValue = true;
                exeCommand(NCT_Emergency, &msgObjectStatus);
                device->setDeviceControl(true);
                continue;
            }
            //闪烁(双向灯和地埋灯除外,它们在方向控制时闪烁）
            if(!isDoubleLamp(lamptypeID))
            {
                lampControl(device);
            }
            //可变状态灯
            if(lamptypeID == 109 || lamptypeID == 11 || lamptypeID == 146)   //109/11/146代表可变状态灯
            {
                msgObjectStatus.nValue = true;
                exeCommand(NCT_ChangeControl, &msgObjectStatus);
                DeviceStartStatus(device, "禁止入内", QDateTime::currentDateTime());
            }

//            //方向灯闪烁配置
//            msgObjectStatus.nValue = CGlobal::instance()->m_TwinkleControl;
//            exeCommand(NCT_FlashConfig, &msgObjectStatus);
            device->setDeviceControl(true);
        }
        //方向控制(该火警点对应页面的双向灯具或地埋灯)
        QSqlQuery querylampcontrol = db.query(QString("SELECT l.LampDirection,r.Remark,r.Address,c.Loop,a.CanPort,a.Address,r.id,r.LampTypeID,r.IsReverse,l.FirePointID from EvacuationPathItem l "
                                           " left join LampInfo r on l.LampInfoID=r.ID "
                                           " left join ChannelInfo c on r.ChannelInfoID = c.ID "
                                           " left join CanDeviceInfo a on c.CanDeviceInfoID = a.ID "
                                           " left join Coordinate p on l.LampInfoID=p.LampInfoID "
                                           " where p.LayoutPageID=%1 and l.FirePointID=%2").arg(*it).arg(firePointID));
        //query.value(0)代表第0列
        //第0列:l.LampDirection  灯具方向
        //第1列:r.Remark         灯具产品编号
        //第2列:r.Address        灯具地址
        //第3列:c.Loop           回路地址
        //第4列:a.CanPort        CAN地址
        //第5列:a.Address        集中电源地址
        //第6列:r.ID             灯具ID
        //第7列:r.LampTypeID     灯具型号ID
        //第8列:r.IsReverse      灯具是否反接
        while(querylampcontrol.next())
        {
            int lampDirection = querylampcontrol.value(0).toInt();
            int canAddress = querylampcontrol.value(4).toInt()+2;
            int distributionAddress = querylampcontrol.value(5).toInt();
            int loopAddress = querylampcontrol.value(3).toInt();
            int deviceAddress = querylampcontrol.value(2).toInt();
//            int lampID = querylampcontrol.value(6).toInt();
//            int lamptypeID = querylampcontrol.value(7).toInt();
//            int lampIsReverse = querylampcontrol.value(8).toInt();
            CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canAddress);
            if(!canport)
                continue;
            CDistribution* distribution = canport->distributionByAddress(distributionAddress);
            if(!distribution || distribution->getStatus(OBJS_DistributionCommunicationFault))
                continue;
            CLoop* loop = distribution->loopByAdd(loopAddress);
            if(!loop || loop->getStatus(OBJS_LoopCommunicationFault) || !loop->isLoopLogin())
                continue;
            CDevice* device = loop->deviceByAdd(deviceAddress);
            if(!device || device->getStatus(OBJS_LightCommunicationFault))
                continue;
            if(!isDoubleLamp(device->deviceTypeId()))
            {
                continue;
            }
            if(lampDirection & 0x01)
            {
                lampControl(device, true, true);
            }
            else if(lampDirection & 0x02)
            {
                lampControl(device, true, false);
            }
            device->setDeviceControl(true);
        }
    }
    deviceControl devicecontrol;
    devicecontrol.saveDeviceControlFile();//保存灯具控制表
    db.close();


    //布局视图界面灯具状态更新
    CGlobal::instance()->designSplashView()->setPageStateUpdate(true);

}

//火警点协议联动应急(按集中电源启动)
void CClientBusiness::firePointEmergencyByDistribution(int firePointID)
{
    qDebug() << "CClientBusiness::firePointEmergencyByDistribution"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    int direction = 0;
    CFirePoint* firepoint = CGlobal::instance()->controller()->firePointByID(firePointID);
    firepoint->setSimulateEmergency(true);
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSet< QPair<int,int> > distrbutionPairs;
    //该火警点对应集中电源
    QSqlQuery queryDistribution = db.query(QString("SELECT c.CanPort,c.Address from CanDeviceInfo c "
                                       " left join ChannelInfo l on l.CanDeviceInfoID=c.ID "
                                       " left join LampInfo d on d.ChannelInfoID=l.ID "
                                       " left join LampToFirePoint f on f.LampInfoID=d.ID "
                                       " where f.FirePointID=%1").arg(firePointID));
    while(queryDistribution.next())
    {
        int canport = queryDistribution.value(0).toInt();
        int distributionAddress = queryDistribution.value(1).toInt();
        distrbutionPairs.insert(QPair<int,int>(canport,distributionAddress));
        CMsgObjectStatus msgObjectStatus;
        msgObjectStatus.nCanportAdd = canport+2;
        msgObjectStatus.nDisID = distributionAddress;
        msgObjectStatus.nValue = true;
        exeCommand(NCT_Emergency, &msgObjectStatus);
    }
    // 遍历输出结果
    for (QSet< QPair<int,int> >::const_iterator it = distrbutionPairs.begin(); it != distrbutionPairs.end(); ++it) {
         const QPair<int, int>& pair= *it;
        //该集中电源所有灯
        QSqlQuery query = db.query(QString("SELECT l.Address,c.Loop,a.CanPort,a.Address,r.Number from LampInfo l "
                                           " left join ChannelInfo c on l.ChannelInfoID = c.ID "
                                           " left join CanDeviceInfo a on c.CanDeviceInfoID = a.ID "
                                           " left join LampType r on l.LampTypeID=r.ID "
                                           " where a.CanPort=%1 and a.Address=%2").arg(pair.first).arg(pair.second));
        //query.value(0)代表第0列
        //第0列:l.Address        灯具地址
        //第1列:c.Loop           回路地址
        //第2列:a.CanPort        CAN地址
        //第3列:a.Address        集中电源地址
        //第4列:r.Number         灯具型号
        while(query.next())
        {
            int canAddress = query.value(2).toInt()+2;
            int distributionAddress = query.value(3).toInt();
            int loopAddress = query.value(1).toInt();
            int deviceAddress = query.value(0).toInt();
            int lamptypeID = query.value(4).toInt();
            CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canAddress);
            if(!canport)
                continue;
            CDistribution* distribution = canport->distributionByAddress(distributionAddress);
            if(!distribution || distribution->getStatus(OBJS_DistributionCommunicationFault))
                continue;
            CLoop* loop = distribution->loopByAdd(loopAddress);
            if(!loop || loop->getStatus(OBJS_LoopCommunicationFault) || !loop->isLoopLogin())
                continue;
            CDevice* device = loop->deviceByAdd(deviceAddress);
            if(!device || device->getStatus(OBJS_LightCommunicationFault))
                continue;
            CMsgObjectStatus msgObjectStatus;
            msgObjectStatus.nCanportAdd = canAddress;
            msgObjectStatus.nDisID = distributionAddress;
            msgObjectStatus.nLoopID = loopAddress;
            msgObjectStatus.nDeviceID = deviceAddress;
            device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"应急");
            direction = 0x07;
            if(CGlobal::instance()->programDeviceView()->type(lamptypeID) == NCT_LightType)
            {
//                msgObjectStatus.nValue = true;
//                exeCommand(NCT_Emergency, &msgObjectStatus);
//                device->setDeviceControl(true);
                continue;
            }
            //闪烁(双向灯和地埋灯除外,它们在方向控制时闪烁）
            if(!isDoubleLamp(lamptypeID))
            {
                lampControl(device);
            }

            //可变状态灯
            if(lamptypeID == 109 || lamptypeID == 11 || lamptypeID == 146)   //109/11/146代表可变状态灯
            {
                msgObjectStatus.nValue = true;
                exeCommand(NCT_ChangeControl, &msgObjectStatus);
                DeviceStartStatus(device, "禁止入内", QDateTime::currentDateTime());
            }

//            //方向灯闪烁配置
//            msgObjectStatus.nValue = CGlobal::instance()->m_TwinkleControl;
//            exeCommand(NCT_FlashConfig, &msgObjectStatus);
            device->setDeviceControl(true);
        }
        //方向控制(该该集中电源的双向灯具或地埋灯)
        QSqlQuery querylampcontrol = db.query(QString("SELECT l.LampDirection,r.Remark,r.Address,c.Loop,a.CanPort,a.Address,r.id,r.LampTypeID,r.IsReverse,l.FirePointID from EvacuationPathItem l "
                                           " left join LampInfo r on l.LampInfoID=r.ID "
                                           " left join ChannelInfo c on r.ChannelInfoID = c.ID "
                                           " left join CanDeviceInfo a on c.CanDeviceInfoID = a.ID "
                                           " where a.CanPort=%1 and a.Address=%2 and l.FirePointID=%3").arg(pair.first).arg(pair.second).arg(firePointID));
        //query.value(0)代表第0列
        //第0列:l.LampDirection  灯具方向
        //第1列:r.Remark         灯具产品编号
        //第2列:r.Address        灯具地址
        //第3列:c.Loop           回路地址
        //第4列:a.CanPort        CAN地址
        //第5列:a.Address        集中电源地址
        //第6列:r.ID             灯具ID
        //第7列:r.LampTypeID     灯具型号ID
        //第8列:r.IsReverse      灯具是否反接
        while(querylampcontrol.next())
        {
            int lampDirection = querylampcontrol.value(0).toInt();
            int canAddress = querylampcontrol.value(4).toInt()+2;
            int distributionAddress = querylampcontrol.value(5).toInt();
            int loopAddress = querylampcontrol.value(3).toInt();
            int deviceAddress = querylampcontrol.value(2).toInt();
//            int lampID = querylampcontrol.value(6).toInt();
//            int lamptypeID = querylampcontrol.value(7).toInt();
//            int lampIsReverse = querylampcontrol.value(8).toInt();
            CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canAddress);
            if(!canport)
                continue;
            CDistribution* distribution = canport->distributionByAddress(distributionAddress);
            if(!distribution || distribution->getStatus(OBJS_DistributionCommunicationFault))
                continue;
            CLoop* loop = distribution->loopByAdd(loopAddress);
            if(!loop || loop->getStatus(OBJS_LoopCommunicationFault) || !loop->isLoopLogin())
                continue;
            CDevice* device = loop->deviceByAdd(deviceAddress);
            if(!device || device->getStatus(OBJS_LightCommunicationFault))
                continue;
            if(!isDoubleLamp(device->deviceTypeId()))
            {
                continue;
            }
            if(lampDirection & 0x01)
            {
                lampControl(device, true, true);
            }
            else if(lampDirection & 0x02)
            {
                lampControl(device, true, false);
            }
            device->setDeviceControl(true);
        }
    }
    deviceControl devicecontrol;
    devicecontrol.saveDeviceControlFile();//保存灯具控制表
    db.close();


    //布局视图界面灯具状态更新
    CGlobal::instance()->designSplashView()->setPageStateUpdate(true);
}

//判断是否为非智能型灯具
bool CClientBusiness::isUCLamp(int ID)//ID为数据库中LampType表中ID
{
    qDebug() << "CClientBusiness::isUCLamp"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((ID >= 34) && (ID <= 42))//集中电源型
        return true;
    else if((ID >= 65) && (ID <= 72))//自带电池型
        return true;
    else
        return false;
}

//判断是否为双向或地埋灯具
bool CClientBusiness::isDoubleLamp(int lamptypeID)
{
    qDebug() << "CClientBusiness::isDoubleLamp"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if(lamptypeID == 102 || lamptypeID == 106 || lamptypeID == 3 || lamptypeID == 9 ||
       lamptypeID == 17 || lamptypeID == 44 || lamptypeID == 117 || lamptypeID == 144 ||
       lamptypeID == 132 || lamptypeID == 136 || lamptypeID == 32 || lamptypeID == 36 ||
       lamptypeID == 121 || lamptypeID == 123)
        return true;
    else
        return false;
}


void CClientBusiness::timeOut()
{
    qDebug() << "CClientBusiness::timeOut"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if(!m_isRecvStatus && CGlobal::instance()->dm()->isLoad())
    {
        CMsgObjectStatusInfo msgObjectStatusInfo;
        msgObjectStatusInfo.statusId = m_currentStatusId;
    }
    else
    {
        static int notRecvStatusCount = 0;
        static int lastRecvStatusNumber = 0;
        if(lastRecvStatusNumber == m_currentRecvStatusNumber)
            notRecvStatusCount++;
        lastRecvStatusNumber = m_currentRecvStatusNumber;
        if(notRecvStatusCount > 10)
        {
            notRecvStatusCount = 0;
            lastRecvStatusNumber = 0;
            m_isRecvStatus = false;
            m_currentRecvStatusNumber = 0;
        }
    }
}

//void CClientBusiness::driveDeviceDirection(CPointDeviceItem *item, uchar direction, uchar delay, bool isFlow)
//{
//    CDevice* device = static_cast<CDevice*>(item->device());
//    CLoop* loop = device->loop();
//    CDistribution* distribution = loop->distribution();
//    CMsgObjectStatus objectStatus;
//    objectStatus.nType = isFlow?1:0;
//    objectStatus.nPanelID = CGlobal::instance()->panelAddress();
//    objectStatus.nDisID = distribution->distributionId();
//    objectStatus.nLoopID = loop->loopId();
//    objectStatus.nDeviceID = device->deviceId();
//    objectStatus.nStatusID = direction;
//    objectStatus.nValue = delay;
//    objectStatus.nTimes = 0;
//    exeCommand(NCT_ObjectStatus, &objectStatus);
//}

void CClientBusiness::objectStatusChanged(CObject* object, short status, unsigned char oldValue, unsigned char newValue, unsigned int time)
{
    qDebug() << "CClientBusiness::objectStatusChanged" << object->type() << status << oldValue << newValue
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    Q_UNUSED(oldValue);

    if(!object)
        return;
    QString objectType = object->type();
    if(objectType != CController::type_s && objectType != CDistribution::type_s  &&
            objectType != CFirePoint::type_s && objectType != CLoop::type_s && objectType != CDevice::type_s)
        return;
    if(object->isFaultStatus())
        m_faultObjects.insert(object->keyId());
    else
        m_faultObjects.remove(object->keyId());
    if(object->isEmergencyStatus())
        m_startObjects.insert(object->keyId());
    else
        m_startObjects.remove(object->keyId());
    QString strType = "";
    if(status == OBJS_LoginDeviceType)
        object->setValue("device_typeId",newValue);
    m_statusInfo.keyId = object->keyId();
    m_statusInfo.statusId = status;
    m_statusInfo.type = object->typeStr();
    m_statusInfo.time = time ? QDateTime::fromTime_t(time) : QDateTime::currentDateTime();
    m_statusInfo.layer = "";

//    if(status == OBJS_None && newValue == OBJS_None)
//    {
//        emit statusRecord(m_statusInfo, SRT_ALL, SRO_DelAll);
//        return;
//    }
    int type = 0;
    if(status == OBJS_Fire)
        type = SRT_Start;
    else if(CGlobal::instance()->isEmergencyStatus(status))
        type = SRT_Emergency;
    else if(CGlobal::instance()->isFaultStatus(status))
        type = SRT_Fault;
    else
        return;
    QString strtext = CGlobal::instance()->getObjectStatusDescription(object, status, newValue);
    if(objectType == CController::type_s)
    {
        strType = "系统";
        m_msgLog->User = strType;
        m_msgLog->Type = (type==SRT_Emergency?"启动":"故障");
        m_msgLog->Time = QDateTime::currentDateTime();
        m_msgLog->Remark = "控制器" + CGlobal::instance()->getObjectStatusStr(status) + CGlobal::instance()->getObjectStatusSuffix(status, newValue);
        m_msgLog->title = m_msgLog->Remark;
        if(type != SRT_Emergency)
            exeCommand(NCT_AddLog, m_msgLog);
        else
            type = SRT_Start;

        if(status == OBJS_SimulateFireEmergency)
            exeCommand(NCT_AddLog, m_msgLog);
        m_statusInfo.distributionId = "";
        m_statusInfo.loopID = 0;
        m_statusInfo.deviceID = 0;
        m_statusInfo.status = CGlobal::instance()->getObjectStatusStr(status) + CGlobal::instance()->getObjectStatusSuffix(status, newValue);
    }
    else if(objectType == CFirePoint::type_s)
    {
        strType = "火警点";
        CFirePoint* firepoint = static_cast<CFirePoint*>(object);
        m_msgLog->User = strType;
        m_msgLog->Type = "启动";
        m_msgLog->Time = QDateTime::currentDateTime();
        m_msgLog->Area = firepoint->firePointValue(FIREPOINT_VALUE_AREA).toString();
        m_msgLog->Location = firepoint->firePointValue(FIREPOINT_VALUE_LOCATION).toString();
        m_msgLog->Remark = CGlobal::instance()->getObjectStatusStr(status) + CGlobal::instance()->getObjectStatusSuffix(status, newValue);
        m_msgLog->Canport = 0;
        m_msgLog->CanDeviceAddress = firepoint->firePointDeviceAddress();
        m_msgLog->Loop = firepoint->firePointLoopAddress();
        m_msgLog->LampDeviceAddress = firepoint->firePointAddress();
        m_msgLog->title = strtext;
        type = SRT_Start;
        exeCommand(NCT_AddLog, m_msgLog);

        m_statusInfo.distributionId = QString::number(m_msgLog->CanDeviceAddress);
        m_statusInfo.loopID = m_msgLog->Loop;
        m_statusInfo.deviceID = m_msgLog->LampDeviceAddress;
        m_statusInfo.area = m_msgLog->Area;
        m_statusInfo.location = m_msgLog->Location;
        m_statusInfo.status = m_msgLog->Remark;
    }
    else if(objectType == CDistribution::type_s)
    {
        CDistribution* distribution = static_cast<CDistribution*>(object);
        strType = distribution->distributionValue((DISTRIBUTION_VALUE_NAME)).toString();
        m_msgLog->User = strType;
        if(type==SRT_Start)
            m_msgLog->Type = "启动";
        else if(type==SRT_Emergency)
            m_msgLog->Type = "应急";
        else
            m_msgLog->Type = "故障";
        m_msgLog->Time = QDateTime::currentDateTime();
        m_msgLog->Area = distribution->distributionValue(DISTRIBUTION_VALUE_AREA).toString();
        m_msgLog->Location = distribution->distributionValue(DISTRIBUTION_VALUE_LOCATION).toString();
        if(CGlobal::instance()->getObjectStatusStr(status) == "通讯故障" && newValue)
            m_msgLog->Remark = CGlobal::instance()->getObjectStatusStr(status) + "(所属回路、灯具通讯故障)";
        else
            m_msgLog->Remark = CGlobal::instance()->getObjectStatusStr(status);
        m_msgLog->Remark = m_msgLog->Remark + CGlobal::instance()->getObjectStatusSuffix(status, newValue);
        m_msgLog->Canport = distribution->canportAddress()-2;
        m_msgLog->CanDeviceAddress = distribution->distributionAddress();
        m_msgLog->Loop = 0;
        m_msgLog->LampDeviceAddress = 0;
        m_msgLog->title = strtext;
        exeCommand(NCT_AddLog, m_msgLog);

        m_statusInfo.distributionId = "CAN" + QString::number(distribution->canportAddress()-2)
                                      + "-" + QString::number(distribution->distributionAddress());
        m_statusInfo.loopID = 0;
        m_statusInfo.deviceID = 0;
        m_statusInfo.area = m_msgLog->Area;
        m_statusInfo.location = m_msgLog->Location;
        m_statusInfo.status = m_msgLog->Remark;
    }
    else if(objectType == CLoop::type_s)
    {
        strType = "回路";
        CLoop* loop = static_cast<CLoop*>(object);
        m_msgLog->User = strType;
        m_msgLog->Type = (type==SRT_Start?"启动":"故障");
        m_msgLog->Time = QDateTime::currentDateTime();
        m_msgLog->Area = "";
        m_msgLog->Location = "";
        if(CGlobal::instance()->getObjectStatusStr(status) == "通讯故障" && newValue)
            m_msgLog->Remark = CGlobal::instance()->getObjectStatusStr(status) + "(所属灯具通讯故障)";
        else
            m_msgLog->Remark = CGlobal::instance()->getObjectStatusStr(status);
        m_msgLog->Remark = m_msgLog->Remark + CGlobal::instance()->getObjectStatusSuffix(status, newValue);
        m_msgLog->Canport = loop->canportAdd()-2;
        m_msgLog->CanDeviceAddress = loop->distributionAdd();
        m_msgLog->Loop = loop->loopAdd();
        m_msgLog->LampDeviceAddress = 0;
        m_msgLog->title = strtext;
        if(type != SRT_Emergency)
            exeCommand(NCT_AddLog, m_msgLog);
        m_statusInfo.distributionId = "CAN" + QString::number(loop->canportAdd()-2)
                                      + "-" + QString::number(loop->distributionAdd());
        m_statusInfo.loopID = m_msgLog->Loop;
        m_statusInfo.deviceID = 0;
        m_statusInfo.area = "";
        m_statusInfo.location = "";
        m_statusInfo.status = m_msgLog->Remark;
    }
    else if(objectType == CDevice::type_s)
    {
        strType = "灯具";
        CDevice* device = static_cast<CDevice*>(object);
        m_msgLog->User = strType;
        m_msgLog->Type = (type==SRT_Start?"启动":"故障");
        m_msgLog->Time = QDateTime::currentDateTime();
        m_msgLog->Area = device->deviceValue(DEVICE_VALUE_AREA).toString();
        m_msgLog->Location = device->deviceValue(DEVICE_VALUE_LOCATION).toString();
        m_msgLog->Remark = CGlobal::instance()->getObjectStatusStr(status) + CGlobal::instance()->getObjectStatusSuffix(status, newValue);
        m_msgLog->Canport = device->canportAdd()-2;
        m_msgLog->CanDeviceAddress = device->distributionAdd();
        m_msgLog->Loop = device->loopAdd();
        m_msgLog->LampDeviceAddress = device->deviceAdd();
        m_msgLog->title = strtext;
        if(type != SRT_Emergency)
            exeCommand(NCT_AddLog, m_msgLog);

        m_statusInfo.distributionId = "CAN" + QString::number(device->canportAdd()-2)
                                      + "-" + QString::number(device->distributionAdd());
        m_statusInfo.loopID = m_msgLog->Loop;
        m_statusInfo.deviceID = m_msgLog->LampDeviceAddress;
        m_statusInfo.area = m_msgLog->Area;
        m_statusInfo.location = m_msgLog->Location;
        m_statusInfo.status = m_msgLog->Remark;
    }
    int opr = newValue ? SRO_Add : SRO_Del;
    CGlobal::instance()->DataProcessing()->slotStateInformation(m_statusInfo, type, opr);
    qDebug() << "CClientBusiness::objectStatusChanged" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
}

void CClientBusiness::DeviceStartStatus(CDevice* device, QString directionStatus, QDateTime time)
{
    qDebug() << "CClientBusiness::DeviceStartStatus"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    StatusInfo statusInfo;
    if(device)
    {
        int type = CGlobal::instance()->programDeviceView()->type(device->deviceTypeId());
        if(type == NCT_ArrowDoubleType)
        {
            if(directionStatus == "左向")
            {
                device->setDeviceValue(DEVICE_VALUE_RIGHTCLOSE,1);
                device->setDeviceValue(DEVICE_VALUE_LEFTCLOSE,0);
            }
            else if(directionStatus == "右向")
            {
                device->setDeviceValue(DEVICE_VALUE_LEFTCLOSE,1);
                device->setDeviceValue(DEVICE_VALUE_RIGHTCLOSE,0);
            }
        }
        statusInfo.keyId = device->keyId();
//                statusInfo.code = device->codeStr();
        statusInfo.type = device->typeStr();
//                statusInfo.description = device->descriptionStr();
        statusInfo.time = time;
        statusInfo.status = directionStatus;
        statusInfo.layer = "";
        statusInfo.distributionId = "CAN" + QString::number(device->canportAdd()-2)
                                      + "-" + QString::number(device->distributionAdd());
        statusInfo.loopID = device->deviceValue(DEVICE_VALUE_LOOPADDR).toInt();
        statusInfo.deviceID = device->deviceValue(DEVICE_VALUE_ADDR).toInt();
        statusInfo.area = device->deviceValue(DEVICE_VALUE_AREA).toString();
        statusInfo.location = device->deviceValue(DEVICE_VALUE_LOCATION).toString();
        emit statusRecord(statusInfo, SRT_Start, SRO_Add);
    }
}


bool CClientBusiness::hasStandbyPowerFault(CCanport *canport) const
{
    qDebug() << "CClientBusiness::hasStandbyPowerFault"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if(canport->getStatus(OBJS_StandbyPowerFault) ||
            canport->getStatus(OBJS_BatteryLowQuantityFault) ||
            canport->getStatus(OBJS_BatteryLowVoltageFault) ||
            canport->getStatus(OBJS_BatteryOverDischargeFault) ||
            canport->getStatus(OBJS_BatteryLineFault) ||
            canport->getStatus(OBJS_ChargerFault) ||
            canport->getStatus(OBJS_PowerOutputFault))
        return true;
    else
        return false;
}

void CClientBusiness::createPath(const QString &path) const
{
    qDebug() << "CClientBusiness::createPath"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    QDir dir(path);
    if(!dir.exists())
        dir.mkpath(path);
}

void CClientBusiness::InitController()
{
    qDebug() << "CClientBusiness::InitController"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if(m_isInitController)
        return;
    if(!CGlobal::instance()->dm()->isLoad()){
        CGlobal::instance()->setWorkspaces(m_projectPath);
        CProgramTreeView *programTreeView = CGlobal::instance()->programTreeView();
        programTreeView ->openController();

    }
    if(!CGlobal::instance()->dm()->isLoad())
    {
        CGlobal::instance()->dm()->loadDefault();
        CController* controller = CGlobal::instance()->dm()->controllerAt(0);
        CGlobal::instance()->programTreeView()->setController(controller);
        CGlobal::instance()->designTreeView()->setController(controller);
        CGlobal::instance()->designTreeView()->setCurrentView(1);
    }
    QList<QVariant> param;
    param.append(0);
    emit notifyInformation(NIT_InitControllerResult, param);
    m_faultObjects.clear();
    m_startObjects.clear();
    m_isInitController = true;
}

void CClientBusiness::clearExpectObject(const int type)
{
    qDebug() << "CClientBusiness::clearExpectObject"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if(type == SRT_ALL)
    {
        m_faultObjects.clear();
        m_startObjects.clear();
    }
    else if(type == SRT_Start)
    {
        m_startObjects.clear();
    }
    else if(type == SRT_Fault)
    {
        m_faultObjects.clear();
    }

}

void CClientBusiness::updateIsLoopRemoveLogin()
{
    qDebug() << "CClientBusiness::updateIsLoopRemoveLogin"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
        if(canport){
            QList<CDistribution*> list = canport->distributions();
            for(int i=0; i<list.count(); i++){
                CDistribution* distribution = list.at(i);
                if(!distribution)
                    continue;
                if(distribution->isDistributionOnline())
                {
                    QList<CLoop*> loops = distribution->loops();
                    for(int i=0; i<loops.count(); i++){
                        CLoop* loop = loops.at(i);
                        if(!loop)
                            continue;
                    }
                }
            }
        }
    }
}

void CClientBusiness::removeLoginObject(int canportAdd, int distributionAdd, int loopAdd)
{
    qDebug() << "CClientBusiness::removeLoginObject"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if(!canportAdd && !distributionAdd && !loopAdd)
    {
        for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
        {
            CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
            if(canport){
//                canport->setCanportValue(OBJECT_VALUE_ONLINE,0);
                QList<CDistribution*> list = canport->distributions();
                for(int i=0; i<list.count(); i++){
                    CDistribution* distribution = list.at(i);
                    if(distribution)
                    {
                        distribution->setDistributionOnline(false);
                        distribution->setStatus(OBJS_Online, 0);
                        QList<CLoop*> loops = distribution->loops();
                        for(int i=0; i<loops.count(); i++){
                            CLoop* loop = loops.at(i);
                            //清除回路状态
                            if(loop)
                            {
                                QList<CDevice*> devices = loop->devices();
                                for(int m=0; m<devices.count(); m++){
                                    CDevice* device = devices.at(m);
                                    if(device)
                                    {
                                        device->setDeviceLogin(false);
                                        device->setDatabaseLogin(false);
                                        device->setDeviceOnline(false);
//                                        device->setDeviceValue(DEVICE_VALUE_NOLOGIN,1);
//                                        device->setDeviceValue(OBJECT_VALUE_ONLINE,0);
//                                        //配置且注册
//                                        if(device->deviceValue(DEVICE_VALUE_ISUSED) == 1)
//                                        {
//                                            //仅配置
//                                            device->setDeviceValue(DEVICE_VALUE_ISUSED, 0);
//                                        }
//                                        //仅注册
//                                        else if(device->deviceValue(DEVICE_VALUE_ISUSED) == 2)
//                                        {
//                                            //未定义
//                                            device->setDeviceValue(DEVICE_VALUE_ISUSED, 3);
//                                        }
                                    }
                                }
                                loop->setLoopLogin(false);
//                                loop->setloopValue(LOOP_VALUE_NOLOGIN,1);
                                //清除注册回路灯具数量置零
                                loop->setDeviceOnlineTotal(0);
                                loop->resetLoopPower();
//                                loop->setDeviceOnlineTotalBak(0);
                                devices.clear();
                            }
                        }
                        loops.clear();
                    }
                }
                list.clear();
                CGlobal::instance()->InformationWindow()->hide();
            }
        }
    }
    else
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canportAdd);
        if(canport){
            if(distributionAdd == 0)
            {
                canport->setCanportValue(OBJECT_VALUE_ONLINE,0);
            }
            QList<CDistribution*> list = canport->distributions();
            for(int i=0; i<list.count(); i++){
                CDistribution* distribution = list.at(i);
                if(distribution)
                {
                    if((distributionAdd == 0) || (distribution->distributionAddress() == distributionAdd)){
                        if(loopAdd == 0)
                        {
                            distribution->setDistributionOnline(false);
                            distribution->setDistributionLogin(false);
                            distribution->setStatus(OBJS_Online, 0);
                        }
                        QList<CLoop*> loops = distribution->loops();
                        for(int i=0; i<loops.count(); i++){
                            CLoop* loop = loops.at(i);
                            if(!loop)
                                continue;
                            //当回路为0（清除整个集中电源）或者回路地址正确时清除回路状态
                            if((loopAdd == 0) || (loopAdd == loop->loopAdd())){
                                QList<CDevice*> devices = loop->devices();
                                for(int m=0; m<devices.count(); m++){
                                    CDevice* device = devices.at(m);
                                    if(device)
                                    {
                                        device->setDatabaseLogin(false);
                                        device->setDeviceLogin(false);
                                        device->setDeviceOnline(false);
//                                        device->setDeviceValue(DEVICE_VALUE_NOLOGIN,1);
//                                        device->setDeviceValue(OBJECT_VALUE_ONLINE,0);
//                                        //配置且注册
//                                        if(device->deviceValue(DEVICE_VALUE_ISUSED) == 1)
//                                        {
//                                            //仅配置
//                                            device->setDeviceValue(DEVICE_VALUE_ISUSED, 0);
//                                        }
//                                        //仅注册
//                                        else if(device->deviceValue(DEVICE_VALUE_ISUSED) == 2)
//                                        {
//                                            //未定义
//                                            device->setDeviceValue(DEVICE_VALUE_ISUSED, 3);
//                                        }
                                    }
                                }
//                                loop->setloopValue(LOOP_VALUE_NOLOGIN,1);
                                loop->setLoopLogin(false);
                                //清除注册回路灯具数量置零
                                loop->setDeviceOnlineTotal(0);
                                loop->resetLoopPower();
//                                loop->setDeviceOnlineTotalBak(0);
                                devices.clear();
                            }
                        }
                        loops.clear();
                    }
                }

            }
            list.clear();
            CGlobal::instance()->InformationWindow()->hide();
        }
    }
    ConfigFileManager cfg;
    cfg.SaveConfigFile();//写入配置文件
}

void CClientBusiness::closeController()
{
    qDebug() << "CClientBusiness::closeController"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    CGlobal::instance()->programTreeView()->closeController();
    m_currentStatusId = 0;
    m_currentRecvStatusId = 0;
    m_currentRecvStatusNumber = 0;
}

void CClientBusiness::clearProjectFile()
{
    qDebug() << "CClientBusiness::clearProjectFile"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    QFile file(m_projectFile);
    file.remove();
}

void CClientBusiness::reset()
{
    qDebug() << "CClientBusiness::reset"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    QList<QVariant> param;
    m_bForceEmergency = true;
    emit notifyInformation(NIT_Reset, param);
}

void CClientBusiness::clearStatusObject(bool reserveLogin)
{
    qDebug() << "CClientBusiness::clearStatusObject"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    m_currentStatusId = 0;
    m_currentRecvStatusId = 0;
    m_currentRecvStatusNumber = 0;
    CGlobal::instance()->dm()->clearStatusObjects(reserveLogin);
}



CCanport *CClientBusiness::createUndefinedCanport(CController *controller, int address, int keyId)
{
    qDebug() << "CClientBusiness::createUndefinedCanport"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    CCanport* canport = new CCanport(keyId);
    if(!canport)
        return NULL;
    canport->setCanportId(address);
    canport->setIsUndefined(true);
    canport->setCanportValue(CANPORT_VALUE_NAME, canport->canportValue(CANPORT_VALUE_NAME).toString().append(QString::number(address)));
    if(!controller->addCanport(canport))
    {
        delete canport;
        canport = NULL;
    }
    return canport;
}

CDistribution *CClientBusiness::createUndefinedDistribution(CCanport *canport, int address, QString str, int keyId)
{
    qDebug() << "CClientBusiness::createUndefinedDistribution"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    CDistribution* distribution = new CDistribution(keyId);
    if(!distribution)
        return NULL;
    distribution->setDistributionId(address);
    distribution->setIsUndefined(true);
    distribution->setDistributionValue(DISTRIBUTION_VALUE_DESCRIPTION,str);
    distribution->setDistributionValue(DISTRIBUTION_VALUE_ADDR,address);
    int canportID = canport->canportValue(CANPORT_VALUE_PORT).toInt();
//    distribution->setDistributionValue(DISTRIBUTION_VALUE_NAME, distribution->distributionValue(DISTRIBUTION_VALUE_NAME).toString().append(QString::number(address)));
    //1为工程，63为控制器，100-1000为集中电源(CAN1为101-200，CAN2为201-300......)，1000-2000为回路，大于2000为灯具
    for(int i = 101; i<1000; i++)
    {
        if(canport->isdistributionByKeyId(i+(canportID-1)*100) == true)
        {
            distribution->setKeyId(i+(canportID-1)*100);
            break;
        }
    }
    if(!canport->addDistribution(distribution))
    {
        delete distribution;
        distribution = NULL;
    }
    return distribution;
}

CLoop *CClientBusiness::createUndefinedLoop(CDistribution* distribution, int address, int keyId)
{
    qDebug() << "CClientBusiness::createUndefinedLoop"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    CLoop* loop = new CLoop(keyId);
    if(!loop)
        return NULL;
    loop->setLoopId(address);
    loop->setIsUndefined(true);
    loop->setloopValue(LOOP_VALUE_NAME, loop->loopValue(LOOP_VALUE_NAME).toString().append(QString::number(address)));
    //1为工程，63为控制器，100-1000为集中电源，1000-2000为回路，大于2000为灯具
    for(int i = 1001; i<2000; i++)
    {
        if(distribution->loopByKeyId(i) == true)
        {
            loop->setKeyId(i);
            break;
        }
    }

    if(!distribution->addLoop(loop))
    {
        delete loop;
        loop = NULL;
    }
    return loop;
}

CDevice *CClientBusiness::createUndefinedDevice(CLoop* loop)
{
    qDebug() << "CClientBusiness::createUndefinedDevice"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    loop->initDevices();
    QList<CDevice*> devices = loop->devices();
    CDevice* device = devices.at(loop->deviceCount()-1);
    if(!device)
        return NULL;
    //1为工程，63为控制器，100-1000为集中电源，1000-2000为回路，大于2000为灯具
    for(int i = 2001; i<10000; i++)
    {
        if(loop->deviceByKeyId(i) == true)
        {
            device->setKeyId(i);
            break;
        }
    }
    devices.clear();
    return device;
}

void CClientBusiness::slot_saveLoginData()
{
    qDebug() << "CClientBusiness::slot_saveLoginData"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    ConfigFileManager cfg;
    cfg.SaveConfigFile();//写入配置文件
}

int CClientBusiness::deviceType(int typeId)
{
    qDebug() << "CClientBusiness::deviceType"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if((typeId >= 150 && typeId < 200) || (typeId >= 50 && typeId < 100))
        return NCT_LightType;
    else if(typeId >= 120 && typeId <= 124)
        return NCT_ArrowType;
    else
        return NCT_UndergroundType;
}

//void CClientBusiness::slot_ledStatusSend()
//{
//    CMsgLedInfo msgLed;
//    msgLed.nLedID = 4;
//    msgLed.nLedStatus = m_faultObjects.isEmpty()?0:1;
//    exeCommand(NCT_LedStatus, &msgLed);
//    msgLed.nLedID = 5;
//    msgLed.nLedStatus = m_startObjects.isEmpty()?0:1;
//    exeCommand(NCT_LedStatus, &msgLed);
//}

void CClientBusiness::ParsePacket(int nMsgType, const char *data, int nLen)
{
    qDebug() << "CClientBusiness::ParsePacket"
             << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    m_communication->ParsePacket(nMsgType, data, nLen);
}
