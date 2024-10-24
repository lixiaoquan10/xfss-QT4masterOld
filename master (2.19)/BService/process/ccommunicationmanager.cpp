#include "ccommunicationmanager.h"
#include "packageanalyzer.h"
#include "cserialport.h"
#include "cprocesscontroller.h"
#include "cglobal.h"
#include <QDebug>
#include <QFile>
#define CANDATALEN 15

CCommunicationManager::CCommunicationManager(QObject *parent) :
    QObject(parent),m_isDebug(false)
{
    connect(&m_LedTimer, SIGNAL(timeout()), this, SLOT(handleSendLedData()));

    connect(&m_Can1Timer, SIGNAL(timeout()), this, SLOT(handleSendCan1Data()));
    connect(&m_Can2Timer, SIGNAL(timeout()), this, SLOT(handleSendCan2Data()));

    connect(&m_ledReciveTimer, SIGNAL(timeout()), this, SLOT(handleLedReciveData()));

    connect(&m_Can1ReciveTimer, SIGNAL(timeout()), this, SLOT(handleCan1ReciveData()));
    connect(&m_Can2ReciveTimer, SIGNAL(timeout()), this, SLOT(handleCan2ReciveData()));

    m_ledReciveTimer.start(30);

    m_Can1ReciveTimer.start(15);
    m_Can2ReciveTimer.start(15);

    m_LedTimer.start(10);

    m_Can1Timer.start(10);
    m_Can2Timer.start(10);

    led_reciveData = 0;
}

CCommunicationManager::~CCommunicationManager()
{
    foreach(QString lineName,m_hashCommunicationLine.keys()){
        m_hashCommunicationLine[lineName]->destroy();
        m_hashCommunicationLine.remove(lineName);
    }
}

int CCommunicationManager::getPortFromCommunicationType(int type)
{
    switch(type)
    {
    case CT_LedCard:
        return 6;
    case CT_CanCard1:
        return 1;
    case CT_CanCard2:
        return 0;
    case CT_PowerCard:
        return 5;
    case CT_NoCard:
        return 2;
    case CT_FASCard:
        return 4;
    case CT_PrinterCard:    /*com3 ttyS5 9600*/
        return 3;

    default:
        return 10;
    }
}

bool CCommunicationManager::createCommunicationLine(const QString &lineName, int type)
{
    if(m_hashCommunicationLine.contains(lineName)){
        return false;
    }
    CSerialPort *pSerialPort ;
    if (lineName == "ledcard" || lineName == "printercard")
        pSerialPort = new CSerialPort(0, 9600);
    else if (lineName == "cancard2")
        pSerialPort = new CSerialPort(0, 38400);
    else
        pSerialPort = new CSerialPort();
    pSerialPort->setName(lineName);
    connect(pSerialPort, SIGNAL(sig_ReciveData(const QString&,const QByteArray&)),
            this, SLOT(doReciveData(const QString&,const QByteArray&)));

    connect(this, SIGNAL(sig_ReceivedPacket(int)), pSerialPort, SLOT(handleConfirmationPacket(int)));

    pSerialPort->setDebugOut(m_isDebug);
    CCommunicationLine *communicationLine = new CCommunicationLine();
    communicationLine->pSerialPort = pSerialPort;
    communicationLine->nPort = getPortFromCommunicationType(type);
    m_hashCommunicationLine.insert(lineName,communicationLine);
    return true;
}

bool CCommunicationManager::removeCommunicationLine(const QString& lineName)
{
    if(m_hashCommunicationLine.contains(lineName)){
        m_hashCommunicationLine[lineName]->destroy();
        m_hashCommunicationLine.remove(lineName);
        return true;
    }else{
        return false;
    }
}

bool CCommunicationManager::open(const QString &lineName)
{
    if(m_hashCommunicationLine.contains(lineName)){
        CCommunicationLine* line = m_hashCommunicationLine[lineName];
        int result = line->pSerialPort->open(line->nPort);
        if(CGlobal::isDebug()) qDebug() << "CCommunicationManager::open, lineName is " << lineName << ",open result is " << result << ",port is " << line->nPort ;
        if(result == 1){
            return true;
        }else{
            return false;
        }

    }else{
        return false;
    }
}

