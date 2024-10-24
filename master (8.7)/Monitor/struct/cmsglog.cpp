#include "cmsglog.h"

CMsgLog::CMsgLog() :
    CMsgStructBase(), id(0)
{
    time = QDateTime::currentDateTime();
}

void CMsgLog::load(QDataStream &stream)
{
    stream>>id>>user>>time>>title>>type>>EventTypeID>>User>>Type>>Time>>Canport>>CanDeviceAddress>>Loop>>LampDeviceAddress>>Area>>Location>>Remark;
}

void CMsgLog::save(QDataStream &stream)
{
    stream<<id<<user<<time<<title<<type<<EventTypeID<<User<<Type<<Time<<Canport<<CanDeviceAddress<<Loop<<LampDeviceAddress<<Area<<Location<<Remark;
}
