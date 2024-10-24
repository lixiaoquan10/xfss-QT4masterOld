#ifndef CMSGFIREPOINT_H
#define CMSGFIREPOINT_H

#include "cmsgstructbase.h"

class CMsgFirePoint : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgFirePoint();
    int msgType() const { return MST_FirePoint; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    QString content;
};

#endif // CMSGFIREPOINT_H
