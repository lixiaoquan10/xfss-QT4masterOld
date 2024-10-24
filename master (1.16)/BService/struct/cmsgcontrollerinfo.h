#ifndef CMSGCONTROLLERINFO_H
#define CMSGCONTROLLERINFO_H

#include "cmsgstructbase.h"

class CMsgControllerInfo : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgControllerInfo();
    int msgType() const { return MST_ControllerInfo; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    int address;
    int netType;
};

#endif // CMSGCONTROLLERINFO_H
