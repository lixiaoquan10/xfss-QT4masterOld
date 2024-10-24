#include "cmsgadminuser.h"

CMsgAdminUser::CMsgAdminUser() :
    CMsgStructBase()
{
}

void CMsgAdminUser::load(QDataStream &stream)
{
    stream>>username>>password;
}

void CMsgAdminUser::save(QDataStream &stream)
{
    stream<<username<<password;
}
