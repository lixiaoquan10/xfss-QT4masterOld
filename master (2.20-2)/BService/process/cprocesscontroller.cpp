#include "cprocesscontroller.h"
#include "cprocessclient.h"
#include "cprocessdistribution.h"
#include "cprocesssocket.h"
#include "ccommunicationroute.h"
#include "print/cprint.h"
#include "log/clog.h"
#include "struct/cmsgnull.h"
#include "struct/cmsglog.h"
#include "struct/cmsgdeviceinfo.h"
#include "struct/cmsgobjectstatus.h"
#include "struct/cmsgfirepoint.h"
#include "struct/cmsgchecktime.h"
#include "struct/cmsgevacuationgroup.h"
#include "dialog/dlginformationwindow.h"
#include "cglobal.h"
//#include "Windows.h"
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QSettings>
#include <QCoreApplication>

CProcessController::CProcessController() :
    CProcessBase("Controller"), m_isSelfCheck(false), m_isYearCheck(false), m_isMonthCheck(false),
    m_isReset(false), m_isClearFlow(false), m_isWillShutdown(false), m_shutdownTime(0),
    m_isSpeedUp(false), m_monthCheckTick(0), m_yearCheckTick(0), m_stopMonthCheckTime(0)
{    
    m_isAuto = true;
    m_isMute = false;
    m_isInputPort = false;
    m_isEmergency = false;
    m_isForceEmergency = false;
    m_isRemoteEmergency = false;
    m_isFireEmergency = false;
    m_isInputEmergency = false;
    m_isOutPutPort = false;
    m_isMainPowerWork = true;
    m_isStandbyPowerWork = false;

    m_isFault = false;
    m_isMainPowerFault = false;
    m_isStandByPowerOff = false;
    m_isStandByPowerShort = false;
    m_isStandbyPowerUndervoltage = false;
    m_isPowerOutputFault = false;
    m_isBatteryLowVoltageFault = false;
    m_isChargerFault = false;
    m_isSystemFault = false;

    m_PrintWarning = true;
    m_PrintFault = true;
    m_PrintOperation = true;
    m_isForbidFaultSound = false;
    m_isForbidEmergencySound = false;
    m_isForbidMonthCheck = false;
    m_isForbidYearCheck = false;
    m_isSendDirectionAndTwinkleByOne = false;
    m_isUCSendDirectionAndTwinkleByOne = false;
    m_TwinkleTime = 0;

    m_bStop = false;
    m_isStartStatus = false;
    m_nLedStatus1 = 0x0A;
    m_nLedStatus2 = 0x00;
    m_nLedStatus3 = 0x00;

    QSettings setting("zsyf", "Monitor");
    m_yearCycle = setting.value("yearCycle", 365).toInt();
    m_yearDuration = setting.value("yearDuration", 30).toInt();
    m_monthCycle = setting.value("monthCycle", 30).toInt();
    m_monthDuration = setting.value("monthDuration", 30).toInt();
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(yearMonthCheck()));
    m_timer->start(1000);
    m_timerLed = new QTimer(this);
    connect(m_timerLed, SIGNAL(timeout()), this, SLOT(updateLedStatus()));
    m_checkTime = new CMsgCheckTime();
    m_checkTime->systemStartTime = QDateTime::currentDateTime();
    m_checkTime->systemMainPowerTime = m_checkTime->systemStartTime;
    m_KeyFlagTime = new QTimer(this);
    connect(m_KeyFlagTime, SIGNAL(timeout()), this, SLOT(KeyFlagTimeStop()));
    m_KeyFlag = false;
    m_msgLog = new CMsgLog();
}

CProcessController::~CProcessController()
{
    QSettings setting("zsyf", "Monitor");
    setting.setValue("yearCycle", m_yearCycle);
    setting.setValue("yearDuration", m_yearDuration);
    setting.setValue("monthCycle", m_monthCycle);
    setting.setValue("monthDuration", m_monthDuration);
    delete m_timer;
    delete m_checkTime;
    delete m_timerLed;
    delete m_KeyFlagTime;
    delete m_msgLog;
}

void CProcessController::procSendEvent(const int &infoType, const QHash<QString, QVariant> &control, const QByteArray &data)
{
    Q_UNUSED(infoType);
    Q_UNUSED(control);
    Q_UNUSED(data);
}

void CProcessController::KeyFlagTimeStop()
{
    m_KeyFlagTime->stop();
    m_KeyFlag = false;
}

