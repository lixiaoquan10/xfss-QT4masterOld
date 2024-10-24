#include "cmsgstructbase.h"
#include "cmsgchecktime.h"
#include "cmsglog.h"
#include "cmsgloginfo.h"
#include "cmsgnull.h"
#include "cmsgevacuationgroup.h"
#include "cmsgprocesstypename.h"
#include "cmsgrouteconfig.h"
#include "cmsgfirepoint.h"
#include "cmsgprintset.h"
#include "cmsgcheckset.h"
#include "cmsgpowerinfo.h"
#include "cmsgobjectstatus.h"
#include "cmsgobjectstatusinfo.h"
#include "cmsgcontrollerinfo.h"
#include "cmsglogin.h"
#include "cmsgledinfo.h"
#include "cmsgdeviceinfo.h"
#include "cmsgdirectiontest.h"

int CMsgStructBase::m_magicNumber = 1122366554;

CMsgStructBase::CMsgStructBase() :
    QObject()
{
}

QByteArray CMsgStructBase::data()
{
    QByteArray byte;
    QDataStream stream(&byte, QIODevice::WriteOnly);
    stream<<(*this);
    return byte;
}

QByteArray CMsgStructBase::data(const int type)
{
    QByteArray byte;
    QDataStream stream(&byte, QIODevice::WriteOnly);
    stream<<type<<(*this);
    return byte;
}

CMsgStructBase *CMsgStructBase::loadChild(const int type, QDataStream &stream)
{
    CMsgStructBase* tmp = NULL;
    switch(type)
    {
    case MST_Log:
        tmp = new CMsgLog();
        break;
    case MST_LogInfo:
        tmp = new CMsgLogInfo();
        break;
    case MST_LoginLoop:
        tmp = new CMsgLoginData();
        break;
    case MST_RouteConfig:
        tmp = new CMsgRouteConfig();
        break;
    case MST_ProcessTypeName:
        tmp = new CMsgProcessTypeName();
        break;
    case MST_ObjectStatus:
        tmp = new CMsgObjectStatus();
        break;
    case MST_Null:
        tmp = new CMsgNull();
        break;
    case MST_ObjectStatusInfo:
        tmp = new CMsgObjectStatusInfo();
        break;
    case MST_CheckTime:
        tmp = new CMsgCheckTime();
        break;
    case MST_PowerInfo:
        tmp = new CMsgPowerInfo();
        break;
    case MST_EvacuationGroup:
        tmp = new CMsgEvacuationGroup();
        break;
    case MST_FirePoint:
        tmp = new CMsgFirePoint();
        break;
    case MST_PrintSet:
        tmp = new CMsgPrintSet();
        break;
    case MST_CheckSet:
        tmp = new CMsgCheckSet();
        break;
    case MST_ControllerInfo:
        tmp = new CMsgControllerInfo();
        break;
    case MST_LEDInfo:
        tmp = new CMsgLedInfo();
        break;
    case MST_DirectionTest:
        tmp = new CMsgDirectionTest();
        break;
    case MST_DeviceInfo:
        tmp = new CMsgDeviceInfo();
        break;
    }
    if(tmp)
        tmp->load(stream);
    return tmp;
}

QDataStream & operator <<(QDataStream &stream, CMsgStructBase &object)
{
    stream<<CMsgStructBase::m_magicNumber<<object.msgType();
    object.save(stream);
    return stream;
}

QDataStream & operator >>(QDataStream &stream, CMsgStructBase **object)
{
    int type = CMsgStructBase::MST_None;
    int magicNumber = 0;
    stream>>magicNumber>>type;
    if(magicNumber == CMsgStructBase::m_magicNumber)
        *object = CMsgStructBase::loadChild(type, stream);
    return stream;
}
