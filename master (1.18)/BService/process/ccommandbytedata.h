#ifndef CCOMMANDBYTEDATA_H
#define CCOMMANDBYTEDATA_H

#include <QObject>

class CCommandByteData : public QObject
{
    Q_OBJECT
public:
    explicit CCommandByteData(int nSerialNumber ,qint64 time,int count,const QByteArray &byteData,int nPriority);
    
signals:
    
public slots:

public:
    int         serialNumber;
    qint64    endTime;
    int         repeatCount;
    QByteArray  data;
    int         priority;
    
};

#endif // CCOMMANDBYTEDATA_H
