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
//    createCommunication(CT_PrinterCard);
    createCommunication(CT_CanCard2);

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
    connect(this, SIGNAL(sendLedData(QString,int,QByteArray)), m_communicationManager,
            SLOT(sendLedData(QString,int,Qt::DirectConnection)));
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
    if(type == CT_NoCard)
        communicationName = "Client";
    else
    {
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
    }
    return communicationName;
}

void CCommunicationRoute::openCommunication()
{
    if(CGlobal::isDebug()) qDebug() << "CCommunicationRoute::openCommunication()" << m_communication.size();
    for(int i = 0; i < m_communication.size(); ++i) /*灯 电源 can1 can2*/
    {
        m_communicationManager->open(m_communication.at(i));
        QHash<int, int>::const_iterator iter = m_interfaceBoardPortSpeed.constBegin();
        while(iter != m_interfaceBoardPortSpeed.constEnd())
        {
            int port = iter.key();
            int speed = iter.value();
 //           setPortSpeed(m_communication.at(i), port, speed);
            ++iter;
        }
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

bool CCommunicationRoute::setPortSpeedValue(const QString linename, const int port, const int speed)
{
    Q_UNUSED(linename);
    if(port <= 0 || port > 7)
        return false;
    if(speed <= 0)
        return false;
    if(!m_interfaceBoardPortSpeed.contains(port))
        return false;    
    m_interfaceBoardPortSpeed[port] = speed;
    return true;
}

int CCommunicationRoute::getPortSpeedValue(const QString linename, const int port) const
{
    Q_UNUSED(linename);
    int speed = 0;
    if(m_interfaceBoardPortSpeed.contains(port))
    {
        if(port > 0 && port <= 7)
        {
            speed = m_interfaceBoardPortSpeed.value(port, 0);
        }
    }
    return speed;
}

void CCommunicationRoute::setFasConfig(const int port, const int commpnyid, const int npanel)
{
    QHash<QString, QVariant> control;
    if(port < 5 || port > 7) return;
    control.insert("type", QVariant(SetFasConfig));
    control.insert("count", QVariant(1));
    QByteArray data;
    data.append(port);
    data.append(commpnyid);
    data.append(npanel);
    QString linename = getCommunicationName(CT_FASCard);
    if(linename.isEmpty())
        return;
    if(communicationIsActive(linename))
        emit sendCommandData(linename, SetFasConfig, control, data);
}

void CCommunicationRoute::setPortSpeed(const QString linename, const int port, const int speed)
{
    if(linename.isEmpty())
        return;
    if(!setPortSpeedValue(linename, port, speed))
        return;
    QHash<QString, QVariant> control;
    control.insert("type", QVariant(SetCardSpeed));
    control.insert("count", QVariant(1));
    QByteArray data;
    data.resize(5);
    data[0] = port;
    data[1] = (speed>>0)&0xFF;
    data[2] = (speed>>8)&0xFF;
    data[3] = (speed>>16)&0xFF;
    data[4] = (speed>>24)&0xFF;
    if(communicationIsActive(linename))
        emit sendCommandData(linename, SetCardSpeed, control, data);
    if(linename == getCommunicationName(CT_FASCard))
    {
        setFasConfig(port, m_interfaceInfo[INTERFACEBOARD_COMMPNYCODE], m_interfaceInfo[INTERFACEBOARD_PANEL]);
    }
}

int CCommunicationRoute::getPortSpeed(const QString linename, const int port) const
{
    return getPortSpeedValue(linename, port);
}

void CCommunicationRoute::setLedStatus(const int ledStatus1, const int ledStatus2, const int ledStatus3)
{
    QString communication = getCommunicationName(CT_LedCard);
    if(communication.isEmpty())
        return;
    QHash<QString, QVariant> control;
    control.insert("type", QVariant(SetLedStatus));
    control.insert("count", QVariant(1));
    QByteArray data;
    data.resize(3);
    data[0] = ledStatus1;
    data[1] = ledStatus2;
    data[2] = ledStatus3;
    if(communicationIsActive(communication))
        emit sendLedData(communication, SetLedStatus, data);
}

void CCommunicationRoute::getInputPortStatus()
{
    QString communication = getCommunicationName(CT_FASCard);
    if(communication.isEmpty())
        return;
    QHash<QString, QVariant> control;
    control.insert("type", QVariant(ReadInputPort));
    control.insert("count", QVariant(0));
    QByteArray data;
    if(communicationIsActive(communication))
        emit sendCommandData(communication, ReadInputPort, control, data);
}

void CCommunicationRoute::setOutputPortStatus(const bool enable)
{
    QString communication = getCommunicationName(CT_FASCard);
    if(communication.isEmpty())
        return;
    QHash<QString, QVariant> control;
    control.insert("type", QVariant(SetOutputPort));
    control.insert("count", QVariant(1));
    QByteArray data;
    data.resize(1);
    data[0] = enable?0x01:0x00;
    if(communicationIsActive(communication))
        emit sendCommandData(communication, SetOutputPort, control, data);
}

void CCommunicationRoute::setPowerStatus(const bool enable)
{
    QString communication = getCommunicationName(CT_PowerCard);
    if(communication.isEmpty())
        return;
    QHash<QString, QVariant> control;
    control.insert("type", QVariant(SetPowerStatus));
    control.insert("count", QVariant(1));
    QByteArray data;
    data.resize(1);
    data[0] = enable?0x00:0xFF;
    if(communicationIsActive(communication))
        emit sendCommandData(communication, SetOutputPort, control, data);
}

#if 0
void CCommunicationRoute::printData(const QByteArray &data)
{
    QString communication = getCommunicationName(CT_LedCard);
    if(communication.isEmpty())
        return;
    QHash<QString, QVariant> control;
    control.insert("type", QVariant(PrintData));
    control.insert("count", QVariant(1));
    QByteArray byte;
    byte.resize(2);
    byte[0] = data.size()%256;
    byte[1] = data.size()/256;
    byte.append(data);
    if(communicationIsActive(communication))
        emit sendCommandData(communication, PrintData, control, byte);
}
#endif

void CCommunicationRoute::printData(const QByteArray &data)
{
    QString communication = getCommunicationName(CT_PrinterCard);
    if(communication.isEmpty())
        return;
    QHash<QString, QVariant> control;
    control.insert("type", QVariant(PrintData));
    control.insert("count", QVariant(1));
    QByteArray byte;
    byte.resize(0);
//    byte[0] = data.size()%256;
//    byte[1] = data.size()/256;
    byte.append(data);
    if(communicationIsActive(communication))
        emit sendCommandData(communication, PrintData, control, byte);
}


void CCommunicationRoute::reset(bool binit)
{
    resetPower();
    resetLed(binit);
    resetFas();
}

void CCommunicationRoute::resetPower()
{
    QString communication = getCommunicationName(CT_PowerCard);
    if(communication.isEmpty())
        return;
    QHash<QString, QVariant> control;
    control.insert("type", QVariant(Reset));
    control.insert("count", QVariant(0));
    QByteArray data;
    if(communicationIsActive(communication))
        emit sendCommandData(communication, Reset, control, data);
}

void CCommunicationRoute::resetLed(bool binit)
{
    QString communication = getCommunicationName(CT_LedCard);
    if(communication.isEmpty())
        return;
    QHash<QString, QVariant> control;
    control.insert("type", QVariant(Reset));
    control.insert("count", QVariant(0));
    QByteArray data;
    if(communicationIsActive(communication))
        emit sendCommandData(communication, Reset, control, data);
    if(binit) m_currentLedStatus.clear();
}

void CCommunicationRoute::resetFas()
{
    QString communication = getCommunicationName(CT_FASCard);
    if(communication.isEmpty())
        return;
    QHash<QString, QVariant> control;
    control.insert("type", QVariant(Reset));
    control.insert("count", QVariant(0));
    QByteArray data;
    if(communicationIsActive(communication))
        emit sendCommandData(communication, Reset, control, data);
}

void CCommunicationRoute::emergency(bool enable)
{
    QString communication = getCommunicationName(CT_PowerCard);
    if(communication.isEmpty())
        return;
    QHash<QString, QVariant> control;
    control.insert("type", QVariant(Emergency));
    control.insert("count", QVariant(1));
    QByteArray data;
    data.resize(1);
    data[0] = enable?0x01:0x00;
    if(communicationIsActive(communication))
        emit sendCommandData(communication, Emergency, control, data);
}

////get line name from line type
QString CCommunicationRoute::getCommunicationName(const int type)
{
    QString name;
    switch(type)
    {
    case CT_PowerCard:
        name = "powercard";
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
    case CT_PrinterCard:
        name = "printercard";
        break;
    default:
        name = "";
        break;
    }
    return name;
}

int CCommunicationRoute::getCommunicationType(const QString lineName)
{
    if(lineName.toLower() == "powercard") return CT_PowerCard;
    else if(lineName.toLower() == "ledcard") return CT_LedCard;
    else if(lineName.toLower() == "cancard1") return CT_CanCard1;
    else if(lineName.toLower() == "cancard2") return CT_CanCard2;
    else if(lineName.toLower() == "fascard") return CT_FASCard;
    else return CT_NoCard;
}

////line type is true line
bool CCommunicationRoute::isCommunicationLine(const int type) const
{
    if(type == CT_LedCard || type == CT_CanCard1 || type == CT_CanCard2 || type == CT_PowerCard || type == CT_FASCard)
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
//        char recvAdd = (char)(control["recvAddr"].toInt()&0x000000ff);
        foreach(QString linename, list){
            if(linename.isEmpty()) continue;
            if(communicationIsActive(linename))
            {
                //广播命令、cancard1对应CAN1/2、cancard2对应CAN3/4
//                if(recvAdd ==0 || (linename == "cancard1" && (recvAdd == 0x03 || recvAdd == 0x04))
//                        || (linename == "cancard2" && (recvAdd == 0x05 || recvAdd == 0x06)))
//                {
                    emit sendCommandData(linename, infoType, control, data);
//                }
            }
        }
    }
}

int CCommunicationRoute::getIntFromByte(QByteArray data, int start, int len)
{
    if(data.size() < start+len) return 0;
    return (*(unsigned int*)data.mid(start,len).data())&0x0000ffff;;
}

///process led , button, printer, power, inputport, outtputport etc event
void CCommunicationRoute::procInterfaceBoardRecvEvent(const QString& linename, const int &infotype, const QByteArray &data)
{
    switch(infotype)
    {
        case UploadTest:
            break;
        case ReadCardSpeed:
        case UploadCardSpeed:
        {
            int port = getIntFromByte(data, 0, 1);
            int speed = getIntFromByte(data, 1, 4);
            setPortSpeedValue(linename, port, speed);
            break;
        }
        case UploadCanState:
            //set can bus status
            break;
        case ReadFasConfig:
            //send fas config info to interfaceborad
            break;
        case SendFasFire:
        {
            break;
        }
        case ReadInputPort:
        case UploadInputPort:
        {
            break;
        }
        case FasStatus:
        {
            break;
        }
        case UploadPowerState:
            //deal the power state
        {
            break;
        }
        case PrintData:
        case UploadPrintState:
        {
            int remainBufferSize = getIntFromByte(data, 0, 1);
            CGlobal::instance()->print()->setPrintBusy(remainBufferSize==0?false:true);
            break;
        }
        case UploadKeyInfo:
        {
            int keyId = getIntFromByte(data, 0, 1);
            int keyStatus = getIntFromByte(data, 1, 1);
            if(keyStatus == 1 && !CGlobal::instance()->processController()->isSelfCheck())
            {
                CMsgNull msgNull;
                switch(keyId)
                {
                    case KeyEmergency:
                    {
                        QByteArray byte = msgNull.data(NCT_ForceEmergency);
                        CGlobal::instance()->DealEvent(NCT_ForceEmergency, byte);
                        break;
                    }
                    case KeyAuto:
                    {
                        QByteArray byte = msgNull.data(NCT_AutomaticEvacuation);
                        CGlobal::instance()->DealEvent(NCT_AutomaticEvacuation, byte);
                        break;
                    }
                    case KeyManual:
                    {
                        QByteArray byte = msgNull.data(NCT_ManualEvacuation);
                        CGlobal::instance()->DealEvent(NCT_ManualEvacuation, byte);
                        break;
                    }
                    case KeySelfCheck:
                    {
                        CGlobal::instance()->processController()->selfCheck();
                        break;
                    }
                    case KeyMute:
                    {
                        QByteArray byte = msgNull.data(NCT_Mute);
                        CGlobal::instance()->DealEvent(NCT_Mute, byte);
                        break;
                    }
                    case KeyReset:
                    {
                        QByteArray byte = msgNull.data(NCT_Reset);
                        CGlobal::instance()->DealEvent(NCT_Reset, byte);
                        break;
                    }
                }
            }
            break;
        }
    }
}

////send data to process
void CCommunicationRoute::procCommunicationRecvEvent(const QString &linename, const int &infoType, const QHash<QString, QVariant> &controlDomain, const QByteArray &data)
{
    if(CGlobal::isDebug()) qDebug() << "CCommunicationRoute::procCommunicationRecvEvent=====" << linename << infoType << data.toHex();
    if(m_communication.contains(linename))
    {
        QList<QString> processList = m_processCommunication.keys(linename);
        if(controlDomain.contains(INTERFACEBOARD_PORT_NUMBER_STR))
        {
            int port = controlDomain.value(INTERFACEBOARD_PORT_NUMBER_STR).toInt();
            if(port == 0)
            {
                procInterfaceBoardRecvEvent(linename, infoType, data);
                return;
            }
            for(int i = processList.size() - 1; i >= 0; --i)
            {
                CProcessBase* process = m_process.value(processList.at(i));
                if(!process->hasCardPort(port))
                    processList.removeAt(i);
            }
        }
        for(int i = 0; i < processList.size(); ++i)
            emit processRecvEvent(processList.at(i), infoType, controlDomain, data);
    }
}