void CProcessController::procRecvEvent(const int &infoType, const QHash<QString, QVariant> &control, const QByteArray &data)
{
    Q_UNUSED(infoType);
    Q_UNUSED(control);
    qDebug() << "CProcessController::procRecvEvent" << data.toHex() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if(data.count() < 9)
        return;
    int Ledstatus_led = data.at(0);
    int Ledstatus_sound = data.at(4);
    int Ledstatus_power = data.at(5);
    int Ledstatus_key = data.at(7);
    int Ledstatus_out = data.at(8);
    if(Ledstatus_key != 0)
    {
        CMsgNull msgNull;
        if(Ledstatus_key & 0x01)
        {
            QByteArray byte = msgNull.data(NCT_InputEmergency);
            CGlobal::instance()->DealEvent(NCT_InputEmergency, byte);
        }
        if(Ledstatus_key & 0x10)
        {
            if(m_KeyFlag == true)
                return;
            QByteArray byte = msgNull.data(NCT_AutomaticEvacuation);
            CGlobal::instance()->DealEvent(NCT_AutomaticEvacuation, byte);
            m_KeyFlagTime->start(500);
            m_KeyFlag = true;
        }
        if(Ledstatus_key & 0x20)
        {
            if(m_KeyFlag == true)
                return;
            QByteArray byte = msgNull.data(NCT_ManualEvacuation);
            CGlobal::instance()->DealEvent(NCT_ManualEvacuation, byte);
            m_KeyFlagTime->start(500);
            m_KeyFlag = true;
        }
        if(Ledstatus_key & 0x40)
        {
            if(m_KeyFlag == true)
                return;
            QByteArray byte = msgNull.data(NCT_ForceEmergency);
            CGlobal::instance()->DealEvent(NCT_ForceEmergency, byte);
            m_KeyFlagTime->start(500);
            m_KeyFlag = true;
        }
        if(Ledstatus_key & 0x80)
        {
            QByteArray byte = msgNull.data(NCT_Mute);
            CGlobal::instance()->DealEvent(NCT_Mute, byte);
        }
    }
    if(Ledstatus_power & 0x01)
    {
        m_isMainPowerWork = true;
        m_isStandbyPowerWork = false;
    }
    if(Ledstatus_power & 0x02)
    {
        m_isMainPowerWork = false;
        m_isStandbyPowerWork = true;
    }
    if(Ledstatus_power != 0x01)
    {
        if(!(Ledstatus_power & 0x01))
        {
            if(m_isMainPowerFault == false)
            {
                uploadStatus(5, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_MainPowerFault, 1, QDateTime::currentDateTime().toTime_t());
            }
                m_isMainPowerFault = true;
        }
        if((Ledstatus_power & 0x40) && (Ledstatus_power & 0x08))
        {
            if(m_isStandByPowerOff == false)
                uploadStatus(5, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_StandbyPowerOff, 1, QDateTime::currentDateTime().toTime_t());
            m_isStandByPowerOff = true;
        }
        else if(Ledstatus_power & 0x08)
        {
            if(m_isStandByPowerShort == false)
                uploadStatus(5, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_StandbyPowerShort, 1, QDateTime::currentDateTime().toTime_t());
            m_isStandByPowerShort = true;
        }
        if((Ledstatus_power & 0x02) && (Ledstatus_power & 0x04))
        {
            if(m_isStandbyPowerUndervoltage == false)
                uploadStatus(5, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_StandbyPowerUndervoltage, 1, QDateTime::currentDateTime().toTime_t());
            m_isStandbyPowerUndervoltage = true;
        }
    }
    else
    {
        if(m_isMainPowerFault == true)
        {
            uploadStatus(5, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_MainPowerFault, 0, QDateTime::currentDateTime().toTime_t());
            m_isMainPowerFault = false;
        }
        if(m_isStandByPowerOff == true)
        {
            uploadStatus(5, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_StandbyPowerOff, 0, QDateTime::currentDateTime().toTime_t());
            m_isStandByPowerOff = false;
        }
        if(m_isStandByPowerShort == true)
        {
            uploadStatus(5, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_StandbyPowerShort, 0, QDateTime::currentDateTime().toTime_t());
            m_isStandByPowerShort = false;
        }
        if(m_isStandbyPowerUndervoltage == true)
        {
            uploadStatus(5, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_StandbyPowerUndervoltage, 0, QDateTime::currentDateTime().toTime_t());
            m_isStandbyPowerUndervoltage = false;
        }
    }
    if(Ledstatus_led)    {    }
    if(Ledstatus_sound)    {    }
    if(Ledstatus_out)    {    }
}

