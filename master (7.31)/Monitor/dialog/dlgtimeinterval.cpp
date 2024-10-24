#include "ui_dlgtimeinterval.h"
#include "dlgtimeinterval.h"
#include "cglobal.h"

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

    QButtonGroup *startRuleGroup = new QButtonGroup(this);
    startRuleGroup->addButton(ui->allStartCheckBox);
    startRuleGroup->addButton(ui->onlyStartGroupCheckBox);
    startRuleGroup->addButton(ui->onlyStartPageCheckBox);
    startRuleGroup->setExclusive(true);
    ui->allStartCheckBox->setChecked(true);

    QButtonGroup *CanGroup = new QButtonGroup(this);
    CanGroup->addButton(ui->Can4CheckBox);
    CanGroup->addButton(ui->Can8CheckBox);
    CanGroup->setExclusive(true);
    ui->Can4CheckBox->setChecked(true);

    QButtonGroup *WLANGroup = new QButtonGroup(this);
    WLANGroup->addButton(ui->singleWLAN);
    WLANGroup->addButton(ui->doubleWLAN);
    WLANGroup->setExclusive(true);
    ui->singleWLAN->setChecked(true);

    ui->EnergyCheckBox->setChecked(CGlobal::instance()->m_energymode);
    ui->EnergyCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->DirectionTwinkleCheckBox->setChecked(CGlobal::instance()->m_SendDirectionAndTwinkleByOne);
    ui->DirectionTwinkleCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->TwinkleCheckBox->setChecked(CGlobal::instance()->m_TwinkleControl);
    ui->TwinkleCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->UCDirectionTwinkleCheckBox->setChecked(CGlobal::instance()->m_UCSendDirectionAndTwinkleByOne);
    ui->UCDirectionTwinkleCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->emergencyAfterDirectionCheckBox->setChecked(CGlobal::instance()->m_emergencyAfterDirection);
    ui->emergencyAfterDirectionCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->emergencyOnlyDistributionCheckBox->setChecked(CGlobal::instance()->m_emergencyOnlyDistribution);
    ui->emergencyOnlyDistributionCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");

    ui->autoPageCheckBox->setChecked(CGlobal::instance()->m_isAutoPage);
    ui->autoPageCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");

    ui->version_ledkey->setText(CGlobal::instance()->m_version_ledkey);
    ui->version_can1can2->setText(CGlobal::instance()->m_version_can1can2);
    ui->version_can3can4->setText(CGlobal::instance()->m_version_can3can4);
    ui->version_can5can6->setText(CGlobal::instance()->m_version_can5can6);
    ui->version_can7can8->setText(CGlobal::instance()->m_version_can7can8);
    int index = ui->comboBox_testLinkageCom->findText(CGlobal::instance()->m_baudRate);
    ui->comboBox_testLinkageCom->setCurrentIndex(index);
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

    m_getTmpip1 = "";
    m_getTmpgateway1 = "";
    m_getTmpmask1 = "255.255.255.0";

    m_getTmpip2 = "";
    m_getTmpgateway2 = "";
    m_getTmpmask2 = "255.255.255.0";

    m_dateTimeStr = "";

    QIcon icon(":/images/startinfo.png"); // 图标路径
    m_triangleButton = new TriangleButton(icon,this);
    // 设置按钮样式表，隐藏按钮的边框和背景
    m_triangleButton->setStyleSheet("border: none; background: none;");
    m_triangleButton->setGeometry(1320,610,32,32);
    connect(m_triangleButton, SIGNAL(clicked()), this, SLOT(on_triangleButton_clicked()));

//    ui->cloudIPSetBox->hide();
    ui->comStatusBox->hide();
    ui->m_cloudComStatus->setText("未连接");
