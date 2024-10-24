#ifndef CCOMMUNICATION_H
#define CCOMMUNICATION_H

#include <QLibrary>
#include <QByteArray>
#include <QStringList>
#include <QVariantList>

typedef int (*OnDealEvent)(int nType, char* params, int nLen);
typedef int (*Type)(char* strPath);
typedef int (*Destroyed)();
typedef int (*Initial)(OnDealEvent);
typedef bool (*Start)(int nCanportID);
typedef bool (*Stop)();
typedef void (*Test)(bool bwrite);

typedef void (*GPacket)(int nMsgType, char *pByteBuf/*out*/,int &nOutLen/*out*/);
typedef void (*PPacket)(int nMsgType, const char* data, int nLen);


class CCommunication : public QLibrary
{
    Q_OBJECT
public:
    explicit CCommunication(const QString &fileName, QObject *parent = 0);
    int     CommunicationType(QString strProjectPath);
    int     DestroyedCommunication();
    int     InitCommunication();
    bool    StartCommunication(int nCanportID);
    bool    StopCommunication();
    void    ShowTest(bool bwrite);
    void    ParsePacket(int nMsgType, const char *data, int nLen);
    QByteArray GetPacket(int nMsgType);

    void emitRecvData(int nMsgType, QByteArray &data);

public slots:

signals:
    void recvData(int nMsgType, QByteArray &data);

};

#endif // CCOMMUNICATION_H
