#include "cmsgrouteconfig.h"

CMsgRouteConfig::CMsgRouteConfig() :
    CMsgStructBase()
{
}

void CMsgRouteConfig::load(QDataStream &stream)
{
    routeConfigs.clear();
    int count = 0;
    stream >> count;
    for(int i = 0; i < count; ++i)
    {
        RouteConfig config;
        stream >> config.nPort >> config.nRate >> config.nType >> config.nCommpnyID >> config.nPanel;
        routeConfigs.append(config);
    }
}

void CMsgRouteConfig::save(QDataStream &stream)
{
    int count = routeConfigs.size();
    stream << count;
    for(int i = 0; i < count; ++i)
    {
        const RouteConfig& config = routeConfigs[i];
        stream << config.nPort << config.nRate << config.nType << config.nCommpnyID << config.nPanel;
    }
}
