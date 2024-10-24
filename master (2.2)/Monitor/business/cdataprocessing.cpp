#include "cdataprocessing.h"
#include "cglobal.h"
#include "dialog/dlgsystemrecord.h"
#include "dialog/dlgsystemrecordmodel.h"
#include "dialog/dlgadministrator.h"
#include "dialog/dlgusetime.h"
#include "dialog/dlgtimeinterval.h"
#include "dialog/dlgdevices.h"

#include "dm/cdm.h"
#include "dm/ccontroller.h"
#include "dm/ccanport.h"
#include "dm/cdistribution.h"
#include "dm/cloop.h"
#include "dm/cdevice.h"
#include "graphics/configfilemanager.h"

#include "model/cinformationmodel.h"

#include "widget/cdesigntreeview.h"
#include "widget/cdesigncontainer.h"
#include "widget/ctoptoolbar.h"
#include "widget/cbottomtoolbar.h"
#include "widget/ccenterframe.h"
#include "widget/cstatustabs.h"
#include "widget/cdesignsplashview.h"
#include "widget.h"

#include <QMessageBox>
#include <QDebug>
#include <QApplication>
#include <QSqlQuery>

CDataProcessing::CDataProcessing(QObject *parent) :
    QObject(parent)
{
    initMember();
    selfCheckSet();
    m_checkTimer = new QTimer(this);
    m_Timer = new QTimer(this);
    m_useTimeCheck = new QTimer(this);
    m_isStatusTab = false;
    m_useTimeCheck->start(86400000);  //一天
    connect(m_checkTimer, SIGNAL(timeout()), this, SLOT(slotChangeColor()));
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(slotcheckLoginInfo()));
    connect(m_useTimeCheck, SIGNAL(timeout()), this, SLOT(slot_usetimeCheck()));
    connect(CGlobal::instance()->ClientBusiness(), SIGNAL(notifyInformation(int,QList<QVariant>)), this, SLOT(slotNotifyInformation(int,QList<QVariant>)));
    connect(CGlobal::instance()->ClientBusiness(), SIGNAL(commandResult(int,CMsgStructBase*)), this, SLOT(slotCommandResult(int,CMsgStructBase*)));
    connect(CGlobal::instance()->ClientBusiness(), SIGNAL(requestVerifyAdmin(int,CMsgStructBase*)), this, SLOT(slotVerifyAdmin(int,CMsgStructBase*)));
    connect(CGlobal::instance()->ClientBusiness(), SIGNAL(statusRecord(StatusInfo,int,int)), this, SLOT(slotStateInformation(StatusInfo,int,int)));
}

CDataProcessing::~CDataProcessing()
{
    delete m_Timer;
    delete m_checkTimer;
    delete m_checkLabel;
    delete m_useTimeCheck;
}

void CDataProcessing::slot_usetimeCheck()
{
    CGlobal::instance()->m_LiftTime = CGlobal::instance()->m_LiftTime - 1;
    CGlobal::instance()->TimeInterval()->setUseTime(CGlobal::instance()->m_LiftTime);
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fileName);
    if(!db.open())
        return;
    QSqlQuery query(db);
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('LiftTime', '%1');").arg(CGlobal::instance()->m_LiftTime));
    db.commit();
    db.close();
    if(CGlobal::instance()->m_LiftTime == 0)
    {
        dlgUseTime useTime;
        useTime.exec();
    }
}

void CDataProcessing::initMember()
{
    m_colorNum = 0;
    soundName = ST_None;
    soundPlay = true;
    m_informationModel = CGlobal::instance()->statusTabs()->m_informationModel;
    m_tabList = CGlobal::instance()->statusTabs()->m_tabList;
    m_tabWidget = CGlobal::instance()->statusTabs()->m_tabWidget;
}

void CDataProcessing::selfCheckSet()
{
    m_checkLabel = new QLabel;
    m_checkLabel->setWindowFlags(Qt::WindowStaysOnTopHint);
    QFont ft;
    ft.setPointSize(50);
    m_checkLabel->setFont(ft);
    m_checkLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    m_checkLabel->setText(QString("<font color=white>%1</font>").arg("正在加载......"));
    m_checkLabel->setStyleSheet("background-color:black");
    m_checkLabel->showFullScreen();

    colorList.append("background-color:red");
    colorList.append("background-color:green");
    colorList.append("background-color:blue");
}

void CDataProcessing::slotChangeColor()
{
    if(m_colorNum == 3)
    {
        QList<QVariant> data;
        selfCheckStop(data);
        m_Timer->start(3000);
        m_checkLabel->setStyleSheet("background-color:white");
        CGlobal::instance()->m_widget->showFullScreen();
    }
    else
    {
        m_checkLabel->setText(QString("<font color=black>%1</font>").arg("系统自检"));
        m_checkLabel->setStyleSheet(colorList.at(m_colorNum%colorList.count()));
        m_checkLabel->showFullScreen();
        CGlobal::instance()->m_widget->hide();
        ++m_colorNum;
    }
}

