#include "cmsgprintset.h"

CMsgPrintSet::CMsgPrintSet() :
    CMsgStructBase(), printLendon(false), printFault(false), printOther(false)
{
}

void CMsgPrintSet::load(QDataStream &stream)
{
    stream>>printLendon>>printFault>>printOther;
}

void CMsgPrintSet::save(QDataStream &stream)
{
    stream<<printLendon<<printFault<<printOther;
}