void CProcessController::initProcess()
{
    m_controllerStr = tr("Panel");
    CGlobal::instance()->processDistribution()->setMidLayerOption();
    Sleep(500);
    CGlobal::instance()->processController()->reset();
    Sleep(500);
    selfCheck();
}

void CProcessController::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void CProcessController::SystemSet(CMsgObjectStatus *msgObjectStatus)
{
    if(msgObjectStatus->ndata.count() < 10)
        return;
    m_PrintOperation = msgObjectStatus->ndata.at(0);
    m_PrintFault = msgObjectStatus->ndata.at(1);
    m_PrintWarning = msgObjectStatus->ndata.at(2);
    m_isForbidFaultSound = msgObjectStatus->ndata.at(3);
    m_isForbidEmergencySound = msgObjectStatus->ndata.at(4);
    m_isForbidMonthCheck = msgObjectStatus->ndata.at(5);
    m_isForbidYearCheck = msgObjectStatus->ndata.at(6);
    m_isSendDirectionAndTwinkleByOne = msgObjectStatus->ndata.at(7);
    m_isUCSendDirectionAndTwinkleByOne = msgObjectStatus->ndata.at(8);
    m_monthDuration = msgObjectStatus->ndata.at(9);
    CGlobal::instance()->processSocket()->setip(msgObjectStatus->nDataServerIP);
    CGlobal::instance()->processSocket()->setport(msgObjectStatus->nDataServerPort);
}

//自动模式
void CProcessController::setAutoEvacuation(const bool enable)
{
    if(m_isAuto == enable) return;
    m_isAuto = enable;
    addLog(CPrint::Other, controllerStr() + tr("Set %1 evacuation mode").arg(enable ? tr("automatic") : tr("manual")));
}
//自检
void CProcessController::selfCheck()
{
    m_isSelfCheck = true;
    QByteArray data = m_msgNull->data(NCT_SelfCheckStart);      /*25 start self-check, data= type(25)+m_msgnull*/
    CGlobal::instance()->DealEvent(NCT_SelfCheckStart, data);   /*call callback func*/
    CGlobal::instance()->processDistribution()->selfCheck();
    if(CGlobal::isDebug()) qDebug() << "selfcheck start" << QDateTime::currentDateTime();
    m_nLedStatus1 = 0xFF;
    m_nLedStatus2 = 0x01;
    m_nLedStatus3 = 0x00;
    CGlobal::instance()->communicationRoute()->setLedStatus(m_nLedStatus1, m_nLedStatus2, m_nLedStatus3);
    QTimer::singleShot(PROCESSCONTROLLER_SELFCHECK_DURATION, this, SLOT(stopSelfCheck()));
    addLog(CPrint::Other, controllerStr() + tr("Self check"));
}
//结束自检
void CProcessController::stopSelfCheck()
{
    m_isSelfCheck = false;
    m_nLedStatus1 = 0x21;
    m_nLedStatus2 = 0x00;
    m_nLedStatus3 = 0x00;
    m_timerLed->start(150);
    CGlobal::instance()->communicationRoute()->setLedStatus(m_nLedStatus1, m_nLedStatus2, m_nLedStatus3);
}
//手动月检
void CProcessController::manualMonthCheck()
{
    addLog(CPrint::Other, controllerStr() + tr("Manual month check"));
    uploadStatus(1, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_MonthEmergency, 1, QDateTime::currentDateTime().toTime_t());
    monthCheck();
}
//手动年检
void CProcessController::manualYearCheck()
{
    addLog(CPrint::Other, controllerStr() + tr("Manual year check"));
    uploadStatus(1, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_YearEmergency, 1, QDateTime::currentDateTime().toTime_t());
    yearCheck();
}
//年检
void CProcessController::autoYearCheck()
{
    if(m_isForbidYearCheck)
        return;
    addLog(CPrint::Other, controllerStr() + tr("Automation year check"));
    uploadStatus(1, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_YearEmergency, 1, QDateTime::currentDateTime().toTime_t());
    yearCheck();
}
//月检
void CProcessController::autoMonthCheck()
{
    if(m_isForbidMonthCheck)
        return;
    addLog(CPrint::Other, controllerStr() + tr("Automation month check"));
    uploadStatus(1, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_MonthEmergency, 1, QDateTime::currentDateTime().toTime_t());
    monthCheck();
}
//消音
void CProcessController::mute(bool enable)
{
    if(m_isMute == enable) return;
    if(m_isFault && m_isStartStatus)
        enable = false;
    m_isMute = enable;
}
//年检开始
void CProcessController::yearCheck()
{
    m_isYearCheck = true;
    m_isEmergency = true;
    sendYearCheck(true);
    m_stopYearCheckTime = ::getTickCount() + m_yearDuration * 1000 * 60;
    QByteArray data = m_msgNull->data(NCT_YearCheckBegin);
    CGlobal::instance()->DealEvent(NCT_YearCheckBegin, data);
//    addLog(CPrint::Other, controllerStr() + tr("Year check start"));
}
//月检开始
void CProcessController::monthCheck()
{
    m_isMonthCheck = true;
    m_isEmergency = true;
    sendMonthCheck(true);
    m_stopMonthCheckTime = ::getTickCount() + m_monthDuration * 1000;
    QByteArray data = m_msgNull->data(NCT_MonthCheckBegin);
    CGlobal::instance()->DealEvent(NCT_MonthCheckBegin, data);
//    addLog(CPrint::Other, controllerStr() + tr("Month check start"));
}
//结束年检
void CProcessController::stopYearCheck()
{
    if(m_isYearCheck == false)
        return;
    m_isYearCheck = false;
    m_yearCheckTick = 0;
    m_isEmergency = false;
    m_bStop = false;
    sendYearCheck(false);
    QByteArray data = m_msgNull->data(NCT_YearCheckEnd);
    CGlobal::instance()->DealEvent(NCT_YearCheckEnd, data);
    addLog(CPrint::Other, controllerStr() + tr("Year check stop"));
    setSpeedUp(false);
}
//结束月检
void CProcessController::stopMonthCheck()
{
    if(m_isMonthCheck == false)
        return;
    m_isMonthCheck = false;
    m_monthCheckTick = 0;
    m_isEmergency = false;
    m_bStop = false;
    sendMonthCheck(false);
    QByteArray data = m_msgNull->data(NCT_MonthCheckEnd);
    CGlobal::instance()->DealEvent(NCT_MonthCheckEnd, data);
    addLog(CPrint::Other, controllerStr() + tr("Month check stop"));
}
//模拟加速
void CProcessController::setSpeedUp(bool enable)
{
    if(m_isSpeedUp == enable) return;
    m_isSpeedUp = enable;
    m_monthCheckTick = 0;
    m_yearCheckTick = 0;
    addLog(CPrint::Other, enable ? tr("Start speed up") : tr("Stop speed up"));
    QByteArray data = m_msgNull->data(enable ? NCT_StartSpeedUp : NCT_StopSpeedUp);
    CGlobal::instance()->DealEvent((enable ? NCT_StartSpeedUp : NCT_StopSpeedUp), data);
}

