#include "ui_cprogramdistributionview.h"
#include "cprogramdistributionview.h"
#include "cglobal.h"


CProgramDistributionView::CProgramDistributionView(QWidget *parent) :
    QWidget(parent), ui(new Ui::CProgramdistributionView)
{
    ui->setupUi(this);
    CGlobal::instance()->setProgramDistributionView(this);

    ui->comboBox_powermode->addItem("送检模式");
    ui->comboBox_powermode->addItem("正常模式");
    ui->comboBox_linkageInput->addItem("关闭");
    ui->comboBox_linkageInput->addItem("有源输入");
    ui->comboBox_linkageInput->addItem("无源输入");
    ui->comboBox_linkageInput->addItem("都打开");
    ui->comboBox_loopAddress->addItem("回路1-8");
    ui->comboBox_loopAddress->addItem("回路1");
    ui->comboBox_loopAddress->addItem("回路2");
    ui->comboBox_loopAddress->addItem("回路3");
    ui->comboBox_loopAddress->addItem("回路4");
    ui->comboBox_loopAddress->addItem("回路5");
    ui->comboBox_loopAddress->addItem("回路6");
    ui->comboBox_loopAddress->addItem("回路7");
    ui->comboBox_loopAddress->addItem("回路8");
    ui->light_detection->setStyleSheet("QCheckBox::indicator {width: 15px; height: 15px;}");
    ui->light_detection->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->CAN_communication->setStyleSheet("QCheckBox::indicator {width: 15px; height: 15px;}");
    ui->CAN_communication->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");

    ui->comboBox_loopoutstatus->addItem("回路输出保持");
    ui->comboBox_loopoutstatus->addItem("回路输出断开");



    ui->label_loopoutstatus->setEnabled(false);
    ui->comboBox_loopoutstatus->setEnabled(false);
    ui->readloopoutstatus_button->setEnabled(false);
    ui->closepowerout_button->setEnabled(false);
    ui->poweroutkeep_button->setEnabled(false);
   //控件赋值
    ui->m_firetime->setText(QString::number(100));
    ui->m_unfiretime->setText(QString::number(30));
    ui->m_opencurrent->setText(QString::number(0));
    ui->m_firetime_slider->setValue(100);
    ui->m_unfiretime_slider->setValue(30);
    ui->m_opencurrent_slider->setValue(0);

    QTimer::singleShot(10000, this, SLOT(readProtectInfo()));
    //定时1s更新集中电源运行状态
    m_timerUpdateStatus = new QTimer();
    connect(m_timerUpdateStatus, SIGNAL(timeout()), this, SLOT(updateStatus()));
    m_timerUpdateStatus->start(1000);

    //隐藏模块
    ui->label_powerouttime->hide();
    ui->comboBox_powermode->hide();
    ui->removeLampLogin_button->hide();
    ui->emergencyStart_button->hide();
    ui->emergencyStop_button->hide();
    ui->comboBox_loopAddress->hide();
}

CProgramDistributionView::~CProgramDistributionView()
{
    CGlobal::instance()->setProgramDistributionView(0);
    delete ui;
    delete m_timer;
    delete m_timerUpdateStatus;
}
//定时10s查询集中电源软件版本信息/读取保护参数
void CProgramDistributionView::readProtectInfo()
{
    qDebug() << "CProgramDistributionView::readProtectInfo" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(CGlobal::instance()->m_bInLogin == true)
        return;
    CMsgObjectStatus msgObjectStatus;
    for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
        if(canport)
        {
            QList<CDistribution*> distributions = canport->distributions();
            for(int i=0; i<distributions.count(); i++)
            {
                CDistribution* distribution = distributions.at(i);
                if(distribution)
                {
                    msgObjectStatus.nDisID = distribution->distributionAddress();
                    msgObjectStatus.nCanportAdd = distribution->canportAddress();
                    //读取保护参数
                    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_ReadProtectInfor,&msgObjectStatus);
                    //读集中电源软件版本信息
                    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_ReadDeviceInfo,&msgObjectStatus);
                }
            }
        }
    }

}

