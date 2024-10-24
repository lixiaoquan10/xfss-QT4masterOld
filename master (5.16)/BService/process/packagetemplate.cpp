#include "packagetemplate.h"
#include "ctrace.h"
#include <QDebug>

CanDataHelp::CanDataHelp():m_txaddr(0),m_rxaddr(0),m_loopaddr(0),m_deviceaddr(0),m_code(0),m_type(0),m_varlen(0)
{
    m_var.clear();
}
void CanDataHelp::init()
{
    m_txaddr = 0;
    m_rxaddr=0;
    m_loopaddr=0;
    m_deviceaddr=0;
    m_code = 0;
    m_type = 0;
    m_varlen = 0;
    m_var.clear();
}
void CanDataHelp::setTxAddr(int addr)
{
    m_txaddr = addr;
}
int CanDataHelp::getTxAddr()
{
    return m_txaddr;
}
void CanDataHelp::setRxAddr(int addr)
{
    m_rxaddr = addr;
}
int CanDataHelp::getRxAddr()
{
    return m_rxaddr;
}

void CanDataHelp::setCode(int code)
{
    m_code = code;
}
int CanDataHelp::getCode()
{
    return m_code;
}
void CanDataHelp::setFrameType(int type)
{
    m_type = type;
}
int CanDataHelp::getFrameType()
{
    return m_type;
}
int CanDataHelp::getLoopAddr()
{
    return m_loopaddr;
}
void CanDataHelp::setLoopAddr(int addr)
{
    m_loopaddr = addr;
}
int CanDataHelp::getDeviceAddr() {
    return m_deviceaddr;
}
void CanDataHelp::setDeviceAddr(int addr)
{
    m_deviceaddr = addr;
}
void CanDataHelp::setVarLen(int varlen)
{
    m_varlen = varlen;
}
int CanDataHelp::getVarlen()
{
    return m_varlen;
}
void CanDataHelp::setVar(QByteArray var)
{
    m_var.clear();
    for (int ix = 0; ix < var.length(); ix++)
        m_var.append(var.at(ix));
    m_varlen = m_var.size();
}
QByteArray CanDataHelp::getVar()
{
    QByteArray var;
    for (int ix = 0; ix < m_var.length(); ix++)
        var.append(m_var.at(ix));
    return var;
}
int CanDataHelp:: getLength()
{
    m_varlen == m_var.size()?(5+m_varlen):5;
}

/*
================
CanDataHelp::fromByteArray
 返回值描述：
  1：正常
 -1：长度异常,应该丢弃
 -2：长度与实际数据大小不匹配,可能数据没收完
 -3：开始符或者结束符不匹配

================
*/

int CanDataHelp::fromByteArray(const QByteArray &bytes)
{
    if (bytes.length() <= 5)
        return -2;
    int len = bytes.at(5);
    if ((len + 5) > bytes.length())
        return -2;
    this->m_rxaddr = bytes.at(0);
    this->m_txaddr = bytes.at(1);
    this->m_loopaddr = bytes.at(2);
    this->m_deviceaddr = bytes.at(3);
//    this->m_code = bytes.at(2);
    this->m_type = bytes.at(4);
    this->m_varlen = len;
    if (len > 0)
        this->m_code = bytes.at(6);
    else
        this->m_code = 0;
    for(int ix = 0; ix < len; ix++)
        m_var.append(bytes.at(ix+6));

    return 1;
}

QByteArray CanDataHelp::toByteArray()
{
    QByteArray bytes;
    bytes.append((char)m_rxaddr);
    bytes.append((char)m_txaddr);
//    bytes.append(m_code);
    bytes.append(char(m_loopaddr));
    bytes.append(char(m_deviceaddr));
    bytes.append((char)m_type);
    if (m_varlen > m_var.size())
        m_varlen = m_var.size();
    bytes.append(char(m_varlen));

    for (int ix = 0; ix < m_varlen; ix++)
        bytes.append(m_var.at(ix));
    return bytes;
}

QByteArray CanDataHelp::getSimpleVar()
{
    if (m_varlen <= 1 || m_var.size() <= 1)
        return QByteArray();
    return m_var.right(m_varlen-1);
}

/*
================
PackageTemplate::PackageTemplate
================
*/
PackageTemplate::PackageTemplate()
{
    m_startSymbol = 0x55AACC33;//0xAA 0xAA
    m_length = 0;
    m_canId = 1;
    m_bytes.clear();
    m_info.init();
}


int  PackageTemplate::getCommandType()
{
    return m_info.getCode();
}

void PackageTemplate::setFrameType(int type) //设置信息体类型-1 代表空数据
{
    m_info.setFrameType(type);

}
int  PackageTemplate::getFrameType()
{
    return m_info.getFrameType();
}
void PackageTemplate::setInfoType(int type)
{
    m_infoType = type;
}      /*cmd type*/
int PackageTemplate::getInfoType()
{
    return m_infoType;
}
int PackageTemplate::getLoopAddr()
{
    return m_info.getLoopAddr();
}
void PackageTemplate::setLoopAddr(int addr)
{
    m_info.setLoopAddr(addr);
}
int PackageTemplate::getDeviceAddr()
{
    return m_info.getDeviceAddr();
}
void PackageTemplate::setDeviceAddr(int addr)
{
    m_info.setDeviceAddr(addr);
}
int PackageTemplate::getRxAddr()
{
    return m_info.getRxAddr();
}
void PackageTemplate::setRxAddr(int addr)
{
    m_info.setRxAddr(addr);
}
int PackageTemplate::getTxAddr()
{
    return m_info.getTxAddr();
}
void PackageTemplate::setTxAddr(int addr)
{
    m_info.setTxAddr(addr);
}
int PackageTemplate::getCode()
{
    return m_info.getCode();
}
void PackageTemplate::setCode(int code)
{
    m_info.setCode(code);
}