//    ui->m_masterStatus->setText("未连接");
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
    qDebug() << "dlgTimeInterval::systemChanged"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgObjectStatus m_MsgObjectStatus;
    QByteArray data;
    CGlobal::instance()->m_DataServerIP = ui->m_hostip->text();
    CGlobal::instance()->m_DataServerPort = ui->m_com->text().toInt();
    data.append(CGlobal::instance()->m_forbidFaultSound);
    data.append(CGlobal::instance()->m_forbidEmergencySound);
    data.append(CGlobal::instance()->m_forbidMonthcheck);
    data.append(CGlobal::instance()->m_forbidYearcheck);
    data.append(CGlobal::instance()->m_nCanNumber);
    data.append(CGlobal::instance()->m_MonthCheckSpanSecond);
    data.append(CGlobal::instance()->m_YearCheckSpanSecond);
    m_MsgObjectStatus.ndata = data;
    m_MsgObjectStatus.nDataServerIP = CGlobal::instance()->m_DataServerIP;
    m_MsgObjectStatus.nDataServerPort = CGlobal::instance()->m_DataServerPort;

    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_SystemSet, &m_MsgObjectStatus);
    saveSystemStatus();
}

void dlgTimeInterval::saveSystemStatus()
{
    qDebug() << "dlgTimeInterval::saveSystemStatus"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");;
    db.setDatabaseName(fileName);
    if(!db.open())
        return;
    QSqlQuery query(db);
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('TwinkleControl', '%1');").arg(CGlobal::instance()->m_TwinkleControl));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('SendDirectionAndTwinkleByOne', '%1');").arg(CGlobal::instance()->m_SendDirectionAndTwinkleByOne));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('UCSendDirectionAndTwinkleByOne', '%1');").arg(CGlobal::instance()->m_UCSendDirectionAndTwinkleByOne));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('EmergencyAfterDirection', '%1');").arg(CGlobal::instance()->m_emergencyAfterDirection));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('EmergencyOnlyDistribution', '%1');").arg(CGlobal::instance()->m_emergencyOnlyDistribution));
    if(ui->m_hostip->text() != "")
        query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('DataServerIP', '%1');").arg(CGlobal::instance()->m_DataServerIP));
    if(ui->m_com->text() != "")
        query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('DataServerPort', '%1');").arg(CGlobal::instance()->m_DataServerPort));
    db.commit();
    db.close();
}

void dlgTimeInterval::on_TwinkleCheckBox_clicked()
{
    CGlobal::instance()->m_TwinkleControl = ui->TwinkleCheckBox->isChecked();
    saveSystemStatus();
}

void dlgTimeInterval::on_DirectionTwinkleCheckBox_clicked()
{
    CGlobal::instance()->m_SendDirectionAndTwinkleByOne = ui->DirectionTwinkleCheckBox->isChecked();
    saveSystemStatus();
}

void dlgTimeInterval::on_UCDirectionTwinkleCheckBox_clicked()
{
    CGlobal::instance()->m_UCSendDirectionAndTwinkleByOne = ui->UCDirectionTwinkleCheckBox->isChecked();
    saveSystemStatus();
}

void dlgTimeInterval::on_emergencyAfterDirectionCheckBox_clicked()
{
    CGlobal::instance()->m_emergencyAfterDirection = ui->emergencyAfterDirectionCheckBox->isChecked();
    saveSystemStatus();
}

void dlgTimeInterval::on_emergencyOnlyDistributionCheckBox_clicked()
{
    CGlobal::instance()->m_emergencyOnlyDistribution = ui->emergencyOnlyDistributionCheckBox->isChecked();
    saveSystemStatus();
}

void dlgTimeInterval::on_allStartCheckBox_clicked()
{
    CGlobal::instance()->m_nStartRule = RULE_AllStart;
}

void dlgTimeInterval::on_onlyStartPageCheckBox_clicked()
{
    CGlobal::instance()->m_nStartRule = RULE_OnlyPage;
}

void dlgTimeInterval::on_onlyStartGroupCheckBox_clicked()
{
    CGlobal::instance()->m_nStartRule = RULE_OnlyGroup;
}

void dlgTimeInterval::on_Can4CheckBox_clicked()
{
    qDebug() << "dlgTimeInterval::on_Can4CheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");;
    db.setDatabaseName(fileName);
    if(!db.open())
        return;
    QSqlQuery query(db);
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('CanNumber', '%1');").arg(4));
    db.commit();
    db.close();
}