void CProgramDistributionView::distributionResetStatus()
{
    qDebug() << "CProgramDistributionView::distributionResetStatus" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
        if(!canport)
            return;
        QList<CDistribution*> distributions = canport->distributions();
        for(int i=0; i<distributions.count(); i++)
        {
            QApplication::processEvents();
            CDistribution* distribution = distributions.at(i);
            if(distribution)
            {
                distribution->setmainPowerFault(false);
                distribution->setbackupPowerFault(false);
                distribution->setoverOut(false);
                distribution->setoutOpen(false);
                distribution->setchargeStatus(false);
                distribution->setemergencyStatus(false);
                distribution->set36vOut(false);
                distribution->setsystemFault(false);
                distribution->setrunMode(false);
                distribution->setoverDischargeFault(false);
                distribution->setbatUndervoltage(false);
                distribution->setbatOverheating(false);
                distribution->setloopCommunication(0);
                distribution->setloopOpen(0);
                if(!distribution->isDistributionOnline())
                {
                    distribution->setDistributionOnline(true);
                    distribution->setStatus(OBJS_Online, 1);
                }
                QList<CLoop*> loops = distribution->loops();
                for(int i=0; i<loops.count(); i++){
                    QApplication::processEvents();
                    CLoop* loop = loops.at(i);
                    if(!loop)
                        continue;
                    if(!loop->isLoopOnline())
                    {
                        loop->setLoopOnline(true);
//                        loop->setDeviceOnlineTotal(loop->deviceCount());
                    }
                    if(!loop->isLoopLogin())
                    {
//                        loop->setloopValue(LOOP_VALUE_NOLOGIN,0);
                        loop->setLoopLogin(true);
                    }
                    loop->setDeviceOnlineTotal(loop->deviceCount());
                }
            }
        }
        distributions.clear();
    }
}

void CProgramDistributionView::setDistribution(CDistribution *distribution)
{
    qDebug() << "CProgramDistributionView::setDistribution" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nDisID = distribution->distributionAddress();
    msgObjectStatus.nCanportAdd = distribution->canportAddress();
    //读取保护参数
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_ReadProtectInfor,&msgObjectStatus);
    m_changeflag = false;
    int typeID = distribution->distributionTypeId();
    setDistributionType(typeID);
    if(typeID >= 240 && typeID <= 250)
    {
        ui->label_loopoutstatus->setEnabled(true);
        ui->comboBox_loopoutstatus->setEnabled(true);
        ui->readloopoutstatus_button->setEnabled(true);
        ui->closepowerout_button->setEnabled(true);
        ui->poweroutkeep_button->setEnabled(true);
        ui->comboBox_loopoutstatus->setCurrentIndex(distribution->getloopOutStatus());
        ui->comboBox_linkageInput->clear();
        ui->comboBox_linkageInput->addItem("关闭");
        ui->comboBox_linkageInput->addItem("打开");
    }
    else
    {
        ui->label_loopoutstatus->setEnabled(false);
        ui->comboBox_loopoutstatus->setEnabled(false);
        ui->readloopoutstatus_button->setEnabled(false);
        ui->closepowerout_button->setEnabled(false);
        ui->poweroutkeep_button->setEnabled(false);
        ui->comboBox_linkageInput->clear();
        ui->comboBox_linkageInput->addItem("关闭");
        ui->comboBox_linkageInput->addItem("有源输入");
        ui->comboBox_linkageInput->addItem("无源输入");
        ui->comboBox_linkageInput->addItem("都打开");
    }
    ui->m_opencurrent_slider->setValue(distribution->getoutA());
    ui->m_opencurrent->setText(QString::number(distribution->getoutA()));
    ui->light_detection->setCheckState(distribution->getlightswitch() == true ? Qt::Checked : Qt::Unchecked);
    ui->CAN_communication->setCheckState(distribution->getCANfaultswitch() == true ? Qt::Checked : Qt::Unchecked);
    ui->comboBox_linkageInput->setCurrentIndex(distribution->getlinkageswitch());
    ui->comboBox_powermode->setCurrentIndex(distribution->getpowermode());
    ui->m_firetime_slider->setValue(distribution->getfiretime());
    ui->m_firetime->setText(QString::number(distribution->getfiretime()));
    ui->m_unfiretime_slider->setValue(distribution->getunfiretime());
    ui->m_unfiretime->setText(QString::number(distribution->getunfiretime()));
    ui->m_productID->setText(distribution->distributionValue(DISTRIBUTION_VALUE_DESCRIPTION).toString());
    m_changeflag = true;
    setDistributionStatus(distribution);
    setLoopStatus(distribution->getloopCommunication(),distribution->getloopOpen());
    qDebug() << distribution->getoutA() << distribution->getlightswitch() << distribution->getCANfaultswitch() << distribution->getlinkageswitch()
             << distribution->getpowermode() << distribution->getfiretime() << distribution->getunfiretime();
    update();
}

