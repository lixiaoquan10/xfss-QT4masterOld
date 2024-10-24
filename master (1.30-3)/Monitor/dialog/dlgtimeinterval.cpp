#include "dlgtimeinterval.h"
#include "ui_dlgtimeinterval.h"
#include "cglobal.h"
#include "struct/cmsgcontrollerinfo.h"
#include "business/cclientbusiness.h"
#include "dm/cdbsqlite.h"
#include "dm/cproject.h"
#include "dm/ccanport.h"
#include "dm/cdistribution.h"
#include "dm/cloop.h"
#include "dm/cdevice.h"
#include "dialog/dlgdevices.h"
#include "widget/ctoptoolbar.h"
#include "dlgusetime.h"
#include "common.h"
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
#include <QProcess>
#include <QNetworkInterface>

dlgTimeInterval::dlgTimeInterval(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgTimeInterval)
{
    ui->setupUi(this);
    this->setWindowTitle("系统设置");
    setWindowFlags(Qt::FramelessWindowHint);
    setMinimumWidth(800);
    setMinimumHeight(480);
    CGlobal::instance()->m_HaveDialog = true;
    isThreadRunning = false;

    ui->PrintOCheckBox->setChecked(CGlobal::instance()->m_PrintOperation);
    ui->PrintOCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->PrintFCheckBox->setChecked(CGlobal::instance()->m_PrintFault);
    ui->PrintFCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->PrintECheckBox->setChecked(CGlobal::instance()->m_PrintWarning);
    ui->PrintECheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->SoundFCheckBox->setChecked(CGlobal::instance()->m_forbidFaultSound);
    ui->SoundFCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->SoundECheckBox->setChecked(CGlobal::instance()->m_forbidEmergencySound);
    ui->SoundECheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->MonthCheckBox->setChecked(CGlobal::instance()->m_forbidMonthcheck);
    ui->MonthCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->YearCheckBox->setChecked(CGlobal::instance()->m_forbidYearcheck);
    ui->YearCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->EnergyCheckBox->setChecked(CGlobal::instance()->m_energymode);
    ui->EnergyCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->DirectionTwinkleCheckBox->setChecked(CGlobal::instance()->m_SendDirectionAndTwinkleByOne);
    ui->DirectionTwinkleCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->TwinkleCheckBox->setChecked(CGlobal::instance()->m_TwinkleControl);
    ui->TwinkleCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->UCDirectionTwinkleCheckBox->setChecked(CGlobal::instance()->m_UCSendDirectionAndTwinkleByOne);
    ui->UCDirectionTwinkleCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->MonthTimeSlider->setValue(CGlobal::instance()->m_MonthCheckSpanSecond);
    ui->label_monthtime->setText(QString::number(CGlobal::instance()->m_MonthCheckSpanSecond));
    ui->version->setText(CGlobal::instance()->m_version);
    if(CGlobal::instance()->m_LiftTime > 1000)
        ui->UseTime->setText("永久");
    else
        ui->UseTime->setText(QString::number(CGlobal::instance()->m_LiftTime));

    ui->dateEdit->setCalendarPopup(true);
    ui->dateEdit->setDateTime(QDateTime::currentDateTime());
    ui->timeEdit->setCalendarPopup(true);
    ui->timeEdit->setDateTime(QDateTime::currentDateTime());
    ui->SwitchLayerBox->setCheckable(true);
    if(CGlobal::instance()->m_bCutLayer)
        ui->SwitchLayerBox->setChecked(true);
    else
        ui->SwitchLayerBox->setChecked(false);
    ui->SwitchLayerBox->hide();
    ui->SwitchLayerTime->setText(QString::number(CGlobal::instance()->m_nTimeCutLayer));
    //隐藏电源板数据
    ui->PowerBox->hide();
    m_getTmpip = "";
    m_getTmpmask = "";
    m_getTmpgateway = "";

    ui->m_masterStatus->setText("未连接");
    ui->m_cloudStatus->setText("未连接");
    CGlobal::instance()->m_DataServerIP = "192.168.112.217";
    CGlobal::instance()->m_DataServerPort = 6002;
    setHostIpPort("192.168.112.217",QString::number(6002));
}


