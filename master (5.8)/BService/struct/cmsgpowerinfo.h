#ifndef CMSGPOWERINFO_H
#define CMSGPOWERINFO_H

#include "cmsgstructbase.h"

class CMsgPowerInfo : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgPowerInfo();
    int msgType() const { return MST_PowerInfo; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    int npanel;
    int keyId;
    int power[2];
    int charger1[2];
    int charger2[2];
    int charger3[2];
    int current[2];
    int voltage[2];
};

#endif // CMSGPOWERINFO_H