void dlgTimeInterval::on_Can8CheckBox_clicked()
{
    qDebug() << "dlgTimeInterval::on_Can8CheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");;
    db.setDatabaseName(fileName);
    if(!db.open())
        return;
    QSqlQuery query(db);
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('CanNumber', '%1');").arg(8));
    db.commit();
    db.close();
}

void dlgTimeInterval::on_singleWLAN_clicked()
{
    qDebug() << "dlgTimeInterval::on_singleWLAN_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_isDoubleWLAN = false;
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");;
    db.setDatabaseName(fileName);
    if(!db.open())
        return;
    QSqlQuery query(db);
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('DoubleWLAN', '%1');").arg(false));
    db.commit();
    db.close();
}

void dlgTimeInterval::on_doubleWLAN_clicked()
{
    qDebug() << "dlgTimeInterval::on_doubleWLAN_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_isDoubleWLAN = true;
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");;
    db.setDatabaseName(fileName);
    if(!db.open())
        return;
    QSqlQuery query(db);
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('DoubleWLAN', '%1');").arg(true));
    db.commit();
    db.close();
}

void dlgTimeInterval::on_EnergyCheckBox_clicked()
{
    qDebug() << "dlgTimeInterval::on_EnergyCheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(CGlobal::instance()->m_energymode != ui->EnergyCheckBox->isChecked())
    {
        CGlobal::instance()->m_energymode = ui->EnergyCheckBox->isChecked();
        for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
        {
            CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
            if(!canport)
                return;
            QList<CDistribution*> list = canport->distributions();
            for(int i=0; i<list.count(); i++){
                CDistribution* distribution = list.at(i);
                if(!distribution)
                    continue;
                QList<CLoop*> loops = distribution->loops();
                for(int i=0; i<loops.count(); i++){
                    CLoop* loop = loops.at(i);
                    if(!loop)
                        continue;
                    QList<CDevice*> devices = loop->devices();
                    for(int i=0; i<devices.count(); i++){
                        CDevice* device = devices.at(i);
                        if(!device)
                            continue;
                        if(device->deviceTypeId() < 150)
                        {
                            CMsgObjectStatus msgObjectStatus;
                            msgObjectStatus.nCanportAdd = device->canportAdd();
                            msgObjectStatus.nDisID = device->distributionAdd();
                            msgObjectStatus.nLoopID = device->loopAdd();
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
}


void dlgTimeInterval::on_pushButton_can1can2_clicked()
{
    qDebug() << "dlgTimeInterval::on_pushButton_can1can2_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = 3;
    msgObjectStatus.nDisID = 63;
    //读设备信息
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_ReadDeviceInfo,&msgObjectStatus);
}

void dlgTimeInterval::on_pushButton_can3can4_clicked()
{
    qDebug() << "dlgTimeInterval::on_pushButton_can3can4_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = 5;
    msgObjectStatus.nDisID = 63;
    //读设备信息
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_ReadDeviceInfo,&msgObjectStatus);
}

void dlgTimeInterval::on_pushButton_can5can6_clicked()
{
    qDebug() << "dlgTimeInterval::on_pushButton_can5can6_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = 7;
    msgObjectStatus.nDisID = 63;
    //读设备信息
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_ReadDeviceInfo,&msgObjectStatus);
}

void dlgTimeInterval::on_pushButton_can7can8_clicked()
{
    qDebug() << "dlgTimeInterval::on_pushButton_can7can8_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = 9;
    msgObjectStatus.nDisID = 63;
    //读设备信息
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_ReadDeviceInfo,&msgObjectStatus);
}


void dlgTimeInterval::on_pushButton_UseTime_clicked()
{
    qDebug() << "dlgTimeInterval::on_pushButton_UseTime_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    dlgUseTime useTime;
    useTime.exec();
}

void dlgTimeInterval::on_autoPageCheckBox_clicked()
{
    qDebug() << "dlgTimeInterval::on_autoPageCheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_isAutoPage = ui->autoPageCheckBox->isChecked();
    CGlobal::instance()->designTreeView()->setAutoPage(CGlobal::instance()->m_isAutoPage);
}

void dlgTimeInterval::on_triangleButton_clicked()
{
    qDebug() << "dlgTimeInterval::on_triangleButton_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(CGlobal::instance()->m_adminParamTime > QDateTime::currentDateTime())
    {
        CGlobal::instance()->AdminParamPage()->updateSqliteData();
        // 替换界面
        CGlobal::instance()->TopToolBar()->changeCenterFrameWidget(CGlobal::instance()->AdminParamPage());
    }
    else
    {
        dlgadminParam adminParam;
        adminParam.exec();
    }
}

void dlgTimeInterval::on_pushButton_testLinkageCom_clicked()
{
    qDebug() << "dlgTimeInterval::on_pushButton_testLinkageCom_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QFont font;
    font.setPointSize(12); // 设置字体大小为12像素
    // 在QMessageBox中设置字体
    QMessageBox messageBox;
    messageBox.setFont(font);
    QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
    messageBox.setStyleSheet(styleSheet);
    messageBox.setIcon(QMessageBox::NoIcon);
    messageBox.setWindowTitle("修改波特率");
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Cancel);
    messageBox.button(QMessageBox::Yes)->setText("是");
    messageBox.button(QMessageBox::Cancel)->setText("取消");
    QString log = "确定要修改与火灾报警控制器通讯的波特率吗?";
    messageBox.setText(log);
    int ret = messageBox.exec();
    if(ret == QMessageBox::Yes)
    {
        CGlobal::instance()->m_baudRate = ui->comboBox_testLinkageCom->currentText();
        QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");;
        db.setDatabaseName(fileName);
        if(!db.open())
            return;
        QSqlQuery query(db);
        query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('BaudRate', '%1');").arg(CGlobal::instance()->m_baudRate));
        db.commit();
        db.close();
        CMsgObjectStatus m_MsgObjectStatus;
        m_MsgObjectStatus.strdata = CGlobal::instance()->m_baudRate;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_LinkageComBaudRate, &m_MsgObjectStatus);
    }
    else if(ret == QMessageBox::Cancel)
    {
        int index = ui->comboBox_testLinkageCom->findText(CGlobal::instance()->m_baudRate);
        ui->comboBox_testLinkageCom->setCurrentIndex(index);
    }
}

