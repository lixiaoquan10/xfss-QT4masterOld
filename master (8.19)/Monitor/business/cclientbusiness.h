#ifndef CCLIENTBUSINESS_H
#define CCLIENTBUSINESS_H

#include "struct/shardEnum.h"
#include <QSet>
#include <QObject>
#include <QList>
#include <QVariant>
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>
#include <QProcess>

enum NotifyInforType {
    NIT_InitControllerResult = 0,
    NIT_Mute = 1,
    NIT_SelfCheckStart = 2,
    NIT_SelfCheckStop = 3,
    NIT_EvacuationMode = 4,
    NIT_MainPowerFault = 5,
    NIT_StandbyPowerFault = 6,
    NIT_Reset = 7,
    NIT_Start = 8,
    NIT_Stop = 9,

    NIT_MonthCheckBegin = 10,
    NIT_MonthCheckEnd = 11,
    NIT_YearCheckBegin = 12,
    NIT_YearCheckEnd = 13,
    NIT_EmergencyStart = 14,

    NIT_EmergencyStop = 15,
    NIT_CheckTimeUpdate = 16,
    NIT_FirstFireAlarmPoint = 17,
    NIT_MainPowerWork = 18,
    NIT_StandbyPowerWork = 19,

    NIT_FASCommunicationFault = 20,
    NIT_HasFASSystem = 21,
    NIT_ExitProgram  = 22,
    NIT_StartSpeedUp = 23,
    NIT_StopSpeedUp = 24,
    NIT_Relogin = 25
};

enum SoundType { ST_None, ST_Fault, ST_Fire, ST_Switch };
enum StatusRecordType { SRT_Start = 1,  SRT_Fault = 2, SRT_Emergency = 3, SRT_ALL = 4 };
enum StatusRecordOpreator { SRO_Add = 1, SRO_Del = 2, SRO_DelAll = 3 };

typedef struct {
    int keyId;
    int statusId;
    QDateTime time;
    QString distributionId;
    int loopID;
    int deviceID;
    QString type;
    QString area;
    QString location;
    QString status;
    QString layer;
} StatusInfo, *pStatusInfo;

class CObject;
class QTimer;
class CMsgDeviceInfo;
class CController;
class CCanport;
class CDistribution;
class CLoop;
class CDevice;
class CFirePoint;
//class CPointDeviceItem;
class CCommunication;
class QDomDocument;
class QDomElement;
class QDomText;
class CClientBusiness : public QObject
{
    Q_OBJECT
public:
    enum { MainPowerFault = 0x01, ChargerFault = 0x02, Charging = 0x04 };
    explicit CClientBusiness(QObject *parent = 0);
    ~CClientBusiness();

    CController* loginController() const { return m_cantroller; }
    void setLoginController(CController* cantroller) { m_cantroller = cantroller; }

    void showTest(bool bwrite);
    void InitController();
    void clearExpectObject(const int type = SRT_ALL);
    void destroyCommunication();
    void setCommunicationConfig();
    void startCommunication();
    void stopCommunication();
    bool verifyAdminUser(const int commandType, const QString& password);
    bool verifyAdminUser(const int commandType);
    void addLog(const QString strUser, const QString strTitle, const QString strArea, const QString strLocation, const QString strRemark,
                const int CanPort = 0, const int CanDeviceAddress = 0, const int Loop = 0, const int LampDevice = 0, const QString strType = "其他");
    bool exeCommand(int commandType, CMsgStructBase *msgData);
    const CMsgPrintSet& getPrintSet() const { return m_printSet; }
    const CMsgCheckSet& getCheckSet() const { return m_checkSet; }
    void  ParsePacket(int nMsgType, const char *data, int nLen);
    void clearStatusObject(bool reserveLogin = false);
    void updateIsLoopRemoveLogin();
    void removeLoginObject(int canportAdd = 0, int distributionAdd = 0, int loopAdd = 0);
    void setNewPwd(const QString strpwd);
    CCanport* currentPanel() const;
    void ForceEmergency();
    void InputEmergency();
    void ActiveInputEmergency();
    void PassiveInputEmergency();
    void lampControl(CDevice* device, bool isDoubleDirection = false, bool isLeft = false);
    void Emergency();
    void firePointEmergencyByGroup(int firePointID);
    void firePointEmergencyByLayoutPage(int firePointID);
    void firePointEmergencyByDistribution(int firePointID);
    void DeviceResetStatus();
    void FirepointResetStatus();
    void DeviceStartStatus(CDevice* device, QString directionStatus, QDateTime time);
    void DeviceBatteryStatus(CDevice* device, int status, int power);

    QString statusToString(bool status) { return status? "True" : "False";}

    QString generateUniqueID();
    void saveDataToXml(const QString& filePath, const QString& newData);
    void mainPowerStart();
    bool isUCLamp(int ID);
    bool isDoubleLamp(int lamptypeID);
    void setCANconfigTrue() { m_isCANconfig = true; }

    int deviceType(int typeId);

