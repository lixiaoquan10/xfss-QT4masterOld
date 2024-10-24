#include "cprogramloopview.h"
#include "ui_cprogramloopview.h"
#include "cglobal.h"
#include "dm/cproject.h"
#include "dm/ccanport.h"
#include "dm/cdistribution.h"
#include "dm/cloop.h"
#include "business/cclientbusiness.h"
#include "model/cdistributionmodel.h"
#include "style/cstyledbar.h"
#include "view/ctableview.h"

#include <QMessageBox>
#include <QToolButton>
#include <QDebug>
#include <QSortFilterProxyModel>

CProgramLoopView::CProgramLoopView(QWidget *parent) :
    QWidget(parent), ui(new Ui::CProgramLoopView)
{
    ui->setupUi(this);
    CGlobal::instance()->setProgramLoopView(this);
    m_distributionAddress = 1;

    m_model = new CDistributionModel();
    ui->tableView->setModel(m_model);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setColumnWidth(DISTRIBUTION_COLUMN_ADDRESS, 40);
    ui->tableView->setColumnWidth(DISTRIBUTION_COLUMN_NAME, 200);
    ui->tableView->setColumnWidth(DISTRIBUTION_COLUMN_DESCRIPTION, 190);
    ui->tableView->setColumnWidth(DISTRIBUTION_COLUMN_AREA, 200);
    ui->tableView->setColumnWidth(DISTRIBUTION_COLUMN_LOCATION, 200);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setHighlightSections(false);

    //定时10s查询集中电源运行参数
    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(readPowerInfo()));
    m_timer->start(10000);
    //定时1s更新集中电源运行状态
    m_timerUpdateStatus = new QTimer();
    connect(m_timerUpdateStatus, SIGNAL(timeout()), this, SLOT(updateStatus()));
    m_timerUpdateStatus->start(1000);
}

CProgramLoopView::~CProgramLoopView()
{
    CGlobal::instance()->setProgramLoopView(0);
    delete ui;
    delete m_timer;
    delete m_model;
    delete m_timerUpdateStatus;
}


void CProgramLoopView::updateDistribution()
{
    CCanport* canport = CGlobal::instance()->controller()->canportById(CGlobal::instance()->panelAddress());
    if(canport)
    {
        m_model->setCanport(canport);
    }
}

//定时10s查询集中电源运行参数/读取保护参数
void CProgramLoopView::readPowerInfo()
{
    if(CGlobal::instance()->m_bInLogin == true)
        return;
    CCanport* canport = CGlobal::instance()->controller()->canportById(CGlobal::instance()->panelAddress());
    CMsgLoginData cmsglogindata;
    CMsgObjectStatus msgObjectStatus;
    if(canport)
    {
        QList<CDistribution*> distributions = canport->distributions();
        for(int i=0; i<distributions.count(); i++)
        {
            CDistribution* distribution = distributions.at(i);
            if(distribution && distribution->distributionValue(DISTRIBUTION_PORT_ID).toInt())
            {
                cmsglogindata.nDistributeID = distribution->distributionAddress();
                msgObjectStatus.nDisID = distribution->distributionAddress();
                //读运行参数
                CGlobal::instance()->ClientBusiness()->exeCommand(NCT_PowerInfo,&msgObjectStatus);
            }
        }
        distributions.clear();
    }
}


void CProgramLoopView::setDistribution(CDistribution *distribution)
{
    setDistributionStatus(distribution);
    setDistributionParameter(distribution);
    setLoopStatus(distribution->getloopCommunication(),distribution->getloopOpen());
    qDebug() << distribution->getoutA() << distribution->getlightswitch() << distribution->getCANfaultswitch() << distribution->getlinkageswitch()
             << distribution->getpowermode() << distribution->getfiretime() << distribution->getunfiretime();
    update();
}

