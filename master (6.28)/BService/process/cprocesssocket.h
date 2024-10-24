#ifndef CPROCESSSOCKET
#define CPROCESSSOCKET

#include <QObject>
#include <QAbstractSocket>
#include <QProcess>

enum XmlType{
    XML_None = 0,                          // 空命令类型
    XML_Heartbeat  = 1,                    // 心跳
    XML_ApplyForProjectID = 2,             // 申请项目识别号
    XML_UploadDeviceInfo = 3,              // 上传应急疏散监控主机信息
    XML_CanDeviceInfo = 4,                 // CAN设备信息
    XML_ChannelInfo = 5,                   // 通道信息
    XML_LampInfo = 6,                      // 灯具信息
    XML_LayoutPage = 7,                    // 布局页面信息
    XML_Picture = 8,                       // 背景图片
    XML_LampCoordinate = 9,                // 灯具坐标
    XML_FirePoint = 10,                    // 着火点信息
    XML_FirePointCoordinate = 11,          // 着火点坐标
    XML_LampToFirePoint = 12,              // 灯具与着火点关联
    XML_LampDefaultDirection = 13,         // 灯具默认方向
    XML_HostElectricParameter = 14,        // 主机电参量
    XML_CanDeviceElectricParameter = 15,   // CAN设备电参量
    XML_EnableUpload = 16,                 // 使能开关
    XML_HostStateUpload = 17,              // 主机故障状态
    XML_CanDeviceStateUpload = 18,         // CAN设备故障状态
    XML_ChannelStateUpload = 19,           // 通道故障状态
    XML_LampStateUpload  = 20,             // 灯具状态
    XML_EmergencyInputUpload = 21,         // 开关量输入报警
    XML_ManualLaunchUpload = 22,           // 手动应急
    XML_UploadFirePointWarning = 23,       // 着火点报警
    XML_UploadLampDirection = 24,          // 灯具指向
    XML_ResetDeclare = 25,                 // 复位申明
    XML_HostControlConfirm = 26,           // 主机控制命令
    XML_LampControlConfirm = 27,           // 灯具控制命令
    XML_QueryAllStateConfirm = 28,         // 当前的所有状态

};
class QTimer;
class QTcpSocket;
class CprocessSocket : public QObject
{
    Q_OBJECT
public:
    explicit CprocessSocket();
    ~CprocessSocket();
    void sendXmlMsg(int type, QByteArray data = NULL);
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

