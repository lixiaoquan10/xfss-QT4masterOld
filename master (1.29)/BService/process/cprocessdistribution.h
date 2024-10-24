#ifndef CPROCESSDISTRIBUTION_H
#define CPROCESSDISTRIBUTION_H

#include "cprocessbase.h"

#include <QDateTime>

#define PROCESS_DISTRIBUTION_HEARTBEAT_TIME 5000

class QTimer;
class CDevice;
class CLoop;
class CDistribution;
class CController;
class CProcessDistribution : public CProcessBase
{
    Q_OBJECT
public:
    enum Direction{NoneDirection = 0, NoneFlashDirection = 8, FlashDirection = 16, LeftDirection = 1, RightDirection = 2,PeopleDirection = 4};
    Q_DECLARE_FLAGS(Directions, Direction)
    Q_FLAGS(Directions)

    CProcessDistribution();
    ~CProcessDistribution();
    bool addCardPort(const int port);
    void setIsShardCommunication(bool enable);
    bool isShardCommunication() const { return m_isShardCommunication; }
    void procSendEvent(const int &infoType, const QHash<QString, QVariant> &control, const QByteArray &data);
    void procRecvEvent(const int &infoType, const QHash<QString, QVariant> &control, const QByteArray &data);

    void reset(int distributionAddress = 0);
    void unLogin(int distributionAddress = 0, int loopAddress = 0);
    void login(int distributionAddress = 0, int loopAddress = 0);
    void powerInfo(int distributionAddress, int loopAddress, int deviceAddress);
    void energysave(const bool enable, int distributionAddress, int loopAddress, int deviceAddress);
    void loopLogin(int distributionAddress = 0);
    void selfCheck(int distributionAddress = 0);
    void monthCheck(bool enable, int distributionAddress = 0);
    void yearCheck(bool enable, int distributionAddress = 0);
    void lampReset(int distributionAddress, int loopAddress, int deviceAddress);
    void lampchange(const bool enable, int distributionAddress, int loopAddress, int deviceAddress);
    void lampdebugmode(const bool enable, int lampPWMmode, int distributionAddress, int loopAddress, int deviceAddress);
    void lampdefaultdirection(int direction, int distributionAddress, int loopAddress, int deviceAddress);
    void lampdefaultlighten(const bool enable, int distributionAddress, int loopAddress, int deviceAddress);

    void inquiryStaInfor(int distributionAddress);
    void readDeviceInfor(int distributionAddress, int loopAddress, int deviceAddress);
    void readProtectInfor(int distributionAddress, int loopAddress, int deviceAddress);
    void writeProtectInfor(int distributionAddress, int loopAddress, int deviceAddress, QByteArray data);
    void readDistributionLoopOut(int distributionAddress, int loopAddress);
    void conttrolDistributionLoopOut(const bool enable,int distributionAddress, int loopAddress);
    void directionTest(Directions direction);
//    void directionControl(Directions direction, int distributionAddress, int loopAddress, int deviceAddress);
    void directionControl(int distributionAddress, int loopAddress, int deviceAddress, int value);
    void ChangeControl(int value, int distributionAddress, int loopAddress, int deviceAddress);
    void CloseBatteryOut(int value, int distributionAddress, int loopAddress, int deviceAddress);
    void SetLoopOutStatus(int value, int distributionAddress, int loopAddress);
    void ReadLoopOutStatus(int distributionAddress, int loopAddress);
    void lightControl(const bool enable, int distributionAddress, int loopAddress, int deviceAddress);
    void lightTest(int distributionAddress, int loopAddress, int deviceAddress);
    void DeviceAdd(int distributionAddress, int loopAddress, int deviceAddress, int value, QString strText);
    void DeviceType(int distributionAddress, int loopAddress, int deviceAddress, int value);
    void lightReset(int distributionAddress, int loopAddress, int deviceAddress);
    void flashConfig(const bool bfalsh, int distributionAddress, int loopAddress, int deviceAddress);
    void flashControl(const bool bfalsh, int distributionAddress, int loopAddress, int deviceAddress);
    void flowConfig(Directions direction, int delay, int distributionAddress, int loopAddress, int deviceAddress);
    void flowSync();
    void clearFlow(int distributionAddress, int loopAddress, int deviceAddress);
    void readDuplicateDevice(const int loopAddress, int distributionAddress = 0);
    void writeDeviceAddress(const int loopAddress, int oldaddress, int newaddress, int ID[5], int distributionAddress = 0);
    void readDeviceType();
    void setJZPowerStatus(bool bCharge, bool bPower, bool bBattery);
    void heartBeat();
    void checkOutTime();
    void sendAckPacket(int sendaddr, int loopaddr, int deviceaddr);

    void setMidLayerOption();
    bool loginstatus() const { return m_bLogin; }
private:
    void setDistributionTime(int distribution, int time) { m_distributionTime[distribution] = time; }
    void addedDistributionTime(int distribution) { m_distributionTime[distribution]++; }
    int  getDistributionTime(int distribution) const { return m_distributionTime.value(distribution, 0); }
    void clearDistributionTime() { m_distributionTime.clear(); }
private slots:
    void timeOut();
    void regeditOK();
public slots:
    void emergency(const bool enable, int distributionAddress = 0, int loopAddress = 0, int deviceAddress = 0);
private:
    int  m_time;
    bool m_bLogin;
    bool m_nLoopID;
    bool m_isShardCommunication;
    bool m_bCommFault;
    QList<int> m_listLoop;
    QTimer* m_timer;
    QTimer* m_timeLogin;
    QHash<int, int> m_distributionTime;
    QDateTime m_lastSendTime;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CProcessDistribution::Directions)

#endif // CPROCESSDISTRIBUTION_H