void CProcessController::setLedStatus(const int nLedID, const int nLedStatus)
{
    if(isSelfCheck()) return;
    if(nLedID == LedEmergency) m_isEmergency = (nLedStatus != 0);
    if(nLedID == LedMainPower) m_isMainPowerWork = (nLedStatus != 0);
    if(nLedID == LedStandbyPower) m_isStandbyPowerWork = (nLedStatus != 0);
    if(nLedID == LedAuto) m_isAuto = (nLedStatus != 0);
    if(nLedID == LedManual) m_isAuto = (nLedStatus != 0);
    if(nLedID == LedFault) m_isFault = (nLedStatus != 0);
    if(nLedID == LedSystemFault) m_isSystemFault = (nLedStatus != 0);
    if(nLedID == LedMute) m_isMute = (nLedStatus != 0);
    updateLedStatus();
}
void CProcessController::setLedupdateTime2s()
{
    m_timerLed->stop();
    m_timerLed->start(2000);
}
void CProcessController::setLedupdateTime150ms()
{
    m_timerLed->stop();
    m_timerLed->start(150);
}
/**
 * LED: 左 zhudian 备电 系统故障 故障指示 应急启动 自动 手动 消音
 *
 * LEDSTATUS1: LED STATE(BIT7-BIT0: EMERGENCY
 * LEDSTATUS2: SOUND(BIT7-BIT6) do(bit5)
 */
