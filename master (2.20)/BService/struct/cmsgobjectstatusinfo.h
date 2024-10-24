#ifndef CMSGOBJECTSTATUSINFO_H
#define CMSGOBJECTSTATUSINFO_H

#include "cmsgstructbase.h"

class CMsgObjectStatusInfo : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgObjectStatusInfo();
    int msgType() const { return MST_ObjectStatusInfo; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    int statusId;
};

#endif // CMSGOBJECTSTATUSINFO_H

