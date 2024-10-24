#ifndef CMSGLOGINDATA_H
#define CMSGLOGINDATA_H

#include "cmsgstructbase.h"

class CMsgLoginData : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgLoginData();
    int msgType() const { return MST_LoginLoop; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    int nDistributeID;
    int nLoopID;
};

#endif // CMSGFILEDATA_H
