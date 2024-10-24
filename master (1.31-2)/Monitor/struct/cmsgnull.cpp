#include "cmsgnull.h"

CMsgNull::CMsgNull() :
    CMsgStructBase()
{
}

void CMsgNull::load(QDataStream &stream)
{
    Q_UNUSED(stream);
}

void CMsgNull::save(QDataStream &stream)
{
    Q_UNUSED(stream);
}
