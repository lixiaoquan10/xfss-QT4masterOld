#include "cprocessserver.h"
#include "cprocesssocket.h"
#include "cglobal.h"
#include "struct/cmsgnull.h"
#include "struct/cmsglog.h"
#include "struct/cmsgdeviceinfo.h"
#include "struct/cmsgobjectstatus.h"
#include "struct/cmsgfirepoint.h"
#include "struct/cmsgchecktime.h"
#include "struct/cmsgevacuationgroup.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QApplication>
#include <QHostAddress>
#include <QMessageBox>
#include <QDebug>
#include <QDomDocument>
#include <QXmlStreamReader>

CprocessServer::CprocessServer()
{
    m_tcpServer = new QTcpServer(this);
    m_timer = new QTimer();
    m_heartTimer = new QTimer();
    m_disconnectTimer = new QTimer();
    m_controlMasterTxtTimer = new QTimer();
    m_isMasterConnected = false;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_serverlisten()));
    connect(m_heartTimer, SIGNAL(timeout()), this, SLOT(slot_heartBeat()));
    connect(m_disconnectTimer, SIGNAL(timeout()), this, SLOT(slot_handleDisconnected()));
    connect(m_controlMasterTxtTimer, SIGNAL(timeout()), this, SLOT(slot_controlMasterTxtFileSize()));
    m_timer->start(5000);
    m_controlMasterTxtTimer->start(5000);
}

CprocessServer::~CprocessServer()
{
    delete m_tcpServer;
    delete m_tcpSocket;
    delete m_timer;
    delete m_heartTimer;
    delete m_disconnectTimer;
}

void CprocessServer::slot_serverlisten()
{
    qDebug() << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if (!m_tcpServer->listen(QHostAddress::Any, 8654))
    {
        qDebug() << "Unable to start the server: " << m_tcpServer->errorString() << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
        return;
    }
    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(slot_handleNewConnection()));
}

// 处理新的连接请求
void CprocessServer::slot_handleNewConnection()
{
    m_tcpSocket = m_tcpServer->nextPendingConnection();  // 接受新连接
    m_isMasterConnected = true;
    m_timer->stop();
    int type;
    if(CGlobal::instance()->processSocket()->m_isCloudConnected)
        type = 0x03;  //云平台、客户端已连接
    else
        type = 0x02;  //客户端已连接
    CGlobal::instance()->TimeInterval()->setComStatus(type);
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(slot_readClientData()));  // 监听客户端数据
    connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(slot_handleDisconnected()));
    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(m_tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(handleBytesWritten(qint64)));

    m_heartTimer->start(4000);
    //清空数据监听文本
    CGlobal::instance()->processSocket()->clearTxtData("/root/masterData.txt");
}

void CprocessServer::onSocketError(QAbstractSocket::SocketError error)
{
    if (m_tcpSocket) {
        qDebug() << "Socket error occurred: " << m_tcpSocket->errorString();
        qDebug() << "Error code: " << error;
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
        QString dataWithTimestamp;
        dataWithTimestamp = "(Socket error occurred:" + timestamp + ")" + '\n' + m_tcpSocket->errorString();
        CGlobal::instance()->processSocket()->saveDataToTxt("/root/masterData.txt", dataWithTimestamp);
    }
}

void CprocessServer::handleBytesWritten(qint64 bytes)
{
    qDebug() << "Bytes written: " << bytes;
}

void CprocessServer::slot_heartBeat()
{
    CGlobal::instance()->ClientBusiness()->serverHeartbeat();
}

// 读取客户端数据
void CprocessServer::slot_readClientData()
{
    if (m_tcpSocket) {
        QByteArray array = m_tcpSocket->readAll();  // 读取收到的数据
        // 处理收到的数据
        qDebug() << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
        qDebug() << array;

        // 将已接收到的数据缓存
        m_receiveBuff.append(array);

        // 查找开始符和结束符的位置
        int start = m_receiveBuff.indexOf("<?");
        int end = m_receiveBuff.indexOf("</R>");

        while (start != -1 && end != -1) {
            // 提取完整的帧数据（包括开始符和结束符）
            QByteArray frameData = m_receiveBuff.mid(start, end - start + 4);

            // 处理帧数据
            processReceiveData(frameData);

            // 移除已处理的帧数据
            m_receiveBuff.remove(0, end + 4);

            // 继续查找下一个开始符和结束符的位置
            start = m_receiveBuff.indexOf("<?");
            end = m_receiveBuff.indexOf("</R>");
        }
    }
}


