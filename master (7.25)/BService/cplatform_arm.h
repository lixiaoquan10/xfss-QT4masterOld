#ifndef _CPLATFORM_ARM_H
#define _CPLATFORM_ARM_H
#include <qglobal.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef union _large_integer
{
    struct {
        long LowPart;
        long HighPart;
    }u;
    long long QuadPart;
} LARGE_INTEGER;

typedef struct date_time
{
    quint16 wYear;
    quint8 wMonth;
    quint8 wDayOfWeek;
    quint8 wDay;
    quint8 wHour;
    quint8 wMinute;
    quint8 wSecond;
}DATE_TIME;

double _sGetSysMsTime_x();
void TM_Get_Local(DATE_TIME &);
void GetLocalTime(DATE_TIME &);
bool SetLocalTime(DATE_TIME );

#ifdef __cplusplus
}
#endif

#endif
