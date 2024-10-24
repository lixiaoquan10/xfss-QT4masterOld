#include "cmsgobjectstatusinfo.h"

CMsgObjectStatusInfo::CMsgObjectStatusInfo() :
    CMsgStructBase(), statusId(0)
{
}

void CMsgObjectStatusInfo::load(QDataStream &stream)
{
    stream>>statusId;
}

void CMsgObjectStatusInfo::save(QDataStream &stream)
{
    stream<<statusId;
}
