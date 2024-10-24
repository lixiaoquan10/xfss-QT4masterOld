#ifndef CCANDATABYTEARRAY_H
#define CCANDATABYTEARRAY_H

#include <QHash>
#include <QList>

/*can���ݰ�������Ƹ�������*/
class CCanDataByteArray
{
public:
    CCanDataByteArray();
    QList<int> getPortList();
    QList<int> getNotNullIndexList(int port);
    QList<QByteArray>& getDataFromIndex(int port,int addr);
    void appendData(int port,int addr,const QByteArray &byteArray);

private:
    QHash<int/*com port*/,QHash<int/*send addr*/, QList<QByteArray> > > m_hashDataTable;
    QList<QByteArray> m_nullByteArray;
};

#endif // CCANDATABYTEARRAY_H
