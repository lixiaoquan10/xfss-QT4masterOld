#ifndef CCANDATABYTEARRAY_H
#define CCANDATABYTEARRAY_H

#include <QHash>
#include <QList>

/*can数据包处理机制辅助对象*/
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
