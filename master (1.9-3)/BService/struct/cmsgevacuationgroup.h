#ifndef CMSGEVACUATIONGROUP_H
#define CMSGEVACUATIONGROUP_H

#include "cmsgstructbase.h"

class CMsgEvacuationGroup : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgEvacuationGroup();
    int msgType() const { return MST_EvacuationGroup; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    int keyId;
    int parentKeyId;
    QString name;
    QByteArray data;
};

#endif // CMSGEVACUATIONGROUP_H