void CProcessController::updateLedStatus()
{
    if(isSelfCheck())
        return;
//    unsigned char nstatus1 = m_nLedStatus1, nstatus2 = m_nLedStatus2, nstatus3 = m_nLedStatus3;
    if(m_isMainPowerWork) m_nLedStatus1 |= 0x01; else m_nLedStatus1 &= 0xFE;
    if(m_isStandbyPowerWork) m_nLedStatus1 |= 0x02; else m_nLedStatus1 &= 0xFD;
    if(m_isSystemFault) m_nLedStatus1 |= 0x04; else m_nLedStatus1 &= 0xFB;
    if(m_isFault || m_isMainPowerFault || m_isStandByPowerOff || m_isStandByPowerShort || m_isStandbyPowerUndervoltage)
        m_nLedStatus1 |= 0x08;
    else
        m_nLedStatus1 &= 0xF7;
    if((m_isFault || m_isMainPowerFault || m_isStandByPowerOff || m_isStandByPowerShort || m_isStandbyPowerUndervoltage) && !m_isForbidFaultSound)
        m_nLedStatus2 = 0x02;
    else
        m_nLedStatus2 &= 0xFD;
    if(m_isEmergency) {m_nLedStatus1 |= 0x10; m_nLedStatus3 |= 0x01;}else {m_nLedStatus1 &= 0xEF; m_nLedStatus3 &= 0xFE;}
    if(m_isEmergency && !m_isForbidEmergencySound) m_nLedStatus2 = 0x01;else m_nLedStatus2 &= 0xFE;
    if(m_isAuto) m_nLedStatus1 |= 0x20; else m_nLedStatus1 &= 0xDF;
    if(!m_isAuto) m_nLedStatus1 |= 0x40; else m_nLedStatus1 &= 0xBF;
    if(m_isMute) {m_nLedStatus1 |= 0x80; m_nLedStatus2 = 0;} else m_nLedStatus1 &= 0x7F;

//    if(m_nLedStatus1 != nstatus1 || m_nLedStatus2 != nstatus2 || m_nLedStatus3 != nstatus3)
    CGlobal::instance()->communicationRoute()->setLedStatus(m_nLedStatus1, m_nLedStatus2, m_nLedStatus3);
}

void CProcessController::emergency(const short emergencyType, const bool enable)
{
    qDebug() << emergencyType << enable << "CProcessController::emergency=============================";

    uploadStatus(1, CGlobal::instance()->panelAddress(), 0, 0, 0, emergencyType, enable, QDateTime::currentDateTime().toTime_t());
    if(emergencyType == OBJS_Emergency){
        m_isEmergency = enable;
    } else if(emergencyType == OBJS_RemoteEmergency){
        m_isRemoteEmergency = enable;
    } else if(emergencyType == OBJS_FireEmergency){
        m_isFireEmergency = enable;
    } else if(emergencyType == OBJS_InputEmergency){
        m_isInputEmergency = enable;
    } else if(emergencyType == OBJS_ForceEmergency){
        m_isForceEmergency = enable;
    }

    bool bEmergencyStatus = isEmergencyStatus();
    sendEmergency(bEmergencyStatus, emergencyType);
}

void CProcessController::sendEmergency(bool enable, short emergencyType)
{
    Q_UNUSED(emergencyType);
    setLedStatus(LedEmergency, (enable?1:0));
    CProcessDistribution* processDistribution = CGlobal::instance()->processDistribution();
    if(processDistribution)
    {
        processDistribution->emergency(enable);
        QTimer::singleShot(200, processDistribution, SLOT(emergency(enable)));
        QTimer::singleShot(400, processDistribution, SLOT(emergency(enable)));
    }
}

void CProcessController::sendMonthCheck(bool enable)
{
    setLedStatus(LedEmergency, (enable?1:0));
    CGlobal::instance()->processDistribution()->monthCheck(enable);
}

void CProcessController::sendYearCheck(bool enable)
{
    setLedStatus(LedEmergency, (enable?1:0));
    CGlobal::instance()->processDistribution()->yearCheck(enable);
}