void CProgramDistributionView::updateStatus()
{
    qDebug() << "CProgramDistributionView::updateStatus" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(CGlobal::instance()->m_nCanportAddress);
    if(canport)
    {
        CDistribution* distribution = canport->distributionByAddress(CGlobal::instance()->m_nDistriAddress);
        if(distribution)
        {
            m_changeflag = false;
            ui->m_opencurrent_slider->setValue(distribution->getoutA());
            ui->m_opencurrent->setText(QString::number(distribution->getoutA()));
            ui->light_detection->setCheckState(distribution->getlightswitch() == true ? Qt::Checked : Qt::Unchecked);
            ui->CAN_communication->setCheckState(distribution->getCANfaultswitch() == true ? Qt::Checked : Qt::Unchecked);
            ui->comboBox_linkageInput->setCurrentIndex(distribution->getlinkageswitch());
            ui->comboBox_powermode->setCurrentIndex(distribution->getpowermode());
            ui->m_firetime_slider->setValue(distribution->getfiretime());
            ui->m_firetime->setText(QString::number(distribution->getfiretime()));
            ui->m_unfiretime_slider->setValue(distribution->getunfiretime());
            ui->m_unfiretime->setText(QString::number(distribution->getunfiretime()));
            ui->m_productID->setText(distribution->distributionValue(DISTRIBUTION_VALUE_DESCRIPTION).toString());
            m_changeflag = true;

            setDistributionStatus(distribution);
            setLoopStatus(distribution->getloopCommunication(),distribution->getloopOpen());
            ui->comboBox_loopoutstatus->setCurrentIndex(distribution->getloopOutStatus());
            update();
        }
    }
}

void CProgramDistributionView::setDistributionType(int typeID)
{
    if(typeID == 120 || typeID == 210 || typeID == 240)
    {
        ui->m_voltageType->setText("36V");
        ui->m_powerType->setText("200W");
    }
    else if(typeID == 121 || typeID == 200 || typeID == 230 || typeID == 211)
    {
        ui->m_voltageType->setText("36V");
        ui->m_powerType->setText("300W");
    }
    else if(typeID == 122 || typeID == 201 || typeID == 212 || typeID == 231 || typeID == 241)
    {
        ui->m_voltageType->setText("36V");
        ui->m_powerType->setText("500W");
    }
    else if(typeID == 202)
    {
        ui->m_voltageType->setText("36V");
        ui->m_powerType->setText("650W");
    }
    else if(typeID == 123 || typeID == 213)
    {
        ui->m_voltageType->setText("36V");
        ui->m_powerType->setText("750W");
    }
    else if(typeID == 124 || typeID == 214 || typeID == 232 || typeID == 242)
    {
        ui->m_voltageType->setText("36V");
        ui->m_powerType->setText("1000W");
    }
    else if(typeID == 125 || typeID == 215 || typeID == 248)
    {
        ui->m_voltageType->setText("24V");
        ui->m_powerType->setText("200W");
    }
    else if(typeID == 126 || typeID == 205 || typeID == 216 || typeID == 235)
    {
        ui->m_voltageType->setText("24V");
        ui->m_powerType->setText("300W");
    }
    else if(typeID == 127 || typeID == 206 || typeID == 217 || typeID == 236 || typeID == 249)
    {
        ui->m_voltageType->setText("24V");
        ui->m_powerType->setText("500W");
    }
    else if(typeID == 207)
    {
        ui->m_voltageType->setText("24V");
        ui->m_powerType->setText("650W");
    }
    else if(typeID == 128 || typeID == 218)
    {
        ui->m_voltageType->setText("24V");
        ui->m_powerType->setText("750W");
    }
    else if(typeID == 129 || typeID == 219 || typeID == 237 || typeID == 250)
    {
        ui->m_voltageType->setText("24V");
        ui->m_powerType->setText("1000W");
    }
    else if(typeID == 252)
    {
        ui->m_voltageType->setText("220V");
        ui->m_powerType->setText("1000W");
    }
    else if(typeID == 253)
    {
        ui->m_voltageType->setText("220V");
        ui->m_powerType->setText("2000W");
    }
    else if(typeID == 254)
    {
        ui->m_voltageType->setText("220V");
        ui->m_powerType->setText("3000W");
    }
    else if(typeID == 255)
    {
        ui->m_voltageType->setText("220V");
        ui->m_powerType->setText("5000W");
    }
}

