#include "cprogramview.h"
#include "cminisplitter.h"
#include "cprogramtreeview.h"
#include "cprogramcontainer.h"
#include "cdesigntreeview.h"
#include "cglobal.h"
#include "ctoptoolbar.h"
#include "dialog/dlglogininformation.h"
#include "dm/cproject.h"
#include "dm/ccontroller.h"
#include "dm/cdistribution.h"
#include "dm/cdevice.h"
#include "dm/cloop.h"
#include "dm/cdbsqlite.h"
#include "graphics/configfilemanager.h"
#include <QToolButton>
#include <QLayout>
#include <QMessageBox>
#include <QDebug>
#include <QFont>

CProgramView::CProgramView(QWidget *parent) :
    QWidget(parent)
{
    QFont font;
    font.setPointSize(12);
    m_regAll = new QToolButton(this);
    m_regAll->setText("全部注册");
    m_regAll->setMinimumHeight(35);
    m_regAll->setMinimumWidth(80);
    m_regAll->setFont(font);
    connect(m_regAll,SIGNAL(clicked(bool)),this,SLOT(slot_regALL()));

    m_clrAllReg = new QToolButton(this);
    m_clrAllReg->setText("全部清除注册");
    m_clrAllReg->setMinimumHeight(35);
    m_clrAllReg->setMinimumWidth(100);
    m_clrAllReg->setFont(font);
    connect(m_clrAllReg,SIGNAL(clicked(bool)),this,SLOT(slot_unLogin()));

    m_regOne = new QToolButton(this);
    m_regOne->setText("单个注册");
    m_regOne->setMinimumHeight(35);
    m_regOne->setMinimumWidth(80);
    m_regOne->setFont(font);
    connect(m_regOne,SIGNAL(clicked(bool)),this,SLOT(slot_regLoop()));

    m_clrOneReg = new QToolButton(this);
    m_clrOneReg->setText("单个清除注册");
    m_clrOneReg->setMinimumHeight(35);
    m_clrOneReg->setMinimumWidth(100);
    m_clrOneReg->setFont(font);
    connect(m_clrOneReg,SIGNAL(clicked(bool)),this,SLOT(slot_unLoginLoop()));

    m_readdressAll = new QToolButton(this);
    m_readdressAll->setText("重编所有回路地址");
    m_readdressAll->setMinimumHeight(35);
    m_readdressAll->setFont(font);
    connect(m_readdressAll,SIGNAL(clicked(bool)),this,SLOT(slot_recodeAllDevice()));

    m_readdress = new QToolButton(this);
    m_readdress->setText("重编当前回路地址");
    m_readdress->setMinimumHeight(35);
    m_readdress->setFont(font);
    connect(m_readdress,SIGNAL(clicked(bool)),this,SLOT(slot_recodeLoopDevice()));

    m_storeAll = new QToolButton(this);
    m_storeAll->setText("保存所有回路灯具");
    m_storeAll->setMinimumHeight(35);
    m_storeAll->setFont(font);
    connect(m_storeAll,SIGNAL(clicked(bool)),this,SLOT(slot_saveAllDevice()));

    m_store = new QToolButton(this);
    m_store->setText("保存当前回路灯具");
    m_store->setMinimumHeight(35);
    m_store->setFont(font);
    connect(m_store,SIGNAL(clicked(bool)),this,SLOT(slot_saveLoopDevice()));

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->setMargin(0);
    buttonLayout->setSpacing(5);
//    buttonLayout->setContentsMargins(50,0,20,0);
    buttonLayout->addWidget(m_regAll);
    buttonLayout->addWidget(m_clrAllReg);
    buttonLayout->addWidget(m_regOne);
    buttonLayout->addWidget(m_clrOneReg);
    buttonLayout->addWidget(m_readdressAll);
    buttonLayout->addWidget(m_readdress);
    buttonLayout->addWidget(m_storeAll);
    buttonLayout->addWidget(m_store);
    buttonLayout->addStretch();

    m_pSplitter = new CMiniSplitter(this);
    CProgramTreeView* treeView = new CProgramTreeView(m_pSplitter);
//    treeView->setFixedWidth(treeView->sizeHint().width());
    treeView->setFixedWidth(400);
    treeView->setFont(font);
    m_pSplitter->addWidget(treeView);
    m_pSplitter->addWidget(new CProgramContainer(m_pSplitter));
    QVBoxLayout* pLayout = new QVBoxLayout();
    pLayout->setMargin(5);
    pLayout->setSpacing(5);
    pLayout->addLayout(buttonLayout);
    pLayout->addWidget(m_pSplitter);
    setLayout(pLayout);

    m_isRemoveLoop = false;

}

