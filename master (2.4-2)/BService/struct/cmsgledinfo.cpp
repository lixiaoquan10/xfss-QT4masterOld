#include "cmsgledinfo.h"

CMsgLedInfo::CMsgLedInfo() :
    CMsgStructBase(), nLedID(0), nLedStatus(0)
{
}

void CMsgLedInfo::load(QDataStream &stream)
{
    stream>>nLedID>>nLedStatus;
}

void CMsgLedInfo::save(QDataStream &stream)
{
    stream<<nLedID<<nLedStatus;
}
