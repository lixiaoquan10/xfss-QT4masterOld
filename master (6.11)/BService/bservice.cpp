#include "bservice.h"
#include "cadapterobject.h"
#include "struct/cmsgprocesstypename.h"
#include "process/cprocessclient.h"
#include "process/cprocesscontroller.h"
#include "process/ccommunicationroute.h"
#include "log/clog.h"
#include "cglobal.h"

#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTextCodec>
#include <QSettings>
#include <QTranslator>
#include <QApplication>
#include <QThread>

int TXType(const char* strPath/*project path*/)
{
    QTextCodec* utf8 = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForCStrings(utf8);
    QTextCodec::setCodecForTr(utf8);
    QTextCodec::setCodecForLocale(utf8);
    QCoreApplication* app = QApplication::instance();
    QTranslator *mytranslator = new QTranslator(app);
    if (mytranslator->load(QLatin1String("BService_zh_CN.qm"), QLatin1String(":/")))
       app->installTranslator(mytranslator);

    QString path = QString::fromUtf8(strPath);
    if(CGlobal::m_dllStatus != DS_TXNone)
        return 0;
    QDir dir(path);
    if(!dir.exists())
        dir.mkpath(path);
    QSettings setting("zsyf", "Monitor");
    setting.setValue("projectpath", path);
    CGlobal::m_workPath = path;
    CGlobal::m_dllStatus = DS_TXType;
    return 0;
}

int TXDestroyed()
{
    CGlobal::destroy();
    CGlobal::m_dllStatus = DS_TXNone;
    return 0;
}

int TXInitial(OnDealEvent event)
{
    if(CGlobal::m_dllStatus != DS_TXType)
        return 0;
    CGlobal::instance()->SetCallBack(event);
    CGlobal::instance()->initParams();
    CGlobal::m_dllStatus = DS_TXInitial;
    return 1;
}

bool TXStart(int nPanelID)
{
    if(CGlobal::m_dllStatus != DS_TXInitial)
        return false;
    CGlobal::instance()->setPanelAddress(nPanelID);
    CGlobal::instance()->communicationRoute()->openCommunication();
    CGlobal::instance()->processClient()->StartCommunication();
    CGlobal::instance()->processController()->initProcess();
    CGlobal::m_dllStatus = DS_TXStart;
    return true;
}

bool TXStop()
{
    if(CGlobal::m_dllStatus != DS_TXStart)
        return false;
    CGlobal::instance()->communicationRoute()->closeCommunication();
    CGlobal::instance()->processClient()->StopCommunication();
    CGlobal::m_dllStatus = DS_TXStop;
    return true;
}

void TXShowTest(bool bWrite)
{
    if(CGlobal::m_dllStatus != DS_TXStart)
        return;
    CGlobal::instance()->communicationRoute()->setDebug(bWrite);
}

void TXGetPacket(int nMsgType, char *pByteBuf/*out*/,int &nOutLen/*out*/)
{
    if(CGlobal::m_dllStatus == DS_TXNone)
        return;
    switch(nMsgType)
    {
        case NCT_GetProcessTypeName:
        {
            CMsgProcessTypeName msgProcessTypeName;
//            msgProcessTypeName.typeName = CGlobal::instance()->processTypeName();
            QByteArray outByte = msgProcessTypeName.data();
            memcpy(pByteBuf, outByte.data(), outByte.size());
            nOutLen = outByte.size();
        }
    }
}

void TXParsePacket(int nMsgType, const char* data, int nLen)
{
    if(nMsgType != NCT_SetRouteConfig && CGlobal::m_dllStatus != DS_TXInitial && CGlobal::m_dllStatus != DS_TXStart)
        return;
    CGlobal::instance()->adapterObject()->TXParsePacket(nMsgType, data, nLen);
}