dlgTimeInterval::~dlgTimeInterval()
{
    delete ui;
    CGlobal::instance()->setTimeInterval(NULL);
    CGlobal::instance()->m_HaveDialog = false;
}


//系统参数变化
void dlgTimeInterval::systemChanged()
{
    CMsgObjectStatus m_MsgObjectStatus;
    QByteArray data;

    CGlobal::instance()->m_PrintOperation = ui->PrintOCheckBox->isChecked();
    CGlobal::instance()->m_PrintFault = ui->PrintFCheckBox->isChecked();
    CGlobal::instance()->m_PrintWarning = ui->PrintECheckBox->isChecked();
    CGlobal::instance()->m_forbidFaultSound = ui->SoundFCheckBox->isChecked();
    CGlobal::instance()->m_forbidEmergencySound = ui->SoundECheckBox->isChecked();
    CGlobal::instance()->m_forbidMonthcheck = ui->MonthCheckBox->isChecked();
    CGlobal::instance()->m_forbidYearcheck = ui->YearCheckBox->isChecked();
    CGlobal::instance()->m_SendDirectionAndTwinkleByOne = ui->DirectionTwinkleCheckBox->isChecked();
    CGlobal::instance()->m_TwinkleControl = ui->TwinkleCheckBox->isChecked();
    CGlobal::instance()->m_UCSendDirectionAndTwinkleByOne = ui->UCDirectionTwinkleCheckBox->isChecked();
    CGlobal::instance()->m_MonthCheckSpanSecond = ui->MonthTimeSlider->value();
    CGlobal::instance()->m_version = ui->version->text();
    CGlobal::instance()->m_DataServerIP = ui->m_hostip->text();
    CGlobal::instance()->m_DataServerPort = ui->m_com->text().toInt();
    data.append(CGlobal::instance()->m_PrintOperation);
    data.append(CGlobal::instance()->m_PrintFault);
    data.append(CGlobal::instance()->m_PrintWarning);
    data.append(CGlobal::instance()->m_forbidFaultSound);
    data.append(CGlobal::instance()->m_forbidEmergencySound);
    data.append(CGlobal::instance()->m_forbidMonthcheck);
    data.append(CGlobal::instance()->m_forbidYearcheck);
    data.append(CGlobal::instance()->m_SendDirectionAndTwinkleByOne);
    data.append(CGlobal::instance()->m_UCSendDirectionAndTwinkleByOne);
    data.append(ui->MonthTimeSlider->value());
    m_MsgObjectStatus.ndata = data;
    m_MsgObjectStatus.nDataServerIP = ui->m_hostip->text();
    m_MsgObjectStatus.nDataServerPort = ui->m_com->text().toInt();

    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_SystemSet, &m_MsgObjectStatus);
    saveSystemStatus();
}

void dlgTimeInterval::saveSystemStatus()
{
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");;
    db.setDatabaseName(fileName);
    if(!db.open())
        return;
    QSqlQuery query(db);
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('PrintFault', '%1');").arg(CGlobal::instance()->m_PrintFault));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('PrintWarning', '%1');").arg(CGlobal::instance()->m_PrintWarning));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('PrintOperation', '%1');").arg(CGlobal::instance()->m_PrintOperation));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('SendDirectionAndTwinkleByOne', '%1');").arg(CGlobal::instance()->m_SendDirectionAndTwinkleByOne));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('UCSendDirectionAndTwinkleByOne', '%1');").arg(CGlobal::instance()->m_UCSendDirectionAndTwinkleByOne));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('ShieldFaultSound', '%1');").arg(!CGlobal::instance()->m_forbidFaultSound));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('ShieldWarningSound', '%1');").arg(!CGlobal::instance()->m_forbidEmergencySound));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('MonthCheckSpanSecond', '%1');").arg(CGlobal::instance()->m_MonthCheckSpanSecond));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('YearCheckSpanSecond', '%1');").arg(CGlobal::instance()->m_YearCheckSpanSecond));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('TwinkleControl', '%1');").arg(CGlobal::instance()->m_TwinkleControl));
    if(ui->m_hostip->text() != "")
        query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('DataServerIP', '%1');").arg(CGlobal::instance()->m_DataServerIP));
    if(ui->m_com->text() != "")
        query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('DataServerPort', '%1');").arg(CGlobal::instance()->m_DataServerPort));
    db.commit();
    db.close();
}


