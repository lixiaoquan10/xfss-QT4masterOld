#ifndef CMSGLOG_H
#define CMSGLOG_H

#include "cmsgstructbase.h"

#include <QDateTime>

class CMsgLog : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgLog();
    int msgType() const { return MST_Log; }
protected:
    void load(QDataStream& stream);
    void save(QDataStream& stream);
public:
    int id;
    QString user;
    QDateTime time;
    QString title;
    QString type;
    int EventTypeID;
    QString User;
    QString Type;
    QDateTime Time;
    int Canport;
    int CanDeviceAddress;
    int Loop;
    int LampDeviceAddress;
    QString Area;
    QString Location;
    QString Remark;
};

#endif // CMSGLOG_H
