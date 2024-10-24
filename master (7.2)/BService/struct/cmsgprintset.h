#ifndef CMSGPRINTSET_H
#define CMSGPRINTSET_H

#include "cmsgstructbase.h"

class CMsgPrintSet : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgPrintSet();
    int msgType() const { return MST_PrintSet; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    bool printStart;
    bool printFault;
    bool printOther;
};

#endif // CMSGPRINTSET_H