void dlgTimeInterval::on_testLinkageComButton_clicked()
{
    qDebug() << "dlgTimeInterval::on_testLinkageComButton_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    ui->m_testLinkageComResult->setText("测试中");
    CMsgNull m_MsgNUll;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_TestLinkageCom, &m_MsgNUll);
    QTimer::singleShot(5000, this, SLOT(slot_testComResult()));
}

void dlgTimeInterval::slot_testComResult()
{
    qDebug() << "dlgTimeInterval::slot_testComResult"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(ui->m_testLinkageComResult->text() == "测试中")
        ui->m_testLinkageComResult->setText("失败！");
}

void dlgTimeInterval::setTestLinkageComResult(QString str)
{
    qDebug() << "dlgTimeInterval::setTestLinkageComResult"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    ui->m_testLinkageComResult->setText(str);
}

void dlgTimeInterval::on_m_ButtonTimeSet_clicked()
{
    qDebug() << "dlgTimeInterval::on_m_ButtonTimeSet_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QDateTime combinedDateTime(ui->dateEdit->date(), ui->timeEdit->time());
    m_dateTimeStr = combinedDateTime.toString("yyyy-MM-dd hh:mm:ss");
    timeThread *thread = new timeThread(this);
    connect(thread, SIGNAL(timeFinished()), this, SLOT(slot_timeFinished()), Qt::QueuedConnection);
    thread->start();
}

void dlgTimeInterval::slot_timeFinished()
{
    qDebug() << "dlgTimeInterval::slot_timeFinished"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 进入 dlgTimeInterval 界面的逻辑
    CGlobal::instance()->TimeInterval()->exec();
    QThread *finishedThread = qobject_cast<QThread*>(sender());
    if (finishedThread) {
        // 等待线程结束，确保线程执行已完成
        finishedThread->wait();
        // 删除线程对象
        finishedThread->deleteLater(); // 推荐使用 deleteLater()，安全删除线程对象
    }
}