void CprocessServer::processReceiveData(QByteArray array)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
    QString dataWithTimestamp;
    dataWithTimestamp = "(Receive:" + timestamp + ")" + '\n' + QString::fromUtf8(array);
    //收到客户端数据保存到txt文件
    CGlobal::instance()->processSocket()->saveDataToTxt("/root/masterData.txt", dataWithTimestamp);
    CMsgObjectStatus xmlObjectStatus;
    QDomDocument doc;
    doc.setContent(array);
    QDomElement root = doc.documentElement();//根节点

    if(root.tagName() == "R")
    {
        QDomNode firstNode = root.firstChild();//第一个子节点
        while (!firstNode.isNull())
        {
            if (firstNode.isElement()) {
                QDomElement element = firstNode.toElement();
                qDebug() << element.tagName();
                //查询CAN设备
                if(element.tagName() == "QueryCanDevice")
                {
                    CGlobal::instance()->ClientBusiness()->serverCanDeviceInfo();
                }
                //查询通道
                else if(element.tagName() == "QueryChannel")
                {
                    CGlobal::instance()->ClientBusiness()->serverChannelInfo();
                }
                //查询灯具
                else if(element.tagName() == "QueryLamp")
                {
                    CGlobal::instance()->ClientBusiness()->serverLampInfo();
                }
                //查询着火点
                else if(element.tagName() == "QueryFirePoint")
                {
                    CGlobal::instance()->ClientBusiness()->serverFirePoint();
                }
                //查询灯具与着火点关联
                else if(element.tagName() == "QueryLampToFirePoint")
                {
                    CGlobal::instance()->ClientBusiness()->serverLampToFirePoint();
                }
                //查询灯具连接信息
                else if(element.tagName() == "QueryLampConnection")
                {
                    CGlobal::instance()->ClientBusiness()->serverLampConnection();
                }
                //查询记录信息
                else if(element.tagName() == "QueryEvent")
                {
                    xmlObjectStatus.strdata = element.attribute("EventType");
                    xmlObjectStatus.nStartTime = QDate::fromString(element.attribute("StartTime"), "yyyy-MM-dd");
                    xmlObjectStatus.nEndTime = QDate::fromString(element.attribute("EndTime"), "yyyy-MM-dd");
                    CGlobal::instance()->ClientBusiness()->serverEventList(&xmlObjectStatus);
                }
                //查询所有状态
                else if(element.tagName() == "QueryAllState")
                {
                    CGlobal::instance()->ClientBusiness()->serverAllState();
                }

                //查询布局页面
                else if(element.tagName() == "QueryPageInfo")
                {
                    CGlobal::instance()->ClientBusiness()->serverPageInfo();
                }
                //查询背景图片
                else if(element.tagName() == "QueryPicture")
                {
                    xmlObjectStatus.strdata = element.attribute("PN");
                    CGlobal::instance()->ClientBusiness()->serverPicture(&xmlObjectStatus);
                }
                //查询灯具坐标
                else if(element.tagName() == "QueryLampCoordinate")
                {
                    CGlobal::instance()->ClientBusiness()->serverLampCoordinate();
                }
                //查询着火点坐标
                else if(element.tagName() == "QueryFirePointCoordinate")
                {
                    CGlobal::instance()->ClientBusiness()->serverFirePointCoordinate();
                }
                //查询灯具软件信息
                else if(element.tagName() == "QueryLampSoftwareInfo")
                {
//                    xmlObjectStatus.nValue = element.attribute("ID").toInt();
                    int ChannelInfoID = 0;
                    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
                    CDBSqlite db(fileName);
                    if(!db.open())
                        return;
                    QSqlQuery querydevice = db.query(QString("SELECT l.id,l.channelinfoid,l.address from LampInfo l "
                                                             "left join lamptype r on l.lamptypeid=r.id "
                                                             "where l.id=%1"
                                                       ).arg(element.attribute("ID").toInt()));
                    QSqlRecord recorddevice = querydevice.record();
                    int fieldChannelInfoID = recorddevice.indexOf("channelinfoid");
                    int fieldLampAddress = recorddevice.indexOf("address");
                    while(querydevice.next())
                    {
                        ChannelInfoID = querydevice.value(fieldChannelInfoID).toInt();
                        xmlObjectStatus.nDeviceID = querydevice.value(fieldLampAddress).toInt();
                    }

                    QSqlQuery query = db.query(QString("SELECT m.id,m.candeviceinfoid,m.loop,c.canport,c.address from ChannelInfo m "
                                                       "left join CanDeviceInfo c on m.candeviceinfoid=c.id "
                                                       "where m.id=%1"
                                                       ).arg(ChannelInfoID));
                    QSqlRecord record = query.record();
                    int fieldLoopAddress = record.indexOf("loop");
                    int fieldCanportAddress = record.indexOf("canport");
                    int fieldDistributionAddress = record.indexOf("address");
                    while(query.next())
                    {
                        xmlObjectStatus.nLoopID = query.value(fieldLoopAddress).toInt();
                        xmlObjectStatus.nDisID = query.value(fieldDistributionAddress).toInt();
                        xmlObjectStatus.nCanportAdd = query.value(fieldCanportAddress).toInt();
                    }
                    db.close();
                    //读灯具软件版本信息
                    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_ReadDeviceInfo,&xmlObjectStatus);
                    //服务端回复见软件版本信息接收部分
                }
                //查询集中电源软件信息
                else if(element.tagName() == "QueryDeviceSoftwareInfo")
                {
//                    xmlObjectStatus.nValue = element.attribute("ID").toInt();
                    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
                    CDBSqlite db(fileName);
                    if(!db.open())
                        return;
                    QSqlQuery query = db.query(QString("SELECT m.id,m.canport,m.address from CanDeviceInfo m "
                                                       "where m.id=%1"
                                                       ).arg(element.attribute("ID").toInt()));
                    QSqlRecord record = query.record();
                    int fieldDistributionAddress = record.indexOf("address");
                    int fieldCanportAddress = record.indexOf("canport");
                    while(query.next())
                    {
                        xmlObjectStatus.nDisID = query.value(fieldDistributionAddress).toInt();
                        xmlObjectStatus.nCanportAdd = query.value(fieldCanportAddress).toInt()+2;
                    }
                    db.close();
                    //读集中电源软件版本信息
                    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_ReadDeviceInfo,&xmlObjectStatus);
                    //服务端回复软件版本信息
                    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(xmlObjectStatus.nCanportAdd);

                    CDistribution* distribution = canport->distributionByAddress(xmlObjectStatus.nDisID);
                    if(distribution)
                    {
                        CGlobal::instance()->ClientBusiness()->serverDeviceSoftwareInfo(distribution);
                    }                }
                //查询集中电源运行参数
                else if(element.tagName() == "QueryDeviceRealtimeData")
                {
                    xmlObjectStatus.nValue = element.attribute("ID").toInt();
                    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
                    CDBSqlite db(fileName);
                    if(!db.open())
                        return;
                    QSqlQuery query = db.query(QString("SELECT id,canport,address from CanDeviceInfo"
                                                       "where id=%1"
                                                       ).arg(element.attribute("ID").toInt()));
                    QSqlRecord record = query.record();
                    int fieldDistributionAddress = record.indexOf("address");
                    int fieldCanport = record.indexOf("canport");
                    while(query.next())
                    {
                        xmlObjectStatus.nDisID = query.value(fieldDistributionAddress).toInt();
                        xmlObjectStatus.nCanportAdd = query.value(fieldCanport).toInt() + 2;
                    }
                    db.close();
                    //读集中电源运行参数
                    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_PowerInfo,&xmlObjectStatus);
                    //服务端回复运行参数
                    CCanport* canport = CGlobal::instance()->controller()->canportById(xmlObjectStatus.nCanportAdd);
                    if(!canport)
                        return;
                    CDistribution* distribution = canport->distributionByAddress(xmlObjectStatus.nDisID);
                    CGlobal::instance()->ClientBusiness()->serverDeviceRealtimeData(distribution);                }

                //主机故障应答
                else if(element.tagName() == "HostFaultUploadConfirm")
                {
                    //
                }
                //CAN设备故障应答
                else if(element.tagName() == "CentralizedPowerStateUploadConfirm")
                {
                    //
                }
                //灯具状态应答
                else if(element.tagName() == "LampStateUploadConfirm")
                {
                    //
                }
                //开关量应急应答
                else if(element.tagName() == "EmergencyInputUploadConfirm")
                {
                    //
                }
                //手动应急应答
                else if(element.tagName() == "ManualLaunchUploadConfirm")
                {
                    //
                }
                //着火点报警应答
                else if(element.tagName() == "FirePointWarningUploadConfirm")
                {
                    //
                }
                //灯具指向应答
                else if(element.tagName() == "LampDirectionUploadConfirm")
                {
                    //
                }
                //主机控制命令
                else if(element.tagName() == "HostControl")
                {
                    xmlObjectStatus.strdata = element.attribute("ControlType");
                    CGlobal::instance()->ClientBusiness()->serverHostControl(&xmlObjectStatus);
                }
                //主机控制命令
                else if(element.tagName() == "PerformReset")
                {
                    xmlObjectStatus.strdata = "复位";
                    CGlobal::instance()->ClientBusiness()->serverHostControl(&xmlObjectStatus);
                }
                //主机控制命令
                else if(element.tagName() == "PerformLaunch")
                {
                    xmlObjectStatus.strdata = "启动";
                    CGlobal::instance()->ClientBusiness()->serverHostControl(&xmlObjectStatus);
                }
                //上传使能开关
                else if(element.tagName() == "SetAutoStateUpload")
                {
                    xmlObjectStatus.strdata = element.attribute("AutoUpload");
                    CGlobal::instance()->ClientBusiness()->serverSetAutoStateUpload(element.attribute("AutoUpload"));;
                }
                //心跳
                else if(element.tagName() == "HeartbeatUploadConfirm")
                {
                    m_disconnectTimer->stop();
                    m_disconnectTimer->start(30000);
                }

            }
            firstNode = firstNode.nextSibling();
        }
    }
}

