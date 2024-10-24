#ifndef CMSGCHECKSET_H
#define CMSGCHECKSET_H

#include "cmsgstructbase.h"

class CMsgCheckSet : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgCheckSet();
    int msgType() const { return MST_CheckSet; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    short yearCycle;
    short yearDuration;
    short monthCycle;
    short monthDuration;
};

#endif // CMSGCHECKSET_H
