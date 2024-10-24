#ifndef CMSGLEDINFO_H
#define CMSGLEDINFO_H

#include "cmsgstructbase.h"

#include <QHash>

class CMsgLedInfo : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgLedInfo();
    int msgType() const { return MST_LEDInfo; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    int nLedID;
    int nLedStatus;
};

#endif // CMSGLEDINFO_H
