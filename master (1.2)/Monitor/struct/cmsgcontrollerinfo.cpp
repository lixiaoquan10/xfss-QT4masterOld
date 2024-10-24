#include "cmsgcontrollerinfo.h"

CMsgControllerInfo::CMsgControllerInfo() :
    CMsgStructBase(), address(0), netType(0)
{
}

void CMsgControllerInfo::load(QDataStream &stream)
{
    stream>>address>>netType;
}

void CMsgControllerInfo::save(QDataStream &stream)
{
    stream<<address<<netType;
}