CProgramView::~CProgramView()
{
    delete m_pSplitter;
    delete m_regAll;
    delete m_clrAllReg;
    delete m_regOne;
    delete m_clrOneReg;
    delete m_readdressAll;
    delete m_readdress;
    delete m_storeAll;
    delete m_store;
    delete m_pSplitter;
}

//全部注册
void CProgramView::slot_regALL()
{
    QFont font;
    font.setPointSize(12); // 设置字体大小为12像素
    // 在QMessageBox中设置字体
    QMessageBox messageBox;
    messageBox.setFont(font);
    QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
    messageBox.setStyleSheet(styleSheet);
    messageBox.setIcon(QMessageBox::NoIcon);
    messageBox.setWindowTitle("全部注册");
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Cancel);
    QString log;
    log = "确定要全部注册吗?";
    messageBox.setText(log);
    int ret = messageBox.exec();
    if(ret == QMessageBox::Yes)
    {
         CMsgNull m_MsgNUll;
         if(!CGlobal::instance()->ClientBusiness()->exeCommand(NCT_LoginInfo, &m_MsgNUll))
         {
             CGlobal::instance()->m_HaveDialog = true;
             QFont font;
             font.setPointSize(12); // 设置字体大小为12像素
             // 在QMessageBox中设置字体
             QMessageBox messageBox;
             messageBox.setFont(font);
             QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
             messageBox.setStyleSheet(styleSheet);
             messageBox.setIcon(QMessageBox::NoIcon);
             messageBox.setWindowTitle("信息");
             messageBox.setStandardButtons(QMessageBox::Ok);
             QString log;
             log = "全部注册失败！";
             messageBox.setText(log);
             messageBox.exec();
             CGlobal::instance()->m_HaveDialog = false;
         }
         else
         {
             CGlobal::instance()->m_bInLogin = true;
//             CGlobal::instance()->ClientBusiness()->clearExpectObject();
//             CGlobal::instance()->ClientBusiness()->clearStatusObject(true);
             CGlobal::instance()->InformationWindow()->setMovie(":/images/allregedit.png");
             CGlobal::instance()->InformationWindow()->setDescription(QString("<h1><font color=black>%1</font></h1>").arg("系统注册......!"));
             CGlobal::instance()->InformationWindow()->show();
             CGlobal::instance()->InformationWindow()->hideCloseButton();
             CGlobal::instance()->setMSgColor(QColor(12, 180, 3));
             CGlobal::instance()->setMsgText("系统注册......!");
         }
    }
}

//全部清除注册
void CProgramView::slot_unLogin()
{
    QFont font;
    font.setPointSize(12); // 设置字体大小为12像素
    // 在QMessageBox中设置字体
    QMessageBox messageBox;
    messageBox.setFont(font);
    QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
    messageBox.setStyleSheet(styleSheet);
    messageBox.setIcon(QMessageBox::NoIcon);
    messageBox.setWindowTitle("全部清除注册");
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Cancel);
    QString log;
    log = "确定要全部清除注册吗?";
    messageBox.setText(log);
    int ret = messageBox.exec();
    if(ret == QMessageBox::Yes)
    {
        CGlobal::instance()->InformationWindow()->setDescription(QString("<h1><font color=black>%1</font></h1>").arg("清除注册......!"));
        CGlobal::instance()->InformationWindow()->show();
        CGlobal::instance()->InformationWindow()->hideCloseButton();
        CMsgNull m_MsgNUll;
        if(!CGlobal::instance()->ClientBusiness()->exeCommand(NCT_Unlogin, &m_MsgNUll))
        {
            CGlobal::instance()->m_HaveDialog = true;
            QFont font;
            font.setPointSize(12); // 设置字体大小为12像素
            // 在QMessageBox中设置字体
            QMessageBox messageBox;
            messageBox.setFont(font);
            QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
            messageBox.setStyleSheet(styleSheet);
            messageBox.setIcon(QMessageBox::NoIcon);
            messageBox.setWindowTitle("信息");
            messageBox.setStandardButtons(QMessageBox::Ok);
            QString log;
            log = "清除注册失败！";
            messageBox.setText(log);
            messageBox.exec();
            CGlobal::instance()->m_HaveDialog = false;
        }
        else
        {
            CGlobal::instance()->m_bInLogin = false;
            QTimer::singleShot(500, this, SLOT(slot_removeLoginObject()));
        }
    }
}