void CprocessServer::replyXmlMsg(QByteArray data)
{
    char lastData = data.at(data.length() - 1);  // 获取最后一个数据
    if (!data.isEmpty() && lastData == '\n') // 判断最后一个数据是否是换行符
    {
        data.chop(1);  // 去掉最后一个数据（字节）
    }
    qDebug() << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    qDebug() << data;

    qint64 datalength = m_tcpSocket->write(data);
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
    QString dataWithTimestamp;
    qint64 bytesWaiting = m_tcpSocket->bytesToWrite();
    if (bytesWaiting > 0)
    {
        if(m_tcpSocket->waitForBytesWritten())
        {
            dataWithTimestamp = "(Send:" + timestamp +" length:"+ QString::number(datalength) + ")" + '\n' + QString::fromUtf8(data);
            //发送客户端数据保存到txt文件
            CGlobal::instance()->processSocket()->saveDataToTxt("/root/masterData.txt", dataWithTimestamp);
        }
        else
        {
            dataWithTimestamp = "(Send Failed:" + timestamp +" length:"+ QString::number(datalength) + ")" + '\n' + QString::fromUtf8(data);
            //发送客户端数据保存到txt文件
            CGlobal::instance()->processSocket()->saveDataToTxt("/root/masterData.txt", dataWithTimestamp);
        }
    }
}


// 处理客户端断开连接
void CprocessServer::slot_handleDisconnected()
{
    if (m_tcpSocket && m_isMasterConnected) {
        m_isMasterConnected = false;
        m_tcpSocket->deleteLater(); // 删除客户端socket对象
        m_timer->start(1000);
        m_heartTimer->stop();
        int type;
        if(CGlobal::instance()->processSocket()->m_isCloudConnected)
            type = 0x01;  //云平台已连接、客户端未连接
        else
            type = 0;     //都未连接
        CGlobal::instance()->TimeInterval()->setComStatus(type);
    }
}

void CprocessServer::slot_controlMasterTxtFileSize()
{
    //设置文档大小最大为1M
    CGlobal::instance()->processSocket()->controlTxtFileSize("/root/masterData.txt",1024*1024);
}
