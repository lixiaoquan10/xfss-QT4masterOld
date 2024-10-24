#ifndef CDATAPROCESSING_H
#define CDATAPROCESSING_H

#include <QObject>
#include <QLabel>
#include <QTabWidget>
#include <QThread>

#include "business/cclientbusiness.h"

class CInformationModel;

class CDataProcessing : public QObject
{
    Q_OBJECT
public:
    explicit CDataProcessing(QObject *parent = 0);
    ~CDataProcessing();
    enum WorkType { MainPower = 1, StandbyPower = 2 };
    bool isStatusTab() { return m_isStatusTab; }

    void resetStatus(const QString str = "");
    void setSoundIcon(bool flag);

    QDateTime m_lastTime;
private:
    void initResult(QList<QVariant> data);
    void soundChangde(QList<QVariant> data);
    void evacuationMode(QList<QVariant> data);
    void mainPowerWork(QList<QVariant> data);
    void mainPowerFault(QList<QVariant> data);
    void standbyPowerWork(QList<QVariant> data);
    void standbyPowerFault(QList<QVariant> data);
    void resetSystem(QList<QVariant> data);
    void selfCheckStart(QList<QVariant> data);
    void selfCheckStop(QList<QVariant> data);
    void stopCommunication(QList<QVariant> data);
    void startCommunication(QList<QVariant> data);
    void monthCheckBegin(QList<QVariant> data);
    void monthCheckEnd(QList<QVariant> data);
    void yearCheckBegin(QList<QVariant> data);
    void yearCheckEnd(QList<QVariant> data);
    void checkTimeUpdate(QList<QVariant> data);
    void exitProgram(QList<QVariant> data);
    void startSpeedUp(QList<QVariant> data);
    void stopSpeedUp(QList<QVariant> data);

    void initMember();
    void selfCheckSet();
    void setTabNum(const QString& name);
    void changeTab();
    QString getStatusName(int type);
    void addStatusInfo(QString name, const StatusInfo& statusInfo);
    void delStatusInfo(QString name, const StatusInfo& statusInfo);
    void delAllStatusInfo(QString name, const StatusInfo& statusInfo);
    void dealStartInfo(); // 当启动信息栏数量为0时
    void dealFaultInfo(); // 当故障信息栏数量为0时

public slots:
    void slotNotifyInformation(int type,QList<QVariant> data);
    void slotCommandResult(int commandType, CMsgStructBase* msgData);
    void slotVerifyAdmin(int commandType, CMsgStructBase* msgData);
    void slotStateInformation(const StatusInfo& statusInfo, int type, int opr);
    void slotChangeColor();
    void slotcheckLoginInfo();
    void slot_usetimeCheck();
    void slot_usetime1h();
    void slot_threadFinished();
    void slot_updateStatus(CObject* ,short ,unsigned char);
    void slot_setDeviceOnlineTotal(CLoop* ,int);

private:
    int m_colorNum;//自检切换背景颜色计时
    QLabel *m_checkLabel;
    QStringList colorList;//自检颜色链表
    QTimer *m_checkTimer;//自检定时器
    QTimer *m_useTimeCheck;//使用期限计时

    int soundName;//播放声音类型
    bool soundPlay;//是否消音
    bool m_isFas;//是否配置连接FAS系统
    WorkType m_workType;//电池工作方式

    QHash<QString, CInformationModel*> m_informationModel;
    QStringList m_tabList;
    QTabWidget *m_tabWidget;
    bool m_isStatusTab;
};

class checkThread : public QThread
{
    Q_OBJECT
public:
    explicit checkThread(QObject *parent = NULL)
        : QThread(parent)
    {
    }
signals:
    void threadFinished();
    void updateStatus(CObject* ,short ,unsigned char);
    void setDeviceOnlineTotal(CLoop* ,int);
protected:
    void run();
};

#endif // CDATAPROCESSING_H
