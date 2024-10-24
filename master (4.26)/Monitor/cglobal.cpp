#include "cglobal.h"
#include "irrKlang/irrKlang.h"


CGlobal* CGlobal::m_instance = NULL;
qreal CGlobal::m_lineWidth = GLOBAL_LINE_WIDTH;
QHash<short, QString> CGlobal::m_objectStatusStr;
QSet<short> CGlobal::m_startStatusList;
QSet<short> CGlobal::m_faultStatusList;
QSet<short> CGlobal::m_emergencyStatusList;
using namespace irrklang;
CGlobal::CGlobal(QObject *parent) : QObject(parent)
{
    m_dm = new CDM(parent);
    m_bWriteLog = false;
    m_bInLogin = false;
    m_nPanelAddress = 63;
    m_interval = 10;
    m_bStartComm = false;
    m_maxCacheLayers = 100;
    m_isShowDeviceToolTip = true;
    m_labelSystemMsg = new QLabel;
    m_simulateFirePixmap = new QPixmap(":/images/simulatefire96.png");
    m_simulateFirstFirePixmap = new QPixmap(":/images/simulatefirstfire96.png");
    m_hideNormalAlarmPoint = false;
    m_isAutoSwitchLayer = false;
    m_allowHandFire = true;
    m_allowHandEvacuation = true;
    m_allowHandFlow = true;
    m_pProgramTreeView = NULL;
    m_pProgramControllerView = NULL;
    m_pProgramContainer = NULL;
    m_pProgramCanportView = NULL;
    m_pProgramCanDeviceView = NULL;
    m_pProgramDistributionView = NULL;
    m_pProgramDeviceView = NULL;
    m_pDesignTreeView = NULL;
    m_pDesignContainer = NULL;
//    m_pDesignMainView = NULL;
    m_pDesignSplashView = NULL;
    m_dlgDevices = NULL;
    m_dlgEvacuationFlow = NULL;
    m_dlgSearchDevice = NULL;
    m_strPassWord = "0000";
    readSetting();
    readConfig();
    initDeviceType();
    initObjectStatusList();
    //////////////////////////////////////////////////////////////////////////////////
    m_strApplicationTitle = "应急疏散监控系统";

    m_widget = NULL;
    m_dlgTimeInterval = NULL;
    m_ClientBusiness = NULL;
    m_SystemRecord = NULL;
    m_dlgInforWindow = NULL;
    m_pLoginInformation = NULL;

    m_bSwitch_LEDButton = false;
    m_bMove_Mouse = false;
    m_bCommOk = false;
    m_bMainPowerFault = false;
    m_bStandbyPowerFault = false;
    m_HaveDialog = false;

    m_SystemMonthCheck = false;
    m_SystemYearCheck = false;

    m_bCharge = true;
    m_loginStatus = false;
    m_UserType = User;
    m_EvacuationMode = Auto;
    m_EmergencyOperation = Stop;

    m_audioOutput = NULL;
    m_soundName = ST_None;
    m_scSoundName = ST_None;
    m_scbStart = false;
    m_scbFault = false;
    m_soundPlay = true;
    m_selfCheck = false;
    m_bStart = false;
    m_bFault = false;
    m_bFull = true;

    //新增参数设置初始化，集中电源状态初始化
    m_ShieldFaultSound = true;
    m_ShieldWarningSound = true;
    m_AutoSwitchToDangerPage = false;
    LastLoginName = "SuperAdmin";
    m_MonthCheckSpanSecond = 30;
    m_YearCheckSpanSecond = 30;
    m_TwinkleWhenLaunch = true;
    m_SendDirectionAndTwinkleByOne = true;
    m_LaunchRule = true;
    m_ProcessSwitchln = true;
    m_LiftTime = 30;
    m_nDistriAddress = 0;
    m_nLoopAddress = 0;
    m_isSelectDistribution = false;
    m_isSelectLoop = false;
    m_isDelUnregLampFlag = false;
    m_TwinkleControl = false;
    m_UCSendDirectionAndTwinkleByOne = true;
    m_province = "";
    m_city = "";
    m_country = "";
    m_addressDetail = "";
    m_ControllerID = "";
    m_tcptype = TCP_None;

    m_forbidFaultSound = false;
    m_forbidEmergencySound = false;
    m_forbidMonthcheck = false;
    m_forbidYearcheck = false;
    m_energymode = true;
    m_version_ledkey = "0V0.00";
    m_version_can1can2 = "0V0.00";
    m_version_can3can4 = "0V0.00";

    m_FireEmergencyTime = 30;
    m_noFireEmergencyTime = 30;
    m_OpenA = 10;
    m_PowerMode = false;
    m_lightTestSwitch = false;
    m_actionInSwitch = false;
    m_CANFaultSwitch = false;

    m_nStartRule = RULE_AllStart;
    m_nCanNumber = 4;

    m_isxmlNull = false;
    m_isReNumeDevice = false;
    m_isReNumeProductId = "";

    m_recordPage = 1;

    QDesktopWidget *desktop = QApplication::desktop();
    m_screenWidth = desktop->width();
    m_screenHeight = desktop->height();

    m_isEmergency = false;
}

CGlobal* CGlobal::instance()
{
    if (!m_instance)
        m_instance = new CGlobal();
    return m_instance;
}

