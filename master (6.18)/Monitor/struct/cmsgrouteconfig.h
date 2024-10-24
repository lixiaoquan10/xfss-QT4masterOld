#ifndef CMSGROUTECONFIG_H
#define CMSGROUTECONFIG_H

#include "cmsgstructbase.h"

#include <QHash>

typedef struct {
    int nPort;  //the scope is 1~7
    int nRate; //the port rate,such as RS232 baudrate
    int nType;  //such as FireCanport,CentralizedPower,Distribution of electric
    int nCommpnyID;
    int nPanel;
} RouteConfig, *pRouteConfig;

class CMsgRouteConfig : public CMsgStructBase
{
    Q_OBJECT
public:
    CMsgRouteConfig();
    int msgType() const { return MST_RouteConfig; }
protected:
    void load(QDataStream &stream);
    void save(QDataStream &stream);
public:
    QList<RouteConfig> routeConfigs;
};

#endif // CMSGROUTECONFIG_H
