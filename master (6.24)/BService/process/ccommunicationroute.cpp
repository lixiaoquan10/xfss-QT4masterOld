#include <QDebug>
#include <QVector>
#include <QTextCodec>
#include "struct/cmsgnull.h"
#include "print/cprint.h"
#include "cprocessbase.h"
#include "cprocesscontroller.h"
#include "ccommunicationroute.h"
#include "ccommunicationmanager.h"
#include "cglobal.h"

CCommunicationRoute::CCommunicationRoute(QObject *parent) :
    QObject(parent), m_communicationManager(NULL)
{
    initCommunicationManager();
    createCommunication(CT_LedCard);
    createCommunication(CT_LinkageCard);
    createCommunication(CT_TestCard);
}

CCommunicationRoute::~CCommunicationRoute()
{
    delete m_communicationManager;
}

void CCommunicationRoute::initCommunicationManager()
{
    if(!m_communicationManager)
        m_communicationManager = new CCommunicationManager();
    connect(m_communicationManager, SIGNAL(sig_ReciveData(QString,int,QHash<QString,QVariant>,QByteArray)), this, SLOT(procCommunicationRecvEvent(QString,int,QHash<QString,QVariant>,QByteArray)));
    connect(this, SIGNAL(sendCommandData(QString,int,QHash<QString,QVariant>,QByteArray)), m_communicationManager, SLOT(sendCommandData(QString,int,QHash<QString,QVariant>,QByteArray)));
//    connect(this, SIGNAL(sendLedData(QString,int,QByteArray)), m_communicationManager, SLOT(sendLedData(QString,int,Qt::DirectConnection)));
//    connect(this, SIGNAL(sendLinkageData(QString,int,QByteArray)), m_communicationManager, SLOT(sendLinkageData(QString,int,Qt::DirectConnection)));
}

bool CCommunicationRoute::createRoute(CProcessBase *process, const int type, const QHash<QString, QVariant> &control)
{
    QString processName = process->name();
    QString communication = createCommunication(type);
    if(communication.isEmpty())
        return false;
    if(isCommunicationLine(type))
    {
        int port = control.value(INTERFACEBOARD_PORT_NUMBER_STR, QVariant(0)).toInt();
        if(!process->addCardPort(port))
            return false;
        m_communicationManager->setTimeOut(communication, INTERFACEBOARD_TIMEOUT);
        int speed = control.value(INTERFACEBOARD_PORT_SPEED_STR, QVariant(0)).toInt();
        m_interfaceBoardPortSpeed.insert(port, speed);
        if(type == CT_FASCard){
            m_interfaceInfo.insert(INTERFACEBOARD_COMMPNYCODE, control.value(INTERFACEBOARD_COMMPNYCODE, QVariant(0)).toInt());
            m_interfaceInfo.insert(INTERFACEBOARD_PANEL, control.value(INTERFACEBOARD_PANEL, QVariant(0)).toInt());
        }
    }
    if(!m_process.contains(processName)){
        m_process.insert(processName, process);
        m_processCommunication.insert(processName, communication);
        connect(process, SIGNAL(sendEvent(QString,int,QHash<QString,QVariant>,QByteArray)), this, SLOT(procProcessSendEvent(QString,int,QHash<QString,QVariant>,QByteArray)));
        connect(this, SIGNAL(processRecvEvent(QString,int,QHash<QString,QVariant>,QByteArray)), process, SLOT(recvEvent(QString,int,QHash<QString,QVariant>,QByteArray)));
    } else{
        if(!m_processCommunication.values(processName).contains(communication))
            m_processCommunication.insertMulti(processName, communication);
    }
    return true;
}

////create communication line
QString CCommunicationRoute::createCommunication(const int type)
{
    QString communicationName;
    communicationName = getCommunicationName(type);
    if(!communicationName.isEmpty() && m_communicationManager)
    {
        if(!m_communication.contains(communicationName))
        {
            if(!m_communicationManager->createCommunicationLine(communicationName, type))
                return QString();
            m_communication.append(communicationName);
        }
    }
    return communicationName;
}

void CCommunicationRoute::openCommunication()
{
    if(CGlobal::isDebug()) qDebug() << "CCommunicationRoute::openCommunication()" << m_communication.size();
    for(int i = 0; i < m_communication.size(); ++i) /*灯 电源 can1 can2*/
    {
        m_communicationManager->open(m_communication.at(i));
    }
}

void CCommunicationRoute::closeCommunication()
{
    for(int i = 0; i < m_communication.size(); ++i)
    {
        m_communicationManager->close(m_communication.at(i));
    }
}

void CCommunicationRoute::setDebug(const bool enable)
{
    m_communicationManager->setDebugOut(enable);
}



void CCommunicationRoute::setLedStatus(const int ledStatus1, const int ledStatus2, const int ledStatus3)
{
    QString communicationLed = getCommunicationName(CT_LedCard);
    if(communicationLed.isEmpty())
        return;
    QByteArray data;
    data.resize(3);
    data[0] = ledStatus1;
    data[1] = ledStatus2;
    data[2] = ledStatus3;
    if(communicationIsActive(communicationLed))
    {
        m_communicationManager->sendLedData(communicationLed, SetLedStatus, data);
    }
    if(!m_communicationManager->isSerialportNameSeted)
    {
        QString communicationLinkage = getCommunicationName(CT_LinkageCard);
        if(communicationLinkage.isEmpty())
            return;
        if(communicationIsActive(communicationLinkage))
        {
            m_communicationManager->sendLinkageData(communicationLinkage, SetLedStatus, data);
        }
    }
}

