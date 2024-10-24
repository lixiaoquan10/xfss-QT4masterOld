#ifndef PACKAGETEMPLATE_H
#define PACKAGETEMPLATE_H

#include <QList>
#include <QByteArray>

/*------------------------------------
  数据包模板类，用于描述数据包格式的内存对象，
  通过该对象可以方便上层数据与底层协议数据的
  转换。
-------------------------------------*/

#define MaxSerialNumber 60000
typedef unsigned char UCHAR;
class PackageAnalyzer;
// can payload template
class CanDataHelp
{
public:
    CanDataHelp();
    void init();
    void setTxAddr(int addr);
    int getTxAddr();
    void setRxAddr(int addr);
    int getRxAddr();

    void setCode(int code) ;
    int getCode();
    void setFrameType(int type) ;
    int getFrameType() ;
    int getLoopAddr();
    void setLoopAddr(int addr);
    int getDeviceAddr();
    void setDeviceAddr(int addr) ;
    void setVarLen(int varlen);
    int getVarlen();
    void setVar(QByteArray var);
    QByteArray getVar() ;
    int fromByteArray(const QByteArray &m_bytes);
    QByteArray toByteArray();
    int getLength();
    QByteArray getSimpleVar() ;

private:
    int m_txaddr;
    int m_rxaddr;
    int m_loopaddr;
    int m_deviceaddr;
    int m_code; /* command type*/
    int m_type; /*0-SINGLE 1-MULTI FIRST 1F-MULTI END ...*/
    int m_varlen;
    QByteArray m_var;
};

//class PackageTemplate
//{
//    friend class PackageAnalyzer;

//public:
//    PackageTemplate();
//    virtual ~PackageTemplate(){}
//    void initializationPackage(bool noClearSerialNumber=true);

//    void setCommandType(int commandType);//设置命令字
//    int  getCommandType(){return m_commandType;}

//    void setInfoType(int type);//设置信息体类型-1 代表空数据
//    int  getInfoType(){return m_infoType;}

//    int  getInfoCount(){return m_infoBodys.size();}
//    int  getInfoSize(){return m_infoSize;}

//    void setSerialNumber(int number);//设置流水号
//    int  getSerialNumber();

//    void appendInfo(QByteArray &informosome);//增加一条信息体
//    QByteArray getInfoFromIndex(int index);

//    const QByteArray& buildBytes(int *serialNumbr=NULL);//返回二进制数据及流水号(从上层数据转换成底层二进制格式数据，serialNumbr用于接收流水号)
//    int fromByteArray(const QByteArray &bytes);//传入预定格式的二进制数据，转换成模板类数据提供上层使用，若二进制格式不符合内定要求则返回false，否则返回true

//    static UCHAR calculationCrc(const QByteArray &byteData,int start=0,int end=-1);//end为-1表示从start开始到byteDate的末尾
//    static bool testcheckSum(const QByteArray &byteData, char sumData);

//private:
//    //----------------数据包格式上层描述----------------
//    int m_startSymbol;//启动符（2）
//    int m_serialNumber;//流水号（2）
//    int m_commandType;//命令字（1）
//    int m_lenght;//长度（2）
//    //-----------------------------------------------
//    //数据单元（最大1014）
//    int m_infoType;//信息体类型（1）
//    int m_infoCount;//信息体数量（1）
//    QList<QByteArray> m_infoBodys;//信息体集合
//    //-----------------------------------------------
//    //char m_crcData;//效验和（1）
//    int m_crcData;      // two bytes, sum of all bytes
//    int m_endSymbol;//结束符（2）
//    //------------------------------------------------

//    int m_infoSize;
//    QByteArray m_bytes;

//private:
//    int buildPackage(bool isIncrease/*用于控制序列号是否自增*/);//返回流水号
//    int buildPackageOpt(bool isIncrease/*用于控制序列号是否自增*/);//返回流水号
//    static QByteArray getByteFromInt(const int &value, int byteCount);//该函数完成从int转换成字节数据的转换，byteCount表示字节数
//};

class PackageTemplate
{
    friend class PackageAnalyzer;

public:
    PackageTemplate();
    virtual ~PackageTemplate(){}
    void initializationPackage();

    void setCommandType(int commandType);//设置命令字
    int  getCommandType();

    void setFrameType(int type);//设置信息体类型-1 代表空数据
    int  getFrameType();
    void setInfoType(int type);
    int getInfoType();
    int getLoopAddr();
    void setLoopAddr(int addr);
    int getDeviceAddr() ;
    void setDeviceAddr(int addr) ;
    int getRxAddr() ;
    void setRxAddr(int addr) ;
    int getTxAddr() ;
    void setTxAddr(int addr) ;
    int getCode();
    void setCode(int code) ;
//    void setSerialNumber(int number);//设置流水号
//    int  getSerialNumber();

    void setCanId(int canId);
    int getCanId() ;

    QByteArray getSimpleVar();
    void appendInfo(QByteArray &informosome);//增加一条信息体
    QByteArray getInfoFromIndex(int index);

    const QByteArray& buildBytes(int *serialNumbr=NULL);//返回二进制数据及流水号(从上层数据转换成底层二进制格式数据，serialNumbr用于接收流水号)
    int fromByteArray(const QByteArray &bytes);//传入预定格式的二进制数据，转换成模板类数据提供上层使用，若二进制格式不符合内定要求则返回false，否则返回true

    static unsigned short calculationCrc(const QByteArray &byteData,int start=0,int end=-1);//end为-1表示从start开始到byteDate的末尾
    static bool testcheckSum(const QByteArray &byteData, char sumData);
    CanDataHelp &getInfo();

    int getBytes();

private:
    //----------------数据包格式上层描述----------------
    int m_startSymbol;//启动符（4）
 //   int m_commandType;//命令字（1）
    int m_infoType;  // 5 setCanspeed 17 sendcandata
    int m_canId;
    //-----------------------------------------------
    //数据单元（最大1014）
    CanDataHelp m_info;
    //-----------------------------------------------

    //------------------------------------------------

    int m_length;//长度（2）
    QByteArray m_bytes;     //save raw data

private:
    int buildPackage(bool isIncrease/*用于控制序列号是否自增*/);//返回流水号
    static QByteArray getByteFromInt(const int &value, int byteCount);//该函数完成从int转换成字节数据的转换，byteCount表示字节数
};

#endif // PACKAGETEMPLATE_H
