#ifndef CADAPTEROBJECT_H
#define CADAPTEROBJECT_H

#include <QObject>
#include <QByteArray>

class CAdapterObject : public QObject
{
    Q_OBJECT
public:
    explicit CAdapterObject(QObject *parent = 0);
    void TXParsePacket(int nMsgType, const char* data, int nLen);
signals:
    void sigParsePacket(int nMsgType, QByteArray data);
private slots:
    void slotParsePacket(int nMsgType, QByteArray data);
};

#endif // CADAPTEROBJECT_H