void CProgramDistributionView::setDistributionStatus(CDistribution *distribution)
{
    qDebug() << "CProgramDistributionView::setDistributionStatus" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(distribution->getmainPowerFault())
    {
        ui->m_mainpower->setStyleSheet("background-color:rgb(255,255,0)");
        ui->m_mainpower->setText("故障");
    }
    else
    {
        ui->m_mainpower->setStyleSheet("background-color:rgb(0, 255, 0)");
        ui->m_mainpower->setText("正常");
    }
    if(distribution->getbackupPowerFault())
    {
        ui->m_backuppower->setStyleSheet("background-color:rgb(255,255,0)");
        ui->m_backuppower->setText("故障");
    }
    else
    {
        ui->m_backuppower->setStyleSheet("background-color:rgb(0, 255, 0)");
        ui->m_backuppower->setText("正常");
    }
    if(distribution->getoverOut())
    {
        ui->m_overload->setStyleSheet("background-color:rgb(255,255,0)");
        ui->m_overload->setText("故障");
    }
    else
    {
        ui->m_overload->setStyleSheet("background-color:rgb(0, 255, 0)");
        ui->m_overload->setText("正常");
    }
    if(distribution->getoutOpen())
    {
        ui->m_openload->setStyleSheet("background-color:rgb(255,255,0)");
        ui->m_openload->setText("故障");
    }
    else
    {
        ui->m_openload->setStyleSheet("background-color:rgb(0, 255, 0)");
        ui->m_openload->setText("正常");
    }
    if(distribution->getchargeStatus())
    {
        ui->m_chargestatus->setStyleSheet("background-color:rgb(255,0,0)");
        ui->m_chargestatus->setText("充电");
    }
    else
    {
        ui->m_chargestatus->setStyleSheet("background-color:rgb(0, 255, 0)");
        ui->m_chargestatus->setText("未充电");
    }
    if(distribution->getemergencyStatus())
    {//                //读运行参数
        ui->m_emergency->setStyleSheet("background-color:rgb(255,0,0)");
        ui->m_emergency->setText("应急");
    }
    else
    {
        ui->m_emergency->setStyleSheet("background-color:rgb(0, 255, 0)");
        ui->m_emergency->setText("正常");
    }
    if(distribution->get36vOut())
    {
        ui->m_out36v->setStyleSheet("background-color:rgb(255,255,0)");
        ui->m_out36v->setText("故障");
    }
    else
    {
        ui->m_out36v->setStyleSheet("background-color:rgb(0, 255, 0)");
        ui->m_out36v->setText("正常");
    }
    if(distribution->getsystemFault())
    {
        ui->m_systemfault->setStyleSheet("background-color:rgb(255,255,0)");
        ui->m_systemfault->setText("故障");
    }
    else
    {
        ui->m_systemfault->setStyleSheet("background-color:rgb(0, 255, 0)");
        ui->m_systemfault->setText("正常");
    }
    if(distribution->getrunMode())
    {
        ui->m_runmode->setStyleSheet("background-color:rgb(255,255,0)");
        ui->m_runmode->setText("手动");
    }
    else
    {
        ui->m_runmode->setStyleSheet("background-color:rgb(0, 255, 0)");
        ui->m_runmode->setText("自动");
    }
    if(distribution->getoverDischargeFault())
    {
        ui->m_overdischarge->setStyleSheet("background-color:rgb(255,255,0)");
        ui->m_overdischarge->setText("故障");
    }
    else
    {
        ui->m_overdischarge->setStyleSheet("background-color:rgb(0, 255, 0)");
        ui->m_overdischarge->setText("正常");
    }
    if(distribution->getbatUndervoltage())
    {
        ui->m_undervoltage->setStyleSheet("background-color:rgb(255,255,0)");
        ui->m_undervoltage->setText("故障");
    }
    else
    {
        ui->m_undervoltage->setStyleSheet("background-color:rgb(0, 255, 0)");
        ui->m_undervoltage->setText("正常");
    }
    if(distribution->getbatOverheating())
    {
        ui->m_overheat->setStyleSheet("background-color:rgb(255,255,0)");
        ui->m_overheat->setText("故障");
    }
    else
    {
        ui->m_overheat->setStyleSheet("background-color:rgb(0, 255, 0)");
        ui->m_overheat->setText("正常");
    }
}

