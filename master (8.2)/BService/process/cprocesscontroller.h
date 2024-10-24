#ifndef CPROCESSCONTROL_H
#define CPROCESSCONTROL_H

#include "cprocessbase.h"

#include <QList>
#include <QDateTime>
#include <QSet>

#define PROCESSCONTROLLER_SELFCHECK_DURATION 4000

class QTimer;
class CMsgLog;
class CMsgCheckTime;
class CMsgDeviceInfo;
class CMsgEvacuationGroup;
class CMsgObjectStatus;
class CProcessController : public CProcessBase
{
    Q_OBJECT
public:
    CProcessController();
    ~CProcessController();
    inline QString controllerStr() const { return m_controllerStr; }
    void procSendEvent(const int &infoType, const QHash<QString, QVariant>& control, const QByteArray &data);
    void procRecvEvent(const int &infoType, const QHash<QString, QVariant>& control, const QByteArray &data);    

    void initProcess();    
    void emergency(const short emergencyType, const bool enable);
    void sendEmergency(bool enable);
    void sendMonthCheck(bool enable);
    void sendYearCheck(bool enable);
    void setPowerWork(bool isMainPower, bool isStandbyPower);

    void setAutoEvacuation(const bool enable);
    void selfCheck();
    bool isSelfCheck() const { return m_isSelfCheck; }
    int  monthCycle() const { return m_monthCycle; }
    int  monthDuration() const { return m_monthDuration; }
    bool isMonthCheck() const { return m_isMonthCheck; }
    int  yearCycle() const { return m_yearCycle; }
    int  yearDuration() const { return m_yearDuration; }
    bool isYearCheck() const { return m_isYearCheck; }

    void mute(bool enable);
    void reset();
//    void reLogin();

    void manualMonthCheck();
    void manualYearCheck();
    void manualStopYearCheck(){stopYearCheck();}

    void setCheck(int yearCycle, int yearDuration, int monthCycle, int monthDuration);

    void RecordPage(CMsgObjectStatus *msgObjectStatus);

    void manualFire(const QString &address);
    void addLog(int type, const QString& title);
    void addLog(CMsgLog* msgLog);
    void setLedStatus(const int nLedID, const bool nLedStatus);
    void setSpeedUp(bool enable);
    bool isSpeedUp() const { return m_isSpeedUp; }
    bool hasStandbyPowerFault() const;
    bool isEmergencyStatus() const { return m_isEmergency||m_isForceEmergency||m_isFireEmergency||m_isSimulateFireEmergency||m_isInputEmergency||m_isRemoteEmergency;}
    void setSystemFault(bool bFault){m_isSystemFault = bFault;updateLedStatus();}
    bool isSystemFault(){return m_isMainPowerFault && hasStandbyPowerFault();}
    void SystemSet(CMsgObjectStatus *msgObjectStatus);
    void stopYearCheck();
    void stopMonthCheck();
    void setLedupdateTime150ms();
    void setLedupdateTime2s();

    void Sleep(int msec);

private:
    void yearCheck();
    void monthCheck();
    void autoYearCheck();
    void autoMonthCheck();

private slots:
    void stopSelfCheck();
    void yearMonthCheck();
    void updateLedStatus();
    void KeyFlagTimeStop();

private:
    QString m_controllerStr;
    quint64 m_stopMonthCheckTime;
    QDateTime m_MonthCheckTime;
    quint64 m_stopYearCheckTime;
    QDateTime m_YearCheckTime;
    CMsgCheckTime* m_checkTime;
    QTimer* m_timer;
    int m_yearCycle;
    int m_yearDuration;
    int m_monthCycle;
    int m_monthDuration;
    bool m_isSelfCheck;
    bool m_isMonthCheck;
    bool m_isYearCheck;
    bool m_bStop;

    bool m_isEmergency;
    bool m_isForceEmergency;
    bool m_isRemoteEmergency;
    bool m_isFireEmergency;
    bool m_isSimulateFireEmergency;
    bool m_isInputEmergency;
    bool m_isAuto;
    bool m_isMute;
    bool m_isInputPort;
    bool m_isOutPutPort;
    bool m_isCharging;
    bool m_isMainPowerWork;
    bool m_isStandbyPowerWork;

    bool m_isMainPowerFault;
    bool m_isStandByPowerOff;
    bool m_isStandByPowerShort;
    bool m_isStandbyPowerUndervoltage;
    bool m_isChargerFault;
    bool m_isBatteryLineFault;
    bool m_isPowerLowVoltageFault;
    bool m_isBatteryLowVoltageFault;
    bool m_isPowerOutputFault;
    bool m_isFault;
    bool m_isSystemFault;

    bool m_isStartStatus;
    bool m_isReset;
    bool m_isClearFlow;
    bool m_isWillShutdown;    
    bool m_isSpeedUp;

    int m_shutdownTime;
    int m_monthCheckTick;
    int m_yearCheckTick;
    int m_CheckTick;

    bool m_isForbidFaultSound;
    bool m_isForbidEmergencySound;
    bool m_isForbidMonthCheck;
    bool m_isForbidYearCheck;
    int  m_CanNumber;
    int  m_TwinkleTime;

    unsigned char m_nLedStatus1;
    unsigned char m_nLedStatus2;
    unsigned char m_nLedStatus3;
    QTimer* m_timerLed;
    QTimer* m_KeyFlagTime;
    bool m_KeyFlag;
    CMsgLog* m_msgLog;
};

#endif // CPROCESSCONTROL_H