void dlgTimeInterval::on_PrintOCheckBox_clicked()
{
    systemChanged();
}

void dlgTimeInterval::on_PrintFCheckBox_clicked()
{
    systemChanged();
}

void dlgTimeInterval::on_PrintECheckBox_clicked()
{
    systemChanged();
}

void dlgTimeInterval::on_SoundFCheckBox_clicked()
{
    systemChanged();
}

void dlgTimeInterval::on_SoundECheckBox_clicked()
{
    systemChanged();
}

void dlgTimeInterval::on_MonthCheckBox_clicked()
{
    systemChanged();
}

void dlgTimeInterval::on_YearCheckBox_clicked()
{
    systemChanged();
}

void dlgTimeInterval::on_DirectionTwinkleCheckBox_clicked()
{
    systemChanged();
}

void dlgTimeInterval::on_UCDirectionTwinkleCheckBox_clicked()
{
    systemChanged();
}

void dlgTimeInterval::on_TwinkleCheckBox_clicked()
{
    systemChanged();
}

void dlgTimeInterval::on_EnergyCheckBox_clicked()
{
    if(CGlobal::instance()->m_energymode != ui->EnergyCheckBox->isChecked())
    {
        CGlobal::instance()->m_energymode = ui->EnergyCheckBox->isChecked();
        CCanport* canport = CGlobal::instance()->project()->canportById(CGlobal::instance()->panelAddress());
        if(!canport)
            return;
        QList<CDistribution*> list = canport->distributions();
        for(int i=0; i<list.count(); i++){
            CDistribution* distribution = list.at(i);
            QList<CLoop*> loops = distribution->loops();
            for(int i=0; i<loops.count(); i++){
                CLoop* loop = loops.at(i);
                QList<CDevice*> devices = loop->devices();
                for(int i=0; i<devices.count(); i++){
                    CDevice* device = devices.at(i);
                    if(device->deviceTypeId() < 150)
                    {
                        CMsgObjectStatus msgObjectStatus;
                        msgObjectStatus.nDisID = distribution->distributionAddress();
                        msgObjectStatus.nLoopID = loop->loopAdd();
                        msgObjectStatus.nDeviceID = device->deviceAdd();
                        msgObjectStatus.nStatusID = CGlobal::instance()->m_energymode;
                        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_EnergyMode, &msgObjectStatus);
                    }
                }
                devices.clear();
            }
            loops.clear();
        }
        list.clear();
    }
}

void dlgTimeInterval::on_MonthTimeSlider_valueChanged(int value)
{
    ui->label_monthtime->setText(QString::number(value));
    systemChanged();
}


void dlgTimeInterval::on_pushButton_clicked()
{
    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nDisID = 63;
    //读设备信息
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_ReadDeviceInfo,&msgObjectStatus);
}

void dlgTimeInterval::on_pushButton_UseTime_clicked()
{
    dlgUseTime UseTime;
    UseTime.exec();
}


void dlgTimeInterval::on_m_ButtonTimeSet_clicked()
{
    QDateTime combinedDateTime(ui->dateEdit->date(), ui->timeEdit->time());
    //    bool bRet = platform_sys_info_c::rtc_check();
    //    if(!bRet)
    //    {
    //        QMessageBox::information(NULL, "信息", "校验时间失败！");
    //    }

    //    bRet = platform_sys_info_c::set_rtc_time(combinedDateTime);
    //    if(!bRet)
    //    {
    //        QMessageBox::information(NULL, "信息", "修改时间失败！");
    //    }
    //    else
    //    {
    //        timeThread *thread = new timeThread(this);
    //        connect(thread, SIGNAL(timeFinished()), this, SLOT(slot_timeFinished()), Qt::QueuedConnection);
    //        thread->start();
    //    }
}

void dlgTimeInterval::slot_timeFinished()
{
    // 进入 dlgTimeInterval 界面的逻辑
    CGlobal::instance()->TimeInterval()->exec();
    QObject::sender()->deleteLater();  // 删除线程对象
}