void CDataProcessing::slotNotifyInformation(int type,QList<QVariant> data)
{
    qDebug() << "CDataProcessing::slotNotifyInformation, Notify type is " << type;
    switch(type)
    {
        case NIT_InitControllerResult:
            initResult(data);
            break;
        case NIT_Mute:
            soundChangde(data);
            break;
        case NIT_EvacuationMode:
            evacuationMode(data);
            break;
        case NIT_MainPowerWork:
            mainPowerWork(data);
            break;
        case NIT_MainPowerFault:
            mainPowerFault(data);
            break;
        case NIT_StandbyPowerWork:
            standbyPowerWork(data);
            break;
        case NIT_StandbyPowerFault:
            standbyPowerFault(data);
            break;
        case NIT_Reset:
            resetSystem(data);
            break;
        case NIT_SelfCheckStart:
            selfCheckStart(data);
            break;
        case NIT_SelfCheckStop:
            selfCheckStop(data);
            break;
        case NIT_Stop:
            stopCommunication(data);
            break;
        case NIT_Start:
            startCommunication(data);
            break;
        case NIT_MonthCheckBegin:
            monthCheckBegin(data);
            break;
        case NIT_MonthCheckEnd:
            monthCheckEnd(data);
            break;
        case NIT_YearCheckBegin:
            yearCheckBegin(data);
            break;
        case NIT_YearCheckEnd:
            yearCheckEnd(data);
            break;
        case NIT_CheckTimeUpdate:
            checkTimeUpdate(data);
            break;
        case NIT_ExitProgram:
            exitProgram(data);
            break;
        case NIT_StartSpeedUp:
            startSpeedUp(data);
            break;
        case NIT_StopSpeedUp:
            stopSpeedUp(data);
            break;
        case NIT_Relogin:
        {
            CGlobal::instance()->InformationWindow()->hide();
            CGlobal::instance()->setMsgText("注册成功！");
            ConfigFileManager cfg;
            cfg.SaveConfigFile();//写入配置文件
        }
            break;
        default:
            break;
    }
}

void CDataProcessing::startSpeedUp(QList<QVariant> data)
{
    Q_UNUSED(data);
    CGlobal::instance()->TopToolBar()->changeInformation(true);
}

void CDataProcessing::stopSpeedUp(QList<QVariant> data)
{
    Q_UNUSED(data);
    CGlobal::instance()->TopToolBar()->changeInformation(false);
}

void CDataProcessing::initResult(QList<QVariant> data)
{
    CGlobal::instance()->InformationWindow()->hide();

    if(data.at(0).toInt() == 0)//初始化成功----------------------------------
    {
        CGlobal::instance()->setMSgColor(QColor(12, 180, 3));
        CGlobal::instance()->setMsgText("系统运行！");

        QList<CController*> tempController;//获取工程列表
        tempController = CGlobal::instance()->dm()->controllers();

        QList<CLayer *> layerList = CGlobal::instance()->getLayers(tempController);//获取图层链
        if(layerList.count() > 0)
        {
            //初始化工程后自动跳转到图层一
            CGlobal::instance()->setLayer(layerList.at(0));
        }
    }
    else if(data.at(0).toInt() == -1)//初始化失败----------------------------------
    {
        CGlobal::instance()->setMSgColor(Qt::red);
        CGlobal::instance()->setMsgText("加载文件失败！");

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
        QString log;
        log = "加载文件失败！";
        messageBox.setText(log);
        messageBox.exec();
        CGlobal::instance()->m_HaveDialog = false;
    }
}

void CDataProcessing::soundChangde(QList<QVariant> data)
{
    if(data.at(0).toBool())//消音
    {
        setSoundIcon(false);
    }
    else//取消消音
    {
        CMsgNull m_MsgNUll;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_CancelMute, &m_MsgNUll);
        setSoundIcon(true);
    }
}

void CDataProcessing::evacuationMode(QList<QVariant> data)
{
    if(data.at(0).toBool())
    {
        CGlobal::instance()->m_EvacuationMode = CGlobal::Auto;
        CGlobal::instance()->BottomToolBar()->setAuto();
    }
    else
    {
        CGlobal::instance()->m_EvacuationMode = CGlobal::Manual;
        CGlobal::instance()->BottomToolBar()->setManual();
    }
}

void CDataProcessing::mainPowerWork(QList<QVariant> data)
{
    if(data.at(0).toBool())
    {
        m_workType = MainPower;
    }
}

void CDataProcessing::mainPowerFault(QList<QVariant> data)
{
    CGlobal::instance()->m_bMainPowerFault = data.at(0).toBool();
}

void CDataProcessing::standbyPowerWork(QList<QVariant> data)
{
    if(data.at(0).toBool())
    {
        m_workType = StandbyPower;
    }
}

void CDataProcessing::standbyPowerFault(QList<QVariant> data)
{
    CGlobal::instance()->m_bStandbyPowerFault = data.at(0).toBool();
}