void CProgramLoopView::updateStatus()
{
    CCanport* canport = CGlobal::instance()->controller()->canportById(CGlobal::instance()->panelAddress());
    if(!canport)
        return;
    CDistribution* distribution = canport->distributionByAddress(m_distributionAddress);
    if(distribution)
    {
        setDistributionStatus(distribution);
        setDistributionParameter(distribution);
        setLoopStatus(distribution->getloopCommunication(),distribution->getloopOpen());
        update();
    }
}
//集中电源状态更新
void CProgramLoopView::setDistributionStatus(CDistribution *distribution)
{
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
        ui->m_chargestatus->setStyleSheet("background-color:rgb(255, 0, 0)");
        ui->m_chargestatus->setText("充电");
    }
    else
    {
        ui->m_chargestatus->setStyleSheet("background-color:rgb(0, 255, 0)");
        ui->m_chargestatus->setText("未充电");
    }
    if(distribution->getemergencyStatus())
    {
        ui->m_emergency->setStyleSheet("background-color:rgb(255, 0, 0)");
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
        ui->m_runmode->setStyleSheet("background-color:rgb(255, 0, 0)");
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

//集中电源软件版本/电参量更新
void CProgramLoopView::setDistributionParameter(CDistribution *distribution)
{
    //集中电源软件版本
    ui->label_CPUnumber->setText(distribution->distributionValue(DISTRIBUTION_VALUE_CPUVERSION).toString());
    ui->label_POWnumber->setText(distribution->distributionValue(DISTRIBUTION_VALUE_POWVERSION).toString());
    ui->label_LCDnumber->setText(distribution->distributionValue(DISTRIBUTION_VALUE_LCDVERSION).toString());
    ui->label_ABUS1number->setText(distribution->distributionValue(DISTRIBUTION_VALUE_ABUS1VERSION).toString());
    ui->label_ABUS2number->setText(distribution->distributionValue(DISTRIBUTION_VALUE_ABUS2VERSION).toString());
    ui->label_ABUS3number->setText(distribution->distributionValue(DISTRIBUTION_VALUE_ABUS3VERSION).toString());
    ui->label_ABUS4number->setText(distribution->distributionValue(DISTRIBUTION_VALUE_ABUS4VERSION).toString());
    ui->label_ABUS5number->setText(distribution->distributionValue(DISTRIBUTION_VALUE_ABUS5VERSION).toString());
    ui->label_ABUS6number->setText(distribution->distributionValue(DISTRIBUTION_VALUE_ABUS6VERSION).toString());
    ui->label_ABUS7number->setText(distribution->distributionValue(DISTRIBUTION_VALUE_ABUS7VERSION).toString());
    ui->label_ABUS8number->setText(distribution->distributionValue(DISTRIBUTION_VALUE_ABUS8VERSION).toString());

    //集中电源电参量
    ui->m_mainpowerV->setText(distribution->distributionValue(DISTRIBUTION_VALUE_MAINV).toString() + "V");
    ui->m_outpowerV->setText(distribution->distributionValue(DISTRIBUTION_VALUE_OUTV).toString() + "V");
    ui->m_outpowerA->setText(distribution->distributionValue(DISTRIBUTION_VALUE_OUTA).toString() + "A");
    ui->m_lightV->setText(distribution->distributionValue(DISTRIBUTION_VALUE_EXTERNALV).toString() + "V");
    ui->m_battery1V->setText(distribution->distributionValue(DISTRIBUTION_VALUE_BAT1V).toString() + "V");
    ui->m_battery2V->setText(distribution->distributionValue(DISTRIBUTION_VALUE_BAT2V).toString() + "V");
    ui->m_battery3V->setText(distribution->distributionValue(DISTRIBUTION_VALUE_BAT3V).toString() + "V");
    ui->m_batteryV->setText(distribution->distributionValue(DISTRIBUTION_VALUE_BATV).toString() + "V");

    ui->m_battery1T->setText(batteryTStatus(distribution->distributionValue(DISTRIBUTION_VALUE_BAT1T).toInt()));
    ui->m_battery2T->setText(batteryTStatus(distribution->distributionValue(DISTRIBUTION_VALUE_BAT2T).toInt()));
    ui->m_battery3T->setText(batteryTStatus(distribution->distributionValue(DISTRIBUTION_VALUE_BAT3T).toInt()));

    CLoop* loop;
    loop = distribution->loopByAdd(1);
    if(loop){
        ui->m_loop1V->setText(loop->loopValue(LOOP_VALUE_V).toString() + "V");
        ui->m_loop1A->setText(loop->loopValue(LOOP_VALUE_A).toString() + "A");
    }
    loop = distribution->loopByAdd(2);
    if(loop){
        ui->m_loop2V->setText(loop->loopValue(LOOP_VALUE_V).toString() + "V");
        ui->m_loop2A->setText(loop->loopValue(LOOP_VALUE_A).toString() + "A");
    }
    loop = distribution->loopByAdd(3);
    if(loop){
        ui->m_loop3V->setText(loop->loopValue(LOOP_VALUE_V).toString() + "V");
        ui->m_loop3A->setText(loop->loopValue(LOOP_VALUE_A).toString() + "A");
    }
    loop = distribution->loopByAdd(4);
    if(loop){
        ui->m_loop4V->setText(loop->loopValue(LOOP_VALUE_V).toString() + "V");
        ui->m_loop4A->setText(loop->loopValue(LOOP_VALUE_A).toString() + "A");
    }
    loop = distribution->loopByAdd(5);
    if(loop){
        ui->m_loop5V->setText(loop->loopValue(LOOP_VALUE_V).toString() + "V");
        ui->m_loop5A->setText(loop->loopValue(LOOP_VALUE_A).toString() + "A");
    }
    loop = distribution->loopByAdd(6);
    if(loop){
        ui->m_loop6V->setText(loop->loopValue(LOOP_VALUE_V).toString() + "V");
        ui->m_loop6A->setText(loop->loopValue(LOOP_VALUE_A).toString() + "A");
    }
    loop = distribution->loopByAdd(7);
    if(loop){
        ui->m_loop7V->setText(loop->loopValue(LOOP_VALUE_V).toString() + "V");
        ui->m_loop7A->setText(loop->loopValue(LOOP_VALUE_A).toString() + "A");
    }
    loop = distribution->loopByAdd(8);
    if(loop){
        ui->m_loop8V->setText(loop->loopValue(LOOP_VALUE_V).toString() + "V");
        ui->m_loop8A->setText(loop->loopValue(LOOP_VALUE_A).toString() + "A");
    }
}

QString CProgramLoopView::batteryTStatus(u_int16_t t)
{
    // 将无符号整数转换为有符号整数
    int16_t signedValue = static_cast<int16_t>(t);

    // 如果值为负数，则计算绝对值并取反
    if (signedValue < 0) {
        signedValue = -signedValue;
        signedValue = ~signedValue + 1;   //得到转换后温度值（负数的绝对值）

        if(signedValue == -3000)  //未开启温度检测
            return "无";
        else if(signedValue == -200)  //温度检测断线
            return "断线";
        else if(signedValue == -100)  //温度检测短路
            return "短路";
        else                    //温度为零下
            return QString::number(signedValue);
    }
    else
    {
        return QString::number(signedValue);
    }
}

//回路通讯、回路开路状态  loopCommunication代表通讯，loopOpen代表开路
void CProgramLoopView::setLoopStatus(int loopCommunication, int loopOpen)
{
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


void CProgramLoopView::on_tableView_clicked(const QModelIndex &index)
{
    CCanport* canport = CGlobal::instance()->controller()->canportById(CGlobal::instance()->panelAddress());
    if(!canport)
        return;
    m_distributionAddress = m_model->index(index.row(),0).data().toInt();
    CDistribution* distribution = canport->distributionByAddress(m_distributionAddress);
    if(distribution)
    {
        setDistribution(distribution);
    }
}