void timeThread::run()
{
    QByteArray cmd = "hwclock --hctosys";
    int exitCode = QProcess::execute(cmd);
    QString errorString;

    if (exitCode == 0) {
        qDebug() << "下发指令成功！" << cmd;
    } else {
        errorString = "下发指令失败：" + QProcess::tr("Exit code: %1").arg(exitCode);
        qDebug() << errorString;
    }

    // 发送信号告知线程执行完毕
    emit timeFinished();
}

void dlgTimeInterval::on_pingButton_clicked()
{
    ui->m_pingResult->setText("测试中");
    pingThread *ping = new pingThread(this);
    connect(ping, SIGNAL(pingFinished(double)), this, SLOT(slot_pingFinished(double)));
    if(ui->m_pingIP->text() != NULL)
        ping->setpingIP(ui->m_pingIP->text());
    ping->start();
}

void dlgTimeInterval::slot_pingFinished(double successRate)
{
    // 进入 dlgTimeInterval 界面的逻辑
    CGlobal::instance()->TimeInterval()->exec();
    QObject::sender()->deleteLater();  // 删除线程对象
    QString result = QString::number(successRate) + "%";
    ui->m_pingResult->setText(result);
}

void pingThread::run()
{
    int receivedPackets = 0; // 收到的数据包数
    double successRate = 0.0;

    QString command = "ping -c 5 " + m_pingIP;
    QProcess process;
    process.start(command);
    process.waitForFinished();
    QString output = process.readAllStandardOutput();


    // 解析输出结果并发射信号
    QStringList lines = output.split("\n");// 存储Ping命令输出的每行结果
    foreach (QString line, lines) {
        if (line.contains("bytes from")) {
            receivedPackets++;
        }
    }
    successRate = (static_cast<double>(receivedPackets) / 5) * 100;
    emit pingFinished(successRate);  // 发送成功率信号
}


//设置IP地址
void dlgTimeInterval::setIPData()
{
    ipsetThread *ipset = new ipsetThread(this);
    connect(ipset, SIGNAL(ipsetFinished()), this, SLOT(slot_ipsetFinished()));
    ipset->start();

//    // 创建一个 QProcess 对象
//    QProcess process;
//    // 设置要执行的命令和参数
//    QString command = "sudo ifconfig enp1s0 192.168.3.101 netmask 255.255.255.0 up";

//    // 启动进程并传递命令
//    process.start(command);

//    // 等待进程启动并准备读取输出
//    if (process.waitForStarted()) {
//        // 输入密码，以换行符结尾
//        process.write("0000\n");
//        process.closeWriteChannel(); // 关闭写通道

//        // 等待进程完成
//        process.waitForFinished();

//        // 获取命令的输出结果
//        QByteArray output = process.readAllStandardOutput();
//        qDebug() << "Output:" << output;
//    }



//    QString command = QString("ip addr add %1/%2 dev %3").arg(ipAddress, subnetMask, "enp1s0");
//    QStringList args;
//    args << command;

//    QProcess process;
//    process.start("sudo", args);
//    process.waitForStarted();
//    process.write("0000\n");  // 替换为你的sudo密码
//    process.closeWriteChannel();
//    process.waitForFinished();

//    command = QString("sudo ip route replace default via %1").arg(gateway);
//    QProcess::execute(command);
//    command = QString("sudo ip route replace default via %1").arg(gateway);
//    QProcess::execute(command);
}

void dlgTimeInterval::slot_ipsetFinished()
{
    // 进入 dlgTimeInterval 界面的逻辑
    CGlobal::instance()->TimeInterval()->exec();
    QObject::sender()->deleteLater();  // 删除线程对象
}