void CDataProcessing::resetSystem(QList<QVariant> data)
{
    Q_UNUSED(data);
    CGlobal::instance()->m_EmergencyOperation = CGlobal::Stop;
    CGlobal::instance()->m_bMainPowerFault = false;
    CGlobal::instance()->m_bStandbyPowerFault = false;
    CGlobal::instance()->m_HaveDialog = false;
    CGlobal::instance()->m_bMove_Mouse = false;

    CGlobal::instance()->m_selfCheck = false;
    CGlobal::instance()->m_bStart = false;
    CGlobal::instance()->m_bFault = false;

    setSoundIcon(true);

    //故障列表清空...
    for(int i=0; i<m_informationModel.count(); ++i)
    {
        m_informationModel.values().at(i)->clear();
    }

    for(int i=0; i<m_tabList.count(); ++i)
    {
        m_tabWidget->setTabText(i, m_tabList.at(i));
    }
    if(m_isStatusTab == true)
    {
        m_isStatusTab = false;
        CGlobal::instance()->SystemRecord()->updateRecord();
    }
    CGlobal::instance()->m_SystemYearCheck = false;
    CGlobal::instance()->m_SystemMonthCheck = false;
    CGlobal::instance()->setMSgColor(QColor(12, 180, 3));
    CGlobal::instance()->setMsgText("系统运行！");
    if(!CGlobal::instance()->m_bInLogin)
        CGlobal::instance()->InformationWindow()->hide();
}

void CDataProcessing::selfCheckStart(QList<QVariant> data)
{
    Q_UNUSED(data);
    soundName = CGlobal::instance()->m_soundName;
    soundPlay = CGlobal::instance()->m_soundPlay;
    CGlobal::instance()->m_scSoundName = ST_None;
    CGlobal::instance()->m_selfCheck = true;
    CGlobal::instance()->m_HaveDialog = true;

    m_checkLabel->showFullScreen();
    m_checkLabel->setStyleSheet("background-color:white");
    m_checkLabel->setText("系统自检");
    setSoundIcon(true);
    m_checkTimer->start(1000);
    ConfigFileManager cfg;
    cfg.ParseConfigFile();//读取注册文件
}

