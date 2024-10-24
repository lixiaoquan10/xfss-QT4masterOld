#ifndef CMSGLOGINFO_H
#define CMSGLOGINFO_H

#include "cmsgstructbase.h"

#include <QDateTime>

class CMsgLogInfo : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgLogInfo();
    int msgType() const { return MST_LogInfo; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    int requestId;
    QDateTime beginTime;
    QDateTime endTime;
    QString type;
};

#endif // CMSGLOGINFO_H