bool CCommunicationManager::isActive(const QString &lineName) const
{
    if(m_hashCommunicationLine.contains(lineName)){
        return m_hashCommunicationLine[lineName]->pSerialPort->isActive();
    }else{
        return false;
    }
}

bool CCommunicationManager::close(const QString &lineName)
{
    if(m_hashCommunicationLine.contains(lineName)){
        int result = m_hashCommunicationLine[lineName]->pSerialPort->close();
        if(result == 1){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

void CCommunicationManager::doReciveData(const QString &lineName,const QByteArray& data)
{
    if(m_hashCommunicationLine.contains(lineName)){
        m_hashCommunicationLine[lineName]->reciveData.append(data);
    }
}

bool CCommunicationManager::setTimeOut(const QString& lineName, int timeOut)
{
    if(m_hashCommunicationLine.contains(lineName)){
        m_hashCommunicationLine[lineName]->pSerialPort->setTimeOut(timeOut);
        return false;
    }else{
        return false;
    }
}

int CCommunicationManager::getTimeOut(const QString& lineName) const
{
    if(m_hashCommunicationLine.contains(lineName)){
        return m_hashCommunicationLine[lineName]->pSerialPort->getTimeOut();
    }else{
        return -1;
    }
}

void CCommunicationManager::setDebugOut(bool isDebug)
{
    m_isDebug=isDebug;
    foreach(CCommunicationLine* lineObj, m_hashCommunicationLine){
        CSerialPort* pSerialPort = lineObj->pSerialPort;
        if(pSerialPort){
            pSerialPort->setDebugOut(m_isDebug);
        }
    }
}

bool CCommunicationManager::sendData(QString lineName, int infoType, const QByteArray &data)
{
    if(m_hashCommunicationLine.contains(lineName)){
        CSerialPort* pSerialPort = m_hashCommunicationLine[lineName]->pSerialPort;
        if(pSerialPort){
            if(m_hashCommunicationLine[lineName]->sendDataTableAsType.contains(infoType)){
                m_hashCommunicationLine[lineName]->sendDataTableAsType[infoType].append(data);
            }else{
                m_hashCommunicationLine[lineName]->sendDataTableAsType[infoType].clear();
                m_hashCommunicationLine[lineName]->sendDataTableAsType[infoType].append(data);
            }
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

/*
================
slot CCommunicationManager::updataRecentSerialNumber
================
*/
void CCommunicationManager::updataRecentSerialNumber(const QString &name, int nSerialNumber)
{
    if(m_hashCommunicationLine.contains(name)){
        m_hashCommunicationLine[name]->recentRespondSerialNumber = nSerialNumber;
    }
}

bool CCommunicationManager::sendCommandData(const QString &lineName, const int &infotype, const QHash<QString, QVariant> &controlDomain, const QByteArray &data)
{
    qDebug() << "CCommunicationManager::sendCommandData" << lineName << infotype << data.toHex();
    QString  data1 = "sendCommandData:  " + lineName + "   " + QString::number(infotype)  + " " + data.toHex() + " " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    QFile file("/home/xfss/root/sendCommandData.txt");

    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << data1 << '\n' << '\n';
        file.close();
    }
    if(infotype == SendCanData){
        if(
            controlDomain.contains("recvAddr") && controlDomain.contains("sendAddr")
                && controlDomain.contains("commandType") && controlDomain.contains("dataLen")
                && (controlDomain["dataLen"].toInt()&0x000000ff) == data.size())
        {
            int frameCount=0;
            int multiple=0;
            int remainder=0;
            int len = controlDomain["dataLen"].toInt()&0x000000ff;
            QByteArray tmpByteArray;
            QByteArray headByteArray;
            QList<QByteArray> listsend;
            headByteArray.resize(6);

            headByteArray[0] = (char)(controlDomain["recvAddr"].toInt()&0x000000ff);
            headByteArray[1] = (char)(controlDomain["sendAddr"].toInt()&0x000000ff);
            if (controlDomain.contains("loopAddr"))
                headByteArray[2] = (char)(controlDomain["loopAddr"].toInt()&0x000000ff);      // loop addr
            else
                headByteArray[2] = 0;
            if (controlDomain.contains("deviceAddr"))
                headByteArray[3] = (char)(controlDomain["deviceAddr"].toInt()&0x000000ff);       // terminate addr
            else
                headByteArray[3] = 0;

            if(len>8){
                multiple = len/8;
                remainder = len%8;

                if(remainder == 0){
                    frameCount = multiple;
                }else{
                    frameCount = multiple+1;
                }
                if(frameCount>31){
                    return false;
                }

                for(int i=0, maxCount=frameCount-1; i<maxCount; ++i){
                    tmpByteArray.clear();
                    headByteArray[5]=(char)((i+1)&0x000000ff);
                    headByteArray[6]=(char)(8&0x000000ff);
                    tmpByteArray.append(headByteArray);
                    tmpByteArray.append(data.mid(i*8,8));
                    listsend.append(tmpByteArray);
                }

                if(remainder>0){
                    tmpByteArray.clear();
                    headByteArray[5] = (char)(31&0x000000ff);
                    headByteArray[6] = (char)(remainder&0x000000ff);
                    tmpByteArray.append(headByteArray);
                    tmpByteArray.append(data.mid((frameCount-1)*8,remainder));
                    listsend.append(tmpByteArray);
                }else{
                    tmpByteArray.clear();
                    headByteArray[5]=(char)(31&0x000000ff);
                    headByteArray[6]=(char)(8&0x000000ff);
                    tmpByteArray.append(headByteArray);
                    tmpByteArray.append(data.mid((frameCount-1)*8, 8));
                    listsend.append(tmpByteArray);
                }
            }else{
                frameCount=1;
                tmpByteArray.clear();
                headByteArray[4]=(char)(0&0x000000ff);
                headByteArray[5]=(char)(len&0x000000ff);
                tmpByteArray.append(headByteArray);
                tmpByteArray.append(data.mid(0,len));
                listsend.append(tmpByteArray);
            }
            bool result = true;
            for(int i=0; i<listsend.count(); ++i){
                result = sendData(lineName, 0x11, listsend.at(i));
                if(!result) return false;
            }
        } else {
            return false;
        }
    } else {
        return sendData(lineName, infotype, data);
    }
    return true;
}

bool CCommunicationManager::sendLedData(const QString &lineName, const int &infotype, const QByteArray &data)
{
    return sendData(lineName, infotype, data);
}


void CCommunicationManager::handleSendLedData()
{
    QString linename = "ledcard";
    if(!m_hashCommunicationLine.contains(linename)) return;
    CSerialPort* pSerialPort = m_hashCommunicationLine[linename]->pSerialPort;

    if(pSerialPort){
        foreach (int infoType, m_hashCommunicationLine[linename]->sendDataTableAsType.keys()){

            if (infoType != SetLedStatus)
                continue;

            QList<QByteArray>& sendDataList = m_hashCommunicationLine[linename]->sendDataTableAsType[infoType];
            if(sendDataList.count() > 0){

                QByteArray tmpArray(sendDataList[0]);
                sendDataList.removeAt(0);

                if (tmpArray.size() < 2) /* LED status1, LED status2 */
                    continue;
                char sum = 0;
                QByteArray byteArray;
                byteArray.append(char(0x55));
                byteArray.append(char(0x13));
                byteArray.append(tmpArray.at(0));
                byteArray.append(char(0));
                byteArray.append(char(0));
                byteArray.append(char(0));
                byteArray.append(tmpArray.at(1));
                byteArray.append(tmpArray.at(2));
                byteArray.append(char(0));
                byteArray.append(char(0));
                byteArray.append(char(0));
                for (int ix = 0; ix < byteArray.size(); ix++)
                    sum += byteArray.at(ix);
                byteArray.append(sum);

                if(CGlobal::isDebug()) qDebug() << "CCommunicationManager::handleSendLEDData():port is " << linename << ",info type is "
                                                << infoType << byteArray.toHex() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
                pSerialPort->sendData(byteArray);
            }
            quint64 tick = ::getTickCount();
            while(::getTickCount() - tick < 1){}

        }
    }
}


void CCommunicationManager::handleSendCan1Data()
{
    QString linename = "cancard1";
    if(!m_hashCommunicationLine.contains(linename)) return;
    CSerialPort* pSerialPort = m_hashCommunicationLine[linename]->pSerialPort;
    PackageTemplate &packetTemplate = m_hashCommunicationLine[linename]->sendPacketTemplate;
    int nSerialNumber=0;
    if(pSerialPort){
        foreach (int infoType, m_hashCommunicationLine[linename]->sendDataTableAsType.keys()){
            QList<QByteArray>& sendDataList = m_hashCommunicationLine[linename]->sendDataTableAsType[infoType];
            packetTemplate.initializationPackage();
            if(sendDataList.count() > 0){
                packetTemplate.setInfoType(infoType);
                packetTemplate.setCanId(CGlobal::instance()->m_canport1ID);

                if(sendDataList[0].size()>0){
                    packetTemplate.appendInfo(sendDataList[0]);
                    sendDataList.removeAt(0);
                }
                const QByteArray& byteArray = packetTemplate.buildBytes(&nSerialNumber);
                if(CGlobal::isDebug()) qDebug() << "CCommunicationManager::handleSendCan1Data():port is " << nSerialNumber << ",info type is " << infoType << byteArray.toHex();
                pSerialPort->sendData(byteArray);
                QString  data = "cancard1SendData:  " + byteArray.toHex() + "   " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
                QFile file("/home/xfss/root/cancard1SendData.txt");

                if (file.open(QIODevice::Append | QIODevice::Text))
                {
                    QTextStream stream(&file);
                    stream << data << '\n' << '\n';
                    file.close();
                }
            }
            quint64 tick = ::getTickCount();
            while(::getTickCount() - tick < 1){}
        }

    }
}
void CCommunicationManager::handleSendCan2Data()
{
    QString linename = "cancard2";
    if(!m_hashCommunicationLine.contains(linename)) return;
    CSerialPort* pSerialPort = m_hashCommunicationLine[linename]->pSerialPort;
    PackageTemplate &packetTemplate = m_hashCommunicationLine[linename]->sendPacketTemplate;
    int nSerialNumber=0;
    if(pSerialPort){
        foreach (int infoType, m_hashCommunicationLine[linename]->sendDataTableAsType.keys()){
            QList<QByteArray>& sendDataList = m_hashCommunicationLine[linename]->sendDataTableAsType[infoType];
            packetTemplate.initializationPackage();
            if(sendDataList.count() > 0){
                packetTemplate.setInfoType(infoType);
                packetTemplate.setCanId(CGlobal::instance()->m_canport2ID);

                if(sendDataList[0].size()>0){
                    packetTemplate.appendInfo(sendDataList[0]);
                    sendDataList.removeAt(0);
                }
                const QByteArray& byteArray = packetTemplate.buildBytes(&nSerialNumber);
                if(CGlobal::isDebug()) qDebug() << "CCommunicationManager::handleSendCan1Data():port is " << nSerialNumber << ",info type is " << infoType << byteArray.toHex();
                pSerialPort->sendData(byteArray);
                QString  data = "cancard2SendData:  " + byteArray.toHex() + "   " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
                QFile file("/home/xfss/root/cancard2SendData.txt");

                if (file.open(QIODevice::Append | QIODevice::Text))
                {
                    QTextStream stream(&file);
                    stream << data << '\n' << '\n';
                    file.close();
                }
            }
            quint64 tick = ::getTickCount();
            while(::getTickCount() - tick < 1){}
        }

    }
}



/*
================
timer slot CCommunicationManager::handleReciveData
================
*/
void CCommunicationManager::handleLedReciveData()
{
    CSerialPort* pSerialPort = m_hashCommunicationLine["ledcard"]->pSerialPort;
    QByteArray &reciveData = m_hashCommunicationLine["ledcard"]->reciveData;
      if (reciveData.isEmpty())
        return;
      if(pSerialPort){
              if (reciveData.size() < 47 || reciveData.at(0) != 0x55) {
                  reciveData.clear();
                  led_reciveData = reciveData;
                  return;
              }
              led_reciveData.append(reciveData);
              char sum = 0;
              for (int ix = 0; ix < (47-1); ix++)
                  sum += led_reciveData.at(ix);
              if (led_reciveData.at(46) != sum) {
                  led_reciveData.clear();
                  reciveData.clear();
                  return;
              }
              if(CGlobal::isDebug()) qDebug() << "led Recive:" << led_reciveData.toHex() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
              QByteArray tmpByteArray;
              tmpByteArray.append(led_reciveData.at(2));
              tmpByteArray.append(led_reciveData.at(3));
              tmpByteArray.append(led_reciveData.at(4));
              tmpByteArray.append(led_reciveData.at(5));
              tmpByteArray.append(led_reciveData.at(6));
              tmpByteArray.append(led_reciveData.at(7));
              tmpByteArray.append(led_reciveData.at(8));
              tmpByteArray.append(led_reciveData.at(9));
              tmpByteArray.append(led_reciveData.at(10));
              tmpByteArray.append(led_reciveData.at(11));
              tmpByteArray.append(led_reciveData.at(12));
              tmpByteArray.append(led_reciveData.at(13));
              led_reciveData.clear();
              reciveData.clear();
              QHash<QString, QVariant> controlDomain;
              /*??????????????????????*/
  //                emit sig_ReciveData(pSerialPort->getName(), 0, controlDomain, tmpByteArray);
              if(CGlobal::isDebug()) qDebug() << "ledcardReciveData" << tmpByteArray.toHex() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
              CGlobal::instance()->processController()->procRecvEvent(0, controlDomain, tmpByteArray);

              return;
      }
}

/*

*/
void CCommunicationManager::handleCan1ReciveData()
{
    CSerialPort* pSerialPort = m_hashCommunicationLine["cancard1"]->pSerialPort;
    QByteArray &reciveData = m_hashCommunicationLine["cancard1"]->reciveData;

    if (reciveData.isEmpty())
        return;
    QString  data = "cancard1ReciveData:  " + reciveData.toHex() + "   " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    QFile file("/home/xfss/root/receiveCan1data.txt");

    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << data << '\n' << '\n';
        file.close();
    }
    if(pSerialPort){
        static PackageAnalyzer packAnalyzer;
        PackageTemplate *pack = packAnalyzer.doAnalysis(reciveData);
        if(pack)
        {
            int nInfoType = pack->getInfoType();

            QByteArray tmpByteArray = pack->getInfo().toByteArray();
            QHash<QString, QVariant> controlDomain;
            controlDomain.insert("_portNumber_", pack->getCanId()-2);
            controlDomain.insert("recvAddr", pack->getRxAddr());
            controlDomain.insert("sendAddr",pack->getTxAddr());
            controlDomain.insert("commandType",pack->getCode());
            controlDomain.insert("loopAddr",pack->getLoopAddr());
            controlDomain.insert("deviceAddr",pack->getDeviceAddr());
            if (nInfoType == 0x61) {
                QByteArray uid = pack->getSimpleVar();
                controlDomain.insert("uid",CGlobal::instance()->getUidString(uid, 6));
            }
            if(CGlobal::isDebug()) qDebug() << "cancard1ReciveData" << tmpByteArray.toHex() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
            emit sig_ReciveData(pSerialPort->getName(), nInfoType, controlDomain, tmpByteArray);
        }

    }
}


void CCommunicationManager::handleCan2ReciveData()
{
    CSerialPort* pSerialPort = m_hashCommunicationLine["cancard2"]->pSerialPort;
    QByteArray &reciveData = m_hashCommunicationLine["cancard2"]->reciveData;

    if (reciveData.isEmpty())
        return;

    QString  data = "cancard2ReciveData:  " + reciveData.toHex() + "   " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    QFile file("/home/xfss/root/receiveCan2data.txt");

    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << data << '\n' << '\n';
        file.close();
    }

    if(pSerialPort){
        static PackageAnalyzer packAnalyzer;
        PackageTemplate *pack = packAnalyzer.doAnalysis(reciveData);
        if(pack)
        {
            int nInfoType = pack->getInfoType();
            QByteArray tmpByteArray = pack->getInfo().toByteArray();
            QHash<QString, QVariant> controlDomain;
            controlDomain.insert("_portNumber_", pack->getCanId()-2);
            controlDomain.insert("recvAddr", pack->getRxAddr());
            controlDomain.insert("sendAddr",pack->getTxAddr());
            controlDomain.insert("commandType",pack->getCode());
            controlDomain.insert("loopAddr",pack->getLoopAddr());
            controlDomain.insert("deviceAddr",pack->getDeviceAddr());
            if (nInfoType == 0x61) {
                QByteArray uid = pack->getSimpleVar();
                controlDomain.insert("uid",CGlobal::instance()->getUidString(uid, 6));
            }
            if(CGlobal::isDebug()) qDebug() << "cancard2ReciveData" << tmpByteArray.toHex() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
            emit sig_ReciveData(pSerialPort->getName(), nInfoType, controlDomain, tmpByteArray);
        }
    }

}

