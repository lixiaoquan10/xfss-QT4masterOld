#include "cmsgevacuationgroup.h"

CMsgEvacuationGroup::CMsgEvacuationGroup() :
    CMsgStructBase(), keyId(0), parentKeyId(0)
{
}

void CMsgEvacuationGroup::load(QDataStream &stream)
{
    stream>>keyId>>parentKeyId>>name>>data;
}

void CMsgEvacuationGroup::save(QDataStream &stream)
{
    stream<<keyId<<parentKeyId<<name<<data;
}
