#ifndef CMSGADMINUSER_H
#define CMSGADMINUSER_H

#include "cmsgstructbase.h"

class CMsgAdminUser : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgAdminUser();
    int msgType() const { return MST_AdminUser; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    QString username;
    QString password;
};

#endif // CMSGADMINUSER_H
