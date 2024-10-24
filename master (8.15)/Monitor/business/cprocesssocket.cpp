#include "cprocesssocket.h"
#include "cprocessserver.h"
#include "cglobal.h"
#include "struct/cmsgnull.h"
#include "struct/cmsglog.h"
#include "struct/cmsgdeviceinfo.h"
#include "struct/cmsgobjectstatus.h"
#include "struct/cmsgfirepoint.h"
#include "struct/cmsgchecktime.h"
#include "struct/cmsgevacuationgroup.h"

#include <QTimer>
#include <QApplication>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <QDebug>
#include <QDomDocument>
#include <QXmlStreamReader>

CprocessSocket::CprocessSocket()
{
    m_isCloudConnected = false;
    m_currentType = 0;
    m_socket = new QTcpSocket(this);
    m_cloudTimer = new QTimer();
    m_disconnectTimer = new QTimer();
    m_heartTimer = new QTimer();
    m_controlCloudTxtTimer = new QTimer();
    m_port = 0;
    connect(m_cloudTimer, SIGNAL(timeout()), this, SLOT(slot_reconnect()));
    connect(m_heartTimer, SIGNAL(timeout()), this, SLOT(slot_heartBeat()));
    connect(m_disconnectTimer, SIGNAL(timeout()), this, SLOT(slot_disconnect()));
    connect(m_controlCloudTxtTimer, SIGNAL(timeout()), this, SLOT(slot_controlCloudTxtFileSize()));

    connect(m_socket,SIGNAL(readyRead()),this,SLOT(slot_readyRead()));

    connect(m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(slot_stateChanged(QAbstractSocket::SocketState)));
    connect(m_socket, SIGNAL(connected()), this, SLOT(slot_connect()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(slot_disconnect()));
    m_cloudTimer->start(1000);
    m_controlCloudTxtTimer->start(5000);
}

CprocessSocket::~CprocessSocket()
{
    delete m_socket;
    delete m_cloudTimer;
    delete m_heartTimer;
    delete m_disconnectTimer;
}

void CprocessSocket::slot_reconnect()
{
    m_socket->connectToHost(QHostAddress(m_ip), m_port);
    m_cloudTimer->stop();
}

void CprocessSocket::setip(QString ip)
{
    if(m_ip == ip)
        return;
    m_ip = ip;
    m_socket->disconnectFromHost();
}
void CprocessSocket::setport(int port)
{
    if(m_port == port)
        return;
    m_port = port;
    m_socket->disconnectFromHost();
}

void CprocessSocket::setCloudConnect(bool enable)
{
    if(enable)
        m_cloudTimer->start(1000);
    else
        m_cloudTimer->stop();
}

void CprocessSocket::slot_heartBeat()
{
    CGlobal::instance()->ClientBusiness()->XmlHeartbeat();
}

void CprocessSocket::slot_connect()
{
    int type;
    qDebug("server connect sucess!");
    m_cloudTimer->stop();
    m_heartTimer->start(15000);
    m_isCloudConnected = true;
    if(CGlobal::instance()->processServer()->m_isMasterConnected)
        type = 0x03;     //云平台、客户端已连接
    else
        type = 0x01;     //云平台已连接、客户端未连接
    CGlobal::instance()->TimeInterval()->setComStatus(type);
    CGlobal::instance()->ClientBusiness()->XmlApplyForProjectID();
    //清空数据监听文本
    clearTxtData("/home/xfss/root/cloudData.txt");
}

void CprocessSocket::slot_disconnect()
{
    int type;
    qDebug() << "server disconnect!" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_cloudTimer->start(1000);
    m_heartTimer->stop();
    if(m_isCloudConnected)
    {
        m_isCloudConnected = false;
        if(CGlobal::instance()->processServer()->m_isMasterConnected)
            type = 0x02;     //云平台未连接、客户端已连接
        else
            type = 0;     //云平台、客户端未连接
        CGlobal::instance()->TimeInterval()->setComStatus(type);
    }
}

void CprocessSocket::slot_stateChanged(QAbstractSocket::SocketState state)
{
    int type;
    if(state == QAbstractSocket::ConnectedState)
    {
        qDebug("server connect sucess!");
        m_cloudTimer->stop();
        m_heartTimer->start(15000);
        m_isCloudConnected = true;
        if(CGlobal::instance()->processServer()->m_isMasterConnected)
            type = 0x03;     //云平台、客户端已连接
        else
            type = 0x01;     //云平台已连接、客户端未连接
        CGlobal::instance()->TimeInterval()->setComStatus(type);
        CGlobal::instance()->ClientBusiness()->XmlApplyForProjectID();

        //清空数据监听文本
        clearTxtData("/home/xfss/root/cloudData.txt");
    }
    else if(state == QAbstractSocket::UnconnectedState)
    {
        qDebug() << "server disconnect!" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
        m_cloudTimer->start(1000);
        m_heartTimer->stop();
        if(m_isCloudConnected)
        {
            m_isCloudConnected = false;
            if(CGlobal::instance()->processServer()->m_isMasterConnected)
                type = 0x02;     //云平台未连接、客户端已连接
            else
                type = 0;        //云平台、客户端未连接
            CGlobal::instance()->TimeInterval()->setComStatus(type);
        }
    }
}

void CprocessSocket::sendXmlMsg(QByteArray data)
{
    char lastData = data.at(data.length() - 1);  // 获取最后一个数据
    if (!data.isEmpty() && lastData == '\n') // 判断最后一个数据是否是换行符
    {
        data.chop(1);  // 去掉最后一个数据（字节）
    }
    qDebug() << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    qDebug() << data;

    qint64 datalength = m_socket->write(data);
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
    QString dataWithTimestamp;
    if(m_socket->waitForBytesWritten())
    {
        dataWithTimestamp = "(Send:" + timestamp +" length:"+ QString::number(datalength) + ")" + '\n' + QString::fromUtf8(data);
        //发送云平台数据保存到txt文件
        saveDataToTxt("/home/xfss/root/cloudData.txt", dataWithTimestamp);
    }
    else
    {
        dataWithTimestamp = "(Send Failed:" + timestamp + ")";
        //发送云平台数据保存到txt文件
        saveDataToTxt("/home/xfss/root/cloudData.txt", dataWithTimestamp);
    }
}

void CprocessSocket::slot_readyRead()
{
    QByteArray array = m_socket->readAll();
    qDebug() << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    qDebug(array);

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
    QString dataWithTimestamp;
    dataWithTimestamp = "(Receive:" + timestamp + ")" + '\n' + QString::fromUtf8(array);
    //收到云平台数据保存到txt文件
    saveDataToTxt("/home/xfss/root/cloudData.txt", dataWithTimestamp);

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
                //心跳
                if (element.tagName() == "Heartbeat") {
                    m_disconnectTimer->stop();
                    m_disconnectTimer->start(30000);
                }
                //返回项目识别号
                else if (element.tagName() == "ReturnNewProjectID") {
                    CGlobal::instance()->m_ControllerID = element.attribute("ProjectID");
                    CGlobal::instance()->setControllerName(element.attribute("ProjectName"));
                    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
                    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");;
                    db.setDatabaseName(fileName);
                    if(!db.open())
                        return;
                    QSqlQuery query(db);
                    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('ProjectID', '%1');").arg(CGlobal::instance()->m_ControllerID));
                    db.commit();
                    db.close();
                    CGlobal::instance()->ClientBusiness()->XmlUploadDeviceInfo();
                }
                //返回已申请过的项目识别号
                else if(element.tagName() == "ReturnExistingProjectID")
                {
                    CGlobal::instance()->m_ControllerID = element.attribute("ProjectID");
                    CGlobal::instance()->setControllerName(element.attribute("ProjectName"));
                    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
                    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");;
                    db.setDatabaseName(fileName);
                    if(!db.open())
                        return;
                    QSqlQuery query(db);
                    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('ProjectID', '%1');").arg(CGlobal::instance()->m_ControllerID));
                    db.commit();
                    db.close();
                    CGlobal::instance()->ClientBusiness()->XmlUploadDeviceInfo();
                }
                //上传应急疏散监控主机信息应答
                else if(element.tagName() == "UploadDeviceInfoConfirm")
                {
                    //
                }
                //上传应急疏散监控主机信息失败应答
                else if(element.tagName() == "UploadDeviceInfoFail")
                {
                    //
                }
                //查询CAN设备
                else if(element.tagName() == "QueryCanDevice")
                {
                    CGlobal::instance()->ClientBusiness()->XmlCanDeviceInfo();
                }
                //查询通道
                else if(element.tagName() == "QueryChannelInfo")
                {
                    CGlobal::instance()->ClientBusiness()->XmlChannelInfo();
                }
                //查询灯具
                else if(element.tagName() == "QueryLampInfo")
                {
                    CGlobal::instance()->ClientBusiness()->XmlLampInfo();
                }
                //查询布局页面
                else if(element.tagName() == "QueryLayoutPage")
                {
                    CGlobal::instance()->ClientBusiness()->XmlLayoutPage();
                }
                //查询背景图片
                else if(element.tagName() == "QueryPicture")
                {
                    xmlObjectStatus.strdata = element.attribute("Picture");
                    CGlobal::instance()->ClientBusiness()->XmlPicture(&xmlObjectStatus);
                }
                //查询灯具坐标
                else if(element.tagName() == "QueryLampCoordinate")
                {
                    CGlobal::instance()->ClientBusiness()->XmlLampCoordinate();
                }
                //查询着火点
                else if(element.tagName() == "QueryFirePointInfo")
                {
                    CGlobal::instance()->ClientBusiness()->XmlFirePoint();
                }
                //查询着火点坐标
                else if(element.tagName() == "QueryFirePointCoordinate")
                {
                    CGlobal::instance()->ClientBusiness()->XmlFirePointCoordinate();
                }
                //查询灯具与着火点关联
                else if(element.tagName() == "QueryLampToFirePoint")
                {
                    CGlobal::instance()->ClientBusiness()->XmlLampToFirePoint();
                }
                //查询灯具默认方向
                else if(element.tagName() == "QueryLampDefaultDirection")
                {
                    CGlobal::instance()->ClientBusiness()->XmlLampDefaultDirection();
                }
                //查询主机电参量
                else if(element.tagName() == "QueryHostElectricParameter")
                {
                    CGlobal::instance()->ClientBusiness()->XmlHostElectricParameter();                }
                //查询CAN设备电参量
                else if(element.tagName() == "QueryCanDeviceElectricParameter")
                {
                    CGlobal::instance()->ClientBusiness()->XmlCanDeviceElectricParameter(element.attribute("ID").toInt());                }
                //上传使能开关
                else if(element.tagName() == "EnableUpload")
                {
                    CGlobal::instance()->ClientBusiness()->XmlEnableUpload(element.attribute("Enable"));
                }
                //主机故障应答
                else if(element.tagName() == "HostStateUploadConfirm")
                {
                    //
                }
                //CAN设备故障应答
                else if(element.tagName() == "CanDeviceStateUploadConfirm")
                {
                    //
                }
                //通道故障应答
                else if(element.tagName() == "ChannelStateUploadConfirm")
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
                else if(element.tagName() == "UploadFirePointWarningConfirm")
                {
                    //
                }
                //灯具指向应答
                else if(element.tagName() == "UploadLampDirectionConfirm")
                {
                    //
                }
                //复位应答
                else if(element.tagName() == "ResetDeclareConfirm")
                {
                    //
                }
                //主机控制命令
                else if(element.tagName() == "HostControl")
                {
                    xmlObjectStatus.strdata = element.attribute("ControlType");
                    CGlobal::instance()->ClientBusiness()->XmlHostControl(&xmlObjectStatus);
                }
                //灯具控制命令
                else if(element.tagName() == "LampControl")
                {
                    xmlObjectStatus.strdata = element.attribute("ControlParam");
                    xmlObjectStatus.nDeviceID = element.attribute("LampInfoID").toInt();
                    CGlobal::instance()->ClientBusiness()->XmlLampControl(&xmlObjectStatus);
                }
                //查询所有状态
                else if(element.tagName() == "QueryAllState")
                {
                    CGlobal::instance()->ClientBusiness()->XmlQueryAllState();
                }
            }
            firstNode = firstNode.nextSibling();
        }
    }
}

