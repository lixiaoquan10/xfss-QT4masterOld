#ifndef CDISTRIBUTION_H
#define CDISTRIBUTION_H

#include "cobject.h"

#define DISTRIBUTION_VALUE_COMMUNICATION "distribution_communication"
#define DISTRIBUTION_VALUE_ID "distribution_id"
#define DISTRIBUTION_VALUE_NAME "distribution_name"
#define DISTRIBUTION_VALUE_NAMEDETAIL "distribution_namedetail"
#define DISTRIBUTION_VALUE_DESCRIPTION "distribution_description"
#define DISTRIBUTION_PORT_ID "distribution_port"
#define DISTRIBUTION_VALUE_ADDR "distribution_addr"
#define DISTRIBUTION_VALUE_TYPENAME "distribution_type"
#define DISTRIBUTION_VALUE_AREA "distribution_area"
#define DISTRIBUTION_VALUE_LOCATION "distribution_location"
#define DISTRIBUTION_VALUE_KEYID "distribution_keyid"
#define DISTRIBUTION_VALUE_TYPEID "distribution_typeid"
#define DISTRIBUTION_VALUE_CANPORTADDR "distribution_canportaddress"

#define DISTRIBUTION_VALUE_NOLOGIN "distribution_nologin"

#define DISTRIBUTION_VALUE_DATABASE "distribution_database"

#define DISTRIBUTION_VALUE_MAINV "distribution_mainv"
#define DISTRIBUTION_VALUE_OUTV  "distribution_outv"
#define DISTRIBUTION_VALUE_OUTA  "distribution_outa"
#define DISTRIBUTION_VALUE_EXTERNALV  "distribution_externalv"
#define DISTRIBUTION_VALUE_BATV  "distribution_batv"
#define DISTRIBUTION_VALUE_BAT1V  "distribution_bat1v"
#define DISTRIBUTION_VALUE_BAT2V  "distribution_bat2v"
#define DISTRIBUTION_VALUE_BAT3V  "distribution_bat3v"
#define DISTRIBUTION_VALUE_BAT1T  "distribution_bat1t"
#define DISTRIBUTION_VALUE_BAT2T  "distribution_bat2t"
#define DISTRIBUTION_VALUE_BAT3T  "distribution_bat3t"

#define DISTRIBUTION_VALUE_CPUVERSION  "distribution_cpuversion"
#define DISTRIBUTION_VALUE_POWVERSION  "distribution_powversion"
#define DISTRIBUTION_VALUE_LCDVERSION  "distribution_lcdversion"
#define DISTRIBUTION_VALUE_ABUS1VERSION  "distribution_abus1version"
#define DISTRIBUTION_VALUE_ABUS2VERSION  "distribution_abus2version"
#define DISTRIBUTION_VALUE_ABUS3VERSION  "distribution_abus3version"
#define DISTRIBUTION_VALUE_ABUS4VERSION  "distribution_abus4version"
#define DISTRIBUTION_VALUE_ABUS5VERSION  "distribution_abus5version"
#define DISTRIBUTION_VALUE_ABUS6VERSION  "distribution_abus6version"
#define DISTRIBUTION_VALUE_ABUS7VERSION  "distribution_abus7version"
#define DISTRIBUTION_VALUE_ABUS8VERSION  "distribution_abus8version"
#define DISTRIBUTION_VALUE_ABUS9VERSION  "distribution_abus9version"
#define DISTRIBUTION_VALUE_ABUS10VERSION  "distribution_abus10version"
#define DISTRIBUTION_VALUE_ABUS11VERSION  "distribution_abus11version"
#define DISTRIBUTION_VALUE_ABUS12VERSION  "distribution_abus12version"

class CCanport;
class CLoop;
class CDistribution : public CObject
{
    Q_OBJECT
public:
    explicit CDistribution(const int keyId = 0);
    CDistribution(CCanport *canport, const int keyId = 0);
    ~CDistribution();

    static void createTable(CDBSqlite* db);
    static void clearTable(CDBSqlite* db);
    static int loadDistribution(CDBSqlite *db, CCanport *canport);
    int save(CDBSqlite *db) { return saveDistribution(db); }
    CObject* parent();
    CObject* child(const int index) { return m_childs.value(index, NULL); }
    QList<CObject*> childs() { return m_childs; }
    QString codeStr() const;
    QString typeStr() const { return distributionValue(DISTRIBUTION_VALUE_NAME).toString(); }
    QString descriptionStr() const { return distributionValue(DISTRIBUTION_VALUE_DESCRIPTION).toString(); }