    //云平台相关函数
    void XmlInit();
    void XmlHeartbeat();
    void XmlApplyForProjectID();
    void XmlUploadDeviceInfo();
    void XmlCanDeviceInfo();
    void XmlChannelInfo();
    void XmlLampInfo();
    void XmlLayoutPage();
    void XmlPicture(CMsgObjectStatus* xmlObjectStatus);
    void XmlLampCoordinate();
    void XmlFirePoint();
    void XmlFirePointCoordinate();
    void XmlLampToFirePoint();
    void XmlLampDefaultDirection();
    void XmlHostElectricParameter();
    void XmlCanDeviceElectricParameter(int ID);
    void XmlEnableUpload(QString enable);
    void XmlHostStateUpload(bool isMainPowerOK, QString batteryState);
    void XmlCanDeviceStateUpload(CDistribution* distribution);
    void XmlChannelStateUpload(CLoop* loop);
    void XmlLampStateUpload(int ID, bool isCommunicationOK, bool isLightFault, bool isWarning);
    void XmlEmergencyInputUpload();
    void XmlManualLaunchUpload();
    void XmlUploadFirePointWarning(int deviceAddress, int loopAddress, int terminalAddress);
    void XmlUploadLampDirection(int lampID, QString direction);
    void XmlResetDeclare();
    void XmlHostControl(CMsgObjectStatus* xmlObjectStatus);
    void XmlLampControl(CMsgObjectStatus* xmlObjectStatus);
    void XmlQueryAllState();

    void uploadLampStatus(CDevice* device);

    //服务端相关函数
    void serverXmlInit();
    void serverHeartbeat();
    void serverCanDeviceInfo();
    void serverChannelInfo();
    void serverLampInfo();
    void serverFirePoint();
    void serverLampToFirePoint();
    void serverLampConnection();

    void serverAllState();
    void serverPageInfo();
    void serverPicture(CMsgObjectStatus* xmlObjectStatus);
    void serverLampCoordinate();
    void serverFirePointCoordinate();
    void serverLampSoftwareInfo(int ID, int SoftwareNumber, int SoftwareVersion);
    void serverDeviceSoftwareInfo(CDistribution* distribution);
    void serverDeviceRealtimeData(CDistribution* distribution);
    void serverHostStateUpload(bool isMainPowerOK, QString batteryState);
    void serverCentralizedPowerStateUpload(CDistribution* distribution);
    void serverLampStateUpload(int ID, bool isCommunicationOK, bool isLightFault, bool isWarning);
    void serverFirePointWarningUpload(int deviceAddress, int loopAddress, int terminalAddress);
    void serverEmergencyInputUpload();
    void serverManualLaunchUpload();
    void serverLampDirectionUpload(int lampID, QString direction);
    void serverEventList(CMsgObjectStatus* xmlObjectStatus);

    void serverHostControl(CMsgObjectStatus* xmlObjectStatus);
    void serverSetAutoStateUpload(QString enable);
    void serverResetDeclareUpload();

public slots:
    void slotCheckLoginInfo();
    void mainPowerStop();
    void slot_saveLoginData();
    void slot_flashControl();
    void objectStatusChanged(CObject* object, short status, unsigned char oldValue, unsigned char newValue, unsigned int time);

private:
    bool hasStandbyPowerFault(CCanport* canport) const;
    void createPath(const QString& path) const;

    void closeController();
    void clearProjectFile();
    void reset();    
    CCanport* createUndefinedCanport(CController* controller, int address, int keyId=0);
    CDistribution *createUndefinedDistribution(CCanport *canport, int address , QString str = "", int keyId = 0);
    CLoop* createUndefinedLoop(CDistribution *distribution, int address, int keyId=0);
    CLoop* createUndefinedLoop(CCanport *canport, int address, int keyId=0);
    CDevice* createUndefinedDevice(CLoop* loop);


private slots:
    void timeOut();
    void recvData(int nMsgType, QByteArray &data);
//    void slot_ledStatusSend();
//    void driveDeviceDirection(CPointDeviceItem *item, uchar direction, uchar delay, bool isFlow);
//    void objectStatusChanged(CObject* object, short status, unsigned char oldValue, unsigned char newValue, unsigned int time);

signals:
    void notifyInformation(int type, const QList<QVariant>& information);
    void commandResult(int commandType, CMsgStructBase* msgData);
    void requestVerifyAdmin(int commandType, CMsgStructBase *msgData);
    void statusRecord(const StatusInfo& statusInfo, int type, int opr);

private:
    const QString m_strNormal;
    const QString m_strFault;
    const QString m_strCharging;
    QString m_strPassword;
    QString m_projectPath;
    QString m_projectFile;
    QString m_advPassword;

    bool m_isInitController;
    bool m_isRecvStatus;
    bool m_bForceEmergency;

    int m_currentStatusId;
    int m_currentRecvStatusId;
    int m_currentRecvStatusNumber;

    QSet<int> m_faultObjects;
    QSet<int> m_startObjects;

    QTimer* m_timer;
    QTimer* m_saveLogintimer;
    QTimer* m_flashTimer;
    StatusInfo m_statusInfo;
    CMsgPrintSet m_printSet;
    CMsgCheckSet m_checkSet;
    CMsgLog* m_msgLog;
    CCommunication* m_communication;    
    CMsgCanportInfo m_canportInfo;
    CController* m_cantroller;

    QDomDocument m_xmldoc;
    QDomElement m_xmlroot;
    QDomElement m_xmlitem_1;
    QDomElement m_xmlitem_2;
    QDomText m_xmltext;
    QString m_xmlheader;

    bool m_isQueryAllState;
    bool m_isCANconfig;

    QTimer* m_mainPowerStoptimer;

    QProcess *m_process_light; // 声明一个 QProcess 成员变量用于执行外部脚本

    QList<CMsgObjectStatus*> m_msg;
public:
    bool m_masterStateUploadFlag;
    bool m_cloudStateUploadFlag;
};

#endif // CCLIENTBUSINESS_H