void PackageTemplate::setCanId(int canId)
{
    m_canId = canId;
}
int PackageTemplate::getCanId()
{
    return m_canId;
}

CanDataHelp & PackageTemplate::getInfo()
{
    return m_info;
}

int PackageTemplate::getBytes()
{
    return m_length;
}
/*
================
PackageTemplate::buildBytes
================
*/
const QByteArray &PackageTemplate::buildBytes(int *serialNumbr)
{
    Q_UNUSED(serialNumbr);
    m_bytes.clear();
    m_bytes.append(char(0x55));
    m_bytes.append(char(0xAA));
    m_bytes.append(char(0xCC));
    m_bytes.append(char(0x33));
    m_bytes.append(char(m_canId));
    m_bytes.append(m_info.toByteArray());
    int sum = calculationCrc(m_bytes,0);
    m_bytes.append(char((sum >> 0) & 0xff));
    m_bytes.append(char((sum >> 8) & 0xff));
    m_length = m_bytes.size();
    return m_bytes;
}

/*
================
PackageTemplate::getByteFromInt
================
*/
//QByteArray PackageTemplate::getByteFromInt(const int &value, int byteCount)
//{
//    if(byteCount<0) return QByteArray();
//    if((uint)byteCount>sizeof(value)) byteCount=sizeof(value);
//    return QByteArray::fromRawData((char*)&value,byteCount);
//}

QByteArray PackageTemplate::getByteFromInt(const int &value, int byteCount)
{
    if(byteCount<0) return QByteArray();

    QByteArray bytes;
    int cnt = byteCount;
    char c = 0;
    while(cnt-- > 0) {
        c = (value >> (8*cnt)) && 0xff;
        bytes.append(c);
    }

    return bytes;

}

/*
================
PackageTemplate::initializationPackage
================
*/
void PackageTemplate::initializationPackage()
{

    m_infoType = 0;
    m_length = 0;
    m_canId = 3;        /*just ref to can1*/
    m_info.init();
}


/*
================
PackageTemplate::fromByteArray
 返回值描述：
  1：正常
 -1：长度异常,应该丢弃
 -2：长度与实际数据大小不匹配,可能数据没收完
 -3：开始符或者结束符不匹配
 -4：校验不正确
================
*/

int PackageTemplate::fromByteArray(const QByteArray &bytes)
{
    initializationPackage();
    int byteLength = bytes.length();

    if(byteLength > 1024) return -1;
    if(byteLength < 10) return -2;
    if((unsigned char)*(bytes.data())!=0x55 || (unsigned char)*(bytes.data()+1)!=0xaa ||
            (unsigned char)*(bytes.data()+2)!=0xcc && (unsigned char)*(bytes.data()+3)!=0x33)//以0xaa 0xaa开始，0xbb 0xbb结束
        return -3;
    unsigned char crcL = (unsigned char)bytes.at(bytes.length()-2);
    unsigned char crcH = (unsigned char)bytes.at(bytes.length()-1);
    unsigned short crc = (crcH << 8) | crcL;
    if (calculationCrc(bytes, 0, bytes.length() - 3) != crc)
        return -4;
     m_canId = bytes.at(4);

     int result = m_info.fromByteArray(bytes.mid(5, bytes.length()-7));      // exclude head(4) canid(1) crc(2)
     if (result == 1)
         m_infoType = m_info.getCode();
    return result;

}

/*
================
sum
================
*/
unsigned short sum(const char *p, int nLen)
{
    unsigned short nSum = 0;
    for (int i = 0; i < nLen; ++i) {
       nSum += (UCHAR)p[i];
    }
    return nSum;
}

/*
================
PackageTemplate::calculationCrc
================
*/
unsigned short PackageTemplate::calculationCrc(const QByteArray &byteData, int start, int end)
{
    if(end == -1){
        end = byteData.length()-1;
    }
    if(start>=0 && start<=end && end<byteData.length()){
        return sum(byteData.data()+start, end-start+1);
    }
    return 0;
}

/*
================
PackageTemplate::testcheckSum
================
*/
bool PackageTemplate::testcheckSum(const QByteArray &byteData, char sumData)
{
    char checkSum=PackageTemplate::calculationCrc(byteData, 0);
    if(checkSum==sumData){
        return true;
    }else{
        return false;
    }

}

/*
================
PackageTemplate::appendInformosome
================
*/
void PackageTemplate::appendInfo(QByteArray &informosome)
{
    m_info.init();
    m_info.fromByteArray(informosome);

}

/*
================
PackageTemplate::getInfoFromIndex
================
*/
QByteArray PackageTemplate::getInfoFromIndex(int index)
{
   return m_info.toByteArray();
}

/*
================
PackageTemplate::setSerialNumber
================
*/
//void PackageTemplate::setSerialNumber(int number)
//{
//    m_serialNumber=number;
//}

/*
================
PackageTemplate::getSerialNumber
================
*/
//int PackageTemplate::getSerialNumber()
//{
//    return m_serialNumber;
//}

/*
================
PackageTemplate::setInfoType
================
*/


/*
================
PackageTemplate::setCommandType
================
*/
void PackageTemplate::setCommandType(int commandType)
{
   m_info.setCode(commandType);
}

QByteArray PackageTemplate::getSimpleVar()
{
    return m_info.getSimpleVar();
}