void CDataProcessing::slotcheckLoginInfo()
{
//    QString  data;
    CGlobal::instance()->ClientBusiness()->setCANconfigTrue();
    m_Timer->stop();
    CCanport* canport = CGlobal::instance()->controller()->canportById(CGlobal::instance()->panelAddress());
    if(CGlobal::instance()->m_isxmlNull)
    {
        if(canport){
            QList<CDistribution*> list = canport->distributions();
            for(int i=0; i<list.count(); i++)
            {
                QApplication::processEvents();
                CDistribution* distribution = list.at(i);
                if(distribution)
                {
                    distribution->setDistributionValue(OBJECT_VALUE_ONLINE,0);
                    distribution->setStatus(OBJS_DistributionCommunicationFault,1);
                    distribution->setloopCommunication(0xFF);
                    distribution->setDistributionValue(DISTRIBUTION_VALUE_NAMEDETAIL,tr("地址:%1 %2").arg(distribution->distributionValue(DISTRIBUTION_VALUE_ADDR).toInt())
                                           .arg(distribution->distributionValue(DISTRIBUTION_VALUE_NAME).toString()));
                    CGlobal::instance()->ClientBusiness()->XmlCanDeviceStateUpload(distribution);
                    CGlobal::instance()->ClientBusiness()->serverCentralizedPowerStateUpload(distribution);
                    QList<CLoop*> loops = distribution->loops();
                    for(int i=0; i<loops.count(); i++){
                        QApplication::processEvents();
                        CLoop* loop = loops.at(i);
                        if(loop)
                        {
                            loop->setloopValue(OBJECT_VALUE_ONLINE,0);
                            CGlobal::instance()->ClientBusiness()->XmlChannelStateUpload(loop);
                        }
                    }
                }
            }
            list.clear();
        }
        return;
    }
    CCanport* checkCanport = new CCanport(63);
    CDistribution* checkDistribution = new CDistribution;
    CLoop* checkLoop = new CLoop;
    CDevice* checkDevice = new CDevice;
    if(canport){
        QList<CDistribution*> list = canport->distributions();
        for(int i=0; i<list.count(); i++){
            QApplication::processEvents();
            CDistribution* distribution = list.at(i);
            if(distribution){
                checkCanport = CGlobal::instance()->ClientBusiness()->loginCanport();
                if(checkCanport)
                    checkDistribution = checkCanport->distributionByAddress(distribution->distributionAddress());
                if(!checkDistribution){
                    distribution->setDistributionValue(OBJECT_VALUE_ONLINE,0);
                    distribution->setStatus(OBJS_DistributionCommunicationFault,1);
                    distribution->setloopCommunication(0xFF);
                    distribution->setDistributionValue(DISTRIBUTION_VALUE_NAME,"集中电源");
                    distribution->setDistributionValue(DISTRIBUTION_VALUE_NAMEDETAIL,tr("地址:%1 %2").arg(distribution->distributionValue(DISTRIBUTION_VALUE_ADDR).toInt())
                                           .arg(distribution->distributionValue(DISTRIBUTION_VALUE_NAME).toString()));
                    CGlobal::instance()->ClientBusiness()->XmlCanDeviceStateUpload(distribution);
                    CGlobal::instance()->ClientBusiness()->serverCentralizedPowerStateUpload(distribution);
                }
                else
                {
                    distribution->setDistributionValue(DISTRIBUTION_VALUE_TYPEID,checkDistribution->distributionTypeId());
                    distribution->setDistributionValue(DISTRIBUTION_VALUE_DESCRIPTION,checkDistribution->descriptionStr());
                    distribution->setDistributionValue(DISTRIBUTION_VALUE_NAME,checkDistribution->typeStr());
                    if(checkDistribution->distributionTypeId() >= 252 && checkDistribution->distributionTypeId() < 255)
                        distribution->setDistributionValue(DISTRIBUTION_VALUE_NAME,"B型集中电源");
                    else if(checkDistribution->distributionTypeId() >= 240 && checkDistribution->distributionTypeId() < 250)
                        distribution->setDistributionValue(DISTRIBUTION_VALUE_NAME,"配电箱");
                    else if(checkDistribution->distributionTypeId() >= 200 && checkDistribution->distributionTypeId() < 240)
                        distribution->setDistributionValue(DISTRIBUTION_VALUE_NAME,"A型集中电源");
                    else if(checkDistribution->distributionTypeId() >= 120 && checkDistribution->distributionTypeId() < 130)
                        distribution->setDistributionValue(DISTRIBUTION_VALUE_NAME,"集中电源(MA)");
                    distribution->setDistributionValue(DISTRIBUTION_VALUE_NAMEDETAIL,tr("地址:%1 %2").arg(distribution->distributionValue(DISTRIBUTION_VALUE_ADDR).toInt())
                                           .arg(distribution->distributionValue(DISTRIBUTION_VALUE_NAME).toString()));
                }
                QList<CLoop*> loops = distribution->loops();
                for(int i=0; i<loops.count(); i++){
                    QApplication::processEvents();
                    CLoop* loop = loops.at(i);
                    if(loop){
                        if(checkDistribution)
                        {
                            checkLoop = checkDistribution->loopByAdd(loop->loopAdd());
                            if(!checkLoop)
                            {
                                loop->setloopValue(OBJECT_VALUE_ONLINE,0);
                                loop->setDeviceOnlineTotal(0);
                                loop->setStatus(OBJS_LoopCommunicationFault,1);
                                CGlobal::instance()->ClientBusiness()->XmlChannelStateUpload(loop);
                            }
                            else
                            {
                                if(checkLoop->deviceCount() == 0)
                                {
                                    loop->setloopValue(LOOP_VALUE_NOLOGIN,1);
                                }
                                else
                                {
                                    loop->setloopValue(LOOP_VALUE_NOLOGIN,0);
                                    QList<CDevice*> devices = loop->devices();
                                    for(int m=0; m<devices.count(); m++){
                                        QApplication::processEvents();
                                        CDevice* device = devices.at(m);
                                        if(device && device->deviceValue(OBJECT_VALUE_ONLINE) == 1)
                                        {
                                            checkDevice = checkLoop->deviceByAdd(device->deviceAdd());
                                            if(!checkDevice)
                                            {
                                                device->setDeviceValue(OBJECT_VALUE_ONLINE,0);
                                                device->setStatus(OBJS_LightCommunicationFault,1);
                                                //上传灯具状态到云平台/客户端
                                                CGlobal::instance()->ClientBusiness()->uploadLampStatus(device);
                                            }
                                        }
                                    }
                                    devices.clear();
                                }

                            }
                        }
                        else
                        {
                            loop->setloopValue(OBJECT_VALUE_ONLINE,0);
                            CGlobal::instance()->ClientBusiness()->XmlChannelStateUpload(loop);
                        }
                    }
                }
                loops.clear();
            }
        }
        list.clear();
    }

    CGlobal::instance()->DlgDevices()->getDeviceInfo();
    //布局视图界面灯具状态更新
    CGlobal::instance()->designSplashView()->setPageStateUpdate(true);
}


void CDataProcessing::selfCheckStop(QList<QVariant> data)
{
    Q_UNUSED(data);
    if(soundPlay)
    {
        setSoundIcon(true);
    }
    else
    {
        if(CGlobal::instance()->m_scSoundName != ST_None)
        {
            setSoundIcon(true);
        }
        else
        {
            CGlobal::instance()->m_soundPlay = soundPlay;
            CGlobal::instance()->m_soundName = soundName;
            setSoundIcon(false);
        }
    }

    m_checkLabel->hide();
    m_checkTimer->stop();
    CGlobal::instance()->m_selfCheck = false;
    CGlobal::instance()->m_scbFault = false;
    CGlobal::instance()->m_scbStart = false;
    CGlobal::instance()->m_HaveDialog = false;
    m_colorNum = 0;
    m_checkLabel->setStyleSheet("background-color:white");
    m_checkLabel->setText(QString("<font color=black>%1</font>").arg("系统自检"));
    if(CGlobal::instance()->m_LiftTime == 0)
    {
        dlgUseTime useTime;
        useTime.exec();
    }
}



