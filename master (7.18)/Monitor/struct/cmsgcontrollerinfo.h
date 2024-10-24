#ifndef CMSGCONTROLLERINFO_H
#define CMSGCONTROLLERINFO_H

#include "cmsgstructbase.h"

class CMsgCanportInfo : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgCanportInfo();
    int msgType() const { return MST_CanportInfo; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    int address;
    int netType;
};

#endif // CMSGCONTROLLERINFO_H
