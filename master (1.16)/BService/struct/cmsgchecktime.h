#ifndef CMSGCHECKTIME_H
#define CMSGCHECKTIME_H

#include "cmsgstructbase.h"

#include <QDateTime>

class CMsgCheckTime : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgCheckTime();
    int msgType() const { return MST_CheckTime; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    QDateTime systemStartTime;
    QDateTime systemMainPowerTime;
    QDateTime systemCurrentTime;
    QDateTime lastYearCheckTime;
    QDateTime yearCheckTime;
    QDateTime lastMonthCheckTime;
    QDateTime monthCheckTime;
    QDateTime speedMonthCheckTime;      //模拟加速月检时间
    QDateTime speedYearCheckTime;       //模拟加速年检时间
    QDateTime speedLastMonthCheckTime;  //上次模拟加速月检时间
    QDateTime speedLastYearCheckTime;   //上次模拟加速年检时间
    QDateTime speedLeftMonthCheckTime;  //模拟加速剩余月检时间
    QDateTime speedLeftYearCheckTime;   //模拟加速剩余年检时间
    bool isEntryCheckTimer;
};

#endif // CMSGCHECKTIME_H
