#include "cadapterobject.h"
#include "log/clog.h"
#include "process/cprocessclient.h"
#include "process/cprocesscontroller.h"
#include "struct/cmsglog.h"
#include "struct/cmsgstructbase.h"
#include "struct/cmsgrouteconfig.h"
#include "struct/cmsgcontrollerinfo.h"
#include "cglobal.h"
#include "struct.h"

#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTextCodec>
#include <QSettings>
#include <QTranslator>
#include <QApplication>
#include <QDebug>

CAdapterObject::CAdapterObject(QObject *parent) :
    QObject(parent)
{
    connect(this, SIGNAL(sigParsePacket(int,QByteArray)), this, SLOT(slotParsePacket(int,QByteArray)), Qt::QueuedConnection);
}

void CAdapterObject::TXParsePacket(int nMsgType, const char *data, int nLen)
{
    QByteArray byte(data, nLen);
    if(nMsgType == NCT_SetRouteConfig) {
        QDataStream stream(&byte, QIODevice::ReadOnly);
        CMsgStructBase* msgStruct = NULL;
        stream>>&msgStruct;
        if(msgStruct)
        {
            CMsgRouteConfig* msgRouteConfig = static_cast<CMsgRouteConfig*>(msgStruct);
            CGlobal::instance()->setRouteConfig(msgRouteConfig->routeConfigs);
        }
        delete msgStruct;
    } else if(nMsgType == NCT_ControllerInfo){
        QDataStream stream(&byte, QIODevice::ReadOnly);
        CMsgStructBase* msgStruct = NULL;
        stream>>&msgStruct;
        if(msgStruct)
        {
            CMsgControllerInfo* msgctrinfo = static_cast<CMsgControllerInfo*>(msgStruct);
            CGlobal::instance()->setPanelAddress(msgctrinfo->address);
        }
        delete msgStruct;
    } else if(nMsgType == NCT_FactoryReset) {
        CGlobal::instance()->processClient()->factoryReset();
        TXStop();
        QFile file(CGlobal::instance()->projectName());
        qDebug()<<"NCT_FactoryReset:  Remvoe"<<file.fileName();
        if(file.exists())
        {
            QString backupPath = CGlobal::m_workPath + "/backup";
            QDir dir(backupPath);
            if(!dir.exists())
                dir.mkpath(backupPath);
            qDebug()<<file.rename(backupPath + "/" + QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz") + ".bak");
        }
        TXDestroyed();
    } else {
        emit sigParsePacket(nMsgType, byte);
    }
}

void CAdapterObject::slotParsePacket(int nMsgType, QByteArray data)
{
    Q_UNUSED(nMsgType);
    CGlobal::instance()->processClient()->procRecvData(data);
}