//单个注册
void CProgramView::slot_regLoop()
{  
    QFont font;
    font.setPointSize(12); // 设置字体大小为12像素
    int ret = 0;
    if(CGlobal::instance()->m_isSelectDistribution)
    {    
        // 在QMessageBox中设置字体
        QMessageBox messageBox;
        messageBox.setFont(font);
        QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
        messageBox.setStyleSheet(styleSheet);
        messageBox.setIcon(QMessageBox::NoIcon);
        messageBox.setWindowTitle("单个注册");
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        messageBox.setDefaultButton(QMessageBox::Cancel);
        QString log;
        log = "确定要让指定的CAN设备单个注册吗?";
        messageBox.setText(log);
        ret = messageBox.exec();
    }
    else if(CGlobal::instance()->m_isSelectLoop)
    {
        // 在QMessageBox中设置字体
        QMessageBox messageBox;
        messageBox.setFont(font);
        QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
        messageBox.setStyleSheet(styleSheet);
        messageBox.setIcon(QMessageBox::NoIcon);
        messageBox.setWindowTitle("单个注册");
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        messageBox.setDefaultButton(QMessageBox::Cancel);
        QString log;
        log = "确定要让指定的回路单个注册吗?";
        messageBox.setText(log);
        ret = messageBox.exec();
    }

    if(ret == QMessageBox::Yes)
    {
        CMsgLoginData m_MsgLogin;
        //获取当前点击的集中电源地址
        m_MsgLogin.nDistributeID = CGlobal::instance()->m_nDistriAddress;
        if(CGlobal::instance()->m_isSelectLoop)
            m_MsgLogin.nLoopID = CGlobal::instance()->m_nLoopAddress;
        else
            m_MsgLogin.nLoopID = 0;
        if(m_MsgLogin.nDistributeID == 0){
            return;
        }
        if(!CGlobal::instance()->ClientBusiness()->exeCommand(NCT_LoginInfoLoop, &m_MsgLogin))
        {
            CGlobal::instance()->m_HaveDialog = true;
            QFont font;
            font.setPointSize(12); // 设置字体大小为12像素
            // 在QMessageBox中设置字体
            QMessageBox messageBox;
            messageBox.setFont(font);
            QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
            messageBox.setStyleSheet(styleSheet);
            messageBox.setIcon(QMessageBox::NoIcon);
            messageBox.setWindowTitle("信息");
            messageBox.setStandardButtons(QMessageBox::Ok);
            QString log;
            log = "单个注册失败！";
            messageBox.setText(log);
            messageBox.exec();
            CGlobal::instance()->m_HaveDialog = false;
        } else {

            CGlobal::instance()->m_bInLogin = true;
            CGlobal::instance()->InformationWindow()->setMovie(":/images/allregedit.png");
            CGlobal::instance()->InformationWindow()->setDescription(QString("<h1><font color=black>%1</font></h1>").arg("单个注册......!"));
            CGlobal::instance()->InformationWindow()->show();
            CGlobal::instance()->InformationWindow()->hideCloseButton();
            CGlobal::instance()->setMSgColor(QColor(12, 180, 3));
            CGlobal::instance()->setMsgText("单个注册......!");
        }
    }
}

