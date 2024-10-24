#ifndef CPROCESSBASE_H
#define CPROCESSBASE_H

#include <QSet>
#include <QObject>
#include <QByteArray>

enum ProcessType { PT_None = 0, PT_Distribution_CAN = 1001, PT_Power_CAN = 2001,
                   PT_FAS_GST = 3001, PT_FAS_BIRD = 3002, PT_FAS_LEADER = 3003,
                   PT_FAS_SONGJIANG = 3004, PT_FAS_YIAI = 3005, PT_FAS_TANDA = 3006,
                   PT_FAS_SHANYING = 3007, PT_FAS_FUAN = 3008};
class CMsgNull;
class CProcessBase : public QObject
{
    Q_OBJECT
public:
    CProcessBase(const QString& name = QString());
    virtual ~CProcessBase();

    QString name() const { return m_name; }
    bool hasCardPort(const int port) { return m_cardPorts.contains(port); }
    void removeCardPort(const int port) { m_cardPorts.remove(port); }
    void uploadStatus(int ntype, int nCanportAdd, int nDisID, int nLoopID, int nDeviceID, int nStatus, int nValue, int nTimes, QString strtext="", int nloginstatus=0, int nCommunicationStatus=0, QString strdata="");
    virtual bool addCardPort(const int port) { Q_UNUSED(port); return false; }
    virtual void procSendEvent(const int &infoType, const QHash<QString, QVariant>& control, const QByteArray &data) = 0;
    virtual void procRecvEvent(const int &infoType, const QHash<QString, QVariant>& control, const QByteArray &data) = 0;

public slots:
    void recvEvent(const QString& processname, const int &infoType, const QHash<QString, QVariant>& control, const QByteArray &data);

signals:
    void sendEvent(const QString& processname, const int &infoType, const QHash<QString, QVariant>& control, const QByteArray &data);

protected:
    int firstCardPort() const;
    QSet<int> m_cardPorts;

protected:
    static CMsgNull* m_msgNull;

private:
    QString m_name;
    int m_timeOut;
};

#endif // CPROCESSBASE_H
