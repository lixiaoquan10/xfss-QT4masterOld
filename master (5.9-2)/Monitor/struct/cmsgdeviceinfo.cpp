#include "cmsgdeviceinfo.h"

CMsgDeviceInfo::CMsgDeviceInfo() :
    CMsgStructBase()
{
}

void CMsgDeviceInfo::load(QDataStream &stream)
{
    stream>>deviceInfo;
}

void CMsgDeviceInfo::save(QDataStream &stream)
{
    stream<<deviceInfo;
}