void timeThread::run()
{
    qDebug() << "timeThread::run"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 创建一个 QProcess 对象
    QProcess process;
    // 设置要执行的 shell 命令
    QStringList args;
    args << "/home/xfss/root/SystemSet/settime.sh" << CGlobal::instance()->TimeInterval()->m_dateTimeStr;

    // 启动 shell 脚本文件并传递参数
    process.start("bash", args);
    if(process.waitForFinished())
    {
        emit timeFinished();  // 发送成功信号
    }
}

void dlgTimeInterval::on_pingButton_clicked()
{
    qDebug() << "dlgTimeInterval::on_pingButton_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    ui->m_pingResult->setText("测试中");
    pingThread *ping = new pingThread(this);
    connect(ping, SIGNAL(pingFinished(double)), this, SLOT(slot_pingFinished(double)), Qt::QueuedConnection);
    if(ui->m_pingIP->text() != NULL)
        ping->setpingIP(ui->m_pingIP->text());
    ping->start();
}

void dlgTimeInterval::slot_pingFinished(double successRate)
{
    qDebug() << "dlgTimeInterval::slot_pingFinished"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QThread *finishedThread = qobject_cast<QThread*>(sender());
    if (finishedThread) {
        // 等待线程结束，确保线程执行已完成
        finishedThread->wait();
        // 删除线程对象
        finishedThread->deleteLater(); // 推荐使用 deleteLater()，安全删除线程对象
    }
    // 进入 dlgTimeInterval 界面的逻辑
    CGlobal::instance()->TimeInterval()->exec();
    QString result = QString::number(successRate) + "%";
    ui->m_pingResult->setText(result);
}

void pingThread::run()
{
    qDebug() << "pingThread::run"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int receivedPackets = 0; // 收到的数据包数
    double successRate = 0.0;

    // 创建一个 QProcess 对象
    QProcess process;

    // 设置要执行的 shell 命令
    QStringList args;
    args << "/home/xfss/root/SystemSet/ping.sh" << m_pingIP;

    // 启动 shell 脚本文件并传递参数
    process.start("bash", args);

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
void dlgTimeInterval::setWLANData()
{
    qDebug() << "dlgTimeInterval::setWLANData"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    ipsetThread *ipset = new ipsetThread(this);
    connect(ipset, SIGNAL(ipsetFinished()), this, SLOT(slot_ipsetFinished()), Qt::QueuedConnection);
    ipset->start();
}

void dlgTimeInterval::slot_ipsetFinished()
{
    qDebug() << "dlgTimeInterval::slot_ipsetFinished"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 进入 dlgTimeInterval 界面的逻辑
    CGlobal::instance()->TimeInterval()->exec();
    QThread *finishedThread = qobject_cast<QThread*>(sender());
    if (finishedThread) {
        // 等待线程结束，确保线程执行已完成
        finishedThread->wait();
        // 删除线程对象
        finishedThread->deleteLater(); // 推荐使用 deleteLater()，安全删除线程对象
    }
}

void ipsetThread::run()
{
    qDebug() << "ipsetThread::run"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 创建一个 QProcess 对象
    QProcess process;
    // 设置要执行的 shell 命令
    QString scriptPath = "/home/xfss/root/SystemSet/setip.sh";
    // 添加要传递给脚本的参数（如果需要的话）
    QStringList arguments;
    arguments << CGlobal::instance()->TimeInterval()->m_getTmpip1 << CGlobal::instance()->TimeInterval()->m_getTmpmask1
              << CGlobal::instance()->TimeInterval()->m_getTmpgateway1 << CGlobal::instance()->TimeInterval()->m_getTmpip2
              << CGlobal::instance()->TimeInterval()->m_getTmpmask2 << CGlobal::instance()->TimeInterval()->m_getTmpgateway2;

    // 启动 shell 脚本文件并传递参数
    process.start("bash", QStringList() << scriptPath << arguments);
    if(process.waitForFinished())
    {
        emit ipsetFinished();  // 发送成功信号
        QFile file("/etc/network/interfaces"); // 指定要修改的文件路径
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            qDebug() << "无法打开文件";
            return;
        }

        QTextStream in(&file);
        QStringList lines;
        bool shouldAddExtraContent = true; // 指示是否需要添加额外内容
        while (!in.atEnd())
        {
            QString line = in.readLine();
            if (line.contains("iface enp1s0 inet static"))
            {
                shouldAddExtraContent = false;
                lines.append("iface enp1s0 inet static");
                lines.append("address " + CGlobal::instance()->TimeInterval()->m_getTmpip1);
                lines.append("netmask " + CGlobal::instance()->TimeInterval()->m_getTmpmask1);
                if(CGlobal::instance()->TimeInterval()->m_getTmpgateway1 != "")
                    lines.append("gateway " + CGlobal::instance()->TimeInterval()->m_getTmpgateway1);
                lines.append("dns-nameservers 8.8.8.8");
            }
            else if(line.contains("iface enp4s0 inet static"))
            {
                shouldAddExtraContent = false;
                lines.append("");
                lines.append("auto enp4s0");
                lines.append("iface enp4s0 inet static");
                lines.append("address " + CGlobal::instance()->TimeInterval()->m_getTmpip2);
                lines.append("netmask " + CGlobal::instance()->TimeInterval()->m_getTmpmask2);
                if(CGlobal::instance()->TimeInterval()->m_getTmpgateway2 != "")
                    lines.append("gateway " + CGlobal::instance()->TimeInterval()->m_getTmpgateway2);
                lines.append("dns-nameservers 8.8.8.8");
            }
            if(shouldAddExtraContent)
            {
                lines.append(line);
            }

        }

        file.resize(0);
        QTextStream out(&file);
        for (int i = 0; i < lines.size(); ++i)
        {
            out << lines.at(i) << "\n"; // 使用 at() 方法获取元素
        }

        file.close();
    }

}


