#ifndef CSERIALPORT_H
#define CSERIALPORT_H

#include <QHash>
#include <QTimer>
#include <QObject>
#include "qextserialport.h"

class CCommandByteData;
class CSerialPort : public QObject
{
    Q_OBJECT
public:
    CSerialPort(QObject *parent = 0, long baudrate = 38400);
    ~CSerialPort();

    int  open(int nPort);//?????
    bool isActive();//???????????
    int  close();//????
    bool sendData(const QByteArray& data);//????

    QextSerialPort* serialPort() { return m_pSerialPort; }

    void Sleep(int msec);
    void  setName(QString name){m_name = name;}
    const QString&getName(){return m_name;}

    void setTimeOut(int timeOut){m_timeout=timeOut;}
    int  getTimeOut(){return m_timeout;}
    void setDebugOut(bool isDebug){m_isDebug=isDebug;}
    void saveDataToTxt(const QString& filePath, const QString& data);

signals:
    void sig_ReciveData(const QString &name,const QByteArray &data);
    void updataRecentSerialNumber(const QString &name,int nSerialNumber);

public slots:
    void handleConfirmationPacket(int nSerialNumber);//????????????????????????????????????
    void doReciveData();
    void checkTimeOut();
private:
    bool m_isDebug;
    qint64 m_timeout;//????
    QString m_name;//??????

    QTimer m_timer;
    QHash<int ,CCommandByteData*> m_sendCommandCacheTable;//?????????????
    QList<CCommandByteData*> m_sendCommandCacheList;//????????
    QextSerialPort * m_pSerialPort;
    //--------------------------------
    int m_nRsPort;
    int m_nRsBaudRate;
    int m_nRsDataBits;
    int m_nRsStopBits;
    int m_nRsParity;
    //--------------------------------

private:
    inline void commandCacheClear();
    inline CCommandByteData* getSendDataASPriority();

};

#endif // CSERIALPORT_H
