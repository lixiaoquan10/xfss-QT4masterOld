#include "cglobal.h"

namespace mcomm {
    CCommunication* communicationInsance = NULL;
}
using namespace mcomm;

int callbackOnDealEvent(int nType, char* params, int nLen)
{
    qDebug() << "CCommunication::callbackOnDealEvent"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    qDebug() << "CCommunication::CommunicationType"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    qDebug() << "CCommunication::DestroyedCommunication"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    Destroyed destroyed = (Destroyed)resolve("TXDestroyed");
    if(destroyed)
    {
        return destroyed();
    }
    return -1;
}

int CCommunication::InitCommunication()
{
    qDebug() << "CCommunication::InitCommunication"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    Initial initcom = (Initial)resolve("TXInitial");
    if(initcom)
    {
        return initcom(callbackOnDealEvent);
    }
    return -1;
}

bool CCommunication::StartCommunication(int nCanportID)
{
    qDebug() << "CCommunication::StartCommunication"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    Start opencom = (Start)resolve("TXStart");
    if(opencom)
    {
        return opencom(nCanportID);
    }
    return false;
}

bool CCommunication::StopCommunication()
{
    qDebug() << "CCommunication::StopCommunication"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    Stop close = (Stop)resolve("TXStop");
    if(close)
    {
        return close();
    }
    return false;
}

void CCommunication::ShowTest(bool bwrite)
{
    qDebug() << "CCommunication::ShowTest"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    Test comtest = (Test)resolve("TXShowTest");
    if(comtest)
    {
        comtest(bwrite);
    }
}

QByteArray CCommunication::GetPacket(int nMsgType)
{
    qDebug() << "CCommunication::GetPacket"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    qDebug() << "CCommunication::ParsePacket"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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


