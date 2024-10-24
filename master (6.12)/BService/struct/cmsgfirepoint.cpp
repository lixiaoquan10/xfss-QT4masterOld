#include "cmsgfirepoint.h"

CMsgFirePoint::CMsgFirePoint() :
    CMsgStructBase()
{
}

void CMsgFirePoint::load(QDataStream &stream)
{
    stream>>content;
}

void CMsgFirePoint::save(QDataStream &stream)
{
    stream<<content;
}
