#include "cprocessdistribution.h"
#include "cprocesscontroller.h"
#include "ccommunicationroute.h"
#include "print/cprint.h"
#include "struct/cmsgnull.h"
#include "struct/cmsgpowerinfo.h"
#include "cglobal.h"
#include <QTimer>
#include <QDebug>

CProcessDistribution::CProcessDistribution() :
    CProcessBase("Distribution_CAN"), m_isShardCommunication(false), m_time(0), m_nLoopID(255), m_bCommFault(false)
{
    m_listLoop.clear();
    m_bLogin = false;
    m_timer = new QTimer();
    m_timeLogin = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeOut()));
    connect(m_timeLogin, SIGNAL(timeout()), this, SLOT(regeditOK()));
    m_timer->start(10000);
}

CProcessDistribution::~CProcessDistribution()
{
    delete m_timer;
    delete m_timeLogin;
}

bool CProcessDistribution::addCardPort(const int port)
{
    if(m_cardPorts.contains(port))
        return false;
    m_cardPorts.insert(port);
    return true;
}

void CProcessDistribution::setIsShardCommunication(bool enable)
{
    if(m_isShardCommunication == enable)
        return;
}

void CProcessDistribution::procSendEvent(const int &infoType, const QHash<QString, QVariant> &control, const QByteArray &data)
{
    if(control.contains("recvAddr") && control.contains("commandType")){
        QHash<QString, QVariant> tmpControl = control;
        tmpControl[INTERFACEBOARD_PORT_NUMBER_STR] = firstCardPort();
        tmpControl["sendAddr"] = CGlobal::instance()->panelAddress();
        tmpControl["dataLen"] = data.size();
        emit sendEvent(name(), infoType, tmpControl, data);
        m_lastSendTime = QDateTime::currentDateTime();
    }
}

