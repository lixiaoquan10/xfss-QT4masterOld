#include "ccommandbytedata.h"

CCommandByteData::CCommandByteData(int nSerialNumber ,qint64 time,int count,const QByteArray &byteData,int nPriority) :
    QObject(0)
{
    serialNumber = nSerialNumber;
    endTime = time;
    repeatCount = count;
    data = byteData;
    priority = nPriority;
}
