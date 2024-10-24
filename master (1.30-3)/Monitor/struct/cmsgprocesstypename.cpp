#include "cmsgprocesstypename.h"

CMsgProcessTypeName::CMsgProcessTypeName() :
    CMsgStructBase()
{
}

void CMsgProcessTypeName::load(QDataStream &stream)
{
    stream>>typeName;
}

void CMsgProcessTypeName::save(QDataStream &stream)
{
    stream<<typeName;
}
