#include "cmsginterfaceboardinfo.h"

CMsgInterfaceBoardInfo::CMsgInterfaceBoardInfo() :
    CMsgStructBase()
{
}

void CMsgInterfaceBoardInfo::load(QDataStream &stream)
{
    stream>>interfaceBoardInfo;
}

void CMsgInterfaceBoardInfo::save(QDataStream &stream)
{
    stream<<interfaceBoardInfo;
}
