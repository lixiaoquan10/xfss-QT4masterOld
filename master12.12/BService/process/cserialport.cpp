#include "cserialport.h"
#include "ccommandbytedata.h"
#include "cglobal.h"
#include <QDebug>
#include <QCoreApplication>
#define TIMEOUTCOUNT 2

CSerialPort::CSerialPort(QObject *parent, long baudrate) :
    QObject(parent),m_isDebug(false)
{
    m_pSerialPort = new QextSerialPort(QextSerialPort::EventDriven);
    m_nRsPort = 1;
    //m_nRsBaudRate = BAUD115200;
//    m_nRsBaudRate = BAUD38400;
    m_nRsBaudRate = baudrate;
    m_nRsDataBits = DATA_8;
    m_nRsStopBits = STOP_1;
    m_nRsParity = PAR_NONE;

    m_pSerialPort->setBaudRate((BaudRateType)m_nRsBaudRate);
    m_pSerialPort->setDataBits((DataBitsType)m_nRsDataBits);
    m_pSerialPort->setStopBits((StopBitsType)m_nRsStopBits);
    m_pSerialPort->setParity((ParityType)m_nRsParity);
    QObject::connect(m_pSerialPort, SIGNAL(readyRead()), this, SLOT(doReciveData()));

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(checkTimeOut()));
    m_timer.start(10);
    setTimeOut(2000);
}

CSerialPort::~CSerialPort()
{
    foreach (CCommandByteData* pByteData, m_sendCommandCacheTable.values()) {
        delete pByteData;
    }
    foreach (CCommandByteData* pByteData, m_sendCommandCacheList) {
        delete pByteData;
    }
    if(isActive()){
        m_pSerialPort->close();
        delete m_pSerialPort;
    }
}


int CSerialPort::open(int nPort)
{
    if(!m_pSerialPort)return -1;
    if(!isActive()){
        m_nRsPort = nPort;
//        m_pSerialPort->setPortName("COM"+ QString::number(m_nRsPort));
        m_pSerialPort->setPortName("/dev/ttyS"+ QString::number(m_nRsPort));
        if(m_pSerialPort->open(QIODevice::ReadWrite))
        {
            commandCacheClear();
            m_timer.start(10);
            return 1;
        }else{
            return -1;
        }
    }else{
        return -2;
    }
}

bool CSerialPort::isActive()
{
    if(m_pSerialPort) {
        return m_pSerialPort->isOpen();
    }
    else {
        return false;
    }
}

int CSerialPort::close()
{
    m_timer.stop();
    m_pSerialPort->close();
    return 1;
}

//������� ����ı�
void CSerialPort::saveDataToTxt(const QString& filePath, const QString& data)
{
    QFile file(filePath);

    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << data << '\n' << '\n';
        file.close();
        qDebug() << "Data saved to TXT file: " << filePath;
    }
    else
    {
        qDebug() << "Failed to open TXT file for writing: " << filePath;
    }
}

bool CSerialPort::sendData(int nSerialNumber, const QByteArray &data, bool isRepeat, int nPriority)
{
    if(m_pSerialPort && m_pSerialPort->isOpen()){

        if(true)
        {
            if (m_pSerialPort->isOpen()) {
                qint64 bytesWritten = m_pSerialPort->write(data);
                //���� ��ݱ��浽�ĵ�
                QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
                QString dataWithTimestamp;
                QString hexString = data.toHex(); // ���ֽ� ���ת��Ϊ ��������ַ���
                // �жϷ��� Ƿ�ɹ�
                if (bytesWritten == data.size()) {
                    // ���ͳɹ�
                    dataWithTimestamp = "(Send:" + timestamp +" length:"+ QString::number(bytesWritten) + ")" + '\n' + hexString;
                } else {
                    // ���� ���
                    dataWithTimestamp = "(Send:" + timestamp +" length:"+ QString::number(bytesWritten) + ")" + '\n' + "Failed to send data!";
                }
//                saveDataToTxt("/root/communicationData.txt",dataWithTimestamp);
//                if(m_isDebug){
//                    CGlobal::instance()->write(m_nRsPort, true,data);
//                }
                m_pSerialPort->flush();
                return true;
            }else{
                return false;
            }
        }else{
//            if(m_sendCommandCacheTable.count()==0&&m_sendCommandCacheList.count()==0){
//                pCommandByteData=new CCommandByteData(nSerialNumber,getTickCount()+m_timeout,TIMEOUTCOUNT,data,nPriority);
//                m_sendCommandCacheTable.insert(nSerialNumber,pCommandByteData);
//            }else{
//                if(m_sendCommandCacheTable.contains(nSerialNumber)){
//                    pCommandByteData=m_sendCommandCacheTable[nSerialNumber];
//                    m_sendCommandCacheTable[nSerialNumber]->endTime=getTickCount()+m_timeout;
//                }else{
//                    m_sendCommandCacheList.append(new CCommandByteData(nSerialNumber,-1,TIMEOUTCOUNT,data,nPriority));
//                }
//            }
//            if (pCommandByteData && m_pSerialPort->isOpen()) {
//                emit updataRecentSerialNumber(m_name,pCommandByteData->serialNumber);
//                if(m_isDebug){
//                    CGlobal::instance()->write(m_nRsPort, true,pCommandByteData->data);
//                }
//                m_pSerialPort->write(pCommandByteData->data);
//                m_pSerialPort->flush();
//                return true;
//            }else{
//                return false;
//            }
        }
    }else{
        return false;
    }
}

