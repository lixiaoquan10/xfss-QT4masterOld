#ifndef CPROCESSCLIENT_H
#define CPROCESSCLIENT_H

#include "cprocessbase.h"

class CProcessClient : public CProcessBase
{
    Q_OBJECT
public:
    CProcessClient();
    void procSendEvent(const int &infoType, const QHash<QString, QVariant>& control, const QByteArray& byte);
    void procRecvEvent(const int &infoType, const QHash<QString, QVariant>& control, const QByteArray& byte);

    void procSendData(const QByteArray& byte);
    void procRecvData(const QByteArray& byte);

    void clientReset();
    void mute(bool enable);
    void factoryReset();
    void StartCommunication();
    void StopCommunication();
    void sendPrintSet();
    void sendCheckSet();
private:

};

#endif // CPROCESSCLIENT_H
