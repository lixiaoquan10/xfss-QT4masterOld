#include "cmsgprintset.h"

CMsgPrintSet::CMsgPrintSet() :
    CMsgStructBase(), printStart(false), printFault(false), printOther(false)
{
}

void CMsgPrintSet::load(QDataStream &stream)
{
    stream>>printStart>>printFault>>printOther;
}

void CMsgPrintSet::save(QDataStream &stream)
{
    stream<<printStart<<printFault<<printOther;
}
