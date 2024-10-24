#include "cmsgpowerinfo.h"

CMsgPowerInfo::CMsgPowerInfo() :
    CMsgStructBase(), npanel(0), keyId(0)
{
    memset(power, 0, sizeof(int) * 2);
    memset(charger1, 0, sizeof(int) * 2);
    memset(charger2, 0, sizeof(int) * 2);
    memset(charger3, 0, sizeof(int) * 2);
    memset(current, 0, sizeof(int) * 2);
    memset(voltage, 0, sizeof(int) * 2);

}

void CMsgPowerInfo::load(QDataStream &stream)
{
    stream>>npanel>>keyId;
    for(int i = 0; i < 2; ++i)
        stream>>power[i];
    for(int i = 0; i < 2; ++i)
        stream>>charger1[i];
    for(int i = 0; i < 2; ++i)
        stream>>charger2[i];
    for(int i = 0; i < 2; ++i)
        stream>>charger3[i];
    for(int i = 0; i < 2; ++i)
        stream>>current[i];
    for(int i = 0; i < 2; ++i)
        stream>>voltage[i];
}

void CMsgPowerInfo::save(QDataStream &stream)
{
    stream<<npanel<<keyId;
    for(int i = 0; i < 2; ++i)
        stream<<power[i];
    for(int i = 0; i < 2; ++i)
        stream<<charger1[i];
    for(int i = 0; i < 2; ++i)
        stream<<charger2[i];
    for(int i = 0; i < 2; ++i)
        stream<<charger3[i];
    for(int i = 0; i < 2; ++i)
        stream<<current[i];
    for(int i = 0; i < 2; ++i)
        stream<<voltage[i];
}
