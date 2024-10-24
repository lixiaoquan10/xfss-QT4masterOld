#include "cplatform_arm.h"
#include <sys/time.h>
#include <sys/times.h>
#include <qglobal.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

static quint32 ticksperSec_x ;
static quint32 lastCheckTime_x = 0;
static quint64 wrapCount_x = 0;
static quint16 initialized_x = 0;

double _sGetSysMsTime_x()
{
    double sysMsTime = (double)0.0;
    struct tms tmsTime;
    quint32 sysTime;

    if (initialized_x == 0) {
        ticksperSec_x = (quint32) sysconf(_SC_CLK_TCK);
        if (ticksperSec_x == (quint32)-1L || ticksperSec_x == 0)
            return 0.0;
        initialized_x = 1;
    }
    sysTime = (quint32) times(&tmsTime);    /* 程序启动以后经过的ticks*/
    if (sysTime != ((quint32) -1)) {
        if (sysTime < lastCheckTime_x)
            wrapCount_x += (quint32) (1 << 32);
        sysMsTime = (double) (wrapCount_x + (quint64)sysTime) * 1000 / (quint64) ticksperSec_x;
        lastCheckTime_x = sysTime;
    }
    return sysMsTime;

}

void GetLocalTime(DATE_TIME &t)
{
   memset(&t, 0, sizeof(DATE_TIME));
   struct tm dt;
   time_t seconds = time((time_t *)NULL);
   localtime_r(&seconds, &dt);
   t.wYear = dt.tm_year+1900;
   t.wMonth = dt.tm_mon + 1;
   t.wDay = dt.tm_mday;
   t.wHour = dt.tm_hour;
   t.wMinute = dt.tm_min;
   t.wSecond = dt.tm_sec;
}

bool SetLocalTime(DATE_TIME dt)
{
    struct tm t_tm;
    struct timeval tv;
    char szCmd[256] = {0};
    t_tm.tm_year = dt.wYear - 1900;
    t_tm.tm_mon = dt.wMonth - 1;
    t_tm.tm_mday = dt.wDay;
    t_tm.tm_hour = dt.wHour;
    t_tm.tm_min = dt.wMinute;
    t_tm.tm_sec = dt.wSecond;
    tv.tv_sec = mktime(&t_tm);
    tv.tv_usec = 0;
    settimeofday(&tv, NULL);
    sprintf(szCmd, "hwclock -w");
    system(szCmd);
    return true;
}

