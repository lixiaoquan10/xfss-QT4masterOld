#include "cmsgfiledata.h"

CMsgFileData::CMsgFileData() :
    CMsgStructBase(), currentPack(0), packCount(0), packSize(0), totalSize(0)
{
}

void CMsgFileData::load(QDataStream &stream)
{
    stream>>fileName>>currentPack>>currentData>>packCount>>packSize>>totalSize;
}

void CMsgFileData::save(QDataStream &stream)
{
    stream<<fileName<<currentPack<<currentData<<packCount<<packSize<<totalSize;
}
