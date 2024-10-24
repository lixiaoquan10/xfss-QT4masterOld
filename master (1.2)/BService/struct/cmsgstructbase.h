#ifndef CSTRUCTBASE_H
#define CSTRUCTBASE_H

#include <QObject>
#include <QDataStream>

class CMsgStructBase : public QObject
{
    Q_OBJECT
public:
    enum MsgStructType { MST_None = 0, MST_Log = 1, MST_RouteConfig = 2, MST_ProcessTypeName = 3, MST_ObjectStatus = 4, MST_Null = 5,
                         MST_LogInfo = 6, MST_FirePoint = 7, MST_PrintSet = 8, MST_CheckSet = 9, MST_CheckTime = 10, MST_DirectionTest = 11,
                         MST_ObjectStatusInfo = 12, MST_ControllerInfo = 13, MST_DeviceInfo = 14, MST_PowerInfo = 15, MST_EvacuationGroup = 16,
                         MST_LoginLoop = 17, MST_LEDInfo = 18};
    CMsgStructBase();
    virtual int msgType() const = 0;
    QByteArray data();
    QByteArray data(const int type);
protected:
    virtual void load(QDataStream& stream) = 0;
    virtual void save(QDataStream& stream) = 0;
private:
    static CMsgStructBase* loadChild(const int type, QDataStream& stream);
    friend QDataStream& operator << (QDataStream& stream, CMsgStructBase& object);
    friend QDataStream& operator >> (QDataStream& stream, CMsgStructBase** object);
private:
    static int m_magicNumber;
};

#endif // CSTRUCTBASE_H