//回路通讯、回路开路状态  loopCommunication代表通讯，loopOpen代表开路
void CProgramDistributionView::setLoopStatus(int loopCommunication, int loopOpen)
{
    qDebug() << "CProgramDistributionView::setLoopStatus" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(loopCommunication & 0x01)
        ui->m_loopc1->setStyleSheet("background-color:rgb(255,255,0)");
    else
        ui->m_loopc1->setStyleSheet("background-color:rgb(0, 255, 0)");
    if(loopCommunication & 0x02)
        ui->m_loopc2->setStyleSheet("background-color:rgb(255,255,0)");
    else
        ui->m_loopc2->setStyleSheet("background-color:rgb(0, 255, 0)");
    if(loopCommunication & 0x04)
        ui->m_loopc3->setStyleSheet("background-color:rgb(255,255,0)");
    else
        ui->m_loopc3->setStyleSheet("background-color:rgb(0, 255, 0)");
    if(loopCommunication & 0x08)
        ui->m_loopc4->setStyleSheet("background-color:rgb(255,255,0)");
    else
        ui->m_loopc4->setStyleSheet("background-color:rgb(0, 255, 0)");
    if(loopCommunication & 0x10)
        ui->m_loopc5->setStyleSheet("background-color:rgb(255,255,0)");
    else
        ui->m_loopc5->setStyleSheet("background-color:rgb(0, 255, 0)");
    if(loopCommunication & 0x20)
        ui->m_loopc6->setStyleSheet("background-color:rgb(255,255,0)");
    else
        ui->m_loopc6->setStyleSheet("background-color:rgb(0, 255, 0)");
    if(loopCommunication & 0x40)
        ui->m_loopc7->setStyleSheet("background-color:rgb(255,255,0)");
    else
        ui->m_loopc7->setStyleSheet("background-color:rgb(0, 255, 0)");
    if(loopCommunication & 0x80)
        ui->m_loopc8->setStyleSheet("background-color:rgb(255,255,0)");
    else
        ui->m_loopc8->setStyleSheet("background-color:rgb(0, 255, 0)");
    if(loopOpen & 0x01)
        ui->m_loopo1->setStyleSheet("background-color:rgb(255,255,0)");
    else
        ui->m_loopo1->setStyleSheet("background-color:rgb(0, 255, 0)");
    if(loopOpen & 0x02)
        ui->m_loopo2->setStyleSheet("background-color:rgb(255,255,0)");
    else
        ui->m_loopo2->setStyleSheet("background-color:rgb(0, 255, 0)");
    if(loopOpen & 0x04)
        ui->m_loopo3->setStyleSheet("background-color:rgb(255,255,0)");
    else
        ui->m_loopo3->setStyleSheet("background-color:rgb(0, 255, 0)");
    if(loopOpen & 0x08)
        ui->m_loopo4->setStyleSheet("background-color:rgb(255,255,0)");
    else
        ui->m_loopo4->setStyleSheet("background-color:rgb(0, 255, 0)");
    if(loopOpen & 0x10)
        ui->m_loopo5->setStyleSheet("background-color:rgb(255,255,0)");
    else
        ui->m_loopo5->setStyleSheet("background-color:rgb(0, 255, 0)");
    if(loopOpen & 0x20)
        ui->m_loopo6->setStyleSheet("background-color:rgb(255,255,0)");
    else
        ui->m_loopo6->setStyleSheet("background-color:rgb(0, 255, 0)");
    if(loopOpen & 0x40)
        ui->m_loopo7->setStyleSheet("background-color:rgb(255,255,0)");
    else
        ui->m_loopo7->setStyleSheet("background-color:rgb(0, 255, 0)");
    if(loopOpen & 0x80)
        ui->m_loopo8->setStyleSheet("background-color:rgb(255,255,0)");
    else
        ui->m_loopo8->setStyleSheet("background-color:rgb(0, 255, 0)");
}

