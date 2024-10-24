#include "cglobal.h"
#include "cplatform_arm.h"
#include "cadapterobject.h"
#include "log/clog.h"
#include "print/cprint.h"
#include "process/cprocessbase.h"
#include "process/cprocessclient.h"
#include "process/cprocessdistribution.h"
#include "process/cprocesscontroller.h"
#include "process/ccommunicationroute.h"
#include "process/cprocesssocket.h"
#include "process/cprocessserver.h"

#include <QFile>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

bool     CGlobal::m_bDebug = true;
int      CGlobal::m_dllStatus = 0;
QString  CGlobal::m_workPath = ".";
CGlobal* CGlobal::m_instance = NULL;

#if 0
qint64 getTickCount()
{
    qint64 tick = 0;
    LARGE_INTEGER TicksPerSecond;
    LARGE_INTEGER Tick;
    memset(&TicksPerSecond, 0, sizeof(TicksPerSecond));
    if (!TicksPerSecond.QuadPart)
        QueryPerformanceFrequency(&TicksPerSecond);
    QueryPerformanceCounter(&Tick);
    __int64 Seconds = Tick.QuadPart / TicksPerSecond.QuadPart;
    __int64 LeftPart = Tick.QuadPart - (TicksPerSecond.QuadPart*Seconds);
    __int64 MillSeconds = LeftPart * 1000 / TicksPerSecond.QuadPart;
    tick = Seconds * 1000 + MillSeconds;
    return tick;
}
#endif

qint64 getTickCount()
{
    return qint64(_sGetSysMsTime_x());
}


CGlobal::CGlobal()
{
    m_dle = NULL;
    m_log = NULL;
    m_print = NULL;
    m_adapterObject = NULL;
    m_processController = NULL;
    m_processDistribution = NULL;
    m_processClient = NULL;
    m_processServer = NULL;
    m_processSocket = NULL;
    m_communicationRoute = NULL;
    m_dlgInforWindow = NULL;

    m_PanelAddress = 0;
    m_nPowerBeatCount = 0;
    m_bPowerFault = false;
    m_recordPage = 1;
    m_recordNumber = 0;
    m_canport1ID = 3;
    m_canport2ID = 5;
}

CGlobal::~CGlobal()
{
    delete m_log;
    delete m_print;
    delete m_adapterObject;
    delete m_processController;
    delete m_processDistribution;
    delete m_processClient;
    delete m_processServer;
    delete m_processSocket;
    delete m_communicationRoute;
}

CGlobal *CGlobal::instance()
{
    if(!m_instance)
        m_instance = new CGlobal();
    return m_instance;
}

void CGlobal::destroy()
{
    if (m_instance) {
        delete m_instance;
        m_instance = NULL;
    }
}

bool CGlobal::setDateTime(const QDateTime& t)
{
    bool b = false;
    DATE_TIME st;
    GetLocalTime(st);

    st.wYear = t.date().year();
    st.wMonth = t.date().month();
    st.wDay = t.date().day();
    st.wDayOfWeek = t.date().dayOfWeek();
    st.wHour = t.time().hour();
    st.wMinute = t.time().minute();
    st.wSecond = t.time().second();

    b = SetLocalTime(st);
    return b;
}

void CGlobal::initParams()
{
    m_adapterObject = new CAdapterObject();
    m_log = new CLog(m_workPath + "/log.db");
    m_print = new CPrint();
    m_processClient = new CProcessClient();
    m_processController = new CProcessController();
    m_communicationRoute = new CCommunicationRoute();
    m_processServer = new CprocessServer();
    m_processSocket = new CprocessSocket();
    QHash<QString, QVariant> control;
    m_communicationRoute->createRoute(m_processClient, CT_NoCard, control);
}

int CGlobal::DealEvent(int nType, const QByteArray& data)
{
    if(!m_dle)
        return 0;
    return m_dle(nType, const_cast<char*>(data.data()), data.size());
}