void dlgTimeInterval::on_pushButton_saveWlan_clicked()
{
    qDebug() << "dlgTimeInterval::on_pushButton_saveWlan_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QFile file("home/xfss/root/logfile/IPChangeRecord.txt");

    // 以追加写入模式打开文件，如果文件不存在则新建
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        if(isWLANChanged())
        {
            QString str = "////" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") + "\n" +
                    "网口1地址:" + ui->m_myip1->text() + "\n" +
                    "网口1网关:" + ui->m_gateway1->text() + "\n";
            if(CGlobal::instance()->m_isDoubleWLAN)
            {
                str = str + "网口2地址:" + ui->m_myip2->text() + "\n" +
                        "网口2网关:" + ui->m_gateway2->text() + "\n" + "\n";
            }
            else
            {
                str = str + "\n";
            }
            out << str;  // 写入数据
        }

        file.close();
    }
    m_getTmpip1 = ui->m_myip1->text();
    setWLANData();
}


void dlgTimeInterval::on_pushButton_saveCloudIP_clicked()
{
    qDebug() << "dlgTimeInterval::on_pushButton_saveCloudIP_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QFile file("home/xfss/root/logfile/IPChangeRecord.txt");

    // 以追加写入模式打开文件，如果文件不存在则新建
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        if(isHostIPChanged())
        {
            QString str = "////" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") + "\n" +
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
    qDebug() << "dlgTimeInterval::setHostIpPort"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    ui->m_hostip->setText(ip);
    ui->m_com->setText(port);
}


//设置显示使用天数
void dlgTimeInterval::setUseTime(int usetime)
{
    qDebug() << "dlgTimeInterval::setUseTime"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    ui->UseTime->setText(QString::number(usetime));
}
//设置显示连接设备通讯状态（云平台/主从）
void dlgTimeInterval::setComStatus(int status)
{
    qDebug() << "dlgTimeInterval::setComStatus"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(status & 0x01)
    {
        ui->m_cloudComStatus->setText("已连接");
//        ui->m_cloudStatus->setText("已连接");
    }
    else
    {
        ui->m_cloudComStatus->setText("未连接");
//        ui->m_cloudStatus->setText("未连接");
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
    qDebug() << "dlgTimeInterval::isCloudConnect"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
//    if(ui->m_cloudStatus->text() == "已连接")
    if(ui->m_cloudComStatus->text() == "已连接")
        return true;
    else
        return false;
}

bool dlgTimeInterval::isMasterConnect()
{
    qDebug() << "dlgTimeInterval::isMasterConnect"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(ui->m_masterStatus->text() == "已连接")
        return true;
    else
        return false;
}



bool dlgTimeInterval::isWLANChanged()
{
    qDebug() << "dlgTimeInterval::isWLANChanged"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(ui->m_myip1->text() != m_getTmpip1)
        return true;
    if(ui->m_gateway1->text() != m_getTmpgateway1)
        return true;
    if(ui->m_myip2->text() != m_getTmpip2)
        return true;
    if(ui->m_gateway2->text() != m_getTmpgateway2)
        return true;
    return false;
}


bool dlgTimeInterval::isHostIPChanged()
{
    qDebug() << "dlgTimeInterval::isHostIPChanged"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(ui->m_hostip->text() != CGlobal::instance()->m_DataServerIP)
        return true;
    if(ui->m_com->text() != QString::number(CGlobal::instance()->m_DataServerPort))
        return true;
    return false;
}

//更新高级参数按钮位置
void dlgTimeInterval::updateAdminPuttonGeometry()
{
    qDebug() << "dlgTimeInterval::updateAdminPuttonGeometry"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(CGlobal::instance()->DataProcessing()->isStatusTab())
        m_triangleButton->setGeometry(1320,450,32,32);
    else
        m_triangleButton->setGeometry(1320,610,32,32);
}

//更新中间层软件版本
void dlgTimeInterval::updateVersion()
{
    qDebug() << "dlgTimeInterval::updateVersion"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    ui->version_ledkey->setText(CGlobal::instance()->m_version_ledkey);
    ui->version_can1can2->setText(CGlobal::instance()->m_version_can1can2);
    ui->version_can3can4->setText(CGlobal::instance()->m_version_can3can4);
    ui->version_can5can6->setText(CGlobal::instance()->m_version_can5can6);
    ui->version_can7can8->setText(CGlobal::instance()->m_version_can7can8);
}

//更新数据库系统参数数据
void dlgTimeInterval::updateSqliteData()
{
    qDebug() << "dlgTimeInterval::updateSqliteData"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    ui->DirectionTwinkleCheckBox->setChecked(CGlobal::instance()->m_SendDirectionAndTwinkleByOne);
    ui->DirectionTwinkleCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->UCDirectionTwinkleCheckBox->setChecked(CGlobal::instance()->m_UCSendDirectionAndTwinkleByOne);
    ui->UCDirectionTwinkleCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->TwinkleCheckBox->setChecked(CGlobal::instance()->m_TwinkleControl);
    ui->TwinkleCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->emergencyAfterDirectionCheckBox->setChecked(CGlobal::instance()->m_emergencyAfterDirection);
    ui->emergencyAfterDirectionCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->emergencyOnlyDistributionCheckBox->setChecked(CGlobal::instance()->m_emergencyOnlyDistribution);
    ui->emergencyOnlyDistributionCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    if(CGlobal::instance()->m_nCanNumber == 4)
    {
        ui->Can4CheckBox->setChecked(true);
        ui->label_CAN5_6->hide();
        ui->version_can5can6->hide();
        ui->pushButton_can5can6->hide();
        ui->label_CAN7_8->hide();
        ui->version_can7can8->hide();
        ui->pushButton_can7can8->hide();
        ui->VersionBox->setFixedHeight(140);
        ui->cloudIPSetBox->move(320, 310);
    }
    else if(CGlobal::instance()->m_nCanNumber == 8)
    {
        ui->Can8CheckBox->setChecked(true);
    }
    if(CGlobal::instance()->m_isDoubleWLAN)
    {
        ui->doubleWLAN->setChecked(true);
    }
    else
    {
        ui->singleWLAN->setChecked(true);
    }
    int index = ui->comboBox_testLinkageCom->findText(CGlobal::instance()->m_baudRate);
    ui->comboBox_testLinkageCom->setCurrentIndex(index);
    if(CGlobal::instance()->m_LiftTime > 1000)
        ui->UseTime->setText("永久");
    else
        ui->UseTime->setText(QString::number(CGlobal::instance()->m_LiftTime));

    ui->dateEdit->setCalendarPopup(true);
    ui->dateEdit->setDateTime(QDateTime::currentDateTime());
    ui->timeEdit->setCalendarPopup(true);
    ui->timeEdit->setDateTime(QDateTime::currentDateTime());
}
//更新WLAN地址相关信息
void dlgTimeInterval::updateWLANData()
{
    qDebug() << "dlgTimeInterval::updateWLANData"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interface, interfaces) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp)
                && !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            QList<QNetworkAddressEntry> entries = interface.addressEntries();
            QString interfaceName = interface.name();
            foreach (QNetworkAddressEntry entry, entries) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
                {
                    qDebug() << "IPv4 Address:" << entry.ip().toString();
                    if(interfaceName == "enp1s0")
                    {
                        m_getTmpip1 = entry.ip().toString();
                        ui->m_myip1->setText(m_getTmpip1);
                    }
                    else if(interfaceName == "enp4s0")
                    {
                        m_getTmpip2 = entry.ip().toString();
                        ui->m_myip2->setText(m_getTmpip2);
                    }
                }
            }
        }
    }
    // 获取 enp1s0 接口的网关
    m_getTmpgateway1 = getGateway("enp1s0");
    // 获取 enp4s0 接口的网关
    m_getTmpgateway2 = getGateway("enp4s0");
    if(m_getTmpgateway1 != "" || m_getTmpgateway2 != "")
    {
        if(m_getTmpgateway1 != "" && m_getTmpgateway2 != "")
        {
            ui->m_gateway1->setText(m_getTmpgateway1);
            ui->m_gateway2->setText(m_getTmpgateway2);
            ui->wlanBox->setFixedHeight(210);
            ui->testLinkageComBox->move(40, 350);
        }
        else if(m_getTmpgateway1 != "")
        {
            ui->m_gateway1->setText(m_getTmpgateway1);
            ui->m_gateway2->hide();
            ui->label_gateway2->hide();
            ui->wlanBox->setFixedHeight(180);
            ui->testLinkageComBox->move(40, 320);
        }
        else if(m_getTmpgateway2 != "")
        {
            ui->m_gateway2->setText(m_getTmpgateway2);
            ui->m_gateway1->hide();
            ui->label_gateway1->hide();
            ui->wlanBox->setFixedHeight(180);
            ui->testLinkageComBox->move(40, 320);
        }
    }
    else
    {
        ui->m_gateway2->hide();
        ui->label_gateway2->hide();
        ui->wlanBox->setFixedHeight(180);
        ui->testLinkageComBox->move(40, 320);
    }
    if(!CGlobal::instance()->m_isDoubleWLAN)
    {
        ui->m_myip2->hide();
        ui->label_myip2->hide();
        ui->m_gateway2->hide();
        ui->label_gateway2->hide();
        ui->wlanBox->setFixedHeight(150);
        ui->testLinkageComBox->move(40, 290);
    }
    ui->m_hostip->setText(CGlobal::instance()->m_DataServerIP);
    ui->m_com->setText(QString::number(CGlobal::instance()->m_DataServerPort));
}
//获取主机网关
QString dlgTimeInterval::getGateway(const QString &interfaceName)
{
    qDebug() << "dlgTimeInterval::getGateway"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString gateway;

    QProcess process;
    // 构建命令，只获取特定接口的网关信息
    QString command = "ip route show dev " + interfaceName + " default";
    process.start(command);
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
    qDebug() << "dlgTimeInterval::setIpAndTimeHide"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(enable)
    {
        ui->wlanBox->hide();
        ui->cloudIPSetBox->hide();
        ui->comStatusBox->hide();
        ui->timeBox->hide();
        ui->pingBox->hide();
    }
    else
    {
        ui->wlanBox->show();
        ui->cloudIPSetBox->show();
        ui->comStatusBox->show();
        ui->timeBox->show();
        ui->pingBox->show();
    }
}

