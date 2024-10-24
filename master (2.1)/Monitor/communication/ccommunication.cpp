#include "ccommunication.h"
#include "cglobal.h"
#include <QDebug>

namespace mcomm {
    CCommunication* communicationInsance = NULL;
}
using namespace mcomm;

int callbackOnDealEvent(int nType, char* params, int nLen)
{
    if(communicationInsance)
    {
        QByteArray data(params, nLen);
        communicationInsance->emitRecvData(nType, data);
    }
    return 1;
}

CCommunication::CCommunication(const QString &fileName, QObject *parent) :
    QLibrary(fileName, parent)
{
    qDebug() << this->errorString();
    communicationInsance = this;
}

int CCommunication::CommunicationType(QString strProjectPath)
{
    Type type = (Type)resolve("TXType");
    qDebug()<< this->errorString();
    if(type)
    {
        return type(strProjectPath.toUtf8().data());
    }
    return -1;
}

int CCommunication::DestroyedCommunication()
{
    Destroyed destroyed = (Destroyed)resolve("TXDestroyed");
    if(destroyed)
    {
        return destroyed();
    }
    return -1;
}

int CCommunication::InitCommunication()
{
    Initial initcom = (Initial)resolve("TXInitial");
    if(initcom)
    {
        return initcom(callbackOnDealEvent);
    }
    return -1;
}

bool CCommunication::StartCommunication(int nCanportID)
{
    Start opencom = (Start)resolve("TXStart");
    if(opencom)
    {
        return opencom(nCanportID);
    }
    return false;
}

bool CCommunication::StopCommunication()
{
    Stop close = (Stop)resolve("TXStop");
    if(close)
    {
        return close();
    }
    return false;
}

void CCommunication::ShowTest(bool bwrite)
{
    Test comtest = (Test)resolve("TXShowTest");
    if(comtest)
    {
        comtest(bwrite);
    }
}

QByteArray CCommunication::GetPacket(int nMsgType)
{
    QByteArray retbyte;
    GPacket getpk = (GPacket)resolve("TXGetPacket");
    if(getpk)
    {
        char ByteBuf[8192]; int nLen = 0;
        memset(ByteBuf, 0, 8192);
        getpk(nMsgType, ByteBuf, nLen);
        retbyte.append(ByteBuf, nLen);
    }
    return retbyte;
}

void CCommunication::ParsePacket(int nMsgType, const char *data, int nLen)
{
    PPacket parsepk = (PPacket)resolve("TXParsePacket");
    if(parsepk)
    {
        parsepk(nMsgType, data, nLen);
    }
}

void CCommunication::emitRecvData(int nMsgType, QByteArray &data)
{
    emit recvData(nMsgType, data);
}


