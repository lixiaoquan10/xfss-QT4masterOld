#ifndef CMSGLEDINFO_H
#define CMSGLEDINFO_H

#include "cmsgstructbase.h"

#include <QHash>


/**
 * @brief The CMsgLedInfo class
 ** 8 个LED 灯状态， 控制声音， 控制输出的
**/
class CMsgLedInfo : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgLedInfo();
    int msgType() const { return MST_LEDInfo; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    int nLedID;
    int nLedStatus; /*8xled*/


};

#endif // CMSGLEDINFO_H