void CProgramDistributionView::on_m_opencurrent_slider_valueChanged(int value)
{
    qDebug() << "CProgramDistributionView::on_m_opencurrent_slider_valueChanged" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    ui->m_opencurrent->setText(QString::number(value));
    if(m_changeflag != true)
        return;
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(CGlobal::instance()->m_nCanportAddress);
    if(canport)
    {
        QByteArray byte;
        CDistribution* distribution = canport->distributionByAddress(CGlobal::instance()->m_nDistriAddress);
        if(!distribution)
            return;
        byte.append(value);
        byte.append(distribution->getlightswitch());
        byte.append(distribution->getCANfaultswitch());
        byte.append(distribution->getlinkageswitch());
        byte.append(distribution->getpowermode());
        byte.append(distribution->getfiretime());
        byte.append(distribution->getunfiretime());
        distribution->setoutA(value);
        CMsgObjectStatus msgobjectstatus;
        msgobjectstatus.nDisID = CGlobal::instance()->m_nDistriAddress;
        msgobjectstatus.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
        msgobjectstatus.ndata = byte;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_WriteProtectInfor, &msgobjectstatus);
    }
}

void CProgramDistributionView::on_m_firetime_slider_valueChanged(int value)
{
    qDebug() << "CProgramDistributionView::on_m_firetime_slider_valueChanged" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    ui->m_firetime->setText(QString::number(value));
    if(m_changeflag != true)
        return;
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(CGlobal::instance()->m_nCanportAddress);
    if(canport)
    {
        QByteArray byte;
        CDistribution* distribution = canport->distributionByAddress(CGlobal::instance()->m_nDistriAddress);
        if(!distribution)
            return;
        byte.append(distribution->getoutA());
        byte.append(distribution->getlightswitch());
        byte.append(distribution->getCANfaultswitch());
        byte.append(distribution->getlinkageswitch());
        byte.append(distribution->getpowermode());
        byte.append(value);
        byte.append(distribution->getunfiretime());
        distribution->setfiretime(value);
        CMsgObjectStatus msgobjectstatus;
        msgobjectstatus.nDisID = CGlobal::instance()->m_nDistriAddress;
        msgobjectstatus.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
        msgobjectstatus.ndata = byte;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_WriteProtectInfor, &msgobjectstatus);
    }
}

void CProgramDistributionView::on_m_unfiretime_slider_valueChanged(int value)
{
    qDebug() << "CProgramDistributionView::on_m_unfiretime_slider_valueChanged" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    ui->m_unfiretime->setText(QString::number(value));
    if(m_changeflag != true)
        return;
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(CGlobal::instance()->m_nCanportAddress);
    if(canport)
    {
        QByteArray byte;
        CDistribution* distribution = canport->distributionByAddress(CGlobal::instance()->m_nDistriAddress);
        if(!distribution)
            return;
        byte.append(distribution->getoutA());
        byte.append(distribution->getlightswitch());
        byte.append(distribution->getCANfaultswitch());
        byte.append(distribution->getlinkageswitch());
        byte.append(distribution->getpowermode());
        byte.append(distribution->getfiretime());
        byte.append(value);
        distribution->setunfiretime(value);
        CMsgObjectStatus msgobjectstatus;
        msgobjectstatus.nDisID = CGlobal::instance()->m_nDistriAddress;
        msgobjectstatus.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
        msgobjectstatus.ndata = byte;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_WriteProtectInfor, &msgobjectstatus);
    }
}

void CProgramDistributionView::on_CAN_communication_clicked(bool checked)
{
    qDebug() << "CProgramDistributionView::on_CAN_communication_clicked" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_changeflag != true)
        return;
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(CGlobal::instance()->m_nCanportAddress);
    if(canport)
    {
        QByteArray byte;
        CDistribution* distribution = canport->distributionByAddress(CGlobal::instance()->m_nDistriAddress);
        if(!distribution)
            return;
        byte.append(distribution->getoutA());
        byte.append(distribution->getlightswitch());
        byte.append(checked);
        byte.append(distribution->getlinkageswitch());
        byte.append(distribution->getpowermode());
        byte.append(distribution->getfiretime());
        byte.append(distribution->getunfiretime());
        distribution->setCANfaultswitch(checked);
        CMsgObjectStatus msgobjectstatus;
        msgobjectstatus.nDisID = CGlobal::instance()->m_nDistriAddress;
        msgobjectstatus.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
        msgobjectstatus.ndata = byte;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_WriteProtectInfor, &msgobjectstatus);
    }
}

