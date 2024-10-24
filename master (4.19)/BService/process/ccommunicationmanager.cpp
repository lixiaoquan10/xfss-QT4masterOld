#include "ccommunicationmanager.h"
#include "packageanalyzer.h"
#include "cserialport.h"
#include "cprocesscontroller.h"
#include "cprocessdistribution.h"
#include "cglobal.h"
#include <QDebug>
#include <QFile>
#define CANDATALEN 15

CCommunicationManager::CCommunicationManager(QObject *parent) :
    QObject(parent),m_isDebug(false)
{
    connect(&m_ledTimer, SIGNAL(timeout()), this, SLOT(handleSendLedData()));
    connect(&m_linkageTimer, SIGNAL(timeout()), this, SLOT(handleSendLinkageData()));

    connect(&m_Can1Timer, SIGNAL(timeout()), this, SLOT(handleSendCan1Data()));
    connect(&m_Can2Timer, SIGNAL(timeout()), this, SLOT(handleSendCan2Data()));

    connect(&m_ledReciveTimer, SIGNAL(timeout()), this, SLOT(handleLedReciveData()));
    connect(&m_linkageReciveTimer, SIGNAL(timeout()), this, SLOT(handleLinkageReciveData()));

    connect(&m_Can1ReciveTimer, SIGNAL(timeout()), this, SLOT(handleCan1ReciveData()));
    connect(&m_Can2ReciveTimer, SIGNAL(timeout()), this, SLOT(handleCan2ReciveData()));

    m_ledReciveTimer.start(30);
    m_linkageReciveTimer.start(30);

    m_Can1ReciveTimer.start(15);
    m_Can2ReciveTimer.start(15);

    m_ledTimer.start(10);
    m_linkageTimer.start(10);

    m_Can1Timer.start(10);
    m_Can2Timer.start(10);

    led_reciveData = 0;

    isSerialportNameSeted = false;
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
        case CT_CanCard1:
            return 1;
        case CT_CanCard2:
            return 0;
        case CT_LedCard:
            return 2;
        case CT_LinkageCard:
            return 3;
        case CT_PrinterCard:
            return 4;
        case CT_NoCard:
            return 5;
        case CT_FASCard:
            return 6;
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
    if (lineName == "ledcard")
        pSerialPort = new CSerialPort(0, 9600);
    else if (lineName == "linkagecard")
        pSerialPort = new CSerialPort(0, 9600);
    else if (lineName == "cancard1" || lineName == "cancard2")
        pSerialPort = new CSerialPort(0, 115200);
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
        qDebug() << "CCommunicationManager::open, lineName is " << lineName << ",open result is " << result << ",port is " << line->nPort ;

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
//    QString  data1 = "sendCommandData:  " + lineName + "   " + QString::number(infotype)  + " " + data.toHex() + " " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
//    QFile file("/home/xfss/root/sendCommandData.txt");

//    if (file.open(QIODevice::Append | QIODevice::Text))
//    {
//        QTextStream stream(&file);
//        stream << data1 << '\n' << '\n';
//        file.close();
//    }
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

bool CCommunicationManager::sendLinkageData(const QString &lineName, const int &infotype, const QByteArray &data)
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
                QString  data = "ledSendData:  " + byteArray.toHex() + "   " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
                QFile file("/home/xfss/root/ledSendData.txt");

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


void CCommunicationManager::handleSendLinkageData()
{
    QString linename = "linkagecard";
    if(!m_hashCommunicationLine.contains(linename)) return;
    CSerialPort* pSerialPort = m_hashCommunicationLine[linename]->pSerialPort;

    if(pSerialPort){
        //未确定该串口是否为灯键板通讯，发送查询控制命令，若有回复则修改串口名称
        if(!isSerialportNameSeted)
        {
            foreach (int infoType, m_hashCommunicationLine[linename]->sendDataTableAsType.keys()){

                if (infoType != SetLedStatus)
                    continue;

                QList<QByteArray>& sendDataList = m_hashCommunicationLine[linename]->sendDataTableAsType[infoType];
                if(sendDataList.count() > 0){

                    QByteArray tmpArray(sendDataList[0]);
                    sendDataList.removeAt(0);

                    if (tmpArray.size() < 2)
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

                    if(CGlobal::isDebug()) qDebug() << "CCommunicationManager::handleSendLinkageData():port is " << linename << ",info type is "
                                                    << infoType << byteArray.toHex() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
                    pSerialPort->sendData(byteArray);

//                    QString  data = "linkageSendData:  " + byteArray.toHex() + "   " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
//                    QFile file("/home/xfss/root/linkageSendData.txt");

//                    if (file.open(QIODevice::Append | QIODevice::Text))
//                    {
//                        QTextStream stream(&file);
//                        stream << data << '\n' << '\n';
//                        file.close();
//                    }
                }
                quint64 tick = ::getTickCount();
                while(::getTickCount() - tick < 1){}
            }
        }
        //火报通讯
        else
        {

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
                if(!CGlobal::instance()->m_canport1ID)
                {
                    for(int i=0; i<4; i++)
                    {
                        if(i<2)
                            packetTemplate.setCanId(i+3);//CAN1/2地址3 4
                        else
                            packetTemplate.setCanId(i+5);//CAN5/6地址7 8
                        const QByteArray& byteArray = packetTemplate.buildBytes(&nSerialNumber);
                        qDebug() << "CCommunicationManager::handleSendCan1Data():port is " << nSerialNumber << ",info type is " << infoType << byteArray.toHex();
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
                }
                else
                {
                    const QByteArray& byteArray = packetTemplate.buildBytes(&nSerialNumber);
                    qDebug() << "CCommunicationManager::handleSendCan1Data():port is " << nSerialNumber << ",info type is " << infoType << byteArray.toHex();
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
                if(!CGlobal::instance()->m_canport2ID)
                {
                    for(int i=0; i<4; i++)
                    {
                        if(i<2)
                            packetTemplate.setCanId(i+5);//CAN3/4地址5 6
                        else
                            packetTemplate.setCanId(i+7);//CAN7/8地址9 10
                        const QByteArray& byteArray = packetTemplate.buildBytes(&nSerialNumber);
                        qDebug() << "CCommunicationManager::handleSendCan2Data():port is " << nSerialNumber << ",info type is " << infoType << byteArray.toHex();
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
                }
                else
                {
                    const QByteArray& byteArray = packetTemplate.buildBytes(&nSerialNumber);
                    qDebug() << "CCommunicationManager::handleSendCan2Data():port is " << nSerialNumber << ",info type is " << infoType << byteArray.toHex();
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
        if (led_reciveData.size() < 47) {
            led_reciveData.append(reciveData);
            reciveData.clear();
            return;
        }
        led_reciveData.append(reciveData);
        reciveData.clear();
        for(int i=0; i<led_reciveData.size(); i++)
        {
            if(led_reciveData.at(0) != 0x55 || led_reciveData.at(1) != 0x13)
              led_reciveData.remove(0,1);
            else
              continue;
        }
        if(led_reciveData.size() < 47)
            return;
        char sum = 0;
        for (int ix = 0; ix < 46; ix++)
            sum += led_reciveData.at(ix);
        if (led_reciveData.at(46) != sum) {
            led_reciveData.remove(0,47);
            return;
        }
        QString data = "handleLedReciveData:  " + led_reciveData.left(47).toHex() + " " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz") + "\n";
        QFile file("/home/xfss/root/LedReciveData.txt");

        if (file.open(QIODevice::Append | QIODevice::Text))
        {
            QTextStream stream(&file);
            stream << data << '\n';
            file.close();
        }
        if(CGlobal::isDebug()) qDebug() << "led Recive:" << led_reciveData.left(47).toHex() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
        QByteArray tmpByteArray;
        tmpByteArray.append(led_reciveData.at(2));
        tmpByteArray.append(led_reciveData.at(6));
        tmpByteArray.append(led_reciveData.at(7));
        tmpByteArray.append(led_reciveData.at(9));
        tmpByteArray.append(led_reciveData.at(10));
        tmpByteArray.append(led_reciveData.at(14));
        tmpByteArray.append(led_reciveData.at(15));
        tmpByteArray.append(led_reciveData.at(16));
        tmpByteArray.append(led_reciveData.at(17));
        tmpByteArray.append(led_reciveData.at(18));
        tmpByteArray.append(led_reciveData.at(19));
        tmpByteArray.append(led_reciveData.at(20));
        led_reciveData.remove(0,47);
        QHash<QString, QVariant> controlDomain;
        controlDomain.insert("communicationType",CT_LedCard);
        if(CGlobal::isDebug()) qDebug() << "ledcardReciveData" << tmpByteArray.toHex() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
        CGlobal::instance()->processController()->procRecvEvent(0, controlDomain, tmpByteArray);

        return;
    }
}

void CCommunicationManager::handleLinkageReciveData()
{
    CSerialPort* pSerialPort = m_hashCommunicationLine["linkagecard"]->pSerialPort;
    QByteArray &reciveData = m_hashCommunicationLine["linkagecard"]->reciveData;
    if (reciveData.isEmpty())
        return;
    if(pSerialPort){
        //检测到灯键板通讯
        if (reciveData.size() >= 47 && reciveData.at(0) == 0x55  && reciveData.at(1) == 0x13) {
            char sum = 0;
            for (int ix = 0; ix < (47-1); ix++)
            {
               sum += reciveData.at(ix);
            }
            //校验和正确
            if (reciveData.at(46) == sum) {
                QString data = "handleLinkageReciveData:  " + reciveData.toHex() + " " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz") + "\n";
                QFile file("/home/xfss/root/LinkageReciveData.txt");

                if (file.open(QIODevice::Append | QIODevice::Text))
                {
                    QTextStream stream(&file);
                    stream << data << '\n';
                    file.close();
                }
                CSerialPort* pSerialPortLed = m_hashCommunicationLine["ledcard"]->pSerialPort;
//                QString  data1 = "oldledPortName:  " + pSerialPortLed->serialPort()->portName() + "\n" +
//                                "oldlinkagePortName:  " + pSerialPort->serialPort()->portName() + "\n" +
//                                QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz") + "\n";
                //关闭串口
                pSerialPort->serialPort()->close();
                pSerialPortLed->serialPort()->close();
                //交换串口名称、波特率
                QString ledPortName = pSerialPortLed->serialPort()->portName();
                BaudRateType ledbaudRate = pSerialPortLed->serialPort()->baudRate();
                QString linkagePortName = pSerialPort->serialPort()->portName();
                BaudRateType linkagebaudRate = pSerialPort->serialPort()->baudRate();
                pSerialPort->serialPort()->setPortName(ledPortName);
                pSerialPort->serialPort()->setBaudRate(ledbaudRate);
                pSerialPortLed->serialPort()->setPortName(linkagePortName);
                pSerialPortLed->serialPort()->setBaudRate(linkagebaudRate);
                QObject::connect(pSerialPort->serialPort(), SIGNAL(readyRead()), pSerialPort->serialPort(), SLOT(doReciveData()));
                QObject::connect(pSerialPortLed->serialPort(), SIGNAL(readyRead()), pSerialPortLed->serialPort(), SLOT(doReciveData()));
                // 重新打开串口
                if (pSerialPort->serialPort()->open(QIODevice::ReadWrite)) {
//                    data1 =data1 + "串口重新打开成功" + "\n";
                    // 连接信号与槽等相关操作
                }  else {
//                    data1 =data1 + "串口重新打开失败：" + pSerialPort->serialPort()->errorString() + "\n";
                }
                // 重新打开串口
                if (pSerialPortLed->serialPort()->open(QIODevice::ReadWrite)) {
//                    data1 =data1 + "串口重新打开成功" + "\n";
                    // 连接信号与槽等相关操作
                }  else {
//                    data1 =data1 + "串口重新打开失败：" + pSerialPort->serialPort()->errorString() + "\n";
                }

//                data1 =data1 + "newledPortName:  " + pSerialPortLed->serialPort()->portName() + "\n" +
//                                "newlinkagePortName:  " + pSerialPort->serialPort()->portName() + "\n" +
//                                QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz") + "\n";
//                QFile file1("/home/xfss/root/portName.txt");

//                if (file1.open(QIODevice::Append | QIODevice::Text))
//                {
//                    QTextStream stream(&file1);
//                    stream << data1 << '\n' << '\n';
//                    file1.close();
//                }

                isSerialportNameSeted = true;
                if(CGlobal::isDebug()) qDebug() << "led Recive:" << reciveData.toHex() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
                QByteArray tmpByteArray;
                tmpByteArray.append(reciveData.at(2));
                tmpByteArray.append(reciveData.at(6));
                tmpByteArray.append(reciveData.at(7));
                tmpByteArray.append(reciveData.at(9));
                tmpByteArray.append(reciveData.at(10));
                tmpByteArray.append(reciveData.at(14));
                tmpByteArray.append(reciveData.at(15));
                tmpByteArray.append(reciveData.at(16));
                tmpByteArray.append(reciveData.at(17));
                reciveData.clear();
                QHash<QString, QVariant> controlDomain;
                controlDomain.insert("communicationType",CT_LedCard);
                qDebug() << "ledcardReciveData" << tmpByteArray.toHex() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
                CGlobal::instance()->processController()->procRecvEvent(0, controlDomain, tmpByteArray);
            }
            else
            {
                QHash<QString, QVariant> controlDomain;
                controlDomain.insert("communicationType",CT_LinkageCard);
                QByteArray tmpByteArray;
                tmpByteArray.append(reciveData.at(0));   //设备
                tmpByteArray.append(reciveData.at(1));   //回路
                tmpByteArray.append(reciveData.at(2));   //地址
                tmpByteArray.append(reciveData.at(3));   //是否火警发生
                reciveData.clear();
                qDebug() << "linkageReciveData" << tmpByteArray.toHex() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
                CGlobal::instance()->processController()->procRecvEvent(0, controlDomain, tmpByteArray);

            }
        }
        //火报通讯
        else
        {
            reciveData.clear();
        }
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
//            if(CGlobal::isDebug()) qDebug() << "cancard1ReciveData" << tmpByteArray.toHex() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
//            emit sig_ReciveData(pSerialPort->getName(), nInfoType, controlDomain, tmpByteArray);
            CGlobal::instance()->processDistribution()->procRecvEvent(nInfoType, controlDomain, tmpByteArray);
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
//            emit sig_ReciveData(pSerialPort->getName(), nInfoType, controlDomain, tmpByteArray);
            CGlobal::instance()->processDistribution()->procRecvEvent(nInfoType, controlDomain, tmpByteArray);
        }
    }

}