void CDataProcessing::stopCommunication(QList<QVariant> data)
{
    int num = data.at(0).toInt();
    CGlobal::instance()->setMSgColor(Qt::red);
    CGlobal::instance()->setMsgText(tr("控制器%1 通讯失败！").arg(num));
}

void CDataProcessing::startCommunication(QList<QVariant> data)
{
    Q_UNUSED(data);
    CGlobal::instance()->InformationWindow()->hide();
    CGlobal::instance()->setMSgColor(QColor(12, 180, 3));
    CGlobal::instance()->setMsgText("系统运行！");

    if(CGlobal::instance()->m_EvacuationMode == CGlobal::Auto)
    {
        CGlobal::instance()->m_EvacuationMode = CGlobal::Manual;
        CGlobal::instance()->BottomToolBar()->setManual();
    }
}

void CDataProcessing::monthCheckBegin(QList<QVariant> data)
{
    Q_UNUSED(data);
    CGlobal::instance()->m_SystemMonthCheck = true;
    CGlobal::instance()->ClientBusiness()->Emergency();
    CGlobal::instance()->InformationWindow()->setMovie(":/images/allregedit.png");
    CGlobal::instance()->InformationWindow()->setDescription("<h1><font color=red>系统月检！</font></h1>");
    CGlobal::instance()->InformationWindow()->showCloseButton();
    CGlobal::instance()->InformationWindow()->show();
}

void CDataProcessing::monthCheckEnd(QList<QVariant> data)
{
    Q_UNUSED(data);
    CGlobal::instance()->m_SystemMonthCheck = false;
    CGlobal::instance()->InformationWindow()->hide();
    CGlobal::instance()->TopToolBar()->resetSystem();
}

void CDataProcessing::yearCheckBegin(QList<QVariant> data)
{
    Q_UNUSED(data);
    CGlobal::instance()->m_SystemYearCheck = true;
    CGlobal::instance()->ClientBusiness()->Emergency();
    CGlobal::instance()->InformationWindow()->setMovie(":/images/allregedit.png");
    CGlobal::instance()->InformationWindow()->setDescription("<h1><font color=red>系统年检！</font></h1>");
    CGlobal::instance()->InformationWindow()->showCloseButton();
    CGlobal::instance()->InformationWindow()->show();
}

void CDataProcessing::yearCheckEnd(QList<QVariant> data)
{
    Q_UNUSED(data);
    CGlobal::instance()->m_SystemYearCheck = false;
    CGlobal::instance()->InformationWindow()->hide();
    CGlobal::instance()->TopToolBar()->resetSystem();
}

void CDataProcessing::checkTimeUpdate(QList<QVariant> data)
{
    QString mTime;
    QString yTime;
    if(data.count() < 5)
        return;
    QDateTime monthCheckTime = data.at(0).toDateTime();
    QDateTime yearCheckTime = data.at(1).toDateTime();
    QDateTime speedMonthCheckTime = data.at(2).toDateTime();
    QDateTime speedYearCheckTime = data.at(3).toDateTime();
    QDateTime speedsystemCurrentTime = data.at(4).toDateTime().addDays(1);
    int nSecords;
    if(CGlobal::instance()->TopToolBar()->speedup() == true)
    {
        mTime = speedMonthCheckTime.toString("yyyy-MM-dd HH:mm:ss");
        yTime = speedYearCheckTime.toString("yyyy-MM-dd HH:mm:ss");
        nSecords = speedsystemCurrentTime.secsTo(speedMonthCheckTime);
    }
    else
    {
        mTime = monthCheckTime.toString("yyyy-MM-dd HH:mm:ss");
        yTime = yearCheckTime.toString("yyyy-MM-dd HH:mm:ss");
        nSecords = QDateTime::currentDateTime().secsTo(monthCheckTime);
    }
    int nDays = nSecords/60/60/24;
    int nHours = nSecords/60/60-nDays*24;
    int nMins = nSecords/60-nDays*24*60-nHours*60;
    nSecords = nSecords%60;
    if(nSecords < 0 || nMins < 0 || nHours < 0 || nDays < 0)
        nDays = nHours = nMins = nSecords = 0;
    QString temp;
    temp.append("下次月检:");
    temp.append(mTime);
    temp.append("\n");
    temp.append("剩余:"+ QString::number(nDays).sprintf("%003d",nDays)+"天" + QString::number(nHours).sprintf("%02d",nHours) + "时" +
                QString::number(nMins).sprintf("%02d",nMins) + "分" + QString::number(nSecords).sprintf("%02d",nSecords) + "秒");
    temp.append("\n");
    if(CGlobal::instance()->TopToolBar()->speedup() == true)
        nSecords = speedsystemCurrentTime.secsTo(speedYearCheckTime);
    else
        nSecords = QDateTime::currentDateTime().secsTo(yearCheckTime);
    nDays = nSecords/60/60/24;
    nHours = nSecords/60/60-nDays*24;
    nMins = nSecords/60-nDays*24*60-nHours*60;
    nSecords = nSecords%60;
    if(nSecords < 0 || nMins < 0 || nHours < 0 || nDays < 0)
        nDays = nHours = nMins = nSecords = 0;

    temp.append("下次年检:");
    temp.append(yTime);
    temp.append("\n");
    temp.append("剩余:"+ QString::number(nDays).sprintf("%003d",nDays)+"天" + QString::number(nHours).sprintf("%02d",nHours) + "时" +
                QString::number(nMins).sprintf("%02d",nMins) + "分" + QString::number(nSecords).sprintf("%02d",nSecords) + "秒");
    CGlobal::instance()->TopToolBar()->m_checkTime->setAlignment(Qt::AlignBottom);
    CGlobal::instance()->TopToolBar()->m_checkTime->setText(temp);
}



