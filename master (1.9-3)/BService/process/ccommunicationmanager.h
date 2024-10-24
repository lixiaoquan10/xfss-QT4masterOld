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
    bool sendCommandData(const QString &lineName,/*线路名称*/
                             const int &infotype,/*命令名称*/
                             const QHash<QString,QVariant> &controlDomain,/*控制域*/
                             const QByteArray &data/*数据体*/);
    bool sendPrintData(const QString &lineName,/*线路名称*/
                             const int &infotype,/*命令名称*/
                             const QHash<QString,QVariant> &controlDomain,/*控制域*/
                             const QByteArray &data/*数据体*/);

protected:
    bool sendData(QString lineName,int infoType,const QByteArray &data);

private:
    bool m_isDebug;
    QHash<int,QString> m_commandNameTable;//按类型索引的命令名列表
    QHash<QString,int> m_commandTypeTable;//按命令名索引的类型列表
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
    void sig_ReceivedPacket(int nSerialNumber);//对外发送已经识别的数据包的流水号
    void sig_ReciveData(const QString &lineName, const int &infoType,
                        const QHash<QString, QVariant> &controlDomain,
                        const QByteArray &data/*info body*/);
};

#endif // CCOMMUNICATIONMANAGER_H
