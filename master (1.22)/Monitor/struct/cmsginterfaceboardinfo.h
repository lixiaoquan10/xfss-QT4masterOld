#ifndef CMSGINTERFACEBOARDINFO_H
#define CMSGINTERFACEBOARDINFO_H

#include "cmsgstructbase.h"

#include <QHash>

enum InterfaceCardType { ICT_NoCard = 0, ICT_PowerCard, ICT_CanCard, ICT_Rs485Card, ICT_Rs232Card };

class CMsgInterfaceBoardInfo : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgInterfaceBoardInfo();
    int msgType() const { return MST_InterfaceBoardInfo; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    QHash<int, int> interfaceBoardInfo;
};

#endif // CMSGINTERFACEBOARDINFO_H
