#include "cmsgdirectiontest.h"

CMsgDirectionTest::CMsgDirectionTest() :
    CMsgStructBase(), left(false), right(false)
{
}

void CMsgDirectionTest::load(QDataStream &stream)
{
    stream>>left>>right;
}

void CMsgDirectionTest::save(QDataStream &stream)
{
    stream<<left<<right;
}
