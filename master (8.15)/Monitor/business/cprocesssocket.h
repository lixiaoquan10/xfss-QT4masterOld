#ifndef CPROCESSSOCKET
#define CPROCESSSOCKET

#include <QObject>
#include <QAbstractSocket>
#include <QProcess>

class QTimer;
class QTcpSocket;
class CprocessSocket : public QObject
{
    Q_OBJECT
public:
    explicit CprocessSocket();
    ~CprocessSocket();
    void sendXmlMsg(QByteArray data = NULL);
    void setip(QString ip);
    void setport(int port);
    void setCloudConnect(bool enable);
    void saveDataToTxt(const QString& filePath, const QString& data);
    void clearTxtData(const QString& filePath);
    void controlTxtFileSize(const QString& filePath, qint64 maxSize);
public slots:
    void slot_reconnect();
    void slot_heartBeat();
    void slot_readyRead();
    void slot_connect();
    void slot_disconnect();
    void slot_stateChanged(QAbstractSocket::SocketState state);
    void slot_controlCloudTxtFileSize();
public:
    bool m_isCloudConnected;
private:
    QTcpSocket *m_socket;
    QTimer *m_cloudTimer;
    QTimer *m_heartTimer;
    QTimer *m_disconnectTimer;
    int m_currentType;
    QString m_projectName;
    int m_deviceID;
    QString m_deviceType;
    QString m_province;
    QString m_city;
    QString m_county;
    QString m_address;
    QString m_projectID;
    QString m_ip;
    int m_port;

    QByteArray m_receiveBuff;

    QTimer *m_controlCloudTxtTimer;
};
#endif // CPROCESSSOCKET