void CGlobal::setRouteConfig(const QList<RouteConfig> &routeConfig)
{
    QHash<QString, QVariant> control;
    CCommunicationRoute* communicationRoute = this->communicationRoute();
    QList<int> listpowerport;
    QList<int> listdistributionport;
    int processtype = CT_NoCard;
    for(int i = 0; i < routeConfig.size(); ++i)
    {
        const RouteConfig& config = routeConfig[i];
        CProcessBase* currentProcess = NULL;
        switch(config.nType)
        {
            case PT_Distribution_CAN:
            {
                if(!m_processDistribution)
                    m_processDistribution = new CProcessDistribution();
                currentProcess = m_processDistribution;
                listdistributionport.append(config.nPort);
                if(config.nPort >= 2) processtype = CT_CanCard2;
                else processtype = CT_CanCard1;
                break;
            }
        }
        if(currentProcess)
        {
            control[INTERFACEBOARD_PORT_NUMBER_STR] = config.nPort;
            control[INTERFACEBOARD_PORT_SPEED_STR] = config.nRate;
            control[INTERFACEBOARD_COMMPNYCODE] = config.nCommpnyID;
            control[INTERFACEBOARD_PANEL] = config.nPanel;
            communicationRoute->createRoute(currentProcess, processtype, control);
        }
    }
    bool bshared = false;
    if(listdistributionport.count() > 0){
        for(int n=0; n<listdistributionport.count(); n++){
            if(listpowerport.contains(listdistributionport[n]))
                bshared = true;
        }
    }
    if(bshared && m_processDistribution){
        m_processDistribution->setIsShardCommunication(true);
    }
}

void CGlobal::closefile()
{
    for(int i=0; i<6; i++){
        if(m_writefile[i].isOpen()) m_writefile[i].close();
    }
}

void CGlobal::write(int nPort, bool bsend, QByteArray byte)
{
    if(m_writefile[nPort - 1].fileName().isEmpty())
    {
        QString filename = QCoreApplication::applicationDirPath() + "/Log/" + QDateTime::currentDateTime().toString("yyyy_MM_dd") + "_COM" + QString::number(nPort) + ".log";
        m_writefile[nPort - 1].setFileName(filename);
    }
    if(!m_writefile[nPort - 1].isOpen()) m_writefile[nPort - 1].open(QIODevice::Append);
    QString temp = (bsend?"Send:  ":"Recive:  ") + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    m_writefile[nPort - 1].write(temp.toLocal8Bit().data());
    m_writefile[nPort - 1].write("\r\n     ");
    m_writefile[nPort - 1].write(byte.toHex().toUpper());
    m_writefile[nPort - 1].write("\r\n");
    m_writefile[nPort - 1].write("\r\n");
    m_writefile[nPort - 1].flush();
    if(CGlobal::isDebug()) qDebug() << nPort << (bsend?"Send:":"Recive:") << byte.toHex() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
}

void CGlobal::writeDebug(QString strmsg)
{
    return;
    QString filename = QCoreApplication::applicationDirPath() + "/" + QDateTime::currentDateTime().toString("yyyy_MM_dd") + "_DLL.log";
    QFile writefile(filename);
    if(!writefile.isOpen()) writefile.open(QIODevice::Append);
    QString temp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    writefile.write(temp.toLocal8Bit().data());
    writefile.write("\r\n     ");
    writefile.write(strmsg.toLocal8Bit().data());
    writefile.write("\r\n");
    writefile.write("\r\n");
    writefile.flush();
}

QString CGlobal::getUidString(const QByteArray &buff,  int n)
{
    QString strResult = "";
    quint64 lResult = 0;
    int nsize = buff.size()<n ? buff.size() : n;
    int ix;
    if (nsize == 0)
        return "";
    if (nsize > 8)
        nsize = 8;
//    for (ix = 0; ix < nsize; ix++)
//    {
//        quint8 tmp = (quint8) buff.at(ix);
//        lResult += ((quint64)tmp) << (8*(nsize-ix-1));
//    }
    for (ix = 0; ix < nsize; ix++)
        {
            quint8 tmp = (quint8)buff.at(ix);
            lResult += ((quint64)tmp) << (8 * ix);
        }
    strResult = QString("%1").arg(lResult);
    if (strResult.size() > 15)
        return strResult.right(15);
    return strResult;

}
