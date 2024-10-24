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
    void Sleep(int msec);
    void setTestLinkageCom(bool enable);
    void setLinkageBaudRate(QString baudrateString);

    void canSendDataClear();

public slots:
    void doReciveData(const QString &name,const QByteArray& data);
    bool sendCommandData(const QString &lineName,/*线路名称*/
                             const int &infotype,/*命令名称*/
                             const QHash<QString,QVariant> &controlDomain,/*控制域*/
                             const QByteArray &data/*数据体*/);
    bool sendLedData(const QString &lineName, const int &infotype, const QByteArray &data);
    bool sendLinkageData(const QString &lineName, const int &infotype, const QByteArray &data);

protected:
    bool sendData(QString lineName,int infoType,const QByteArray &data);

public:
    bool isSerialportNameSeted;

private:
    bool m_isDebug;
    QHash<int,QString> m_commandNameTable;//按类型索引的命令名列表
    QHash<QString,int> m_commandTypeTable;//按命令名索引的类型列表
    QHash<int,int> m_commandWordTypeTable;
    QHash<QString /*line name*/,CCommunicationLine*> m_hashCommunicationLine;

    QTimer m_ledReciveTimer;
    QTimer m_linkageReciveTimer;
    QTimer m_testReciveTimer;
    QTimer m_Can1ReciveTimer;
    QTimer m_Can2ReciveTimer;

    QTimer m_ledTimer;
    QTimer m_linkageTimer;
    QTimer m_Can1Timer;
    QTimer m_Can2Timer;

    QByteArray led_reciveData;
    QByteArray CAN1_reciveData;
    QByteArray CAN2_reciveData;

private slots:
    void handleSendLedData();
    void handleSendLinkageData();
    void handleSendCan1Data();
    void handleSendCan2Data();
    void handleLedReciveData();
    void handleLinkageReciveData();
    void handleTestReciveData();
    void handleCan1ReciveData();
    void handleCan2ReciveData();
    void updataRecentSerialNumber(const QString &name,int nSerialNumber);

signals:
    void sig_ReceivedPacket(int nSerialNumber);//对外发送已经识别的数据包的流水号
    void sig_ReciveData(const QString &lineName, const int &infoType,
                        const QHash<QString, QVariant> &controlDomain,
                        const QByteArray &data/*info body*/);
};

#endif // CCOMMUNICATIONMANAGER_H
