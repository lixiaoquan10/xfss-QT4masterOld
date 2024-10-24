#ifndef CGLOBAL_H
#define CGLOBAL_H

#include "bservice.h"
#include "struct/cmsgrouteconfig.h"
#include <QHash>
#include <QFile>
#include <QVariant>
#include <QDateTime>
#include <QTextCodec>

enum DllStatus {DS_TXNone = 0, DS_TXType = 1, DS_TXInitial = 2, DS_TXStart = 3, DS_TXStop = 4};

enum {Confirm = 0x01, Refuse = 0x02, TestCom = 0x03, UploadTest = 0x04,
      SetCardSpeed = 0x05, ReadCardSpeed = 0x06, UploadCardSpeed = 0x07,
      Reset = 0x08, Emergency = 0x09,
      SendCanData = 0x11, ReadCanState = 0x12, UploadCanState = 0x13,
      SetFasConfig = 0x21, ReadFasConfig = 0x22, SendFasFire = 0x23, SetOutputPort = 0x24,
      ReadInputPort = 0x25, UploadInputPort = 0x26, FasStatus = 0x27,
      ReadPowerState = 0x31, UploadPowerState = 0x32, SetPowerStatus = 0x33,
      SetLedStatus = 0x41, UploadKeyInfo = 0x42,
      PrintData = 0x43,  ReadPrintState = 0x44, UploadPrintState = 0x45};
/*调整顺序*/
//enum {LedEmergency = 1, LedMainPower = 2, LedStandbyPower = 3, LedAuto = 4, LedManual = 5, LedFault = 6, LedSystemFault = 7, LedMute = 8, LedRun = 9, LedMonthCheck = 10, LedYearCheck = 11};
enum {LedMainPower = 1, LedStandbyPower = 2, LedSystemFault = 3, LedFault = 4, LedEmergency = 5, LedAuto = 6, LedManual = 7, LedMute = 8};


enum {LedOff = 0, LedOn = 1, LedFlash = 2};

enum {KeyEmergency = 1, KeyAuto = 2, KeyManual = 3, KeySelfCheck = 4, KeyMute = 5, KeyReset = 6 };
enum CommunicationType {CT_NoCard = 0, CT_LinkageCard = 1, CT_LedCard = 2, CT_CanCard1 = 3, CT_CanCard2 = 4, CT_FASCard = 5, CT_TestCard=6};
enum {MainPower = 1, StandbyPower = 2, Charger = 3, BatteryLine = 4, PowerOutput = 5, BatteryLowVoltage = 6, BatteryLowQuantity = 7, BatteryOverDischarge = 8,
      MainPowerR = 129, StandbyPowerR = 130, ChargerR = 131, BatteryLineR = 132, PowerOutputR = 133, BatteryLowVoltageR = 134, BatteryLowQuantityR = 135, BatteryOverDischargeR = 136};

