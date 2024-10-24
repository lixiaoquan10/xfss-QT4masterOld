#include "ccandatabytearray.h"
#include "cglobal.h"
#include <QDebug>

CCanDataByteArray::CCanDataByteArray()
{
    m_hashDataTable.clear();
}

QList<int> CCanDataByteArray::getPortList()
{
    return m_hashDataTable.keys();
}

QList<int> CCanDataByteArray::getNotNullIndexList(int port)
{
    qDebug() << "CCanDataByteArray::getNotNullIndexList" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QList<int> tmpList;
    if(m_hashDataTable.contains(port)){
        foreach(int addr,m_hashDataTable[port].keys()){

            if(m_hashDataTable[port][addr].size()>0){
                tmpList.append(addr);
            }
        }
    }
    return tmpList;
}

QList<QByteArray>& CCanDataByteArray::getDataFromIndex(int port,int addr)
{
    qDebug() << "CCanDataByteArray::getDataFromIndex" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_hashDataTable.contains(port)){
        if(m_hashDataTable[port].contains(addr)){
            return m_hashDataTable[port][addr];
        }else{
            return m_nullByteArray;
        }
    }else{
        return m_nullByteArray;
    }
}

void CCanDataByteArray::appendData(int port,int addr,const QByteArray &byteArray)
{
    qDebug() << "CCanDataByteArray::appendData" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(CGlobal::isDebug()) qDebug() << "CCanDataByteArray::appendData " << port << addr << byteArray.toHex();
    if(!m_hashDataTable.contains(port)){
        QHash< int, QList<QByteArray> > tmpTable;
        tmpTable.insert(addr,QList<QByteArray>());
        tmpTable[addr].append(byteArray);
        m_hashDataTable.insert(port,tmpTable);
    }else{
        if(!m_hashDataTable[port].contains(addr)){
            QList<QByteArray> tmpList;
            tmpList.append(byteArray);
            m_hashDataTable[port].insert(addr,tmpList);
        }else{
            m_hashDataTable[port][addr].append(byteArray);
        }
    }
}