CGlobal::~CGlobal()
{
    if(m_dm)
        delete m_dm;
    for(int i = 0; i < m_deviceTypes.size(); ++i)
        delete m_deviceTypes.at(i);
    delete m_simulateFirePixmap;
    delete m_simulateFirstFirePixmap;
    delete m_labelSystemMsg;
}

void CGlobal::destroy()
{
    if (m_instance) {
        delete m_instance;
        m_instance = NULL;
    }
}

QByteArray CGlobal::loadRawData(const QString &str)
{
    QFile file(str);
    if (file.open(QIODevice::ReadOnly)) {
        return file.readAll();
    }
    return QByteArray();
}

int CGlobal::colorToInt(const QColor &color)
{
    int n = color.red() << 24 | color.green() << 16 | color.blue() << 8 | color.alpha();
    return n;
}

QColor CGlobal::intToColor(const int n)
{
    unsigned c = n;
    QColor color( c >> 24, (c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF);
    return color;
}

CController* CGlobal::controller()
{
    if(m_dm)
        return m_dm->controllerAt(0);
    else
        return 0;
}

CCanport* CGlobal::panel()
{
    CController *p = controller();
    if(p && p->hasCanport(m_nPanelAddress))
        return p->canportById(m_nPanelAddress);
    else
        return 0;
}

void CGlobal::saveSetting()
{
    QSettings settingcfg("Evacuation", "CFG");
    settingcfg.setValue("interval", m_interval);
    settingcfg.setValue("workspaces", m_workspaces);
    settingcfg.setValue("controllername", m_controllerName);
    settingcfg.setValue("maxcachelayers", m_maxCacheLayers);

    QSettings setting("Evacuation", "Monitor");
    setting.setValue("CutLayer", m_bCutLayer);
    setting.setValue("FullScreen", m_bFullScreen);
    setting.setValue("TimeCutLayer", m_nTimeCutLayer);
    setting.setValue("StartComm", m_bStartComm);
}
//1为工程，63为控制器，100-1000为集中电源，1000-2000为回路，大于2000为灯具
int CGlobal::loadKeyId(int type,int Id)
{
    int keyId;
    if(type == OBJT_Controller)
    {
        keyId = Id;
    }
    else if(type == OBJT_Canport)
    {
        keyId = Id;
    }
    else if(type == OBJT_Distribution)
    {
        keyId = Id + 100;
    }
    else if(type == OBJT_Loop)
    {
        keyId = Id + 1000;
    }
    else if(type == OBJT_Device)
    {
        keyId = Id + 2000;
    }
    return keyId;

}

//1为工程，63为控制器，100-1000为集中电源，1000-2000为回路，大于2000为灯具
int CGlobal::saveKeyId(int type,int Id)
{
    int keyId;
    if(type == OBJT_Controller)
    {
        keyId = Id;
    }
    else if(type == OBJT_Canport)
    {
        keyId = Id;
    }
    else if(type == OBJT_Distribution)
    {
        keyId = Id - 100;
    }
    else if(type == OBJT_Loop)
    {
        keyId = Id - 1000;
    }
    else if(type == OBJT_Device)
    {
        keyId = Id - 2000;
    }
    return keyId;

}


void CGlobal::readSetting()
{
    QSettings settingcfg("Evacuation", "CFG");
    m_interval = settingcfg.value("interval", 10).toInt();
    m_workspaces = settingcfg.value("workspaces").toString();
    m_controllerName = settingcfg.value("controllername").toString();
    m_maxCacheLayers = settingcfg.value("maxcachelayers", 100).toInt();

    QSettings setting("Evacuation", "Monitor");
    m_bCutLayer = setting.value("CutLayer", true).toBool();
    m_bFullScreen = setting.value("FullScreen", true).toBool();
    m_nTimeCutLayer = setting.value("TimeCutLayer", 5).toInt();
    m_bStartComm = setting.value("StartComm", false).toBool();
    setAutoSwitchLayer(m_bCutLayer);
}


void CGlobal::readConfig()
{
    if(m_hashCard.count() == 0){
        int nIndex = 0;
        QString strkey = "";
        for(int i=1; i<3; i++){
            RouteConfig *pCard = new RouteConfig();
            pCard->nPort= i;
            pCard->nRate= 10000;
            pCard->nType = PT_Distribution_CAN;
            pCard->nCommpnyID = 0;
            pCard->nPanel = 0;
            strkey = QString::number(nIndex).rightJustified(3, '0');
            m_hashCard.insert(strkey, pCard);
            nIndex++;
        }
    }
    if(m_nPanelAddress < 0 || m_nPanelAddress > 255) m_nPanelAddress = 1;
    if(m_nTimeCutLayer < 5 || m_nTimeCutLayer > 120) m_nTimeCutLayer = 5;
}

CDeviceType* CGlobal::deviceType(int i) const
{
    return m_deviceTypes.value(i, NULL);
}


QString CGlobal::getObjectStatusSuffix(short status, unsigned char value) const
{
    QString str;
    if(isEmergencyStatus(status))
    {
        if(value)
            str.append("(启动)");
        else
            str.append("(停止)");
    }
    else if(isStartStatus(status))
    {
        if(value)
            str.append("(启动)");
        else
            str.append("(停止)");
    }
    else if(isFaultStatus(status))
    {
        if(!value)
            str.append("(恢复)");
    }
    return str;
}

QString CGlobal::getObjectStatusDescription(const CObject *object, short status, unsigned char value) const
{
    return object->codeStr() + getObjectStatusStr(status) + getObjectStatusSuffix(status, value);
}

void CGlobal::initObjectStatusList()
{
    if(m_objectStatusStr.isEmpty())
    {
        m_objectStatusStr.insert(OBJS_None, "无");
        m_objectStatusStr.insert(OBJS_Start, "启动");
        m_objectStatusStr.insert(OBJS_Fault, "故障");
        m_objectStatusStr.insert(OBJS_Emergency, "应急启动");
        m_objectStatusStr.insert(OBJS_Fire, "火警");
        m_objectStatusStr.insert(OBJS_Online, "在线");
        m_objectStatusStr.insert(OBJS_LoginDeviceType, "登录类型");
        m_objectStatusStr.insert(OBJS_PowerChargerValue, "充电状态");
        m_objectStatusStr.insert(OBJS_PowerAutoValue, "集中电源手自动状态");
        m_objectStatusStr.insert(OBJS_AutoEvacuation, "自动模式");
        m_objectStatusStr.insert(OBJS_InputPort, "输入端口启动");
        m_objectStatusStr.insert(OBJS_OutputPort, "输出端口启动");
        m_objectStatusStr.insert(OBJS_NewRegister, "新注册");
        m_objectStatusStr.insert(OBJS_PathStart, "路径启动");
        m_objectStatusStr.insert(OBJS_UpStart, "启动上");
        m_objectStatusStr.insert(OBJS_DownStart, "启动下");
        m_objectStatusStr.insert(OBJS_LeftStart, "启动左");
        m_objectStatusStr.insert(OBJS_RightStart, "启动右");
        m_objectStatusStr.insert(OBJS_LightStart, "启动光源");
        m_objectStatusStr.insert(OBJS_SystemFault, "系统故障");
        m_objectStatusStr.insert(OBJS_PowerFault, "电源故障");
        m_objectStatusStr.insert(OBJS_MainPowerFault, "主电故障");
        m_objectStatusStr.insert(OBJS_StandbyPowerFault, "备电故障");
        m_objectStatusStr.insert(OBJS_ConcenterPowerFault, "集中电源故障");
        m_objectStatusStr.insert(OBJS_ChargerFault, "充电故障");
        m_objectStatusStr.insert(OBJS_PowerOutputFault, "DC36V故障");
        m_objectStatusStr.insert(OBJS_PowerKaiLuFault, "输出开路故障");
        m_objectStatusStr.insert(OBJS_BatteryLineFault, "备电故障");
        m_objectStatusStr.insert(OBJS_BatteryLineFault_1, "备电线路1故障");
        m_objectStatusStr.insert(OBJS_BatteryLineFault_2, "备电线路2故障");
        m_objectStatusStr.insert(OBJS_BatteryLineFault_3, "备电线路3故障");
        m_objectStatusStr.insert(OBJS_BatteryLowVoltageFault, "备电低压故障");
        m_objectStatusStr.insert(OBJS_BatteryLowVoltageFault_1, "备电1低压故障");
        m_objectStatusStr.insert(OBJS_BatteryLowVoltageFault_2, "备电2低压故障");
        m_objectStatusStr.insert(OBJS_BatteryLowVoltageFault_3, "备电3低压故障");
        m_objectStatusStr.insert(OBJS_BatteryLowQuantityFault, "过放电故障");
        m_objectStatusStr.insert(OBJS_BatteryOverDischargeFault, "充电器故障");
        m_objectStatusStr.insert(OBJS_BatteryOffFault, "备电断线");
        m_objectStatusStr.insert(OBJS_BatteryShortFault, "备电短路");
        m_objectStatusStr.insert(OBJS_LoopOutShortFault, "回路输出短路");
        m_objectStatusStr.insert(OBJS_LoopOutOverloadFault, "回路输出过载");
        m_objectStatusStr.insert(OBJS_LoopCommunicationFault, "通讯故障");
        m_objectStatusStr.insert(OBJS_NetCommunicationFault, "网络通讯故障");
        m_objectStatusStr.insert(OBJS_LightCommunicationFault, "通讯故障");
        m_objectStatusStr.insert(OBJS_LightFault, "光源故障");
        m_objectStatusStr.insert(OBJS_CheckFault, "检测故障");
        m_objectStatusStr.insert(OBJS_OutLoopFault, "回路输出故障");
        m_objectStatusStr.insert(OBJS_DistributionFault, "集中电源故障");
        m_objectStatusStr.insert(OBJS_DistributionCommunicationFault, "通讯故障");
        m_objectStatusStr.insert(OBJS_PowerCommunicationFault, "集中电源通讯故障");
        m_objectStatusStr.insert(OBJS_FASCommunicationFault, "FAS通讯故障");
        m_objectStatusStr.insert(OBJS_RemoteEmergency, "远程应急");
        m_objectStatusStr.insert(OBJS_FireEmergency, "火警应急");
        m_objectStatusStr.insert(OBJS_ForceEmergency, "强制启动");
        m_objectStatusStr.insert(OBJS_InputEmergency, "输入应急");
        m_objectStatusStr.insert(OBJS_PowerOutShortFault, "输出短路故障");
        m_objectStatusStr.insert(OBJS_ManualEmergency, "手动应急");
        m_objectStatusStr.insert(OBJS_DIEmergency, "DI联动应急");
        m_objectStatusStr.insert(OBJS_TestEmergency, "主备电测试应急");
        m_objectStatusStr.insert(OBJS_SystemEmergency, "系统应急");
        m_objectStatusStr.insert(OBJS_UndervoltageEmergency, "照明回路欠压应急");
        m_objectStatusStr.insert(OBJS_AutoEmergency, "自动应急");
        m_objectStatusStr.insert(OBJS_MonthEmergency, "系统月检");
        m_objectStatusStr.insert(OBJS_YearEmergency, "系统年检");
        m_objectStatusStr.insert(OBJS_SimulateFireEmergency, "模拟火警应急");
        m_objectStatusStr.insert(OBJS_DeviceBatteryOffFault, "电池断线");
        m_objectStatusStr.insert(OBJS_DeviceBatteryShortFault, "电池短路");
        m_objectStatusStr.insert(OBJS_DeviceBatteryOverdischargeFault, "电池过放电");
        m_objectStatusStr.insert(OBJS_StandbyPowerOff, "备电断线");
        m_objectStatusStr.insert(OBJS_StandbyPowerShort, "备电短路");
        m_objectStatusStr.insert(OBJS_StandbyPowerUndervoltage, "备电欠压");
    }
    if(m_startStatusList.isEmpty())
    {
        m_startStatusList<<OBJS_Start<<OBJS_PathStart<<OBJS_UpStart<<OBJS_DownStart<<OBJS_LeftStart<<OBJS_RightStart<<OBJS_LightStart;
    }
    if(m_faultStatusList.isEmpty())
    {
        m_faultStatusList<<OBJS_Fault<<OBJS_SystemFault<<OBJS_PowerFault<<OBJS_MainPowerFault<<OBJS_StandbyPowerFault<<OBJS_ConcenterPowerFault
                         <<OBJS_ChargerFault<<OBJS_PowerOutputFault<<OBJS_MainPowerFault<<OBJS_BatteryLineFault<<OBJS_BatteryLineFault_1<<OBJS_BatteryLineFault_2<<OBJS_BatteryLineFault_3
                         <<OBJS_BatteryLowVoltageFault<<OBJS_BatteryLowVoltageFault_1<<OBJS_BatteryLowVoltageFault_2<<OBJS_BatteryLowVoltageFault_3<<OBJS_BatteryOffFault
                         <<OBJS_BatteryShortFault<<OBJS_BatteryLowQuantityFault<<OBJS_BatteryOverDischargeFault<<OBJS_LoopOutShortFault<<OBJS_LoopOutOverloadFault
                         <<OBJS_LoopCommunicationFault<<OBJS_NetCommunicationFault<<OBJS_LightCommunicationFault<<OBJS_LightFault<<OBJS_CheckFault
                         <<OBJS_OutLoopFault<<OBJS_DistributionFault<<OBJS_DistributionCommunicationFault<<OBJS_PowerCommunicationFault
                         <<OBJS_FASCommunicationFault<<OBJS_PowerKaiLuFault<<OBJS_PowerOutShortFault<<OBJS_DeviceBatteryOffFault
                         <<OBJS_DeviceBatteryShortFault<<OBJS_DeviceBatteryOverdischargeFault<<OBJS_StandbyPowerOff<<OBJS_StandbyPowerShort<<OBJS_StandbyPowerUndervoltage;
    }
    if(m_emergencyStatusList.isEmpty())
    {
        m_emergencyStatusList<<OBJS_Emergency<<OBJS_RemoteEmergency<<OBJS_FireEmergency<<OBJS_ForceEmergency<<OBJS_InputEmergency<<OBJS_ManualEmergency<<OBJS_DIEmergency
                             <<OBJS_TestEmergency<<OBJS_SystemEmergency<<OBJS_UndervoltageEmergency<<OBJS_AutoEmergency<<OBJS_MonthEmergency<<OBJS_YearEmergency<<OBJS_SimulateFireEmergency;
    }
}


void CGlobal::initDeviceType()
{
    m_deviceTypes.resize(256);
    m_deviceTypes.fill(0);
    CDeviceType* pDevType = NULL;

    pDevType = new CDeviceType(0, "未定义");
    pDevType->setRawImgReal(":/images/devtype/0.svg");
    m_deviceTypes[0] = pDevType;

    pDevType = new CDeviceType(255, "未定义");
    pDevType->setRawImgReal(":/images/devtype/0.svg");
    m_deviceTypes[255] = pDevType;

    pDevType = new CDeviceType(100, "吊装安全出口标志灯(智能型)");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[100] = pDevType;

    pDevType = new CDeviceType(101, "吊装左向标志灯(智能型)");
    pDevType->setRawImgReal(":/images/devtype/50.svg");
    m_deviceTypes[101] = pDevType;

    pDevType = new CDeviceType(102, "吊装双向标志灯(智能型)");
    pDevType->setRawImgReal(":/images/devtype/56.svg");
    m_deviceTypes[102] = pDevType;

    pDevType = new CDeviceType(103, "壁挂安全出口标志灯(智能型)");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[103] = pDevType;

    pDevType = new CDeviceType(104, "壁挂左向标志灯(智能型)");
    pDevType->setRawImgReal(":/images/devtype/50.svg");
    m_deviceTypes[104] = pDevType;

    pDevType = new CDeviceType(105, "壁挂右向标志灯(智能型)");
    pDevType->setRawImgReal(":/images/devtype/51.svg");
    m_deviceTypes[105] = pDevType;

    pDevType = new CDeviceType(106, "壁挂双向标志灯(智能型)");
    pDevType->setRawImgReal(":/images/devtype/56.svg");
    m_deviceTypes[106] = pDevType;

    pDevType = new CDeviceType(107, "壁挂楼层标志灯0F-9F(智能型)");
    pDevType->setRawImgReal(":/images/devtype/57.svg");
    m_deviceTypes[107] = pDevType;

    pDevType = new CDeviceType(108, "壁挂楼层标志灯0F-9F之外(智能型)");
    pDevType->setRawImgReal(":/images/devtype/57.svg");
    m_deviceTypes[108] = pDevType;

    pDevType = new CDeviceType(109, "可变状态灯(智能型)");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[109] = pDevType;

    pDevType = new CDeviceType(110, "多信息复合灯(智能型)");
    pDevType->setRawImgReal(":/images/devtype/57.svg");
    m_deviceTypes[110] = pDevType;

    pDevType = new CDeviceType(111, "安全出口灯(带字)");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[111] = pDevType;

    pDevType = new CDeviceType(112, "向前灯");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[112] = pDevType;

    pDevType = new CDeviceType(120, "单向地埋标志灯(智能型)");
    pDevType->setRawImgReal(":/images/devtype/52.svg");
    m_deviceTypes[120] = pDevType;

    pDevType = new CDeviceType(121, "双向地埋标志灯(智能型)");
    pDevType->setRawImgReal(":/images/devtype/53.svg");
    m_deviceTypes[121] = pDevType;

    pDevType = new CDeviceType(130, "吊装安全出口标志灯(非智能型)");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[130] = pDevType;

    pDevType = new CDeviceType(131, "吊装左向标志灯(非智能型)");
    pDevType->setRawImgReal(":/images/devtype/50.svg");
    m_deviceTypes[131] = pDevType;

    pDevType = new CDeviceType(132, "吊装双向标志灯(非智能型)");
    pDevType->setRawImgReal(":/images/devtype/56.svg");
    m_deviceTypes[132] = pDevType;

    pDevType = new CDeviceType(133, "壁挂安全出口标志灯(非智能型)");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[133] = pDevType;

    pDevType = new CDeviceType(134, "壁挂左向标志灯(非智能型)");
    pDevType->setRawImgReal(":/images/devtype/50.svg");
    m_deviceTypes[134] = pDevType;

    pDevType = new CDeviceType(135, "壁挂右向标志灯(非智能型)");
    pDevType->setRawImgReal(":/images/devtype/51.svg");
    m_deviceTypes[135] = pDevType;

    pDevType = new CDeviceType(136, "壁挂双向标志灯(非智能型)");
    pDevType->setRawImgReal(":/images/devtype/56.svg");
    m_deviceTypes[136] = pDevType;

    pDevType = new CDeviceType(137, "壁挂楼层标志灯0F-9F(非智能型)");
    pDevType->setRawImgReal(":/images/devtype/57.svg");
    m_deviceTypes[137] = pDevType;

    pDevType = new CDeviceType(138, "壁挂楼层标志灯0F-9F之外(非智能型)");
    pDevType->setRawImgReal(":/images/devtype/57.svg");
    m_deviceTypes[138] = pDevType;

    pDevType = new CDeviceType(150, "应急照明灯(3W)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[150] = pDevType;

    pDevType = new CDeviceType(151, "应急照明灯(5W)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[151] = pDevType;

    pDevType = new CDeviceType(152, "应急照明灯(6W)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[152] = pDevType;

    pDevType = new CDeviceType(153, "应急照明灯(9W)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[153] = pDevType;

    pDevType = new CDeviceType(154, "应急照明灯(10w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[154] = pDevType;

    pDevType = new CDeviceType(155, "应急照明灯(12w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[155] = pDevType;

    pDevType = new CDeviceType(156, "应急照明灯(15w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[156] = pDevType;

    pDevType = new CDeviceType(157, "应急照明灯(30w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[157] = pDevType;

    pDevType = new CDeviceType(170, "应急照明感应灯(3w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[170] = pDevType;

    pDevType = new CDeviceType(171, "应急照明感应灯(6w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[171] = pDevType;

    pDevType = new CDeviceType(172, "应急照明感应灯(9w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[172] = pDevType;

    pDevType = new CDeviceType(173, "应急照明感应灯(12w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[173] = pDevType;

    pDevType = new CDeviceType(174, "应急照明感应灯(15w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[174] = pDevType;

    pDevType = new CDeviceType(180, "B型照明灯(30W-吸顶)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[180] = pDevType;

    pDevType = new CDeviceType(181, "B型照明灯(30W-吊装)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[181] = pDevType;

    pDevType = new CDeviceType(182, "B型照明灯(30W-嵌顶)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[182] = pDevType;

    pDevType = new CDeviceType(183, "B型照明灯(50W-座式)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[183] = pDevType;

    pDevType = new CDeviceType(183, "B型照明灯(100W-座式)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[183] = pDevType;

    pDevType = new CDeviceType(218, "吊装出口标志灯");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[218] = pDevType;

    pDevType = new CDeviceType(219, "吊装左向标志灯");
    pDevType->setRawImgReal(":/images/devtype/50.svg");
    m_deviceTypes[219] = pDevType;

    pDevType = new CDeviceType(220, "吊装双向标志灯");
    pDevType->setRawImgReal(":/images/devtype/56.svg");
    m_deviceTypes[220] = pDevType;

    pDevType = new CDeviceType(221, "壁挂左向标志灯");
    pDevType->setRawImgReal(":/images/devtype/50.svg");
    m_deviceTypes[221] = pDevType;

    pDevType = new CDeviceType(222, "壁挂右向标志灯");
    pDevType->setRawImgReal(":/images/devtype/51.svg");
    m_deviceTypes[222] = pDevType;

    pDevType = new CDeviceType(223, "壁挂双向标志灯");
    pDevType->setRawImgReal(":/images/devtype/56.svg");
    m_deviceTypes[223] = pDevType;

    pDevType = new CDeviceType(224, "单向地埋标志灯");
    pDevType->setRawImgReal(":/images/devtype/52.svg");
    m_deviceTypes[224] = pDevType;

    pDevType = new CDeviceType(225, "双向地埋标志灯");
    pDevType->setRawImgReal(":/images/devtype/53.svg");
    m_deviceTypes[225] = pDevType;

    pDevType = new CDeviceType(226, "楼层标志灯0F-9F");
    pDevType->setRawImgReal(":/images/devtype/57.svg");
    m_deviceTypes[226] = pDevType;

    pDevType = new CDeviceType(227, "安全出口灯");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[227] = pDevType;

    pDevType = new CDeviceType(228, "应急照明灯");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[228] = pDevType;

    pDevType = new CDeviceType(229, "楼层标志灯(0F-9F之外)");
    pDevType->setRawImgReal(":/images/devtype/57.svg");
    m_deviceTypes[229] = pDevType;

    pDevType = new CDeviceType(1, "带电池双面疏散出口灯");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[1] = pDevType;

    pDevType = new CDeviceType(2, "带电池双面左向灯");
    pDevType->setRawImgReal(":/images/devtype/50.svg");
    m_deviceTypes[2] = pDevType;

    pDevType = new CDeviceType(3, "带电池双面双向灯");
    pDevType->setRawImgReal(":/images/devtype/56.svg");
    m_deviceTypes[3] = pDevType;

    pDevType = new CDeviceType(4, "带电池双面多信息复合灯");
    pDevType->setRawImgReal(":/images/devtype/57.svg");
    m_deviceTypes[4] = pDevType;

    pDevType = new CDeviceType(5, "带电池单面疏散出口灯");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[5] = pDevType;

    pDevType = new CDeviceType(6, "带电池单面安全出口灯");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[6] = pDevType;

    pDevType = new CDeviceType(7, "带电池单面左向灯");
    pDevType->setRawImgReal(":/images/devtype/50.svg");
    m_deviceTypes[7] = pDevType;

    pDevType = new CDeviceType(8, "带电池单面右向灯");
    pDevType->setRawImgReal(":/images/devtype/51.svg");
    m_deviceTypes[8] = pDevType;

    pDevType = new CDeviceType(9, "带电池单面双向灯");
    pDevType->setRawImgReal(":/images/devtype/56.svg");
    m_deviceTypes[9] = pDevType;

    pDevType = new CDeviceType(10, "带电池单面楼层灯");
    pDevType->setRawImgReal(":/images/devtype/57.svg");
    m_deviceTypes[10] = pDevType;

    pDevType = new CDeviceType(11, "带电池单面可变状态灯");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[11] = pDevType;

    pDevType = new CDeviceType(12, "带电池单面向前灯");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[12] = pDevType;

    pDevType = new CDeviceType(30, "带电池双面疏散出口灯(UC)");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[30] = pDevType;

    pDevType = new CDeviceType(31, "带电池双面左向灯(UC)");
    pDevType->setRawImgReal(":/images/devtype/50.svg");
    m_deviceTypes[31] = pDevType;

    pDevType = new CDeviceType(32, "带电池双面双向灯(UC)");
    pDevType->setRawImgReal(":/images/devtype/56.svg");
    m_deviceTypes[32] = pDevType;

    pDevType = new CDeviceType(33, "带电池单面疏散出口灯(UC)");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[33] = pDevType;

    pDevType = new CDeviceType(34, "带电池单面左向灯(UC)");
    pDevType->setRawImgReal(":/images/devtype/50.svg");
    m_deviceTypes[34] = pDevType;

    pDevType = new CDeviceType(35, "带电池单面右向灯(UC)");
    pDevType->setRawImgReal(":/images/devtype/51.svg");
    m_deviceTypes[35] = pDevType;

    pDevType = new CDeviceType(36, "带电池单面双向灯(UC)");
    pDevType->setRawImgReal(":/images/devtype/56.svg");
    m_deviceTypes[36] = pDevType;

    pDevType = new CDeviceType(37, "带电池单面楼层灯(UC)");
    pDevType->setRawImgReal(":/images/devtype/57.svg");
    m_deviceTypes[37] = pDevType;

    pDevType = new CDeviceType(50, "带电池照明灯(3w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[50] = pDevType;

    pDevType = new CDeviceType(51, "带电池照明灯(5w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[51] = pDevType;

    pDevType = new CDeviceType(52, "带电池照明灯(6w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[52] = pDevType;

    pDevType = new CDeviceType(53, "带电池照明灯(9w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[53] = pDevType;

    pDevType = new CDeviceType(54, "带电池照明灯(10w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[54] = pDevType;

    pDevType = new CDeviceType(55, "带电池照明灯(12w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[55] = pDevType;

    pDevType = new CDeviceType(56, "带电池照明灯(15w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[56] = pDevType;

    pDevType = new CDeviceType(57, "带电池照明灯(30w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[57] = pDevType;

    pDevType = new CDeviceType(70, "带电池感应照明灯(3w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[70] = pDevType;

    pDevType = new CDeviceType(71, "带电池感应照明灯(6w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[71] = pDevType;

    pDevType = new CDeviceType(72, "带电池感应照明灯(9w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[72] = pDevType;

    pDevType = new CDeviceType(73, "带电池感应照明灯(12w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[73] = pDevType;

    pDevType = new CDeviceType(74, "带电池感应照明灯(15w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[74] = pDevType;

    pDevType = new CDeviceType(80, "带电池微波感应照明灯(3w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[80] = pDevType;

    pDevType = new CDeviceType(81, "带电池微波感应照明灯(6w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[81] = pDevType;

    pDevType = new CDeviceType(82, "带电池微波感应照明灯(9w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[82] = pDevType;

    pDevType = new CDeviceType(83, "带电池微波感应照明灯(12w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[83] = pDevType;

    pDevType = new CDeviceType(84, "带电池微波感应照明灯(15w)");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[84] = pDevType;

    pDevType = new CDeviceType(15, "带电池双面疏散出口灯m");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[15] = pDevType;

    pDevType = new CDeviceType(16, "带电池双面左向灯m");
    pDevType->setRawImgReal(":/images/devtype/50.svg");
    m_deviceTypes[16] = pDevType;

    pDevType = new CDeviceType(17, "带电池双面双向灯m");
    pDevType->setRawImgReal(":/images/devtype/56.svg");
    m_deviceTypes[17] = pDevType;

    pDevType = new CDeviceType(18, "带电池双面多信息复合灯m");
    pDevType->setRawImgReal(":/images/devtype/57.svg");
    m_deviceTypes[18] = pDevType;

    pDevType = new CDeviceType(19, "未知带电池疏散标志灯m");
    pDevType->setRawImgReal(":/images/devtype/50.svg");
    m_deviceTypes[19] = pDevType;

    pDevType = new CDeviceType(40, "带电池单面疏散出口灯m");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[40] = pDevType;

    pDevType = new CDeviceType(41, "带电池单面安全出口灯m");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[41] = pDevType;

    pDevType = new CDeviceType(42, "带电池单面左向灯m");
    pDevType->setRawImgReal(":/images/devtype/50.svg");
    m_deviceTypes[42] = pDevType;

    pDevType = new CDeviceType(43, "带电池单面右向灯m");
    pDevType->setRawImgReal(":/images/devtype/51.svg");
    m_deviceTypes[43] = pDevType;

    pDevType = new CDeviceType(44, "带电池单面双向灯m");
    pDevType->setRawImgReal(":/images/devtype/56.svg");
    m_deviceTypes[44] = pDevType;

    pDevType = new CDeviceType(45, "带电池单面楼层灯m");
    pDevType->setRawImgReal(":/images/devtype/57.svg");
    m_deviceTypes[45] = pDevType;

    pDevType = new CDeviceType(46, "带电池单面可变状态灯m");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[46] = pDevType;

    pDevType = new CDeviceType(47, "带电池单面向前灯m");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[47] = pDevType;

    pDevType = new CDeviceType(79, "带电池照明灯m");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[79] = pDevType;

    pDevType = new CDeviceType(115, "双面疏散出口灯m");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[115] = pDevType;

    pDevType = new CDeviceType(116, "双面左向灯m");
    pDevType->setRawImgReal(":/images/devtype/50.svg");
    m_deviceTypes[116] = pDevType;

    pDevType = new CDeviceType(117, "双面双向灯m");
    pDevType->setRawImgReal(":/images/devtype/51.svg");
    m_deviceTypes[117] = pDevType;

    pDevType = new CDeviceType(118, "双面多信息复合灯m");
    pDevType->setRawImgReal(":/images/devtype/57.svg");
    m_deviceTypes[118] = pDevType;

    pDevType = new CDeviceType(119, "未知疏散标志灯m");
    pDevType->setRawImgReal(":/images/devtype/56.svg");
    m_deviceTypes[119] = pDevType;

    pDevType = new CDeviceType(122, "单向地埋灯m");
    pDevType->setRawImgReal(":/images/devtype/52.svg");
    m_deviceTypes[122] = pDevType;

    pDevType = new CDeviceType(123, "双向地埋灯m");
    pDevType->setRawImgReal(":/images/devtype/53.svg");
    m_deviceTypes[123] = pDevType;

    pDevType = new CDeviceType(124, "未知地埋灯m");
    pDevType->setRawImgReal(":/images/devtype/53.svg");
    m_deviceTypes[124] = pDevType;

    pDevType = new CDeviceType(140, "单面疏散出口灯m");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[140] = pDevType;

    pDevType = new CDeviceType(141, "单面安全出口灯m");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[141] = pDevType;

    pDevType = new CDeviceType(142, "单面左向灯m");
    pDevType->setRawImgReal(":/images/devtype/50.svg");
    m_deviceTypes[142] = pDevType;

    pDevType = new CDeviceType(143, "单面右向灯m");
    pDevType->setRawImgReal(":/images/devtype/51.svg");
    m_deviceTypes[143] = pDevType;

    pDevType = new CDeviceType(144, "单面双向灯m");
    pDevType->setRawImgReal(":/images/devtype/56.svg");
    m_deviceTypes[144] = pDevType;

    pDevType = new CDeviceType(145, "单面楼层灯m");
    pDevType->setRawImgReal(":/images/devtype/57.svg");
    m_deviceTypes[145] = pDevType;

    pDevType = new CDeviceType(146, "单面可变状态灯m");
    pDevType->setRawImgReal(":/images/devtype/58.svg");
    m_deviceTypes[146] = pDevType;

    pDevType = new CDeviceType(147, "单面向前灯m");
    pDevType->setRawImgReal(":/images/devtype/57.svg");
    m_deviceTypes[147] = pDevType;

    pDevType = new CDeviceType(179, "应急照明灯m");
    pDevType->setRawImgReal(":/images/devtype/59.svg");
    m_deviceTypes[179] = pDevType;

}

QGridLayout* CGlobal::CreateGridLayout(QWidget *parent, int nmargin, int nspacing)//????????
{
    QGridLayout *gridlayout = new QGridLayout(parent);
    gridlayout->setMargin(nmargin);
    gridlayout->setSpacing(nspacing);
    return gridlayout;
}

QVBoxLayout* CGlobal::CreateVBoxLayout(QWidget *parent, int nmargin, int nspacing)//???????????
{
    QVBoxLayout *vlayout = new QVBoxLayout(parent);
    vlayout->setMargin(nmargin);
    vlayout->setSpacing(nspacing);
    return vlayout;
}

QHBoxLayout* CGlobal::CreateHBoxLayout(QWidget *parent, int nmargin, int nspacing)//??????????
{
    QHBoxLayout *hlayout = new QHBoxLayout(parent);
    hlayout->setMargin(nmargin);
    hlayout->setSpacing(nspacing);
    return hlayout;
}

QPushButton* CGlobal::CreatePushButton(QWidget *parent, QString strText, QString strico, QString strtooltip, QSizePolicy::Policy horizontal, QSizePolicy::Policy vertical)
{
    QPushButton *newPushButton = new QPushButton(QIcon(strico), strText, parent);
    newPushButton->setToolTip(strtooltip);
    newPushButton->setSizePolicy(horizontal, vertical);
    return newPushButton;
}

QToolButton* CGlobal::CreateToolButton(QWidget *parent, QString strico, QString strtooltip, QSizePolicy::Policy horizontal, QSizePolicy::Policy vertical)
{
    QToolButton *newToolButton = new QToolButton(parent);
    QFont font;
    font.setPointSize(10);
    newToolButton->setFont(font);
    newToolButton->setText(strtooltip);
    newToolButton->setIcon(QIcon(strico));
//    newToolButton->setToolTip(strtooltip);
    newToolButton->setFixedSize(80, 80);
    newToolButton->setIconSize(QSize(32, 32));
    newToolButton->setCursor(Qt::PointingHandCursor);
    newToolButton->setAutoRaise(false);
    newToolButton->setSizePolicy(horizontal, vertical);
    newToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    return newToolButton;
}

QToolButton* CGlobal::CreateLedButton(QWidget *parent, QString strText, QString strico, QString strtooltip, QSizePolicy::Policy horizontal, QSizePolicy::Policy vertical)
{
    QToolButton *newToolButton = new QToolButton(parent);
    newToolButton->setText(strText);
    newToolButton->setIcon(QIcon(strico));
    newToolButton->setToolTip(strtooltip);
    newToolButton->setAutoRaise(true);
    newToolButton->setSizePolicy(horizontal, vertical);
    newToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
//    newToolButton->setStyleSheet("border-radius: 10px; border: 1px solid #6F6F6F;");
    return newToolButton;
}

void CGlobal::setLayer(CLayer *layer)
{
    CGlobal::instance()->designTreeView()->setLayer(layer);
    CGlobal::instance()->designContainer()->setCurrentView(0);
}


const QList<CLayer *> CGlobal::getLayers(const QList<CController*> &controllerList)
{
    QList<CLayer *> layerList;
    for(int i=0; i<controllerList.count(); ++i)
    {
        QList<CLayer *> tempLayerList = controllerList.at(i)->layers();//??????????
        for(int j=0; j<tempLayerList.count(); ++j)//?????????????????????????
        {
            layerList.append(tempLayerList.at(j));
        }
    }
    return layerList;
}

void CGlobal::soundType(SoundType iType, QString sType)
{
    QString strPath = QCoreApplication::applicationDirPath() + sType;
    if(!QFile::exists(strPath))
    {
       return;
    }
    QTextCodec* utf8 = QTextCodec::codecForName("UTF-8");
    QTextCodec* gb2312 = QTextCodec::codecForName("GB2312");
    QByteArray byte = gb2312->fromUnicode(utf8->toUnicode(strPath.toUtf8()));
    m_audioOutput->stopAllSounds();
    m_audioOutput->play2D(byte.data(), true);
    m_audioOutput->setSoundVolume(1.0);
    m_soundName = iType;
}


void CGlobal::setMsgText(const QString &str)
{
    m_information = str;
    m_labelSystemMsg->setText(m_information);
}


void CGlobal::setMSgColor(const QColor &color)
{
    QFont font;
    font.setPointSize(12);
    m_infoColor = color;
    QPalette pa;
    pa.setColor(QPalette::WindowText, m_infoColor);
    m_labelSystemMsg->setPalette(pa);
    m_labelSystemMsg->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    m_labelSystemMsg->setFont(font);
}
