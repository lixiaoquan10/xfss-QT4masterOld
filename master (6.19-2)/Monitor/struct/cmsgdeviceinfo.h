#ifndef CMSGDEVICEINFO_H
#define CMSGDEVICEINFO_H

#include "cmsgstructbase.h"

#include <QHash>

enum DeviceInfo { DI_DeviceType = 1, DI_KeyId, DI_ContollerId, DI_DistributionId, DI_LoopId, DI_DeviceId, DI_PowerId, DI_AlarmPointId };
enum DeviceType { DT_Canport = 1, DT_Distribution, DT_Loop, DT_Device, DT_Power, DT_AlarmPoint };
class CMsgDeviceInfo : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgDeviceInfo();
    int msgType() const { return MST_DeviceInfo; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    QHash<short, int> deviceInfo;
};

#endif // CMSGDEVICEINFO_H