void CDataProcessing::exitProgram(QList<QVariant> data)
{
    Q_UNUSED(data);
    CGlobal::instance()->saveSetting();
    CGlobal::destroy();
    exit(0);
}

///获取查询的日志信息
void CDataProcessing::slotCommandResult(int commandType, CMsgStructBase* msgData)
{
    if(commandType == NCT_Log)
    {
        if(CGlobal::instance()->SystemRecord() != NULL)
        {
            CGlobal::instance()->SystemRecord()->m_Model->addData(static_cast<CMsgLog*>(msgData));
        }
    }
}

///用户操作按键时要求验证用户权限
void CDataProcessing::slotVerifyAdmin(int commandType, CMsgStructBase* msgData)
{
    static bool flag = true;
    static QByteArray tempData;

    if(CGlobal::instance()->m_UserType == CGlobal::User)
    {
        tempData = msgData->data(commandType);
        if(flag)
        {
            flag = false;
            dlgAdministrator m_Administrator;
            m_Administrator.setWindowTitle("用户验证");
            m_Administrator.setUserDisabled(true);
            if(QDialog::Accepted == m_Administrator.exec())
            {
                CGlobal::instance()->BottomToolBar()->setAdministrator();
                CMsgStructBase* msgData2;
                QDataStream stream(&tempData, QIODevice::ReadOnly);
                stream>>commandType>>&msgData2;
                if(!CGlobal::instance()->ClientBusiness()->exeCommand(commandType, msgData2))
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
                    QString log;
                    log = "操作失败！";
                    messageBox.setText(log);
                    messageBox.exec();
                    CGlobal::instance()->m_HaveDialog = false;
                }
                delete msgData2;
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
                m_Administrator.setWindowTitle("用户验证");
                m_Administrator.setUserDisabled(true);
                if(QDialog::Accepted == m_Administrator.exec())
                {
                    CGlobal::instance()->BottomToolBar()->setAdministrator();
                    if(!CGlobal::instance()->ClientBusiness()->exeCommand(commandType, msgData))
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
                        QString log;
                        log = "操作失败！";
                        messageBox.setText(log);
                        messageBox.exec();
                        CGlobal::instance()->m_HaveDialog = false;
                    }
                }
                flag = true;
            }
        }
        else
        {
            if(!CGlobal::instance()->ClientBusiness()->exeCommand(commandType, msgData))
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
                QString log;
                log = "操作失败！";
                messageBox.setText(log);
                messageBox.exec();
                CGlobal::instance()->m_HaveDialog = false;
            }
        }
    }
}

