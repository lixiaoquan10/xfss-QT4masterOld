#include "cmsgcheckset.h"

CMsgCheckSet::CMsgCheckSet() :
    CMsgStructBase(), yearCycle(0), yearDuration(0), monthCycle(0), monthDuration(0)
{
}

void CMsgCheckSet::load(QDataStream &stream)
{
    stream>>yearCycle>>yearDuration>>monthCycle>>monthDuration;
}

void CMsgCheckSet::save(QDataStream &stream)
{
    stream<<yearCycle<<yearDuration<<monthCycle<<monthDuration;
}