void CProcessDistribution::procRecvEvent(const int &infoType, const QHash<QString, QVariant> &control, const QByteArray &data)
{
    Q_UNUSED(infoType);
    static int distributionStatus = 0;
    if(control.contains("recvAddr") && control.contains("sendAddr")){        
        unsigned int recvAddress = control["recvAddr"].toInt();
        unsigned int nCanportAdd = control.value("_portNumber_").toInt();
        if(recvAddress == 0 || recvAddress == CGlobal::instance()->panelAddress())
        {            
            unsigned int distributionAddress = control["sendAddr"].toInt();
            unsigned int cmdType = control["commandType"].toInt();
            unsigned int loopAddr = control.value("loopAddr").toInt();
            unsigned int deviceAddr = control.value("deviceAddr").toInt();
            QString struid = control.value("uid").toString();
            QString strdata = data.toHex();
            qDebug() << "CProcessDistribution::procRecvEvent" << cmdType << data.toHex();
            if(0x60 == cmdType)
                m_time = 0;
            else
                sendAckPacket(nCanportAdd + 2,distributionAddress,loopAddr,deviceAddr);        // confirm
            if(distributionAddress > 0 && distributionAddress < 128)
            {
                m_time = 0;
                unsigned int deviceLogin = 2;
                setDistributionTime(distributionAddress, 0);
                if(0x01 == cmdType){//读取运行参数回复
                    uploadStatus(3, nCanportAdd, distributionAddress, 0, 0, OBJS_None, 1, QDateTime::currentDateTime().toTime_t(),"",0,0,strdata);
                }else if(0x02 == cmdType){//读取保护参数回复
                    uploadStatus(3, nCanportAdd, distributionAddress, 0, 0, OBJS_None, 1, QDateTime::currentDateTime().toTime_t(),"",0,0,strdata);
                } else if(0x08 == cmdType) {//读取设备信息回复
                    uploadStatus(3, nCanportAdd, distributionAddress, loopAddr, deviceAddr, OBJS_None, 1, QDateTime::currentDateTime().toTime_t(),"",0,0,strdata);
                } else if(0x11 == cmdType) {//上传运行状态
                    if(data.count() < 8)
                        return;
//                    distributionStatus = data.at(7);
                    bool ok;
                    distributionStatus = data.toHex().mid(14, 2).toInt(&ok, 16); // 将十六进制转换为十进制
                    if(distributionStatus == 0x81){//通讯故障
                        if(loopAddr == 0)
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_DistributionCommunicationFault, 1, QDateTime::currentDateTime().toTime_t());
                        else if(deviceAddr == 0)
                            uploadStatus(5, nCanportAdd, distributionAddress, loopAddr, 0, OBJS_LoopCommunicationFault, 1, QDateTime::currentDateTime().toTime_t());
                        else
                            uploadStatus(5, nCanportAdd, distributionAddress, loopAddr, deviceAddr, OBJS_LightCommunicationFault, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x01){//通讯恢复
                        if(loopAddr == 0)
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_DistributionCommunicationFault, 0, QDateTime::currentDateTime().toTime_t());
                        else if(deviceAddr == 0)
                            uploadStatus(5, nCanportAdd, distributionAddress, loopAddr, 0, OBJS_LoopCommunicationFault, 0, QDateTime::currentDateTime().toTime_t());
                        else
                            uploadStatus(5, nCanportAdd, distributionAddress, loopAddr, deviceAddr, OBJS_LightCommunicationFault, 0, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x87){//主电故障
                        uploadStatus(5, nCanportAdd, distributionAddress, loopAddr, deviceAddr, OBJS_MainPowerFault, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x07){//主电故障恢复
                        uploadStatus(5, nCanportAdd, distributionAddress, loopAddr, deviceAddr, OBJS_MainPowerFault, 0, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x88){//备电断线
                        uploadStatus(5, nCanportAdd, distributionAddress, loopAddr, deviceAddr, OBJS_BatteryOffFault, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x08){//备电断线恢复
                        uploadStatus(5, nCanportAdd, distributionAddress, loopAddr, deviceAddr, OBJS_BatteryOffFault, 0, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x89){//备电短路
                        uploadStatus(5, nCanportAdd, distributionAddress, loopAddr, deviceAddr, OBJS_BatteryShortFault, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x09){//备电短路恢复
                        uploadStatus(5, nCanportAdd, distributionAddress, loopAddr, deviceAddr, OBJS_BatteryShortFault, 0, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x8A){//备电欠压
                        uploadStatus(5, nCanportAdd, distributionAddress, loopAddr, deviceAddr, OBJS_BatteryLowVoltageFault, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x0A){//备电欠压恢复
                        uploadStatus(5, nCanportAdd, distributionAddress, loopAddr, deviceAddr, OBJS_BatteryLowVoltageFault, 0, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0xC0){//光源故障
                        uploadStatus(5, nCanportAdd, distributionAddress, loopAddr, deviceAddr, OBJS_LightFault, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x40){//光源故障恢复
                        uploadStatus(5, nCanportAdd, distributionAddress, loopAddr, deviceAddr, OBJS_LightFault, 0, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0xC1){//灯具应急启动
//                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_Emergency, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x41){//灯具应急停止

                    }
                    else if(distributionStatus == 0xC6){//集中电源输出过载
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_LoopOutOverloadFault, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x46){//集中电源过载恢复
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_LoopOutOverloadFault, 0, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0xC7){//集中电源输出开路
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_PowerKaiLuFault, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x47){//集中电源输出开路恢复
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_PowerKaiLuFault, 0, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0xC8){//集中电源电池欠压
                        if(data.at(8) == 0x01)
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_BatteryLowVoltageFault_1, 1, QDateTime::currentDateTime().toTime_t());
                        else if(data.at(8) == 0x02)
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_BatteryLowVoltageFault_2, 1, QDateTime::currentDateTime().toTime_t());
                        else if(data.at(8) == 0x03)
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_BatteryLowVoltageFault_3, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x48){//集中电源电池欠压恢复
                        if(data.at(8) == 0x01)
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_BatteryLowVoltageFault_1, 0, QDateTime::currentDateTime().toTime_t());
                        else if(data.at(8) == 0x02)
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_BatteryLowVoltageFault_2, 0, QDateTime::currentDateTime().toTime_t());
                        else if(data.at(8) == 0x03)
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_BatteryLowVoltageFault_3, 0, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0xC9){//集中电源备电故障
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_StandbyPowerFault, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x49){//集中电源备电故障恢复
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_StandbyPowerFault, 0, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0xCA){//集中电源充电器故障
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_BatteryOverDischargeFault, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x4A){//集中电源充电器故障恢复
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_BatteryOverDischargeFault, 0, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0xCB){//集中电源过放电故障
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_BatteryLowQuantityFault, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x4B){//集中电源过放电故障恢复
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_BatteryLowQuantityFault, 0, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0xCC){//集中电源开始应急
                        if(data.at(8) & 0x01)
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_ForceEmergency, 1, QDateTime::currentDateTime().toTime_t());
                        if(data.at(8) & 0x02)
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_ManualEmergency, 1, QDateTime::currentDateTime().toTime_t());
                        if(data.at(8) & 0x04)
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_TestEmergency, 1, QDateTime::currentDateTime().toTime_t());
                        if(data.at(8) & 0x08)
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_SystemEmergency, 1, QDateTime::currentDateTime().toTime_t());
                        if(data.at(8) & 0x10)
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_DIEmergency, 1, QDateTime::currentDateTime().toTime_t());
                        if(data.at(8) & 0x20)
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_UndervoltageEmergency, 1, QDateTime::currentDateTime().toTime_t());
                        if(data.at(8) & 0x40)
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_AutoEmergency, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x4C){//集中电源停止应急
                        if(data.at(8) == 0x00)
                        {
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_ForceEmergency, 0, QDateTime::currentDateTime().toTime_t());
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_ManualEmergency, 0, QDateTime::currentDateTime().toTime_t());
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_TestEmergency, 0, QDateTime::currentDateTime().toTime_t());
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_SystemEmergency, 0, QDateTime::currentDateTime().toTime_t());
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_DIEmergency, 0, QDateTime::currentDateTime().toTime_t());
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_UndervoltageEmergency, 0, QDateTime::currentDateTime().toTime_t());
                            uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_AutoEmergency, 0, QDateTime::currentDateTime().toTime_t());
                        }
                    }
                    else if(distributionStatus == 0xCD){//集中电源开始充电
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_ChargeStatus, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x4D){//集中电源停止充电
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_ChargeStatus, 0, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0xCE){//集中电源24/36V输出故障
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_PowerOutputFault, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x4E){//集中电源24/36V输出故障恢复
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_PowerOutputFault, 0, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0xCF){//集中电源系统故障
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_SystemFault, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x4F){//集中电源系统故障恢复
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_SystemFault, 0, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0xD0){//集中电源处于手动状态
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_RunMode, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x50){//集中电源处于自动状态
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_RunMode, 0, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0xD1){//集中电源电池高温报警
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_BatteryOverheating, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x51){//集中电源电池高温报警恢复
                        uploadStatus(5, nCanportAdd, distributionAddress, 0, 0, OBJS_BatteryOverheating, 0, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0xD2){//集中电源回路输出开路
                        uploadStatus(5, nCanportAdd, distributionAddress, loopAddr, 0, OBJS_OutLoopFault, 1, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0x52){//集中电源回路输出开路恢复
                        uploadStatus(5, nCanportAdd, distributionAddress, loopAddr, 0, OBJS_OutLoopFault, 0, QDateTime::currentDateTime().toTime_t());
                    }
                    else if(distributionStatus == 0xD3){//自带电池灯具电池状态异常
                        uploadStatus(3, nCanportAdd, distributionAddress, loopAddr, deviceAddr, OBJS_None, 1, QDateTime::currentDateTime().toTime_t(),"",0,0,strdata);
                    }
                    else if(distributionStatus == 0x53){//自带电池灯具电池状态恢复
                        uploadStatus(3, nCanportAdd, distributionAddress, loopAddr, deviceAddr, OBJS_None, 1, QDateTime::currentDateTime().toTime_t(),"",0,0,strdata);
                    }
                } else if(0x61 == cmdType) {//注册
                    m_timeLogin->stop();
                    m_timeLogin->start(15000);
//                    QString  data1 = "0x61:  " + data.toHex() + "   " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
//                    QFile file("/home/xfss/root/login.txt");

//                    if (file.open(QIODevice::Append | QIODevice::Text))
//                    {
//                        QTextStream stream(&file);
//                        stream << data1 << '\n';
//                        file.close();
//                    }
                    m_listLoop.clear();
                    if(data.count() < 14)
                        return;
                    bool ok;
                    int nValue = data.toHex().mid(26, 2).toInt(&ok, 16); // 将十六进制转换为十进制
                    if(loopAddr == 0 && deviceAddr == 0)
                    {
                        uploadStatus(2, nCanportAdd, distributionAddress, 0, 0, OBJS_Online, nValue, QDateTime::currentDateTime().toTime_t(),struid);
                        for(int i = 0; i < 8; i++){
                            uploadStatus(5, nCanportAdd, distributionAddress, i+1, 0, OBJS_Online, 1, QDateTime::currentDateTime().toTime_t());
                        }
                    }

                    if (deviceAddr != 0) {
                        uploadStatus(4, nCanportAdd, distributionAddress, loopAddr, deviceAddr, OBJS_LoginDeviceType, nValue, QDateTime::currentDateTime().toTime_t(),struid,deviceLogin,OBJS_Online);
                    }

                } else if(0x77 == cmdType) {//读集中电源信息应答
                    uploadStatus(3, nCanportAdd, distributionAddress, 0, 0, OBJS_None, 1, QDateTime::currentDateTime().toTime_t(),"",0,0,strdata);
                } else if(0x78 == cmdType) {//配电箱回路板应急输出状态
                    uploadStatus(3, nCanportAdd, distributionAddress, 0, 0, OBJS_None, 1, QDateTime::currentDateTime().toTime_t(),"",0,0,strdata);
                }
            }
        }
    }
}