void CCommunicationRoute::testLinkageCom()
{
//    m_communicationManager->setTestLinkageCom(true);
    QByteArray data;
    data.resize(3);
    data[0] = 0xFF;
    data[1] = 0xFF;
    data[2] = 0xFF;
    QString communicationLinkage = getCommunicationName(CT_LinkageCard);
    if(communicationLinkage.isEmpty())
        return;
    if(communicationIsActive(communicationLinkage))
    {
        m_communicationManager->sendLinkageData(communicationLinkage, TestCom, data);
    }
}



//void CCommunicationRoute::printData(const QByteArray &data)
//{
//    QString communication = getCommunicationName(CT_PrinterCard);
//    if(communication.isEmpty())
//        return;
//    QHash<QString, QVariant> control;
//    control.insert("type", QVariant(PrintData));
//    control.insert("count", QVariant(1));
//    QByteArray byte;
//    byte.resize(0);
//    byte.append(data);
//    if(communicationIsActive(communication))
//    {
//        emit sendCommandData(communication, PrintData, control, byte);
//    }
//}


//void CCommunicationRoute::reset(bool binit)
//{
//    resetLed(binit);
//}

//void CCommunicationRoute::resetLed(bool binit)
//{
//    QString communication = getCommunicationName(CT_LedCard);
//    if(communication.isEmpty())
//        return;
//    QHash<QString, QVariant> control;
//    control.insert("type", QVariant(Reset));
//    control.insert("count", QVariant(0));
//    QByteArray data;
//    if(communicationIsActive(communication))
//        emit sendCommandData(communication, Reset, control, data);
//    if(binit) m_currentLedStatus.clear();
//}



////get line name from line type
QString CCommunicationRoute::getCommunicationName(const int type)
{
    QString name;
    switch(type)
    {
    case CT_LinkageCard:
        name = "linkagecard";
        break;
    case CT_LedCard:
        name = "ledcard";
        break;
    case CT_CanCard1:
        name = "cancard1";
        break;
    case CT_CanCard2:
        name = "cancard2";
        break;
    case CT_FASCard:
        name = "fascard";
        break;
    case CT_TestCard:
        name = "testcard";
        break;
    default:
        name = "Client";
        break;
    }
    return name;
}

int CCommunicationRoute::getCommunicationType(const QString lineName)
{
    if(lineName.toLower() == "linkagecard") return CT_LinkageCard;
    else if(lineName.toLower() == "ledcard") return CT_LedCard;
    else if(lineName.toLower() == "cancard1") return CT_CanCard1;
    else if(lineName.toLower() == "cancard2") return CT_CanCard2;
    else if(lineName.toLower() == "testcard") return CT_TestCard;
    else if(lineName.toLower() == "fascard") return CT_FASCard;
    else return CT_NoCard;
}

////line type is true line
bool CCommunicationRoute::isCommunicationLine(const int type) const
{
    if(type == CT_CanCard1 || type == CT_CanCard2 || type == CT_LedCard || type == CT_LinkageCard
           || type == CT_TestCard || type == CT_FASCard)
        return true;
    else
        return false;
}

bool CCommunicationRoute::communicationIsActive(const QString &name) const
{
    return m_communicationManager->isActive(name);
}

////deal the process send event
void CCommunicationRoute::procProcessSendEvent(const QString &processname, const int &infoType, const QHash<QString, QVariant> &control, const QByteArray &data)
{
    if(m_processCommunication.contains(processname))
    {
        QList<QString> list = m_processCommunication.values(processname);
        int canportAdd = control["canportAdd"].toInt();
        foreach(QString linename, list){
            if(linename.isEmpty()) continue;
            if(communicationIsActive(linename))
            {
                //cancard1:3/4/7/8   cancard2:5/6/9/10         canportAdd == 0广播
                if(!canportAdd)
                {
//                    if(linename == "cancard1")
//                        CGlobal::instance()->m_canport1ID = canportAdd;
//                    else if(linename == "cancard2")
//                        CGlobal::instance()->m_canport2ID = canportAdd;
                    emit sendCommandData(linename, infoType, control, data);
                }
                else
                {
                    if(canNameByAdd(canportAdd) == linename)
                    {
//                        if(linename == "cancard1")
//                            CGlobal::instance()->m_canport1ID = canportAdd;
//                        else if(linename == "cancard2")
//                            CGlobal::instance()->m_canport2ID = canportAdd;
                        emit sendCommandData(linename, infoType, control, data);
                    }
                }
            }
        }
    }
}

QString CCommunicationRoute::canNameByAdd(int canportAdd)
{
    if(canportAdd == 3 || canportAdd == 4 || canportAdd == 7 || canportAdd == 8)
        return "cancard1";
    else if(canportAdd == 5 || canportAdd == 6 || canportAdd == 9 || canportAdd == 10)
        return "cancard2";
    return "";
}

int CCommunicationRoute::getIntFromByte(QByteArray data, int start, int len)
{
    if(data.size() < start+len) return 0;
    return (*(unsigned int*)data.mid(start,len).data())&0x0000ffff;;
}

////send data to process
void CCommunicationRoute::procCommunicationRecvEvent(const QString &linename, const int &infoType, const QHash<QString, QVariant> &controlDomain, const QByteArray &data)
{
    if(CGlobal::isDebug()) qDebug() << "CCommunicationRoute::procCommunicationRecvEvent=====" << linename << infoType << data.toHex();
    if(m_communication.contains(linename))
    {
        QList<QString> processList = m_processCommunication.keys(linename);
        for(int i = 0; i < processList.size(); ++i)
            emit processRecvEvent(processList.at(i), infoType, controlDomain, data);
    }
}
