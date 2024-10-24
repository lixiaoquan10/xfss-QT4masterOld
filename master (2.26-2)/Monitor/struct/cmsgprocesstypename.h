#ifndef CMSGPROCESSTYPENAME_H
#define CMSGPROCESSTYPENAME_H

#include "cmsgstructbase.h"

#include <QHash>

class CMsgProcessTypeName : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgProcessTypeName();
    int msgType() const { return MST_ProcessTypeName; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    QHash<int, QString> typeName;
};

#endif // CMSGPROCESSTYPENAME_H
