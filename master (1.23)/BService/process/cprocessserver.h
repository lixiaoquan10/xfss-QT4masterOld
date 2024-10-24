#ifndef CPROCESSSERVER
#define CPROCESSSERVER

#include <QObject>
#include <QAbstractSocket>
#include <QProcess>

class QTimer;
class QTcpSocket;
class QTcpServer;
class CprocessServer : public QObject
{
    Q_OBJECT
public:
    explicit CprocessServer();
    ~CprocessServer();
    void replyXmlMsg(QByteArray data);
    void processReceiveData(QByteArray array);
public slots:
    void slot_readClientData();
    void slot_handleNewConnection();
    void slot_serverlisten();
    void slot_handleDisconnected();
    void onSocketError(QAbstractSocket::SocketError error);
    void slot_heartBeat();
    void handleBytesWritten(qint64 bytes);
    void slot_controlMasterTxtFileSize();
public:
    bool m_isMasterConnected;
private:
    QTcpServer* m_tcpServer;       // TCP服务器对象
    QTcpSocket* m_tcpSocket;  // 客户端连接列表
    QTimer* m_timer;
    QTimer *m_heartTimer;
    QTimer *m_disconnectTimer;

    QByteArray m_receiveBuff;

    QTimer *m_controlMasterTxtTimer;

//    QString m_projectName;
//    int m_deviceID;
//    QString m_deviceType;
//    QString m_province;
//    QString m_city;
//    QString m_county;
//    QString m_address;
//    QString m_projectID;
//    QString m_ip;
//    int m_port;
};

#endif // CPROCESSSERVER

