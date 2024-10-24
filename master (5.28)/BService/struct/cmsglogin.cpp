#include "cmsglogin.h"

CMsgLoginData::CMsgLoginData() :
    CMsgStructBase()
{
}

void CMsgLoginData::load(QDataStream &stream)
{
    stream>>nDistributeID>>nLoopID;
}

void CMsgLoginData::save(QDataStream &stream)
{
    stream<<nDistributeID<<nLoopID;
}