void CProcessController::yearMonthCheck()
{
    static int count = 0;

    if(m_isMonthCheck)
    {
        quint64 tick = ::getTickCount();
        if(m_stopMonthCheckTime <= tick)
        {
            stopMonthCheck();
            uploadStatus(1, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_MonthEmergency, 0, QDateTime::currentDateTime().toTime_t());
        }
        return;
    }
    if(m_isYearCheck)
    {
        quint64 tick = ::getTickCount();
        if(m_stopYearCheckTime <= tick)
        {
            stopYearCheck();
            uploadStatus(1, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_YearEmergency, 0, QDateTime::currentDateTime().toTime_t());
        }
        return;
    }
    m_checkTime->systemCurrentTime = QDateTime::currentDateTime();


    if(!m_isSpeedUp)//正常月检年检
    {
        // 进入计时状态后的计时天数
        int days = m_checkTime->systemMainPowerTime.daysTo(m_checkTime->systemCurrentTime);
        if((days < m_monthCycle) && !m_bStop)// 小于月检周期
        {// 更新界面显示下次月检、年检提示信息
            m_checkTime->yearCheckTime = m_checkTime->systemMainPowerTime.addDays(m_yearCycle);
            m_checkTime->monthCheckTime = m_checkTime->systemMainPowerTime.addDays(m_monthCycle);
        }
        else
        {
            if(days % m_yearCycle == 0)// 满足年检条件
            {
                QDateTime tmpYearTime = m_checkTime->systemMainPowerTime.addDays(m_yearCycle * ((int)(days / m_yearCycle)));
                if(m_checkTime->yearCheckTime <= m_checkTime->systemCurrentTime)
                {
                    if(m_checkTime->lastYearCheckTime.date() < m_checkTime->systemCurrentTime.date())
                    {
                        if(!m_isMonthCheck)// 当前不存在月检，则进行年检
                        {
                            m_checkTime->lastYearCheckTime = m_checkTime->systemCurrentTime;
                            m_checkTime->yearCheckTime = m_checkTime->systemMainPowerTime.addDays(m_yearCycle * ((int)(days / m_yearCycle) + 1));
                            autoYearCheck();
                        }
                    }
                }
                else
                {
                    if(!m_isYearCheck && (tmpYearTime >= m_checkTime->systemCurrentTime)) m_checkTime->yearCheckTime = tmpYearTime;
                }
            }
            else
            {
                if(m_checkTime->yearCheckTime <= m_checkTime->systemCurrentTime){//更新下次年检时间
                    m_checkTime->yearCheckTime = m_checkTime->systemMainPowerTime.addDays(m_yearCycle * ((int)(days / m_yearCycle) + 1));
                }
            }

            if(days % m_monthCycle == 0)//满足月检条件
            {
                QDateTime tmpMonthTime = m_checkTime->systemMainPowerTime.addDays(m_monthCycle * (int)(days / m_monthCycle));
                if(m_checkTime->monthCheckTime <= m_checkTime->systemCurrentTime)
                {
                    if(m_checkTime->lastMonthCheckTime < m_checkTime->systemCurrentTime)
                    {
                        if(!m_isYearCheck)//当前不存在年检，则进行月检
                        {
                            m_checkTime->lastMonthCheckTime = m_checkTime->systemCurrentTime;
                            m_checkTime->monthCheckTime = m_checkTime->systemMainPowerTime.addDays(m_monthCycle * ((int)(days / m_monthCycle) + 1));
                            autoMonthCheck();
                        }
                    }
                }
                else
                {
                    if(!m_isMonthCheck && (tmpMonthTime >= m_checkTime->systemCurrentTime)) {
                        m_checkTime->monthCheckTime = tmpMonthTime;
                    }
                }
            }
            else
            {
                if(m_checkTime->monthCheckTime <= m_checkTime->systemCurrentTime){//更新下次月检时间
                    m_checkTime->monthCheckTime = m_checkTime->systemMainPowerTime.addDays(m_monthCycle * ((int)(days / m_monthCycle) + 1));
                }
            }
        }
        m_checkTime->speedMonthCheckTime = m_checkTime->monthCheckTime;
        m_checkTime->speedYearCheckTime = m_checkTime->yearCheckTime;
        m_CheckTick = 0;
    }
    else//模拟加速
    {
        if(!m_isMonthCheck && !m_isYearCheck)
        {
            m_CheckTick++;
            m_checkTime->systemCurrentTime = QDateTime::currentDateTime().addDays(m_CheckTick);
        }
        int days = m_checkTime->systemMainPowerTime.daysTo(m_checkTime->systemCurrentTime);
        if(days % m_yearCycle == 0)// 满足年检条件
        {
            QDateTime tmpYearTime = m_checkTime->systemMainPowerTime.addDays(m_yearCycle * ((int)(days / m_yearCycle)));
            if(m_checkTime->speedYearCheckTime <= m_checkTime->systemCurrentTime)
            {
                if(m_checkTime->speedLastYearCheckTime.date() < m_checkTime->systemCurrentTime.date())
                {
                    if(!m_isMonthCheck)// 当前不存在月检，则进行年检
                    {
                        m_checkTime->speedLastYearCheckTime = m_checkTime->systemCurrentTime;
                        m_checkTime->speedYearCheckTime = m_checkTime->systemMainPowerTime.addDays(m_yearCycle * ((int)(days / m_yearCycle) + 1));
                        autoYearCheck();
                    }
                }
            }
            else
            {
                if(!m_isYearCheck && (tmpYearTime >= m_checkTime->systemCurrentTime)) m_checkTime->speedYearCheckTime = tmpYearTime;
            }
        }
        else
        {
            if(m_checkTime->speedYearCheckTime <= m_checkTime->systemCurrentTime){//更新下次年检时间
                m_checkTime->speedYearCheckTime = m_checkTime->systemMainPowerTime.addDays(m_yearCycle * ((int)(days / m_yearCycle) + 1));
            }
        }

        if(days % m_monthCycle == 0)//满足月检条件
        {
            QDateTime tmpMonthTime = m_checkTime->systemMainPowerTime.addDays(m_monthCycle * (int)(days / m_monthCycle));
            if(m_checkTime->speedMonthCheckTime <= m_checkTime->systemCurrentTime)
            {
                if(m_checkTime->speedLastMonthCheckTime < m_checkTime->systemCurrentTime)
                {
                    if(!m_isYearCheck)//当前不存在年检，则进行月检
                    {
                        m_checkTime->speedLastMonthCheckTime = m_checkTime->systemCurrentTime;
                        m_checkTime->speedMonthCheckTime = m_checkTime->systemMainPowerTime.addDays(m_monthCycle * ((int)(days / m_monthCycle) + 1));
                        autoMonthCheck();
                    }
                }
            }
            else
            {
                if(!m_isMonthCheck && (tmpMonthTime >= m_checkTime->systemCurrentTime)) {
                    m_checkTime->speedMonthCheckTime = tmpMonthTime;
                }
            }
        }

    }

    if(++count % 1 == 0)
    {
        if(m_isMonthCheck || m_isYearCheck)
            return;
        QByteArray data = m_checkTime->data(NCT_CheckTimeSync);
        CGlobal::instance()->DealEvent(NCT_CheckTimeSync, data);
    }
}