//单个清除注册
void CProgramView::slot_unLoginLoop()
{
    QFont font;
    font.setPointSize(12); // 设置字体大小为12像素
    // 在QMessageBox中设置字体
    QMessageBox messageBox;
    messageBox.setFont(font);
    QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
    messageBox.setStyleSheet(styleSheet);
    messageBox.setIcon(QMessageBox::NoIcon);
    messageBox.setWindowTitle("单个清除注册");
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Cancel);
    QString log;
    if(CGlobal::instance()->m_isSelectDistribution)
    {
        log = "确定要让指定的CAN设备清除注册吗?";
    }
    else if(CGlobal::instance()->m_isSelectLoop)
    {
        log = "确定要让指定的回路清除注册吗?";
    }
    messageBox.setText(log);
    int ret = messageBox.exec();
    if(ret == QMessageBox::Yes)
    {
        m_isRemoveLoop = true;
        CGlobal::instance()->InformationWindow()->setMovie(":/images/allregedit.png");
        CGlobal::instance()->InformationWindow()->setDescription(QString("<h1><font color=black>%1</font></h1>").arg("单个清除注册......!"));
        CGlobal::instance()->InformationWindow()->show();
        CGlobal::instance()->InformationWindow()->hideCloseButton();
        CMsgLoginData m_MsgLogin;
        //获取当前点击的集中电源地址
        m_MsgLogin.nDistributeID = CGlobal::instance()->m_nDistriAddress;
        if(CGlobal::instance()->m_isSelectLoop)
            m_MsgLogin.nLoopID = CGlobal::instance()->m_nLoopAddress;
        else
            m_MsgLogin.nLoopID = 0;
        if(m_MsgLogin.nDistributeID == 0){
            return;
        }
        if(!CGlobal::instance()->ClientBusiness()->exeCommand(NCT_UnLoginLoop, &m_MsgLogin))
        {
            CGlobal::instance()->m_HaveDialog = true;
            QFont font;
            font.setPointSize(12); // 设置字体大小为12像素
            // 在QMessageBox中设置字体
            QMessageBox messageBox;
            messageBox.setFont(font);
            QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
            messageBox.setStyleSheet(styleSheet);
            messageBox.setIcon(QMessageBox::NoIcon);
            messageBox.setWindowTitle("信息");
            messageBox.setStandardButtons(QMessageBox::Ok);
            QString log;
            log = "单个清除注册失败！";
            messageBox.setText(log);
            messageBox.exec();
            CGlobal::instance()->m_HaveDialog = false;
        }
        else
        {
            CGlobal::instance()->m_bInLogin = false;
            QTimer::singleShot(500, this, SLOT(slot_removeLoginObject()));

        }
    }
}
void CProgramView::slot_removeLoginObject()
{
    if(m_isRemoveLoop)
        CGlobal::instance()->ClientBusiness()->removeLoginObject(CGlobal::instance()->m_nDistriAddress,CGlobal::instance()->m_nLoopAddress);
    else
        CGlobal::instance()->ClientBusiness()->removeLoginObject();
}


//重编所有回路灯具
void CProgramView::slot_recodeAllDevice()
{
    int deviceAdd;
    CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
    if(controller){
        for(int i=0;i<62;i++)
        {
            CDistribution* distribution = controller->distributionByAddress(i+1);
            if(!distribution)
                continue;
            for(int j=0;j<8;j++)
            {
                CLoop* loop = distribution->loopByAdd(j+1);
                if(!loop)
                    continue;
                QList<CDevice*> devices = loop->devices();
                for(int m=0; m<devices.count(); m++){
                    deviceAdd = devices.at(m)->deviceAdd();
                    for(int n = m+1; n<devices.count(); n++)
                    {
                        if(deviceAdd == devices.at(n)->deviceAdd())
                        {
                            CGlobal::instance()->InformationWindow()->setDescription(QString("<h1><font color=black>%1</font></h1>").arg("重编地址......!"));
                            CGlobal::instance()->InformationWindow()->show();
                            CGlobal::instance()->InformationWindow()->hideCloseButton();
                            CGlobal::instance()->setMSgColor(QColor(12, 180, 3));
                            CGlobal::instance()->setMsgText("重编地址......!");
                            qDebug() << m << n << deviceAdd;
                            for(int z = 1; z<256; z++)
                            {
                                CDevice* device = loop->deviceByAdd(z);
                                if(!device)
                                {
                                    qDebug() << z;
                                    device = devices.at(n);
                                    device->setDeviceValue(DEVICE_VALUE_ADDR,z);
                                    device->setDeviceValue(DEVICE_VALUE_CHANGEADD,1);
                                    CMsgObjectStatus msgobjectstatus;
                                    msgobjectstatus.nDisID = distribution->distributionAddress();
                                    msgobjectstatus.nLoopID = loop->loopAdd();
                                    msgobjectstatus.nDeviceID = deviceAdd;
                                    msgobjectstatus.nValue = z;
                                    msgobjectstatus.strText = device->deviceProductionId();
                                    Sleep(500);               //延时发送重编地址
                                    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceAdd, &msgobjectstatus);
                                    break;
                                }
                            }
                        }
                    }
                }
            }  
        }
        CGlobal::instance()->InformationWindow()->hide();
    }
}