void CProgramDistributionView::on_light_detection_clicked(bool checked)
{
    qDebug() << "CProgramDistributionView::on_light_detection_clicked" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_changeflag != true)
        return;
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(CGlobal::instance()->m_nCanportAddress);
    if(canport)
    {
        QByteArray byte;
        CDistribution* distribution = canport->distributionByAddress(CGlobal::instance()->m_nDistriAddress);
        if(!distribution)
            return;
        byte.append(distribution->getoutA());
        byte.append(checked);
        byte.append(distribution->getCANfaultswitch());
        byte.append(distribution->getlinkageswitch());
        byte.append(distribution->getpowermode());
        byte.append(distribution->getfiretime());
        byte.append(distribution->getunfiretime());
        distribution->setlightswitch(checked);
        CMsgObjectStatus msgobjectstatus;
        msgobjectstatus.nDisID = CGlobal::instance()->m_nDistriAddress;
        msgobjectstatus.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
        msgobjectstatus.ndata = byte;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_WriteProtectInfor, &msgobjectstatus);
    }
}

void CProgramDistributionView::on_comboBox_powermode_currentIndexChanged(int index)
{
    qDebug() << "CProgramDistributionView::on_comboBox_powermode_currentIndexChanged" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_changeflag != true)
        return;
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(CGlobal::instance()->m_nCanportAddress);
    if(canport)
    {
        QByteArray byte;
        CDistribution* distribution = canport->distributionByAddress(CGlobal::instance()->m_nDistriAddress);
        if(!distribution)
            return;
        byte.append(distribution->getoutA());
        byte.append(distribution->getlightswitch());
        byte.append(distribution->getCANfaultswitch());
        byte.append(distribution->getlinkageswitch());
        byte.append(index);
        byte.append(distribution->getfiretime());
        byte.append(distribution->getunfiretime());
        distribution->setpowermode(index);
        CMsgObjectStatus msgobjectstatus;
        msgobjectstatus.nDisID = CGlobal::instance()->m_nDistriAddress;
        msgobjectstatus.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
        msgobjectstatus.ndata = byte;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_WriteProtectInfor, &msgobjectstatus);
    }
}

void CProgramDistributionView::on_comboBox_linkageInput_currentIndexChanged(int index)
{
    qDebug() << "CProgramDistributionView::on_comboBox_linkageInput_currentIndexChanged" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_changeflag != true)
        return;
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(CGlobal::instance()->m_nCanportAddress);
    if(canport)
    {
        QByteArray byte;
        CDistribution* distribution = canport->distributionByAddress(CGlobal::instance()->m_nDistriAddress);
        if(!distribution)
            return;
        byte.append(distribution->getoutA());
        byte.append(distribution->getlightswitch());
        byte.append(distribution->getCANfaultswitch());
        byte.append(index);
        byte.append(distribution->getpowermode());
        byte.append(distribution->getfiretime());
        byte.append(distribution->getunfiretime());
        distribution->setlinkageswitch(index);
        CMsgObjectStatus msgobjectstatus;
        msgobjectstatus.nDisID = CGlobal::instance()->m_nDistriAddress;
        msgobjectstatus.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
        msgobjectstatus.ndata = byte;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_WriteProtectInfor, &msgobjectstatus);
    }
}


void CProgramDistributionView::on_comboBox_loopoutstatus_currentIndexChanged(int index)
{
    qDebug() << "CProgramDistributionView::on_comboBox_loopoutstatus_currentIndexChanged" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_changeflag != true)
        return;

    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(CGlobal::instance()->m_nCanportAddress);
    if(canport)
    {
        CDistribution* distribution = canport->distributionByAddress(CGlobal::instance()->m_nDistriAddress);
        if(distribution)
        {
            CMsgObjectStatus msgobjectstatus;
            msgobjectstatus.nDisID = CGlobal::instance()->m_nDistriAddress;
            msgobjectstatus.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
            msgobjectstatus.nValue = index;
            CGlobal::instance()->ClientBusiness()->exeCommand(NCT_SetLoopOutStatus, &msgobjectstatus);
            distribution->setloopOutStatus(index);
        }
    }
}

