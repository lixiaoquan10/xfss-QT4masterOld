#include "cmsgcontrollerinfo.h"

CMsgCanportInfo::CMsgCanportInfo() :
    CMsgStructBase(), address(0), netType(0)
{
}

void CMsgCanportInfo::load(QDataStream &stream)
{
    stream>>address>>netType;
}

void CMsgCanportInfo::save(QDataStream &stream)
{
    stream<<address<<netType;
}