//保存监听数据文本
void CprocessSocket::saveDataToTxt(const QString& filePath, const QString& data)
{
    QFile file(filePath);

    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << data << '\n' << '\n';
        file.close();
        qDebug() << "Data saved to TXT file: " << filePath;
    }
    else
    {
        qDebug() << "Failed to open TXT file for writing: " << filePath;
    }
}

//清空监听数据文本
void CprocessSocket::clearTxtData(const QString& filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly  | QIODevice::Truncate))
    {
        qDebug() << "Open File Failed!";
        return;
    }
    // 关闭文件
    file.close();
    qDebug() << "File Cleared";
}


void CprocessSocket::slot_controlCloudTxtFileSize()
{
    //设置文档大小最大为1M
    controlTxtFileSize("/home/xfss/root/cloudData.txt",1024*1024);
}

void CprocessSocket::controlTxtFileSize(const QString& filePath, qint64 maxSize)
{
    QFile file(filePath);

    if (file.size() > maxSize) {
        // 读取文件内容
        QString content;
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            content = stream.readAll();
            file.close();
        } else {
            qDebug() << "Failed to open TXT file for reading: " << filePath;
            return;
        }

        // 删除最早的数据
        QStringList lines = content.split("\n", QString::SkipEmptyParts);
        while (file.size() > maxSize && !lines.isEmpty()) {
            lines.removeFirst();
        }

        // 重新写入文件
        if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            QTextStream stream(&file);
            QString newData = lines.join("\n\n");
            stream << newData;
            file.close();
        } else {
            qDebug() << "Failed to open TXT file for writing: " << filePath;
        }
    }
}