void ipsetThread::run()
{
    // 创建一个 QProcess 对象
//    QProcess process;
//    // 设置要执行的命令和参数
//    QString command = "sudo -s ifconfig enp1s0 "
//            + CGlobal::instance()->TimeInterval()->m_getTmpip + " netmask "
//            + CGlobal::instance()->TimeInterval()->m_getTmpmask + " up && sudo route add default gw "
//            + CGlobal::instance()->TimeInterval()->m_getTmpgateway + " enp1s0";

//    // 启动进程并传递命令
//    process.start(command);

//    // 等待进程启动并准备读取输出
//    if (process.waitForStarted()) {
//        // 输入密码，以换行符结尾
//        process.write("0000\n");
//        process.closeWriteChannel(); // 关闭写通道

//        // 等待进程完成
//        if(process.waitForFinished())
//        {
//            emit ipsetFinished();  // 发送成功信号
//        }
//        // 获取命令的输出结果
//        QByteArray output = process.readAllStandardOutput();
//        qDebug() << "Output:" << output;
//    }

//    process.start("bash /home/xfss/root/setip.sh");
//    if(process.waitForFinished())
//    {
//        emit ipsetFinished();  // 发送成功信号
//    }

}

void dlgTimeInterval::on_pushButton_saveHostIP_clicked()
{
    QFile file("home/xfss/root/IP修改记录.txt");

    // 以追加写入模式打开文件，如果文件不存在则新建
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        if(isIpChanged())
        {
            QString str = "////" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") + "\n" +
                    "主机IP地址:" + ui->m_myip->text() + "\n" +
                    "主机子网掩码:" + ui->m_subnetMask->text() + "\n" +
                    "主机网关:" + ui->m_gateway->text() + "\n" +
                    "云平台IP地址:" + ui->m_hostip->text() + "\n" +
                    "云平台端口号:" + ui->m_com->text() + "\n" + "\n";
            out << str;  // 写入数据
        }

        file.close();
    }
    m_getTmpip = ui->m_myip->text();
    m_getTmpmask = ui->m_subnetMask->text();
    m_getTmpgateway = ui->m_gateway->text();
    setIPData();
}

void dlgTimeInterval::on_pushButton_saveCloudIP_clicked()
{
    QFile file("home/xfss/root/IP修改记录.txt");

    // 以追加写入模式打开文件，如果文件不存在则新建
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        if(isIpChanged())
        {
            QString str = "////" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") + "\n" +
                    "主机IP地址:" + ui->m_myip->text() + "\n" +
                    "主机子网掩码:" + ui->m_subnetMask->text() + "\n" +
                    "主机网关:" + ui->m_gateway->text() + "\n" +
                    "云平台IP地址:" + ui->m_hostip->text() + "\n" +
                    "云平台端口号:" + ui->m_com->text() + "\n" + "\n";
            out << str;  // 写入数据
        }

        file.close();
    }
    systemChanged();
}

//设置显示云平台IP端口号
void dlgTimeInterval::setHostIpPort(QString ip, QString port)
{
    ui->m_hostip->setText(ip);
    ui->m_com->setText(port);
}


//设置显示使用天数
void dlgTimeInterval::setUseTime(int usetime)
{
    ui->UseTime->setText(QString::number(usetime));
}
//设置显示连接设备通讯状态（云平台/主从）
void dlgTimeInterval::setComStatus(int status)
{
    if(status & 0x01)
    {
        ui->m_cloudStatus->setText("已连接");
    }
    else
    {
        ui->m_cloudStatus->setText("未连接");
        CGlobal::instance()->ClientBusiness()->m_cloudStateUploadFlag = false;
    }
    if(status & 0x02)
    {
        ui->m_masterStatus->setText("已连接");
    }
    else
    {
        ui->m_masterStatus->setText("未连接");
        CGlobal::instance()->ClientBusiness()->m_masterStateUploadFlag = false;
    }
    CGlobal::instance()->m_tcptype = status;
}

bool dlgTimeInterval::isCloudConnect()
{
    if(ui->m_cloudStatus->text() == "已连接")
        return true;
    else
        return false;
}

bool dlgTimeInterval::isMasterConnect()
{
    if(ui->m_masterStatus->text() == "已连接")
        return true;
    else
        return false;
}



bool dlgTimeInterval::isIpChanged()
{
    if(ui->m_myip->text() != m_getTmpip)
        return true;
    if(ui->m_subnetMask->text() != m_getTmpmask)
        return true;
    if(ui->m_gateway->text() != m_getTmpgateway)
        return true;
    if(ui->m_hostip->text() != CGlobal::instance()->m_DataServerIP)
        return true;
    if(ui->m_com->text().toInt() != CGlobal::instance()->m_DataServerPort)
        return true;
    return false;
}


