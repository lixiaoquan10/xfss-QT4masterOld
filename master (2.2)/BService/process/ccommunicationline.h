#ifndef CCOMMUNICATIONLINE_H
#define CCOMMUNICATIONLINE_H

#include <QHash>
#include "packagetemplate.h"
#include "ccandatabytearray.h"

/*��·����*/
class CSerialPort;
class CCommunicationLine
{
public:
    CCommunicationLine();
    void destroy();

public:
    CSerialPort* pSerialPort;//ͨѶ�ӿ�
    QHash<int/*commandtype*/, QList<QByteArray> > sendDataTableAsType;//����������Ϊkey�ķ��ͻ�����
    PackageTemplate sendPacketTemplate;//�������͵����ݰ�ģ�����
    QByteArray reciveData;//����������Ϣ�建����
    CCanDataByteArray canReciveData;//����can���ݷ������Ƶĸ���������
    int recentRespondSerialNumber;//���Ӧ����ˮ��
    int recentReceiveSerialNumber;//������յ��ķ��Ͱ���ˮ��
    int nPort;
};

struct Block/*��ʾ��ʼ�������Ŀ����*/
{
    int start;
    int end;
    void clear(){start=-1;end=-1;}
    Block():start(-1),end(-1){}
};

#endif // CCOMMUNICATIONLINE_H