void CProcessDistribution::powerInfo(int distributionAddress, int loopAddress, int deviceAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::powerInfo, address is" << distributionAddress << loopAddress << deviceAddress;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;  /*0-broadcast*/
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x01;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x01);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::readProtectInfor(int distributionAddress, int loopAddress, int deviceAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::readProtectInfor, address is" << distributionAddress << loopAddress << deviceAddress;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x02;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x02);
    procSendEvent(SendCanData, control, byte);
}


void CProcessDistribution::readDeviceInfor(int distributionAddress, int loopAddress, int deviceAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::readDeviceInfor, address is" << distributionAddress << loopAddress << deviceAddress;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x08;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x08);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::writeProtectInfor(int distributionAddress, int loopAddress, int deviceAddress, QByteArray data, int canportAdd)
{
    qDebug() << "==CProcessDistribution::writeProtectInfor, address is" << distributionAddress << loopAddress << deviceAddress;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x12;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x12);
    byte.append(data);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::DeviceType(int distributionAddress, int loopAddress, int deviceAddress, int value, int canportAdd)
{
    qDebug() << "==CProcessDistribution::DeviceType, address is" << distributionAddress << loopAddress << deviceAddress << value;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x18;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x18);
    byte.append(deviceAddress);
    byte.append(value);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::DeviceAdd(int distributionAddress, int loopAddress, int deviceAddress, int value, QString strText, int canportAdd)
{
    bool ok;
    qDebug() << "==CProcessDistribution::DeviceAdd, address is" << distributionAddress << loopAddress << deviceAddress << value;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x20;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x20);
//    QString str = QString::number(u_int64_t(strText.toDouble()), 16);

    // 将字符串转换为无前导零的 unsigned long long 类型
    unsigned long long num = strText.toULongLong();

    // 将num转换为十六进制字符串，并在后面补0到12位，然后反转字符串
    QString result = QString("%1").arg(num, 12, 16, QChar('0')).toUpper();
    for (int i = 10; i >= 0; i -= 2)
    {
        byte.append((char)result.mid(i, 2).toInt(&ok, 16));
    }
    byte.append(value);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::lampReset(int distributionAddress, int loopAddress, int deviceAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::lampReset, address is" << distributionAddress << loopAddress << deviceAddress;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x55;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x55);
    byte.append(0x11);

    procSendEvent(SendCanData, control, byte);
}


void CProcessDistribution::flowConfig(Directions direction, int delay, int distributionAddress, int loopAddress, int deviceAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::flowConfig, address is" << distributionAddress << loopAddress << deviceAddress <<
                ",flow is" << direction << delay;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x55;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x55);
    byte.append(0x12);
    byte.append(direction);
    byte.append(delay);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::clearFlow(int distributionAddress, int loopAddress, int deviceAddress)
{
    Q_UNUSED(loopAddress);
    Q_UNUSED(deviceAddress);
    m_lastSendTime = QDateTime::currentDateTime();
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["commandType"] = 0x55;
    QByteArray byte;
    int n = 0x00;
    byte.append(0x55);
    byte.append(0x12);
    byte.append(n);
    byte.append(n);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::flashControl(const bool bfalsh, int distributionAddress, int loopAddress, int deviceAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::flashControl, address is" << distributionAddress << loopAddress << deviceAddress <<
                ",flashControl is" << bfalsh;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x55;//led flash or not falsh
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x55);
    byte.append(0x13);
    byte.append(bfalsh ? 1 : 0);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::flowSync()
{
    qDebug() << "==CProcessDistribution::flowSync";
    m_lastSendTime = QDateTime::currentDateTime();
    QHash<QString, QVariant> control;
    control["recvAddr"] = 0x00;
    control["commandType"] = 0x55;
    QByteArray byte;
    byte.append(0x55);
    byte.append(0x14);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::lightControl(const bool enable, int distributionAddress, int loopAddress, int deviceAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::lightControl, address is" << distributionAddress << loopAddress << deviceAddress <<
                ",lightControl is" << enable;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x55;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x55);
    byte.append(0x15);
    byte.append(enable ? 1 : 0);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::directionControl(int value, int distributionAddress, int loopAddress, int deviceAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::directionControl, address is" << distributionAddress << loopAddress << deviceAddress <<
                ",directionControl is" << value;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x55;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x55);
    byte.append(0x16);
    byte.append(value);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::ChangeControl(int value, int distributionAddress, int loopAddress, int deviceAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::ChangeControl, address is" << distributionAddress << loopAddress << deviceAddress <<
                ",ChangeControl is" << value;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x55;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x55);
    byte.append(0x17);
    byte.append(value);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::lampchange(const bool enable, int distributionAddress, int loopAddress, int deviceAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::lampchange, address is" << distributionAddress << loopAddress << deviceAddress <<
                ",lampchange is" << enable;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x55;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x55);
    byte.append(0x17);
    byte.append(enable);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::lampdebugmode(const bool enable, int lampPWMmode, int distributionAddress, int loopAddress, int deviceAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::lampdebugmode, address is" << distributionAddress << loopAddress << deviceAddress <<
                ",lampdebugmode is" << enable << lampPWMmode;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x55;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x55);
    byte.append(0x18);
    byte.append(enable);
    byte.append(lampPWMmode);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::energysave(const bool enable, int distributionAddress, int loopAddress, int deviceAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::energysave, is" << enable;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;  /*0-broadcast*/
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x55;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    int n = 0x00;
    byte.append(0x55);
    byte.append(0x19);
    byte.append(enable ? 0x01 : 0x00);
    byte.append(0xA0);
    byte.append(n);
    byte.append(n);
    byte.append(n);

    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::lampdefaultdirection(int direction, int distributionAddress, int loopAddress, int deviceAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::lampdefaultdirection, address is" << distributionAddress << loopAddress << deviceAddress <<
                ",lampdefaultdirection is" << direction;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x55;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x55);
    byte.append(0x22);
    byte.append(direction);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::lampdefaultlighten(const bool enable, int distributionAddress, int loopAddress, int deviceAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::lampdefaultlighten, address is" << distributionAddress << loopAddress << deviceAddress <<
                ",lampdefaultlighten is" << enable;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x55;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x55);
    byte.append(0x28);
    byte.append(enable);
    procSendEvent(SendCanData, control, byte);
}


void CProcessDistribution::CloseBatteryOut(int value, int distributionAddress, int loopAddress, int deviceAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::CloseBatteryOut, address is" << distributionAddress << loopAddress << deviceAddress <<
                ",CloseBatteryOut is" << value;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x55;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x55);
    byte.append(0x46);
    byte.append(value);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::flashConfig(const bool bfalsh, int distributionAddress, int loopAddress, int deviceAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::flashConfig, address is" << distributionAddress << loopAddress << deviceAddress <<
                ",flashConfig is" << bfalsh;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x55;//led flash or not falsh
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x55);
    byte.append(0x49);
    byte.append(bfalsh ? 1 : 0);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::login(int distributionAddress,int loopAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::login" << distributionAddress;
    clearDistributionTime();
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;  /*0-broadcast*/
    control["loopAddr"] = loopAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x61;
    QByteArray byte;
    int n = 0x00;
    byte.append(0x61);
    byte.append(n);
    procSendEvent(SendCanData, control, byte);
    m_bLogin = true;
    m_timeLogin->stop();
    m_timeLogin->start(15000);
}

void CProcessDistribution::unLogin(int distributionAddress, int loopAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::unLogin" << distributionAddress;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;  /*0-broadcast*/
    control["loopAddr"] = loopAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x61;
    QByteArray byte;
    byte.append(0x61);
    byte.append(0xaa);
    procSendEvent(SendCanData, control, byte);
}
void CProcessDistribution::reset(int distributionAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::reset";
    m_lastSendTime = QDateTime::currentDateTime();
    clearDistributionTime();
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x62;
    QByteArray byte;
    byte.append(0x62);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::lightReset(int distributionAddress, int loopAddress, int deviceAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::lightReset, address is" << distributionAddress << loopAddress << deviceAddress;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x62;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x62);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::selfCheck(int distributionAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::selfCheck";
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x63;
    QByteArray byte;
    QDateTime datetime = QDateTime::currentDateTime(); //当前时间
    quint16 year = datetime.date().year();             //年
    quint8 month = datetime.date().month();			   //月
    quint8 day = datetime.date().day();				   //日
    quint8 hour = datetime.time().hour();			   //时
    quint8 minute = datetime.time().minute();          //分
    quint8 second = datetime.time().second();          //秒
    byte.append(0x63);
    byte.append(year%2000);
    byte.append(month);
    byte.append(day);
    byte.append(hour);
    byte.append(minute);
    byte.append(second);
    byte.append(0x11);
    byte.resize(8);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::monthCheck(bool enable, int distributionAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::monthCheck";
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x63;
    QByteArray byte;
    QDateTime datetime = QDateTime::currentDateTime(); //当前时间
    quint16 year = datetime.date().year();             //年
    quint8 month = datetime.date().month();			   //月
    quint8 day = datetime.date().day();				   //日
    quint8 hour = datetime.time().hour();			   //时
    quint8 minute = datetime.time().minute();          //分
    quint8 second = datetime.time().second();          //秒
    byte.append(0x63);
    byte.append(year%2000);
    byte.append(month);
    byte.append(day);
    byte.append(hour);
    byte.append(minute);
    byte.append(second);
    if(enable)
        byte.append(0x41);
    else
        byte.append(0x40);
    byte.resize(8);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::yearCheck(bool enable, int distributionAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::yearCheck";
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x63;
    QByteArray byte;
    QDateTime datetime = QDateTime::currentDateTime(); //当前时间
    quint16 year = datetime.date().year();             //年
    quint8 month = datetime.date().month();			   //月
    quint8 day = datetime.date().day();				   //日
    quint8 hour = datetime.time().hour();			   //时
    quint8 minute = datetime.time().minute();          //分
    quint8 second = datetime.time().second();          //秒
    byte.append(0x63);
    byte.append(year%2000);
    byte.append(month);
    byte.append(day);
    byte.append(hour);
    byte.append(minute);
    byte.append(second);
    if(enable)
        byte.append(0x21);
    else
        byte.append(0x20);
    byte.resize(8);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::emergency(const bool enable, int distributionAddress, int loopAddress, int deviceAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::emergency, is" << enable;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x64;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x64);
    byte.append(enable ? 0x01 : 0x00);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::heartBeat()
{
    qDebug() << "==CProcessDistribution::heartBeat";
    QHash<QString, QVariant> control;
    control["recvAddr"] = 0x00;
    control["commandType"] = 0x66;
    QByteArray byte;
    byte.append(0x66);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::lightTest(int distributionAddress, int loopAddress, int deviceAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::lightTest, address is" << distributionAddress << loopAddress << deviceAddress;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x6a;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x6a);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::readDeviceType()
{
    qDebug() << "==CProcessDistribution::readDeviceType";
//    uploadStatus(5, CGlobal::instance()->panelAddress(), distributionAddress, loopAddress, 0, OBJS_Online, 1, QDateTime::currentDateTime().toTime_t());
    m_lastSendTime = QDateTime::currentDateTime();
//    m_listLoop.append(loopAddress);
//    if(CGlobal::isDebug()) qDebug() << "CProcessDistribution::readDeviceType, loop id is " << loopAddress << " at " << distributionAddress;
    QHash<QString, QVariant> control;
    control["canportAdd"] = 0;
    control["recvAddr"] = 0;
    control["commandType"] = 0x77;
    control["loopAddr"] = 0;
    QByteArray byte;
    byte.append(0x77);
    procSendEvent(SendCanData, control, byte);
    m_bLogin = true;
    m_timeLogin->start(15000);
}

void CProcessDistribution::readDistributionLoopOut(int distributionAddress, int loopAddress)
{
    qDebug() << "==CProcessDistribution::readDistributionLoopOut, address is" << distributionAddress << loopAddress;
    QHash<QString, QVariant> control;
    control["canportAdd"] = 0;
    control["recvAddr"] = distributionAddress;
    control["commandType"] = 0x78;
    control["loopAddr"] = loopAddress;
    QByteArray byte;
    byte.append(0x78);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::ReadLoopOutStatus(int distributionAddress, int loopAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::ReadLoopOutStatus, address is" << distributionAddress << loopAddress;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x78;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = 0;
    QByteArray byte;
    byte.append(0x78);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::conttrolDistributionLoopOut(const bool enable,int distributionAddress, int loopAddress)
{
    qDebug() << "==CProcessDistribution::conttrolDistributionLoopOut, address is" << distributionAddress << loopAddress << ",loopOut is" << enable;
    QHash<QString, QVariant> control;
    control["canportAdd"] = 0;
    control["recvAddr"] = distributionAddress;
    control["commandType"] = 0x79;
    control["loopAddr"] = loopAddress;
    QByteArray byte;
    byte.append(0x79);
    byte.append(enable);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::SetLoopOutStatus(int value, int distributionAddress, int loopAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::SetLoopOutStatus, address is" << distributionAddress << loopAddress <<
                ",SetLoopOutStatus is" << value;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x79;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = 0;
    QByteArray byte;
    byte.append(0x79);
    byte.append(value);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::nologinSendToDistribution(int distributionAddress, int loopAddress, int deviceAddress, int typeID, QString strText, int canportAdd)
{
    bool ok;
    qDebug() << "==CProcessDistribution::nologinSendToDistribution, address is" << distributionAddress << loopAddress << deviceAddress << typeID;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x82;
    control["loopAddr"] = loopAddress;
    control["deviceAddr"] = deviceAddress;
    QByteArray byte;
    byte.append(0x82);

    // 将字符串转换为无前导零的 unsigned long long 类型
    unsigned long long num = strText.toULongLong();

    // 将num转换为十六进制字符串，并在后面补0到12位，然后反转字符串
    QString result = QString("%1").arg(num, 12, 16, QChar('0')).toUpper();
    for (int i = 10; i >= 0; i -= 2)
    {
        byte.append((char)result.mid(i, 2).toInt(&ok, 16));
    }
    byte.append(typeID);
    procSendEvent(SendCanData, control, byte);
}

void CProcessDistribution::removeFaultLamp(int distributionAddress, int loopAddress, int canportAdd)
{
    qDebug() << "==CProcessDistribution::removeFaultLamp" << distributionAddress;
    QHash<QString, QVariant> control;
    control["recvAddr"] = distributionAddress;
    control["loopAddr"] = loopAddress;
    control["canportAdd"] = canportAdd;
    control["commandType"] = 0x83;
    QByteArray byte;
    byte.append(0x83);
    procSendEvent(SendCanData, control, byte);
}


void CProcessDistribution::checkOutTime()
{
    m_time++;
    if(m_time >= 12)
    {
        m_bCommFault = true;
//        uploadStatus(5, CGlobal::instance()->panelAddress(), 0xFF, 0, 0, OBJS_DistributionCommunicationFault, 1, QDateTime::currentDateTime().toTime_t());
        CGlobal::instance()->processController()->setSystemFault(true);
    }
    else
    {
        if(m_bCommFault){
            m_bCommFault = false;
//            uploadStatus(5, CGlobal::instance()->panelAddress(), 0xFF, 0, 0, OBJS_DistributionCommunicationFault, 0, QDateTime::currentDateTime().toTime_t());
        }
        if(!CGlobal::instance()->processController()->isSystemFault())
            CGlobal::instance()->processController()->setSystemFault(false);
    }
}

void CProcessDistribution::timeOut()
{
    heartBeat();
    checkOutTime();
}

void CProcessDistribution::regeditOK()
{
    m_bLogin = false;
    CMsgNull msg;
    CGlobal::instance()->DealEvent(NCT_Relogin, msg.data(NCT_Relogin));
    m_timeLogin->stop();
//    CGlobal::instance()->processController()->setLedupdateTime150ms();
}

void CProcessDistribution::setMidLayerOption()
{
    qDebug() << "==CProcessDistribution::setMidLayerOption";
    QHash<QString,QVariant> control;
    QByteArray byte;
    const int infoType = 0x11;
    byte.append(0x70);
    byte.append(0x01);
    control["canportAdd"] = 0;
    control["recvAddr"] = 0;
    control["commandType"] = 0x70;
    control[INTERFACEBOARD_PORT_NUMBER_STR] = firstCardPort();
    control["sendAddr"] = CGlobal::instance()->panelAddress();
    control["dataLen"] = byte.size();
    emit sendEvent(name(), infoType, control, byte);
}

void CProcessDistribution::sendAckPacket(int canportAdd, int recvAddr, int loopAddr, int deviceAddr)
{
    qDebug() << "==CProcessDistribution::sendAckPacket";
    QHash<QString, QVariant> control;
    control["canportAdd"] = canportAdd;
    control["recvAddr"] = recvAddr;
    control["loopAddr"] = loopAddr;
    control["deviceAddr"] = deviceAddr;
    control["commandType"] = 0x60;
    QByteArray byte;
    int n = 0x60;
    byte.append(n);
    procSendEvent(SendCanData, control, byte);
}