enum ObjectStatus {
    OBJS_None = 0,
    OBJS_Start = 1,
    OBJS_Fault = 2,
    OBJS_Emergency = 3,
    OBJS_Fire = 4,
    OBJS_Online = 5,
    OBJS_LoginDeviceType = 6,
    OBJS_PowerChargerValue = 7,
    OBJS_PowerAutoValue = 8,
    OBJS_AutoEvacuation = 9,
    OBJS_InputPort = 10,
    OBJS_OutputPort = 11,
    OBJS_NewRegister = 12,
//Start:
    OBJS_PathStart = 13,
    OBJS_EvacuationGroupStart = 14,
    OBJS_UpStart = 15,
    OBJS_DownStart = 16,
    OBJS_LeftStart = 17,
    OBJS_RightStart = 18,
    OBJS_LightStart = 19,
//Fault:
    OBJS_SystemFault = 20,
    OBJS_PowerFault = 21,
    OBJS_MainPowerFault = 22,
    OBJS_StandbyPowerFault = 23,
    OBJS_ConcenterPowerFault = 24,
    OBJS_ChargerFault = 25,
    OBJS_PowerOutputFault = 26,
    OBJS_BatteryLineFault = 27,
    OBJS_BatteryLineFault_1 = 28,
    OBJS_BatteryLineFault_2 = 29,
    OBJS_BatteryLineFault_3 = 30,
    OBJS_BatteryLowVoltageFault = 31,
    OBJS_BatteryLowVoltageFault_1 = 32,
    OBJS_BatteryLowVoltageFault_2 = 33,
    OBJS_BatteryLowVoltageFault_3 = 34,
    OBJS_BatteryLowQuantityFault = 35,
    OBJS_BatteryOverDischargeFault = 36,
    OBJS_LoopOutShortFault = 37,
    OBJS_LoopOutOverloadFault = 38,
    OBJS_LoopCommunicationFault = 39,
    OBJS_NetCommunicationFault = 40,
    OBJS_LightCommunicationFault = 41,
    OBJS_LightFault = 42,
    OBJS_CheckFault = 43,
    OBJS_OutLoopFault = 44,
    OBJS_DistributionFault = 45,
    OBJS_DistributionCommunicationFault = 46,
    OBJS_PowerCommunicationFault = 47,
    OBJS_FASCommunicationFault = 48,
    OBJS_BatteryOffFault = 69,
    OBJS_BatteryShortFault = 70,
    OBJS_StandbyPowerOff = 74,
    OBJS_StandbyPowerShort = 75,
    OBJS_StandbyPowerUndervoltage = 76,
//Emergency:
    OBJS_RemoteEmergency = 49,
    OBJS_FireEmergency = 50,
    OBJS_ForceEmergency = 51,
    OBJS_InputEmergency = 52,
    OBJS_ManualEmergency = 60,
    OBJS_DIEmergency = 64,
    OBJS_TestEmergency = 65,
    OBJS_SystemEmergency = 66,
    OBJS_UndervoltageEmergency = 67,
    OBJS_AutoEmergency = 68,
    OBJS_MonthEmergency = 69,
    OBJS_YearEmergency = 70,
    OBJS_SimulateFireEmergency = 77,
    OBJS_UnknownFireEmergency = 78,
//Other:
    OBJS_Mute = 53,
    OBJS_FirstFire = 54,
    OBJS_MainPowerWork = 55,
    OBJS_StandbyPowerWork = 56,
    OBJS_DirectionFlash = 57,
    OBJS_PowerKaiLuFault = 58,
    OBJS_PowerOutShortFault = 59,
    OBJS_RunMode = 61,
    OBJS_BatteryOverheating = 62,
    OBJS_ChargeStatus = 63
};

qint64 getTickCount();

class CLog;
class CPrint;
class CAdapterObject;
class CProcessClient;
class CProcessController;
class CProcessDistribution;
class CCommunicationRoute;
class dlgInformationWindow;
class CGlobal
{
private:
    CGlobal();
    ~CGlobal();
public:
    static bool isDebug(){return m_bDebug;}
    static CGlobal* instance();
    static void destroy();
    static bool setDateTime(const QDateTime& t);

    void initParams();
    void SetCallBack(OnDealEvent dlevent){m_dle = dlevent;}
    int DealEvent(int nType, const QByteArray &data);

    void setRouteConfig(const QList<RouteConfig>& routeConfig);
    void write(int nPort, bool bsend, QByteArray byte);
    void closefile();

    void setPanelAddress(const int n) { m_PanelAddress = n; }
    int  panelAddress() const { return m_PanelAddress; }
    QString projectName() const { return m_projectName; }
    void setProjectName(const QString& name) { m_projectName = name; }
    CLog* log() const { return m_log; }
    CPrint* print() const { return m_print; }
    CAdapterObject* adapterObject() { return m_adapterObject; }
    CProcessClient* processClient() const { return m_processClient; }
    CProcessDistribution* processDistribution() const { return m_processDistribution; }
    CProcessController* processController() const { return m_processController; }
    CCommunicationRoute* communicationRoute() const { return m_communicationRoute; }
    dlgInformationWindow* DlgInformationWindow() const { return m_dlgInforWindow; }
    void setDlgInformationWindow(dlgInformationWindow* InforWindow){ m_dlgInforWindow = InforWindow; }

    QTextCodec* getCodec() { return QTextCodec::codecForName("UTF-8"); }

    QString getUidString(const QByteArray &buff, int n);

    void setRecordPage(const int page) { m_recordPage = page; }
    void setRecordNumber(const int number) { m_recordNumber = number; }

public:
    static QString m_workPath;
    static int m_dllStatus;
    static bool m_bDebug;
    int m_nPowerBeatCount;
    bool m_bPowerFault;
    int m_recordPage;
    int m_recordNumber;
    int m_canport1ID;
    int m_canport2ID;

private:
    static CGlobal* m_instance;
    QString m_projectName;
    OnDealEvent m_dle;

    int m_PanelAddress;    

    CLog* m_log;    
    CPrint* m_print;
    QFile m_writefile[6];
    CProcessController* m_processController;
    CProcessDistribution* m_processDistribution;
    CProcessClient* m_processClient;
    CAdapterObject* m_adapterObject;
    CCommunicationRoute* m_communicationRoute;
    dlgInformationWindow* m_dlgInforWindow;
};

#endif // CGLOBAL_H
