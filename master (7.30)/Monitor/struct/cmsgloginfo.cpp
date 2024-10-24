#include "cmsgloginfo.h"

CMsgLogInfo::CMsgLogInfo() :
    CMsgStructBase(), requestId(0)
{
}

void CMsgLogInfo::load(QDataStream &stream)
{
    stream>>requestId>>beginTime>>endTime>>type;
}

void CMsgLogInfo::save(QDataStream &stream)
{
    stream<<requestId<<beginTime<<endTime<<type;
}