void CProgramDistributionView::on_readloopoutstatus_button_clicked()
{
    qDebug() << "CProgramDistributionView::on_readloopoutstatus_button_clicked" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_changeflag != true)
        return;
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nDisID = CGlobal::instance()->m_nDistriAddress;
    msgobjectstatus.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_ReadLoopOutStatus, &msgobjectstatus);
}

void CProgramDistributionView::on_closepowerout_button_clicked()
{
    qDebug() << "CProgramDistributionView::on_closepowerout_button_clicked" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_changeflag != true)
        return;
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nDisID = CGlobal::instance()->m_nDistriAddress;
    msgobjectstatus.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
    msgobjectstatus.nValue = true;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_CloseBatteryOut, &msgobjectstatus);
}

void CProgramDistributionView::on_poweroutkeep_button_clicked()
{
    qDebug() << "CProgramDistributionView::on_poweroutkeep_button_clicked" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_changeflag != true)
        return;
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nDisID = CGlobal::instance()->m_nDistriAddress;
    msgobjectstatus.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
    msgobjectstatus.nValue = false;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_CloseBatteryOut, &msgobjectstatus);
}


void CProgramDistributionView::on_emergencyStart_button_clicked()
{
    qDebug() << "CProgramDistributionView::on_emergencyStart_button_clicked" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nDisID = CGlobal::instance()->m_nDistriAddress;
    msgobjectstatus.nLoopID = ui->comboBox_loopAddress->currentIndex();
    msgobjectstatus.nValue = true;
    msgobjectstatus.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_Emergency, &msgobjectstatus);
}

void CProgramDistributionView::on_emergencyStop_button_clicked()
{
    qDebug() << "CProgramDistributionView::on_emergencyStop_button_clicked" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nDisID = CGlobal::instance()->m_nDistriAddress;
    msgobjectstatus.nLoopID = ui->comboBox_loopAddress->currentIndex();
    msgobjectstatus.nValue = false;
    msgobjectstatus.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_Emergency, &msgobjectstatus);
}

void CProgramDistributionView::on_removeLampLogin_button_clicked()
{
    qDebug() << "CProgramDistributionView::read" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QFont font;
    font.setPointSize(12); // 设置字体大小为12像素
    // 在QMessageBox中设置字体
    QMessageBox messageBox;
    messageBox.setFont(font);
    QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
    messageBox.setStyleSheet(styleSheet);
    messageBox.setIcon(QMessageBox::NoIcon);
    messageBox.setWindowTitle("提示");
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Cancel);
    // 设置按钮显示文字
    messageBox.button(QMessageBox::Yes)->setText("是");
    messageBox.button(QMessageBox::No)->setText("否");
    messageBox.button(QMessageBox::Cancel)->setText("取消");
    QString log;
    log = "是否要给集中电源发送命令，清除灯具注册状态?";
    messageBox.setText(log);
    int ret = messageBox.exec();
    CMsgObjectStatus msg;;
    //获取当前点击的集中电源地址
    msg.nDisID = CGlobal::instance()->m_nDistriAddress;
    msg.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
    msg.nLoopID = 0;
    if(msg.nDisID == 0){
        return;
    }
    if(ret == QMessageBox::Cancel)
    {
        return;
    }
    else if(ret == QMessageBox::Yes)
    {
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_UnLoginLoop, &msg);
        CGlobal::instance()->ClientBusiness()->removeLoginObject(CGlobal::instance()->m_nCanportAddress, CGlobal::instance()->m_nDistriAddress,0);
    }
    else if(ret == QMessageBox::No)
    {
        CGlobal::instance()->ClientBusiness()->removeLoginObject(CGlobal::instance()->m_nCanportAddress, CGlobal::instance()->m_nDistriAddress,0);
    }
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_RemoveFaultLamp, &msg);
    ConfigFileManager cfg;
    cfg.SaveConfigFile();//写入配置文件
}