void CDataProcessing::resetStatus()
{
    CCanport* canport = CGlobal::instance()->controller()->canportById(CGlobal::instance()->panelAddress());
    QString name;
    int count;
    for(int i=0; i<m_informationModel.count(); i++)
    {
        name = m_tabList.at(i);
        count = m_informationModel.value(name)->count();
        if(count > 0)
        {
            for(int i=0; i < count; i++)
            {
                //由于setStatus会删除当前statusInfo，即data(0)，所以原表中的data(i)就是新表中的data(0)
                if(m_informationModel.value(name)->getStatusInfo(0).deviceID != 0)
                {
                    CDistribution* distribution = canport->distributionByAddress(m_informationModel.value(name)->getStatusInfo(0).canID);
                    CLoop* loop = distribution->loopByAdd(m_informationModel.value(name)->getStatusInfo(0).loopID);
                    CDevice* device = loop->deviceByAdd(m_informationModel.value(name)->getStatusInfo(0).deviceID);
                    device->setStatus(m_informationModel.value(name)->getStatusInfo(0).statusId,0);
                    if(device->deviceValue(OBJECT_VALUE_ONLINE) == 0)
                    {
                        device->setDeviceValue(OBJECT_VALUE_ONLINE,1);
                    }
                }
                else
                {
                    if(m_informationModel.value(name)->getStatusInfo(0).loopID != 0)
                    {
                        CDistribution* distribution = canport->distributionByAddress(m_informationModel.value(name)->getStatusInfo(0).canID);
                        CLoop* loop = distribution->loopByAdd(m_informationModel.value(name)->getStatusInfo(0).loopID);
                        loop->setStatus(m_informationModel.value(name)->getStatusInfo(0).statusId,0);
                        if(loop->loopValue(OBJECT_VALUE_ONLINE) == 0)
                        {
                            loop->setloopValue(OBJECT_VALUE_ONLINE,1);
                            //回路通讯状态显示
                            distribution->setloopCommunication(distribution->getloopCommunication() & (~(0x01 << (loop->loopAdd()-1))));
                        }
                    }
                    else
                    {
                        if(m_informationModel.value(name)->getStatusInfo(0).canID != 0)
                        {
                            CDistribution* distribution = canport->distributionByAddress(m_informationModel.value(name)->getStatusInfo(0).canID);
                            distribution->setStatus(m_informationModel.value(name)->getStatusInfo(0).statusId,0);
                            //回路通讯状态显示
                            distribution->setloopCommunication(0);
                            if(distribution->distributionValue(OBJECT_VALUE_ONLINE) == 0)
                            {
                                distribution->setDistributionValue(OBJECT_VALUE_ONLINE,1);
                                for(int i=1; i<9; i++)
                                {
                                    CLoop* loop = distribution->loopByAdd(i);
                                    loop->setloopValue(OBJECT_VALUE_ONLINE,1);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    CGlobal::instance()->ClientBusiness()->clearStatusObject(true);
    CGlobal::instance()->ClientBusiness()->clearExpectObject();
}

void CDataProcessing::slotStateInformation(const StatusInfo& statusInfo, int type, int opr)
{
    QString name = getStatusName(type);
    CMsgNull m_MsgNUll;
    if(name == "")
    {
        return;
    }

    switch(opr)
    {
        case SRO_Add:
            addStatusInfo(name, statusInfo);
            if(CGlobal::instance()->ClientBusiness()->exeCommand(NCT_CancelMute, &m_MsgNUll))
                setSoundIcon(true);
            break;
        case SRO_Del:
            delStatusInfo(name, statusInfo);
            break;
        case SRO_DelAll:
            delAllStatusInfo(name, statusInfo);
            break;
        default:
            return;
    }

    if (m_informationModel.contains(name)) {
        if(m_informationModel.value(name)->rowCount() < 1) {
            if(name == "启动") {
                dealStartInfo();
            } else if(name == "故障") {
                dealFaultInfo();
            }
        }
    } else if (name == "全部") {
        int startNum = m_informationModel.value("启动")->rowCount();
        int faultNum = m_informationModel.value("故障")->rowCount();

        if (faultNum < 1 && startNum < 1) {
            CGlobal::instance()->m_bStart = false;
            CGlobal::instance()->m_bFault = false;
        } else if (faultNum < 1) {
            dealFaultInfo();
        } else if (startNum < 1) {
            dealStartInfo();
        }
    }

    for(int i=0; i<m_tabList.count(); ++i)
    {
        if(m_informationModel.value(m_tabList.at(i))->rowCount() > 0)
        {
            return;
        }
    }
    CGlobal::instance()->m_bStart = false;
    CGlobal::instance()->m_bFault = false;
    if(m_isStatusTab == true)
    {
        m_isStatusTab = false;
        CGlobal::instance()->SystemRecord()->updateRecord();
    }
//    m_tabWidget->hide();

}

void CDataProcessing::dealStartInfo()
{
    CGlobal::instance()->m_bStart = false;
    if(CGlobal::instance()->m_selfCheck)
    {
        if(CGlobal::instance()->m_scSoundName == ST_Switch)
        {
            if(CGlobal::instance()->m_scbFault)
            {
                CGlobal::instance()->m_scSoundName = ST_Fault;
            }
            else
            {
                CGlobal::instance()->m_scSoundName = ST_None;
            }
        }
    }
}

void CDataProcessing::dealFaultInfo()
{
    CGlobal::instance()->m_bFault = false;
    if(CGlobal::instance()->m_selfCheck)
    {
        if(CGlobal::instance()->m_scSoundName == ST_Fault)
        {
            if(CGlobal::instance()->m_scbStart)
            {
                CGlobal::instance()->m_scSoundName = ST_Switch;
            }
            else
            {
                CGlobal::instance()->m_scSoundName = ST_None;
            }
        }
    }
}

QString CDataProcessing::getStatusName(int type)
{
    QString name;
    switch(type)
    {
        case SRT_Start:
            name = "启动";
            break;
        case SRT_Fault:
            name = "故障";
            break;
        case SRT_Emergency:
            name = "应急";
            break;
        case SRT_ALL:
            name = "全部";
            break;
        default:
            name = "";
            break;
    }
    return name;
}

void CDataProcessing::addStatusInfo(QString name, const StatusInfo& statusInfo)
{
    StatusInfo tempInfo = statusInfo;
    if (m_informationModel.contains(name))
    {
        m_informationModel.value(name)->addData(tempInfo);
        setTabNum(name);
        m_tabWidget->setCurrentIndex(m_tabList.indexOf(name));
    }
    else
    {
        for (int i=0; i<m_tabList.size(); ++i)
        {
            m_informationModel.value(m_tabList.at(i))->addData(tempInfo);
            setTabNum(m_tabList.at(i));
        }
        m_tabWidget->setCurrentIndex(0);
    }
    if(m_isStatusTab == false)
    {
        m_isStatusTab = true;
        CGlobal::instance()->SystemRecord()->updateRecord();
    }
//    m_tabWidget->show();
    CGlobal::instance()->m_bMove_Mouse = true;
    if(name == "启动")
    {
        setSoundIcon(true);
        CGlobal::instance()->m_bStart = true;
        CGlobal::instance()->m_scSoundName = ST_Switch;
        if(CGlobal::instance()->m_selfCheck)
        {
            CGlobal::instance()->m_scbStart = true;
        }
    }
    else if(name == "故障")
    {
        setSoundIcon(true);
        CGlobal::instance()->m_bFault = true;
        CGlobal::instance()->m_scSoundName = ST_Fault;
        if(CGlobal::instance()->m_selfCheck)
        {
            CGlobal::instance()->m_scbFault = true;
        }
        else
        {
            if((CGlobal::instance()->m_soundPlay && CGlobal::instance()->m_soundName == ST_Fault) ||\
                    (CGlobal::instance()->m_soundPlay && CGlobal::instance()->m_soundName == ST_Switch))
            {
                //-------------------
            }
            else
            {
                CMsgNull m_MsgNUll;
                CGlobal::instance()->ClientBusiness()->exeCommand(NCT_CancelMute, &m_MsgNUll);
            }
        }
    }
    else if(name == "全部")
    {
        setSoundIcon(true);
        for (int i=0; i<m_tabList.size(); ++i)
        {
            setTabNum(m_tabList.at(i));
            m_informationModel.value(m_tabList.at(i))->addData(tempInfo);
        }
        CGlobal::instance()->m_bStart = true;
        CGlobal::instance()->m_bFault = true;
        CGlobal::instance()->m_scSoundName = ST_Switch;
        if(CGlobal::instance()->m_selfCheck)
        {
            CGlobal::instance()->m_scSoundName = ST_Switch;
            CGlobal::instance()->m_scbStart = true;
            CGlobal::instance()->m_scbFault = true;
        }
    }
}

void CDataProcessing::delStatusInfo(QString name, const StatusInfo& statusInfo)
{
    StatusInfo tempInfo = statusInfo;
    if(name == "全部")
    {
        for(int i=0; i<m_informationModel.count(); ++i)
        {
            m_informationModel.values().at(i)->removeData(tempInfo.keyId, tempInfo.statusId);
        }
    }
    else
    {
        m_informationModel.value(name)->removeData(tempInfo.keyId, tempInfo.statusId);
    }
    for (int i=0; i<m_tabList.size(); ++i) // ll 2015/6/4
    {
        setTabNum(m_tabList.at(i));
    }
}

void CDataProcessing::delAllStatusInfo(QString name, const StatusInfo& statusInfo)
{
    StatusInfo tempInfo = statusInfo;
    if(name == "全部")
    {
        for(int i=0; i<m_informationModel.count(); ++i)
        {
            m_informationModel.values().at(i)->removeData(tempInfo.keyId);
        }
    }
    else
    {
        m_informationModel.value(name)->removeData(tempInfo.keyId);
    }
    for (int i=0; i<m_tabList.size(); ++i)
    {
        setTabNum(m_tabList.at(i));
    }
}

void CDataProcessing::setSoundIcon(bool flag)
{
    if(flag)
    {
        CGlobal::instance()->TopToolBar()->m_btnMuteTool->setIcon(QIcon(":/images/play.png"));
    }
    else
    {
        CGlobal::instance()->TopToolBar()->m_btnMuteTool->setIcon(QIcon(":/images/mute.png"));
    }
}

void CDataProcessing::setTabNum(const QString& name)
{
    int tabNum = m_tabList.indexOf(name);
    int num = m_informationModel.value(name)->rowCount();
    if(tabNum > -1)
    {
        if(num > 0)
        {
            m_tabWidget->setTabText(tabNum, name+QString("(%1)").arg(num));
        }
        else
        {
            m_tabWidget->setTabText(tabNum, name);
            changeTab();
        }
    }
}

//当前tab数据变为0时,遍历所有tab,跳转到从左到右第一个有数据的tab
void CDataProcessing::changeTab()
{
    QHashIterator<QString, CInformationModel*> i(m_informationModel);
     while (i.hasNext())
     {
         i.next();
         if(i.value()->rowCount() > 0)
         {
             m_tabWidget->setCurrentIndex(m_tabList.indexOf(i.key()));
         }
     }
}
