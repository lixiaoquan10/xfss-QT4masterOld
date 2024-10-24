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

    int  open(int nPort);//��ͨѶ��
    bool isActive();//���ص�ǰ�Ƿ�������״̬
    int  close();//�ر�ͨѶ
    bool sendData(int nSerialNumber,const QByteArray& data,bool isRepeat=true/*�Ƿ��ط�*/,int nPriority=1);//��������

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
    void handleConfirmationPacket(int nSerialNumber);//�ϲ���������ݰ������ˮ�Ÿ���ͨѶ����������ͨѶ����ɾ������ˮ�ŵķ��Ͱ�
    void doReciveData();
    void checkTimeOut();
private:
    bool m_isDebug;
    qint64 m_timeout;//��ʱʱ��
    QString m_name;//ͨѶ��·����

    QTimer m_timer;
    QHash<int ,CCommandByteData*> m_sendCommandCacheTable;//����������ڳ�ʱ�ط�ʹ��
    QList<CCommandByteData*> m_sendCommandCacheList;//�������ݻ������
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