void CSerialPort::handleConfirmationPacket(int nSerialNumber)
{
    if(m_sendCommandCacheTable.contains(nSerialNumber)){
        delete m_sendCommandCacheTable[nSerialNumber];
        m_sendCommandCacheTable.remove(nSerialNumber);
    }
}


void CSerialPort::doReciveData()
{
    QByteArray bytes;
//    int nSize = m_pSerialPort->bytesAvailable();

//    QList<QByteArray> frameHeaders;
//    frameHeaders << QByteArray::fromHex("55AA") << QByteArray::fromHex("5513") << QByteArray::fromHex("5545");
//    QByteArray buffer; // �����������ڴ洢��ȡ�� ���

    int nSize;
    while(m_pSerialPort->bytesAvailable() > 0)
    {
//        Sleep(5);
        nSize = m_pSerialPort->bytesAvailable();
        bytes.resize(nSize);
        m_pSerialPort->read(bytes.data(), bytes.size());

//        buffer.append(bytes);
//        QByteArray frameData;
//        int frameStart = -1;

//        // ����֡ͷ�� �λ��
//        foreach (const QByteArray& header, frameHeaders) {
//           frameStart = buffer.indexOf(header);
//           if (frameStart != -1) {
//               frameData = buffer.mid(frameStart);
//               break;
//           }
//        }

//        if (frameStart != -1) {
//           // Ѱ����һ��֡ͷ�� �λ��
//           int frameEnd = -1;
//           foreach (const QByteArray& header, frameHeaders) {
//               frameEnd = buffer.indexOf(header, frameStart + header.length());
//               if (frameEnd != -1) {
//                   frameData = buffer.mid(frameStart, frameEnd - frameStart);
//                   buffer.remove(0, frameEnd);
//                   break;
//               }
//           }

//           if (frameEnd == -1) {
//               // û���ҵ���һ��֡ͷ�������ȴ� ���
//               break;
//           }

//           // ����������֡ ���
//           emit sig_ReciveData(m_name, frameData);
//           qDebug() << "DataSize:" << frameData;
//        } else {
////           buffer.clear(); // ��ջ������е���Ч ���
//           break;
//        }
    }
//    qDebug() << "DataSize:" << nSize;
//    if(m_isDebug){
//        CGlobal::instance()->write(m_nRsPort, false, bytes);
//    }
    if(bytes.size() > 14)
    {
        if(bytes.at(11) == 0x11)
            qDebug() << "================================" << bytes.toHex();
    }
    emit sig_ReciveData(m_name, bytes);
}

void CSerialPort::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void CSerialPort::checkTimeOut()
{
    if(m_sendCommandCacheTable.count() > 0){
        foreach(int key, m_sendCommandCacheTable.keys())
        {
            CCommandByteData* pData = m_sendCommandCacheTable[key];
            if(pData && pData->endTime<getTickCount())//�� �
            {
                sendData(key,pData->data);
                --(pData->repeatCount);

                if(pData->repeatCount<1){//�ط��� ��������ֵ
                    delete pData;
                    m_sendCommandCacheTable.remove(key);

                    if(m_sendCommandCacheTable.count()==0){
                        if(m_sendCommandCacheList.count()>0){
                            CCommandByteData* pSendData=m_sendCommandCacheList.takeFirst();
                            m_sendCommandCacheTable.insert(pSendData->serialNumber,pSendData);
                            sendData(pSendData->serialNumber,pSendData->data);
                        }
                    }
                }
            }
        }
    } else {
        if(m_sendCommandCacheList.count()>0){
            CCommandByteData* pSendData = getSendDataASPriority();
            if(pSendData){
                m_sendCommandCacheTable.insert(pSendData->serialNumber,pSendData);
                sendData(pSendData->serialNumber,pSendData->data);
            }
        }
    }
}

void CSerialPort::commandCacheClear()
{
    foreach(CCommandByteData* pData,m_sendCommandCacheTable.values()){
        delete pData;
    }
    m_sendCommandCacheTable.clear();
}

CCommandByteData* CSerialPort::getSendDataASPriority()
{
    CCommandByteData* pSendData=NULL;
    int nPriority=999;
    int nIndex=-1;
    for(int i=0; i<m_sendCommandCacheList.size(); ++i) {
        CCommandByteData* tmpData = m_sendCommandCacheList[i];
        if(tmpData && tmpData->priority<nPriority){
            nPriority = tmpData->priority;
            pSendData = tmpData;
            nIndex = i;
        }
    }
    if(nIndex>-1 && nIndex<m_sendCommandCacheList.size()){
        m_sendCommandCacheList.removeAt(nIndex);
    }
    return pSendData;
}