    bool setDistributionId(int id) { return setDistributionValue(DISTRIBUTION_VALUE_ID, id); }
    int distributionKeyId() const { return distributionValue(DISTRIBUTION_VALUE_KEYID).toInt(); }
    int distributionId() const { return distributionValue(DISTRIBUTION_VALUE_ID).toInt(); }
    int distributionAddress() const { return distributionValue(DISTRIBUTION_VALUE_ADDR).toInt(); }
    int canportAddress() const { return distributionValue(DISTRIBUTION_VALUE_CANPORTADDR).toInt(); }
    int distributionTypeId() const { return distributionValue(DISTRIBUTION_VALUE_TYPEID).toInt(); }
    QString distributionProductId() const { return distributionValue(DISTRIBUTION_VALUE_DESCRIPTION).toString(); }

    void setCanport(CCanport* canport) { m_canport = canport; }
    CCanport* canport() const { return m_canport; }

    bool addLoop(CLoop* loop);
    void removeLoop(CLoop* loop);
    bool hasLoop(const int id) const;
    int loopCount() const { return m_loops.size(); }
    CLoop* loop(const int index) const { return m_loops.value(index, NULL); }
    CLoop* loopById(const int id) const;
    CLoop* loopByAdd(const int add) const;
    bool loopByKeyId(const int keyid) const;
    QList<CLoop*> loops() const { return m_loops; }
    int maxLoopId() const { return m_maxLoopId; }
    void calcMaxLoopId(const int n) { m_maxLoopId = m_maxLoopId > n ? m_maxLoopId : n; }

    QHash<int, int> getDeviceTypeNumber() const { return m_deviceTypeNumber; }
    void setDeviceTypeNumber(int type, int n);
    void adjustedDeviceTypeNumber(int type, int adj);
    QHash<int, int> getDeviceOnlineNumber() const { return m_deviceOnlineNumber; }
    void setDeviceOnlineNumber(int type, int n);
    void adjustedDeviceOnlineNumber(int type, int adj);
    int getDeviceOnlineTotal() const { return m_deviceOnlineTotal; }
    void setDeviceOnlineTotal(int n);
    void adjustedDeviceOnlineTotal(int adj);
    void resetDeviceOnlineTotal();

    int getoutA() const { return m_outA; }
    bool getlightswitch() const { return m_lightswitch; }
    bool getCANfaultswitch() const { return m_CANfaultswitch; }
    int getlinkageswitch() const { return m_linkageswitch; }
    int getpowermode() const { return m_powermode; }
    int getfiretime() const { return m_firetime; }
    int getunfiretime() const { return m_unfiretime; }
    int getloopCommunication() const { return m_loopCommunication; }
    int getloopOpen() const { return m_loopOpen; }

    bool getmainPowerFault()  const { return m_mainPowerFault; }
    bool getbackupPowerFault()  const { return m_backupPowerFault; }
    bool getoverOut()  const { return m_overOut; }
    bool getoutOpen()  const { return m_outOpen; }
    bool getchargeStatus()  const { return m_chargeStatus; }
    bool getemergencyStatus()  const { return m_emergencyStatus; }
    bool get36vOut()  const { return m_36vOut; }
    bool getsystemFault()  const { return m_systemFault; }
    bool getrunMode()  const { return m_runMode; }
    bool getoverDischargeFault()  const { return m_overDischargeFault; }
    bool getbatUndervoltage()  const { return m_batUndervoltage; }
    bool getbatOverheating()  const { return m_batOverheating; }
    bool getloopOutStatus()  const { return m_loopOutStatus; }
    bool getbat1Undervoltage()  const { return m_bat1Undervoltage; }
    bool getbat2Undervoltage()  const { return m_bat2Undervoltage; }
    bool getbat3Undervoltage()  const { return m_bat3Undervoltage; }
    bool getchargeFault()  const { return m_chargeFault; }
    QString batteryStatus();

