#ifndef CMSGOBJECTSTATUS_H
#define CMSGOBJECTSTATUS_H

#include "cmsgstructbase.h"

#include <QHash>
#include <QDateTime>

class CMsgObjectStatus : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgObjectStatus();
    int msgType() const { return MST_ObjectStatus; }

protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    int nType;
    int nCanportAdd;
    int nDisID;
    int nLoopID;
    int nDeviceID;
    int nStatusID;
    int nValue;
    unsigned int nTimes;
    QString strText;
    int nLoginStatus;
    int nCommunicationStatus;
    QString strdata;
    QByteArray ndata;
    QString nDataServerIP;
    int nDataServerPort;
    QString nProjectName;
    QString nProvince;
    QString nCity;
    QString nCountry;
    QString nAddress;
    QDate nStartTime;
    QDate nEndTime;
};

#endif // CMSGOBJECTSTATUS_H