//更新中间层软件版本
void dlgTimeInterval::updateVersion()
{
    ui->version->setText(CGlobal::instance()->m_version);
}

//更新数据库系统参数数据
void dlgTimeInterval::updateSqliteData()
{
    ui->PrintOCheckBox->setChecked(CGlobal::instance()->m_PrintOperation);
    ui->PrintOCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->PrintFCheckBox->setChecked(CGlobal::instance()->m_PrintFault);
    ui->PrintFCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->PrintECheckBox->setChecked(CGlobal::instance()->m_PrintWarning);
    ui->PrintECheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->SoundFCheckBox->setChecked(CGlobal::instance()->m_forbidFaultSound);
    ui->SoundFCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->SoundECheckBox->setChecked(CGlobal::instance()->m_forbidEmergencySound);
    ui->SoundECheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->MonthCheckBox->setChecked(CGlobal::instance()->m_forbidMonthcheck);
    ui->MonthCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->YearCheckBox->setChecked(CGlobal::instance()->m_forbidYearcheck);
    ui->YearCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->DirectionTwinkleCheckBox->setChecked(CGlobal::instance()->m_SendDirectionAndTwinkleByOne);
    ui->DirectionTwinkleCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->UCDirectionTwinkleCheckBox->setChecked(CGlobal::instance()->m_UCSendDirectionAndTwinkleByOne);
    ui->UCDirectionTwinkleCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->TwinkleCheckBox->setChecked(CGlobal::instance()->m_TwinkleControl);
    ui->TwinkleCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->MonthTimeSlider->setValue(CGlobal::instance()->m_MonthCheckSpanSecond);
    ui->label_monthtime->setText(QString::number(CGlobal::instance()->m_MonthCheckSpanSecond));
    if(CGlobal::instance()->m_LiftTime > 1000)
        ui->UseTime->setText("永久");
    else
        ui->UseTime->setText(QString::number(CGlobal::instance()->m_LiftTime));
}
//更新IP地址相关信息
void dlgTimeInterval::updateIPData()
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interface, interfaces) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp)
                && !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            QList<QNetworkAddressEntry> entries = interface.addressEntries();
            foreach (QNetworkAddressEntry entry, entries) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    qDebug() << "IPv4 Address:" << entry.ip().toString();
                    qDebug() << "Subnet Mask:" << entry.netmask().toString();
                    m_getTmpip = entry.ip().toString();
                    m_getTmpmask = entry.netmask().toString();

                    ui->m_myip->setText(m_getTmpip);
                    ui->m_subnetMask->setText(m_getTmpmask);

                }
            }
        }
    }
    QString gateway = getGateway();
    qDebug() << "Default Gateway:" << gateway;
    m_getTmpgateway = gateway;
    ui->m_gateway->setText(m_getTmpgateway);
    ui->m_hostip->setText(CGlobal::instance()->m_DataServerIP);
    ui->m_com->setText(QString::number(CGlobal::instance()->m_DataServerPort));
}
//获取主机网关
QString dlgTimeInterval::getGateway()
{
    QString gateway;

    QProcess process;
    process.start("ip route show default");
    process.waitForFinished();

    QString result = process.readAllStandardOutput();
    QStringList lines = result.split("\n");

    foreach (QString line, lines) {
        if (line.contains("default via")) {
            QStringList parts = line.split(" ");
            gateway = parts.at(2);
            break;
        }
    }
    return gateway;
}


//设置ip和时间以及ping测试是否可见
void dlgTimeInterval::setIpAndTimeHide(bool enable)
{
    if(enable)
    {
        ui->hostIPSetBox->hide();
        ui->cloudIPSetBox->hide();
        ui->comStatusBox->hide();
        ui->timeBox->hide();
        ui->pingBox->hide();
    }
    else
    {
        ui->hostIPSetBox->show();
        ui->cloudIPSetBox->show();
        ui->comStatusBox->show();
        ui->timeBox->show();
        ui->pingBox->show();
    }
}

