#ifndef CMSGFILEINFO_H
#define CMSGFILEINFO_H

#include "cmsgstructbase.h"

#include <QHash>

class CMsgFileInfo : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgFileInfo();
    int msgType() const { return MST_FileInfo; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    QHash<QString, int> size;
    QHash<QString, QString> verify;
};

#endif // CMSGFILEINFO_H