//信息记录页码
void CProcessController::RecordPage(CMsgObjectStatus *msgObjectStatus)
{
    if(msgObjectStatus->ndata.count() < 2)
        return;
    bool customMode = msgObjectStatus->ndata.at(0);
    int page = msgObjectStatus->ndata.at(1);
    //查询模式：1为自定义查询，有故障或应急时显示5条记录，否则显示x条;0为普通查询，有故障或应急时显示6条记录，否则显示x条
    if(customMode)
    {
        CGlobal::instance()->setRecordNumber(11);
    }
    else
    {
         CGlobal::instance()->setRecordNumber(12);
    }

    CGlobal::instance()->setRecordPage(page);

}

void CProcessController::reset()
{
    m_isReset = true;
    m_isMonthCheck = false;
    m_isYearCheck = false;
    m_isMute = false;
    m_isInputPort = false;
    m_isEmergency = false;
    m_isForceEmergency = false;
    m_isRemoteEmergency = false;
    m_isFireEmergency = false;
    m_isInputEmergency = false;
    m_isOutPutPort = false;
    m_isMainPowerWork = true;
    m_isStandbyPowerWork = false;

    m_isFault = false;
    m_isMainPowerFault = false;
    m_isStandByPowerOff = false;
    m_isStandByPowerShort = false;
    m_isStandbyPowerUndervoltage = false;
    m_isPowerOutputFault = false;
    m_isBatteryLowVoltageFault = false;
    m_isChargerFault = false;
    m_isStartStatus = false;
    m_isSystemFault = false;
//    m_isSpeedUp = false;
    m_monthCheckTick = 0;
    m_yearCheckTick = 0;
    m_bStop = false;

    CGlobal::instance()->processClient()->clientReset();
    CProcessDistribution* processDistribution = CGlobal::instance()->processDistribution();
    if(processDistribution)
        processDistribution->reset();
    CGlobal::instance()->communicationRoute()->reset();
    setAutoEvacuation(m_isAuto);
    setPowerWork(true, false);
    if(!processDistribution)
        m_isSystemFault = true;
    addLog(CPrint::Other, controllerStr() + tr("System reset"));
    m_isReset = false;
}

/**
 * @brief CProcessController::reLogin
 * call power login
 * call distribution login
 * reset
*/
void CProcessController::reLogin()
{
    m_isReset = true;
    m_isMonthCheck = false;
    m_isYearCheck = false;
    m_isEmergency = false;
    m_isMute = false;
    m_isInputPort = false;
    m_isEmergency = false;
    m_isForceEmergency = false;
    m_isRemoteEmergency = false;
    m_isFireEmergency = false;
    m_isInputEmergency = false;
    m_isOutPutPort = false;
    m_isMainPowerWork = true;
    m_isStandbyPowerWork = false;

    m_isFault = false;
    m_isMainPowerFault = false;
    m_isStandByPowerOff = false;
    m_isStandByPowerShort = false;
    m_isStandbyPowerUndervoltage = false;
    m_isPowerOutputFault = false;
    m_isBatteryLowVoltageFault = false;
    m_isChargerFault = false;
    m_isStartStatus = false;
    m_isSystemFault = false;
    m_bStop = false;
    CGlobal::instance()->processDistribution()->readDeviceType();
    CGlobal::instance()->communicationRoute()->reset();
    CGlobal::instance()->processClient()->clientReset();
    setAutoEvacuation(m_isAuto);
    setPowerWork(true, false);
    addLog(CPrint::Other, controllerStr() + tr("System relogin"));
    m_isReset = false;
}


