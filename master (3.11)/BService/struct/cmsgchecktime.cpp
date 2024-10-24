#include "cmsgchecktime.h"

CMsgCheckTime::CMsgCheckTime() :
    CMsgStructBase(), isEntryCheckTimer(false)
{
}

void CMsgCheckTime::load(QDataStream &stream)
{
    stream>>systemStartTime>>systemMainPowerTime>>systemCurrentTime>>lastYearCheckTime>>yearCheckTime>>lastMonthCheckTime>>monthCheckTime>>
            speedMonthCheckTime>>speedYearCheckTime>>speedLastMonthCheckTime>>speedLastYearCheckTime>>speedLeftMonthCheckTime>>speedLeftYearCheckTime>>isEntryCheckTimer;
}

void CMsgCheckTime::save(QDataStream &stream)
{
    stream<<systemStartTime<<systemMainPowerTime<<systemCurrentTime<<lastYearCheckTime<<yearCheckTime<<lastMonthCheckTime<<monthCheckTime<<
            speedMonthCheckTime<<speedYearCheckTime<<speedLastMonthCheckTime<<speedLastYearCheckTime<<speedLeftMonthCheckTime<<speedLeftYearCheckTime<<isEntryCheckTimer;
}