    void setoutA(const int outA) { m_outA = outA; }
    void setlightswitch(const bool lightswitch) { m_lightswitch = lightswitch; }
    void setCANfaultswitch(const bool CANfaultswitch) { m_CANfaultswitch = CANfaultswitch; }
    void setlinkageswitch(const int linkageswitch) { m_linkageswitch = linkageswitch; }
    void setpowermode(const int powermode) { m_powermode = powermode; }
    void setfiretime(const int firetime) { m_firetime = firetime; }
    void setunfiretime(const int unfiretime) { m_unfiretime = unfiretime; }
    void setloopCommunication(const int loopCommunication) { m_loopCommunication = loopCommunication; }
    void setloopOpen(const int loopOpen) { m_loopOpen = loopOpen; }
    void setloopOutStatus(const int loopOutStatus) { m_loopOutStatus = loopOutStatus; }

    void setmainPowerFault(const int mainPowerFault) { m_mainPowerFault = mainPowerFault; }
    void setbackupPowerFault(const int backupPowerFault) { m_backupPowerFault = backupPowerFault; }
    void setoverOut(const int overOut) { m_overOut = overOut; }
    void setoutOpen(const int outOpen) { m_outOpen = outOpen; }
    void setchargeStatus(const int chargeStatus) { m_chargeStatus = chargeStatus; }
    void setemergencyStatus(const int emergencyStatus) { m_emergencyStatus = emergencyStatus; }
    void set36vOut(const int m36vOut) { m_36vOut = m36vOut; }
    void setsystemFault(const int systemFault) { m_systemFault = systemFault; }
    void setrunMode(const int runMode) { m_runMode = runMode; }
    void setoverDischargeFault(const int overDischargeFault) { m_overDischargeFault = overDischargeFault; }
    void setbatUndervoltage(const int batUndervoltage) { m_batUndervoltage = batUndervoltage; }
    void setbatOverheating(const int batOverheating) { m_batOverheating = batOverheating; }
    void setbat1Undervoltage(const int bat1Undervoltage) { m_bat1Undervoltage = bat1Undervoltage; }
    void setbat2Undervoltage(const int bat2Undervoltage) { m_bat2Undervoltage = bat2Undervoltage; }
    void setbat3Undervoltage(const int bat3Undervoltage) { m_bat3Undervoltage = bat3Undervoltage; }
    void setchargeFault(const int chargeFault) { m_chargeFault = chargeFault; }

    bool setDistributionValue(QString name, QVariant value);
    QVariant distributionValue(const QString name) const;

    bool isDistributionLogin() const { return m_islogin; }
    void setDistributionLogin(bool islogin) { m_islogin = islogin ;}

    bool isDistributionOnline() const { return m_isOnline; }
    void setDistributionOnline(bool isOnline) { m_isOnline = isOnline ;}


protected:
    void afterSetValue(QString& name, QVariant& oldValue, QVariant &newValue);
    QVariant afterGetValue(QString name, QVariant value) const;
    void afterSetStatus(short status, unsigned char value);
private:
    void initDistribution();
    int saveDistribution(CDBSqlite *db);
    int saveLoop(CDBSqlite* db);
    void addDeviceTypeAndOnline(CLoop* loop);
    void removeDeviceTypeAndOnline(CLoop* loop);
public:
    const static QString type_s;
private:
    CCanport* m_canport;
    QList<CLoop*> m_loops;
    QList<CObject*> m_childs;
    int m_maxLoopId;
    QHash<int, int> m_deviceTypeNumber;
    QHash<int, int> m_deviceOnlineNumber;
    QHash<QString, QVariant> m_distributionValue;
    int m_deviceOnlineTotal;
    int m_deviceOnlineTotalBak;
    int m_outA;
    bool m_lightswitch;
    bool m_CANfaultswitch;
    int m_linkageswitch;
    int m_powermode;
    int m_firetime;
    int m_unfiretime;
    int m_loopCommunication;
    int m_loopOpen;

    bool m_mainPowerFault;
    bool m_backupPowerFault;
    bool m_overOut;
    bool m_outOpen;
    bool m_chargeStatus;
    bool m_emergencyStatus;
    bool m_36vOut;
    bool m_systemFault;
    bool m_runMode;
    bool m_overDischargeFault;
    bool m_batUndervoltage;
    bool m_batOverheating;

    bool m_loopOutStatus;

    bool m_bat1Undervoltage;
    bool m_bat2Undervoltage;
    bool m_bat3Undervoltage;
    bool m_chargeFault;
    //注册状态
    bool m_islogin;
    //在线状态
    bool m_isOnline;

};

#endif // CDISTRIBUTION_H