void CProcessController::addLog(int type, const QString &title)
{
    m_msgLog->User = tr("System");
    m_msgLog->Time = QDateTime::currentDateTime();
    m_msgLog->Remark = title;
    if(type == CPrint::Start)
        m_msgLog->Type = tr("Start");
    else if(type == CPrint::Fault)
        m_msgLog->Type = tr("Fault");
    else if(type == CPrint::Error)
        m_msgLog->Type = tr("Error");
    else
        m_msgLog->Type = tr("Other");
    CGlobal::instance()->log()->addLog(m_msgLog);
    print(type, title);
}

void CProcessController::addLog(CMsgLog* msgLog)
{
    int type = 0;
    CGlobal::instance()->log()->addLog(msgLog);
    if(msgLog->Type == tr("Start"))
        type = CPrint::Start;
    else if(msgLog->Type == tr("Fault"))
        type = CPrint::Fault;
    else if(msgLog->Type == tr("Error"))
        type = CPrint::Error;
    else
        type = CPrint::Other;
    print(type, msgLog->title);
}

void CProcessController::print(int type, const QString &title)
{
    if(type == CPrint::Other && !m_PrintOperation)
        return;
    if(type == CPrint::Fault && !m_PrintFault)
        return;
    if(type == CPrint::Start && !m_PrintWarning)
        return;
    CGlobal::instance()->print()->print((CPrint::PrintType)type, title);
}



void CProcessController::setPowerWork(bool isMainPower, bool isStandbyPower)
{
    m_isMainPowerWork = isMainPower;
    m_isStandbyPowerWork = isStandbyPower;
}



bool CProcessController::hasStandbyPowerFault() const
{
    if(m_isStandByPowerOff || m_isStandByPowerShort || m_isStandbyPowerUndervoltage || m_isChargerFault || m_isBatteryLineFault || m_isBatteryLowVoltageFault)
        return true;
    else
        return false;
}
void CProcessController::setJZPowerStatus(bool bCharge, bool bPower, bool bBattery)
{
    CProcessDistribution* processDistribution = CGlobal::instance()->processDistribution();
    if(processDistribution){
        processDistribution->setJZPowerStatus(bCharge, bPower, bBattery);
    }
    addLog(CPrint::Other, controllerStr() + tr("Set Power Status"));
}

void CProcessController::setCheck(int yearCycle, int yearDuration, int monthCycle, int monthDuration)
{
    m_yearCycle = yearCycle;
    m_yearDuration = yearDuration;
    m_monthCycle = monthCycle;
    m_monthDuration = monthDuration;
//    addLog(CPrint::Other, controllerStr() + tr("Set check"));
}

void CProcessController::manualFire(const QString &address)
{
    addLog(CPrint::Other, controllerStr() + tr("Manual fire %1").arg(address));
    CGlobal::instance()->processController()->emergency(OBJS_FireEmergency, true);
}



void CProcessController::login()
{
    m_isReset = true;
    m_isMonthCheck = false;
    m_isYearCheck = false;
    m_isEmergency = false;
    m_isSelfCheck = false;
    m_isMute = false;
    m_isInputPort = false;
    m_isEmergency = false;
    m_isForceEmergency = false;
    m_isRemoteEmergency = false;
    m_isFireEmergency = false;
    m_isInputEmergency = false;
    m_isOutPutPort = false;
    m_isMainPowerWork = true;
    m_isStandbyPowerWork = false;

    m_isFault = false;
    m_isMainPowerFault = false;
    m_isStandByPowerOff = false;
    m_isStandByPowerShort = false;
    m_isStandbyPowerUndervoltage = false;
    m_isPowerOutputFault = false;
    m_isBatteryLowVoltageFault = false;
    m_isChargerFault = false;
    m_isStartStatus = false;
    m_isSystemFault = false;
    CProcessDistribution* processDistribution = CGlobal::instance()->processDistribution();

    CGlobal::instance()->communicationRoute()->reset();
    CGlobal::instance()->processClient()->clientReset();
    setAutoEvacuation(true);
    setPowerWork(true, false);
    if(!processDistribution)
        m_isSystemFault = true;
//    updateLedStatus();
    m_isReset = false;
}
