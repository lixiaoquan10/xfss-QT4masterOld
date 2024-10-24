#ifndef CMSGNULL_H
#define CMSGNULL_H

#include "cmsgstructbase.h"

class CMsgNull : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgNull();
    int msgType() const { return MST_Null; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
};

#endif // CMSGNULL_H
