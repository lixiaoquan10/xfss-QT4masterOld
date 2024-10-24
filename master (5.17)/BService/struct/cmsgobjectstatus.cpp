#include "cmsgobjectstatus.h"

CMsgObjectStatus::CMsgObjectStatus() :
    CMsgStructBase(), nType(0), nCanportAdd(0), nDisID(0), nLoopID(0), nDeviceID(0), nStatusID(0),
    nValue(0), nTimes(0),strText(""),nLoginStatus(0), nCommunicationStatus(0), strdata(""), ndata(0),
    nDataServerIP(""), nDataServerPort(0), nProjectName(""), nProvince(""), nCity(""), nCountry(""), nAddress(""),
    nStartTime(0,0,0), nEndTime(0,0,0)
{
}

void CMsgObjectStatus::load(QDataStream &stream)
{
    stream>>nType>>nCanportAdd>>nDisID>>nLoopID>>nDeviceID>>nStatusID>>nValue>>nTimes>>strText>>nLoginStatus>>nCommunicationStatus>>strdata>>ndata
            >>nDataServerIP>>nDataServerPort>>nProjectName>>nProvince>>nCity>>nCountry>>nAddress>>nStartTime>>nEndTime;
}

void CMsgObjectStatus::save(QDataStream &stream)
{
    stream<<nType<<nCanportAdd<<nDisID<<nLoopID<<nDeviceID<<nStatusID<<nValue<<nTimes<<strText<<nLoginStatus<<nCommunicationStatus<<strdata<<ndata
         <<nDataServerIP<<nDataServerPort<<nProjectName<<nProvince<<nCity<<nCountry<<nAddress<<nStartTime<<nEndTime;
}
