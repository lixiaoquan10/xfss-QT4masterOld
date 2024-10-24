#ifndef CCOMMUNICATIONMANAGER_H
#define CCOMMUNICATIONMANAGER_H

#include <QHash>
#include <QTimer>
#include <QObject>
#include "ccommunicationline.h"

class CCommunicationManager : public QObject
{
    Q_OBJECT
public:
    explicit CCommunicationManager(QObject *parent = 0);
    ~CCommunicationManager();
    int getPortFromCommunicationType(int type);
    
    bool createCommunicationLine(const QString &lineName, int type);
    bool removeCommunicationLine(const QString &lineName);

    bool open(const QString &lineName);
    bool isActive(const QString &lineName) const;
    bool close(const QString &lineName);

    bool setTimeOut(const QString &lineName,int timeOut);
    int  getTimeOut(const QString &lineName) const;
    void setDebugOut(bool isDebug);

public slots:
    void doReciveData(const QString &name,const QByteArray& data);
    bool sendCommandData(const QString &lineName,/*��·����*/
                             const int &infotype,/*��������*/
                             const QHash<QString,QVariant> &controlDomain,/*������*/
                             const QByteArray &data/*������*/);
    bool sendPrintData(const QString &lineName,/*��·����*/
                             const int &infotype,/*��������*/
                             const QHash<QString,QVariant> &controlDomain,/*������*/
                             const QByteArray &data/*������*/);

protected:
    bool sendData(QString lineName,int infoType,const QByteArray &data);

private:
    bool m_isDebug;
    QHash<int,QString> m_commandNameTable;//�������������������б�
    QHash<QString,int> m_commandTypeTable;//�������������������б�
    QHash<int,int> m_commandWordTypeTable;
    QHash<QString /*line name*/,CCommunicationLine*> m_hashCommunicationLine;
    QTimer m_CANTimer;
    QTimer m_SendTimer;
    QTimer m_ReciveTimer;
    QTimer m_LedTimer;
    QTimer m_PowerTimer;
    QTimer m_Can1Timer;
    QTimer m_Can2Timer;
    QTimer m_FasTimer;
    QTimer m_PrinterTimer;

    QByteArray led_reciveData;

private slots:
    void handleSendData();
    void handleSendLEDData();
    void handleSendPowerData();
    void handleSendCan1Data();
    void handleSendCan2Data();
    void handleSendFasData();
    void handleReciveData();
    void handleCanData();
    void updataRecentSerialNumber(const QString &name,int nSerialNumber);
    void handlePrinterData();

signals:
    void sig_ReceivedPacket(int nSerialNumber);//���ⷢ���Ѿ�ʶ������ݰ�����ˮ��
    void sig_ReciveData(const QString &lineName, const int &infoType,
                        const QHash<QString, QVariant> &controlDomain,
                        const QByteArray &data/*info body*/);
};

#endif // CCOMMUNICATIONMANAGER_H