//重编当前回路灯具
void CProgramView::slot_recodeLoopDevice()
{
    int deviceAdd;
    CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
    if(controller){
       CDistribution* distribution = controller->distributionByAddress(CGlobal::instance()->m_nDistriAddress);
        if(distribution){
            CLoop* loop = distribution->loopByAdd(CGlobal::instance()->m_nLoopAddress);
            if(loop){
                 QList<CDevice*> devices = loop->devices();
                for(int m=0; m<devices.count(); m++){
                    deviceAdd = devices.at(m)->deviceAdd();
                    for(int n = m+1; n<devices.count(); n++)
                    {
                        if(deviceAdd == devices.at(n)->deviceAdd())
                        {
                            qDebug() << m << n << deviceAdd;
                            for(int i = 1; i<256; i++)
                            {
                                CDevice* device = loop->deviceByAdd(i);
                                if(!device)
                                {
                                    CGlobal::instance()->InformationWindow()->setDescription(QString("<h1><font color=black>%1</font></h1>").arg("重编地址......!"));
                                    CGlobal::instance()->InformationWindow()->show();
                                    CGlobal::instance()->InformationWindow()->hideCloseButton();
                                    CGlobal::instance()->setMSgColor(QColor(12, 180, 3));
                                    CGlobal::instance()->setMsgText("重编地址......!");
                                    qDebug() << i;
                                    device = devices.at(n);
                                    device->setDeviceValue(DEVICE_VALUE_ADDR,i);
                                    device->setDeviceValue(DEVICE_VALUE_CHANGEADD,1);
                                    CMsgObjectStatus msgobjectstatus;
                                    msgobjectstatus.nDisID = CGlobal::instance()->m_nDistriAddress;
                                    msgobjectstatus.nLoopID = CGlobal::instance()->m_nLoopAddress;
                                    msgobjectstatus.nDeviceID = deviceAdd;
                                    msgobjectstatus.nValue = i;
                                    msgobjectstatus.strText = device->deviceProductionId();
                                    Sleep(500);               //延时发送重编地址
                                    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceAdd, &msgobjectstatus);
//                                    ConfigFileManager cfg;
//                                    cfg.SaveConfigFile();//写入配置文件
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        CGlobal::instance()->InformationWindow()->hide();
    }
}

void CProgramView::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

//保存所有回路灯具
void CProgramView::slot_saveAllDevice()
{
    int deviceAdd;
    bool reflag = false;
    CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
    if(controller){
        QList<CDistribution*> distributions = controller->distributions();
        for(int i=0; i<distributions.count(); i++){
            CDistribution* distribution = distributions.at(i);
            if(distribution)
                continue;
            for(int j=0;j<8;j++)
            {
                CLoop* loop = distribution->loopByAdd(j+1);
                if(!loop)
                    continue;
                QList<CDevice*> devices = loop->devices();
                for(int m=0; m<devices.count(); m++){
                    deviceAdd = devices.at(m)->deviceAdd();
                    for(int n = m+1; n<devices.count(); n++)
                    {
                        if(deviceAdd == devices.at(n)->deviceAdd())
                        {
                            qDebug() << m << n << deviceAdd;
                            QFont font;
                            font.setPointSize(12); // 设置字体大小为12像素
                            // 在QMessageBox中设置字体
                            QMessageBox messageBox;
                            messageBox.setFont(font);
                            QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
                            messageBox.setStyleSheet(styleSheet);
                            messageBox.setIcon(QMessageBox::NoIcon);
                            messageBox.setWindowTitle("信息");
                            messageBox.setStandardButtons(QMessageBox::Ok);
                            QString log;
                            log = tr("集中电源%1").arg(distribution->distributionAddress())
                                    + tr("回路%2").arg(loop->loopAdd()) + "存在重码";
                            messageBox.setText(log);
                            messageBox.exec();
                            reflag = true;
                            return;
                        }
                    }
                }
                devices.clear();
            }
            distributions.clear();
        }
    }
    if(reflag == false){
        QFont font;
        font.setPointSize(12); // 设置字体大小为12像素
        // 在QMessageBox中设置字体
        QMessageBox messageBox;
        messageBox.setFont(font);
        QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
        messageBox.setStyleSheet(styleSheet);
        messageBox.setIcon(QMessageBox::NoIcon);
        messageBox.setWindowTitle("信息");
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        messageBox.setDefaultButton(QMessageBox::Cancel);
        QString log;
        log = "是否从数据库中删除未注册到的灯具?" + tr("\n") + "Yes:是" + tr("\n") + "No:否" + tr("\n") + "Cancel:取消";
        messageBox.setText(log);
        int ret = messageBox.exec();
        if(ret == QMessageBox::Cancel)
        {
            return;
        }
        if(ret == QMessageBox::Yes)
        {
            QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");;
            db.setDatabaseName(fileName);
            if(!db.open())
                return;
            QSqlQuery query(db);
            if(controller){
                QList<CDistribution*> distributions = controller->distributions();
                for(int i=0; i<distributions.count(); i++){
                    qDebug()<< distributions.count() << "------";
                    CDistribution* distribution = distributions.at(i);
                    if(!distribution)
                        continue;
                    for(int j=0;j<8;j++)
                    {
                        CLoop* loop = distribution->loopByAdd(j+1);
                        if(!loop)
                            continue;
                        QList<CDevice*> devices = loop->devices();
                        for(int m=0; m<devices.count(); m++){
                            CDevice* device = devices.at(m);
                            if(device->deviceValue(DEVICE_VALUE_ISUSED) == 0)
                            {
                                int id = CGlobal::instance()->saveKeyId(OBJT_Device,device->keyId());
                                query.exec(QString("DELETE FROM LampInfo WHERE ID=%1").arg(id));
                                query.exec(QString("DELETE FROM Coordinate WHERE LampInfoID=%1").arg(id));
                                query.exec(QString("DELETE FROM EvacuationPathItem WHERE LampInfoID=%1").arg(id));
                                query.exec(QString("DELETE FROM LampConnection WHERE StartLampID=%1").arg(id));
                                query.exec(QString("DELETE FROM LampConnection WHERE EndLampID=%1").arg(id));
                                query.exec(QString("DELETE FROM LampDefaultDirection WHERE LampInfoID=%1").arg(id));
                            }
                        }
                        devices.clear();
                    }
                }
                distributions.clear();
            }
            if(!query.exec())
            {
                qDebug() << query.lastError();
            }
            db.close();
            CGlobal::instance()->m_isDelUnregLampFlag = true;
        }
        CGlobal::instance()->programTreeView()->slotSaveProject();
        CGlobal::instance()->m_isDelUnregLampFlag = false;
    }
//    ConfigFileManager cfg;
//    cfg.SaveConfigFile();//写入配置文件
}

//保存当前回路灯具
void CProgramView::slot_saveLoopDevice()
{
    int deviceAdd;
    CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
    if(controller){
       CDistribution* distribution = controller->distributionByAddress(CGlobal::instance()->m_nDistriAddress);
        if(distribution){
            CLoop* loop = distribution->loopByAdd(CGlobal::instance()->m_nLoopAddress);
            if(loop){
                QList<CDevice*> devices = loop->devices();
                for(int m=0; m<devices.count(); m++){
                    deviceAdd = devices.at(m)->deviceAdd();
                    for(int n = m+1; n<devices.count(); n++)
                    {
                        if(deviceAdd == devices.at(n)->deviceAdd())
                        {
                            qDebug() << m << n << deviceAdd;
                            QFont font;
                            font.setPointSize(12); // 设置字体大小为12像素
                            // 在QMessageBox中设置字体
                            QMessageBox messageBox;
                            messageBox.setFont(font);
                            QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
                            messageBox.setStyleSheet(styleSheet);
                            messageBox.setIcon(QMessageBox::NoIcon);
                            messageBox.setWindowTitle("信息");
                            messageBox.setStandardButtons(QMessageBox::Ok);
                            QString log;
                            log = "当前回路存在重码";
                            messageBox.setText(log);
                            messageBox.exec();
                            return;
                        }
                    }
                }
                QFont font;
                font.setPointSize(12); // 设置字体大小为12像素
                // 在QMessageBox中设置字体
                QMessageBox messageBox;
                messageBox.setFont(font);
                QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
                messageBox.setStyleSheet(styleSheet);
                messageBox.setIcon(QMessageBox::NoIcon);
                messageBox.setWindowTitle("信息");
                messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
                messageBox.setDefaultButton(QMessageBox::Cancel);
                QString log;
                log = "是否从数据库中删除未注册到的灯具?" + tr("\n") + "Yes:是" + tr("\n") + "No:否" + tr("\n") + "Cancel:取消";
                messageBox.setText(log);
                int ret = messageBox.exec();
                if(ret == QMessageBox::Cancel)
                {
                    return;
                }
                if(ret == QMessageBox::Yes)
                {
                    CGlobal::instance()->m_isDelUnregLampFlag = true;
                    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
                    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",fileName);
                    db.setDatabaseName(fileName);
                    if(!db.open())
                        return;
                    QSqlQuery query(db);
                    for(int m=0; m<devices.count(); m++){
                        CDevice* device = devices.at(m);
                        if(device)
                        {
                            QString sql = QString("DELETE FROM LampInfo WHERE ChannelInfoID = %1").arg(CGlobal::instance()->saveKeyId(OBJT_Loop,loop->keyId()));
                            query.prepare(sql);
                            query.exec();
                        }

                    }
                    if(!query.exec())
                    {
                        qDebug()<<query.lastError();
                    }
                    db.close();
                }
                const QString &dbName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
                CDBSqlite db(dbName);
                if(!db.open())
                {
                    QFont font;
                    font.setPointSize(12); // 设置字体大小为12像素
                    // 在QMessageBox中设置字体
                    QMessageBox messageBox;
                    messageBox.setFont(font);
                    QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
                    messageBox.setStyleSheet(styleSheet);
                    messageBox.setIcon(QMessageBox::NoIcon);
                    messageBox.setWindowTitle("信息");
                    messageBox.setStandardButtons(QMessageBox::Ok);
                    QString log;
                    log = "保存失败！";
                    messageBox.setText(log);
                    messageBox.exec();
                    return;
                }
                else
                {
                    if(loop->saveLoopdevice(&db) != 0)
                    {
                        QFont font;
                        font.setPointSize(12); // 设置字体大小为12像素
                        // 在QMessageBox中设置字体
                        QMessageBox messageBox;
                        messageBox.setFont(font);
                        QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
                        messageBox.setStyleSheet(styleSheet);
                        messageBox.setIcon(QMessageBox::NoIcon);
                        messageBox.setWindowTitle("信息");
                        messageBox.setStandardButtons(QMessageBox::Ok);
                        QString log;
                        log = "保存失败！";
                        messageBox.setText(log);
                        messageBox.exec();
                    }
                    else
                    {
                        QFont font;
                        font.setPointSize(12); // 设置字体大小为12像素
                        // 在QMessageBox中设置字体
                        QMessageBox messageBox;
                        messageBox.setFont(font);
                        QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
                        messageBox.setStyleSheet(styleSheet);
                        messageBox.setIcon(QMessageBox::NoIcon);
                        messageBox.setWindowTitle("信息");
                        messageBox.setStandardButtons(QMessageBox::Ok);
                        QString log;
                        log = "保存成功，请重启软件！(主菜单中“系统”->“重启”)";
                        messageBox.setText(log);
                        messageBox.exec();
                    }
                }
                db.close();
                CGlobal::instance()->m_isDelUnregLampFlag = false;
            }
        }
    }
//    ConfigFileManager cfg;
//    cfg.SaveConfigFile();//写入配置文件
}
