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

    int  open(int nPort);//打开通讯器
    bool isActive();//返回当前是否是运行状态
    int  close();//关闭通讯
    bool sendData(int nSerialNumber,const QByteArray& data,bool isRepeat=true/*是否重发*/,int nPriority=1);//发送数据

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
    void handleConfirmationPacket(int nSerialNumber);//上层分析出数据包后把流水号告诉通讯部件，以让通讯部件删除该流水号的发送包
    void doReciveData();
    void checkTimeOut();
private:
    bool m_isDebug;
    qint64 m_timeout;//超时时间
    QString m_name;//通讯线路名称

    QTimer m_timer;
    QHash<int ,CCommandByteData*> m_sendCommandCacheTable;//命令缓存区用于超时重发使用
    QList<CCommandByteData*> m_sendCommandCacheList;//发送数据缓存队列
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
