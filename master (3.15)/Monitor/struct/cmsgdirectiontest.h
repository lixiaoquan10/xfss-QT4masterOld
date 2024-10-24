#ifndef CMSGDIRECTIONTEST_H
#define CMSGDIRECTIONTEST_H

#include "cmsgstructbase.h"

class CMsgDirectionTest : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgDirectionTest();
    int msgType() const { return MST_DirectionTest; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    bool left;
    bool right;
};

#endif // CMSGDIRECTIONTEST_H
