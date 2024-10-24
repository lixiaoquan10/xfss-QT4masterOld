#include "cglobal.h"


CProgramView::CProgramView(QWidget *parent) :
    QWidget(parent)
{
    CGlobal::instance()->setProgramview(this);
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
    buttonLayout->addWidget(m_store);
    buttonLayout->addWidget(m_storeAll);
    buttonLayout->addStretch();

    m_pSplitter = new CMiniSplitter(this);
    CProgramTreeView* treeView = new CProgramTreeView(m_pSplitter);
//    treeView->setFixedWidth(treeView->sizeHint().width());
    treeView->setFixedWidth(320);
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

    //更新注册页面状态
    m_timerReset = new QTimer();
    connect(m_timerReset, SIGNAL(timeout()), this, SLOT(slot_resetControllerView()));
    m_timerReset->start(1000);

}

CProgramView::~CProgramView()
{
    CGlobal::instance()->setProgramview(0);
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
    delete m_timerReset;
}

void CProgramView::slot_resetControllerView()
{
    qDebug() << "CProgramView::slot_resetControllerView" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(CGlobal::instance()->m_isSelectLoop)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(CGlobal::instance()->m_nCanportAddress);
        if(canport)
        {
            CDistribution* distribution = canport->distributionByAddress(CGlobal::instance()->m_nDistriAddress);
            if(distribution)
            {
                CLoop* loop = distribution->loopByAdd(CGlobal::instance()->m_nLoopAddress);
                if(loop)
                {
                    CGlobal::instance()->programDeviceView()->setUpdateEnble();
                    CGlobal::instance()->programDeviceView()->setLoop(loop);
                }
            }
        }
    }
    CGlobal::instance()->programTreeView()->resetControllerTreeView();
}


//全部注册
void CProgramView::slot_regALL()
{
    qDebug() << "CProgramView::slot_regALL" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    // 设置按钮显示文字
    messageBox.button(QMessageBox::Yes)->setText("是");
    messageBox.button(QMessageBox::Cancel)->setText("取消");
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
             messageBox.button(QMessageBox::Ok)->setText("确定");
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
    qDebug() << "CProgramView::slot_unLogin" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_isRemoveLoop = false;
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
    // 设置按钮显示文字
    messageBox.button(QMessageBox::Yes)->setText("是");
    messageBox.button(QMessageBox::Cancel)->setText("取消");
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
            messageBox.button(QMessageBox::Ok)->setText("确定");
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
    qDebug() << "CProgramView::slot_regLoop" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
        // 设置按钮显示文字
        messageBox.button(QMessageBox::Yes)->setText("是");
        messageBox.button(QMessageBox::Cancel)->setText("取消");
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
        // 设置按钮显示文字
        messageBox.button(QMessageBox::Yes)->setText("是");
        messageBox.button(QMessageBox::Cancel)->setText("取消");
        QString log;
        log = "确定要让指定的回路单个注册吗?";
        messageBox.setText(log);
        ret = messageBox.exec();
    }

    if(ret == QMessageBox::Yes)
    {
        CMsgObjectStatus msg;
        //获取当前点击的集中电源地址
        msg.nDisID = CGlobal::instance()->m_nDistriAddress;
        if(CGlobal::instance()->m_isSelectLoop)
            msg.nLoopID = CGlobal::instance()->m_nLoopAddress;
        else
            msg.nLoopID = 0;
        msg.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
        if(msg.nDisID == 0){
            return;
        }
        if(!CGlobal::instance()->ClientBusiness()->exeCommand(NCT_LoginInfoLoop, &msg))
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
            messageBox.button(QMessageBox::Ok)->setText("确定");
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
    qDebug() << "CProgramView::slot_unLoginLoop" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    // 设置按钮显示文字
    messageBox.button(QMessageBox::Yes)->setText("是");
    messageBox.button(QMessageBox::Cancel)->setText("取消");
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
        CMsgObjectStatus msg;;
        //获取当前点击的集中电源地址
        msg.nDisID = CGlobal::instance()->m_nDistriAddress;
        msg.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
        if(CGlobal::instance()->m_isSelectLoop)
            msg.nLoopID = CGlobal::instance()->m_nLoopAddress;
        else
            msg.nLoopID = 0;
        if(msg.nDisID == 0){
            return;
        }
        if(!CGlobal::instance()->ClientBusiness()->exeCommand(NCT_UnLoginLoop, &msg))
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
            messageBox.button(QMessageBox::Ok)->setText("确定");
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
    qDebug() << "CProgramView::slot_removeLoginObject" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_isRemoveLoop)
        CGlobal::instance()->ClientBusiness()->removeLoginObject(CGlobal::instance()->m_nCanportAddress,CGlobal::instance()->m_nDistriAddress,
                                                                 CGlobal::instance()->m_nLoopAddress);
    else
        CGlobal::instance()->ClientBusiness()->removeLoginObject();
}


//重编所有回路灯具
void CProgramView::slot_recodeAllDevice()
{
    qDebug() << "CProgramView::slot_recodeAllDevice" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int deviceAdd;
    for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
        if(canport){
            for(int i=0;i<62;i++)
            {
                CDistribution* distribution = canport->distributionByAddress(i+1);
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
                                        msgobjectstatus.nCanportAdd = device->canportAdd();
                                        msgobjectstatus.nDisID = device->distributionAdd();
                                        msgobjectstatus.nLoopID = device->loopAdd();
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
}

//重编当前回路灯具
void CProgramView::slot_recodeLoopDevice()
{
    qDebug() << "CProgramView::slot_recodeLoopDevice" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int deviceAdd;
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(CGlobal::instance()->m_nCanportAddress);
    if(canport){
       CDistribution* distribution = canport->distributionByAddress(CGlobal::instance()->m_nDistriAddress);
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
                                    msgobjectstatus.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
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
    qDebug() << "CProgramView::setController" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

//保存所有回路灯具
void CProgramView::slot_saveAllDevice()
{
    qDebug() << "CProgramView::slot_saveAllDevice" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    bool reflag = false;
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
    // 设置按钮显示文字
    messageBox.button(QMessageBox::Yes)->setText("是");
    messageBox.button(QMessageBox::No)->setText("否");
    messageBox.button(QMessageBox::Cancel)->setText("取消");
    QString log;
    log = "是否从数据库中删除未注册到的灯具?";
    messageBox.setText(log);
    int ret = messageBox.exec();
    if(ret == QMessageBox::Cancel)
    {
        return;
    }
    else if(ret == QMessageBox::Yes)
    {
        QMessageBox messageBox;
        messageBox.setFont(font);
        QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
        messageBox.setStyleSheet(styleSheet);
        messageBox.setIcon(QMessageBox::NoIcon);
        messageBox.setWindowTitle("信息");
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        messageBox.setDefaultButton(QMessageBox::Cancel);
        // 设置按钮显示文字
        messageBox.button(QMessageBox::Yes)->setText("是");
        messageBox.button(QMessageBox::Cancel)->setText("取消");
        QString log;
        log = "删除未注册到的灯具可能会影响已配置的平面图，确定要删除吗?";
        messageBox.setText(log);
        int ret = messageBox.exec();
        if(ret == QMessageBox::Cancel)
        {
            return;
        }
        else if(ret == QMessageBox::Yes)
        {
            //先判断是否存在重码
            for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
            {
                CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
                if(canport){
                    QList<CDistribution*> distributions = canport->distributions();
                    for(int i=0; i<distributions.count(); i++){
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
                                for(int n = m+1; n<devices.count(); n++)
                                {
                                    //地址相同并且都为注册过的灯具
                                    if((devices.at(m)->deviceAdd() == devices.at(n)->deviceAdd())
                                            && devices.at(m)->isDeviceLogin() && devices.at(n)->isDeviceLogin())
                                    {
                                        qDebug() << m << n << devices.at(m)->deviceAdd();
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
                                        messageBox.button(QMessageBox::Ok)->setText("确定");
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
            }
            //存在重码，直接返回
            if(reflag) return;
            QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
            // 生成当前日期时间作为备份文件名后缀
            QDateTime currentDateTime = QDateTime::currentDateTime();
            QString backupSuffix = currentDateTime.toString("_yyyyMMdd_hhmmss");
            QString backupFileName = QString("ESSQLiteCE100%1.db").arg(backupSuffix);
            QString backupFilePath = CGlobal::instance()->workspaces() + "/logfile/DatabaseBackup/" + backupFileName;
            // 检查目标文件夹下文件数量
            QDir backupDir(CGlobal::instance()->workspaces() + "/logfile/DatabaseBackup");
            QStringList filters;
            filters << "ESSQLiteCE100*.db";
            backupDir.setNameFilters(filters);
            QFileInfoList fileInfoList = backupDir.entryInfoList(QDir::Files, QDir::Name);
            // 如果文件数量超过50个，删除最早的文件
            if (fileInfoList.size() >= 50) {
                // 获取最早的文件信息
                QFileInfo oldestFileInfo = fileInfoList.first();
                QString oldestFilePath = oldestFileInfo.absoluteFilePath();

                // 删除最早的文件
                if (!QFile::remove(oldestFilePath)) {
                    QMessageBox::critical(this, "错误", "无法删除最早的备份文件！");
                    return;
                }
            }
            if((QFile::copy(fileName, backupFilePath)))
            {
                CGlobal::instance()->InformationWindow()->setDescription(QString("<h1><font color=black>%1</font></h1>").arg("数据库保存中......!"));
                CGlobal::instance()->InformationWindow()->show();
                CGlobal::instance()->InformationWindow()->hideCloseButton();
                CGlobal::instance()->setMSgColor(QColor(12, 180, 3));
                CGlobal::instance()->setMsgText("数据库保存中......!");
                QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");;
                db.setDatabaseName(fileName);
                if(!db.open())
                    return;
                QSqlQuery query(db);
                for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
                {
                    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
                    if(canport){
                        QList<CDistribution*> distributions = canport->distributions();
                        for(int i=0; i<distributions.count(); i++){
                            QApplication::processEvents();
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
                                    if(!device)
                                        continue;
    //                                if(device->deviceValue(DEVICE_VALUE_ISUSED) == 0)
                                    if(!device->isDeviceLogin())
                                    {
                                        int id = CGlobal::instance()->saveKeyId(OBJT_Device,device->keyId());
                                        query.exec(QString("DELETE FROM LampInfo WHERE ID=%1").arg(id));
                                        query.exec(QString("DELETE FROM Coordinate WHERE LampInfoID=%1").arg(id));
                                        query.exec(QString("DELETE FROM EvacuationPathItem WHERE LampInfoID=%1").arg(id));
                                        query.exec(QString("DELETE FROM LampConnection WHERE StartLampID=%1").arg(id));
                                        query.exec(QString("DELETE FROM LampConnection WHERE EndLampID=%1").arg(id));
                                        query.exec(QString("DELETE FROM LampDefaultDirection WHERE LampInfoID=%1").arg(id));
                                        query.exec(QString("DELETE FROM LampToFirePoint WHERE LampInfoID=%1").arg(id));
                                        query.exec(QString("DELETE FROM LampToLaunchGroup WHERE LampInfoID=%1").arg(id));
                                    }
                                }
                                devices.clear();
                            }
                        }
                        distributions.clear();
                    }
                }
                if(!query.exec())
                {
                    qDebug() << query.lastError();
                }
                db.close();
                CGlobal::instance()->m_isDelUnregLampFlag = true;
            }
        }
    }
    else if(ret == QMessageBox::No)
    {
        //先判断是否存在重码
        for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
        {
            CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
            if(canport){
                QList<CDistribution*> distributions = canport->distributions();
                for(int i=0; i<distributions.count(); i++){
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
                            for(int n = m+1; n<devices.count(); n++)
                            {
                                //地址相同
                                if(devices.at(m)->deviceAdd() == devices.at(n)->deviceAdd())
                                {
                                    qDebug() << m << n << devices.at(m)->deviceAdd();
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
        }
        //存在重码，直接返回
        if(reflag) return;
        CGlobal::instance()->InformationWindow()->setDescription(QString("<h1><font color=black>%1</font></h1>").arg("数据库保存中......!"));
        CGlobal::instance()->InformationWindow()->show();
        CGlobal::instance()->InformationWindow()->hideCloseButton();
        CGlobal::instance()->setMSgColor(QColor(12, 180, 3));
        CGlobal::instance()->setMsgText("数据库保存中......!");
    }
    CGlobal::instance()->programTreeView()->slotSaveController();
    CGlobal::instance()->m_isDelUnregLampFlag = false;
}

//保存当前回路灯具
void CProgramView::slot_saveLoopDevice()
{
    qDebug() << "CProgramView::slot_saveLoopDevice" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    bool reflag = false;
    QFont font;
    font.setPointSize(12); // 设置字体大小为12像素
    QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(CGlobal::instance()->m_nCanportAddress);
    if(canport){
       CDistribution* distribution = canport->distributionByAddress(CGlobal::instance()->m_nDistriAddress);
        if(distribution){
            CLoop* loop = distribution->loopByAdd(CGlobal::instance()->m_nLoopAddress);
            if(loop){
                // 在QMessageBox中设置字体
                QMessageBox messageBox;
                messageBox.setFont(font);
                messageBox.setStyleSheet(styleSheet);
                messageBox.setIcon(QMessageBox::NoIcon);
                messageBox.setWindowTitle("信息");
                messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
                messageBox.setDefaultButton(QMessageBox::Cancel);
                // 设置按钮显示文字
                messageBox.button(QMessageBox::Yes)->setText("是");
                messageBox.button(QMessageBox::No)->setText("否");
                messageBox.button(QMessageBox::Cancel)->setText("取消");
                QString log;
                log = "是否从数据库中删除未注册到的灯具?";
                messageBox.setText(log);
                int ret = messageBox.exec();
                if(ret == QMessageBox::Cancel)
                {
                    return;
                }
                else if(ret == QMessageBox::Yes)
                {
                    QList<CDevice*> devices = loop->devices();
                    for(int m=0; m<devices.count(); m++){
                        for(int n = m+1; n<devices.count(); n++)
                        {
                            //地址相同并且都为注册过的灯具
                            if((devices.at(m)->deviceAdd() == devices.at(n)->deviceAdd())
                                    && devices.at(m)->isDeviceLogin() && devices.at(n)->isDeviceLogin())
                            {
                                qDebug() << m << n << devices.at(m)->deviceAdd();
                                // 在QMessageBox中设置字体
                                QMessageBox messageBox;
                                messageBox.setFont(font);
                                messageBox.setStyleSheet(styleSheet);
                                messageBox.setIcon(QMessageBox::NoIcon);
                                messageBox.setWindowTitle("信息");
                                messageBox.setStandardButtons(QMessageBox::Ok);
                                messageBox.button(QMessageBox::Ok)->setText("确定");
                                QString log;
                                log = "当前回路存在重码";
                                messageBox.setText(log);
                                messageBox.exec();
                                reflag = true;
                                return;
                            }
                        }
                    }
                    //存在重码，直接返回
                    if(reflag) return;
                    QMessageBox messageBox;
                    messageBox.setFont(font);
                    QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
                    messageBox.setStyleSheet(styleSheet);
                    messageBox.setIcon(QMessageBox::NoIcon);
                    messageBox.setWindowTitle("信息");
                    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
                    messageBox.setDefaultButton(QMessageBox::Cancel);
                    // 设置按钮显示文字
                    messageBox.button(QMessageBox::Yes)->setText("是");
                    messageBox.button(QMessageBox::Cancel)->setText("取消");
                    QString log;
                    log = "删除未注册到的灯具可能会影响已配置的平面图，确定要删除吗?";
                    messageBox.setText(log);
                    int ret = messageBox.exec();
                    if(ret == QMessageBox::Cancel)
                    {
                        return;
                    }
                    else if(ret == QMessageBox::Yes)
                    {
                        QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
                        // 生成当前日期时间作为备份文件名后缀
                        QDateTime currentDateTime = QDateTime::currentDateTime();
                        QString backupSuffix = currentDateTime.toString("_yyyyMMdd_hhmmss");
                        QString backupFileName = QString("ESSQLiteCE100%1.db").arg(backupSuffix);
                        QString backupFilePath = CGlobal::instance()->workspaces() + "/logfile/DatabaseBackup/" + backupFileName;
                        // 检查目标文件夹下文件数量
                        QDir backupDir(CGlobal::instance()->workspaces() + "/logfile/DatabaseBackup");
                        QStringList filters;
                        filters << "ESSQLiteCE100*.db";
                        backupDir.setNameFilters(filters);
                        QFileInfoList fileInfoList = backupDir.entryInfoList(QDir::Files, QDir::Name);
                        // 如果文件数量超过50个，删除最早的文件
                        if (fileInfoList.size() >= 50) {
                            // 获取最早的文件信息
                            QFileInfo oldestFileInfo = fileInfoList.first();
                            QString oldestFilePath = oldestFileInfo.absoluteFilePath();

                            // 删除最早的文件
                            if (!QFile::remove(oldestFilePath)) {
                                QMessageBox::critical(this, "错误", "无法删除最早的备份文件！");
                                return;
                            }
                        }
                        if((QFile::copy(fileName, backupFilePath)))
                        {
                            CGlobal::instance()->InformationWindow()->setDescription(QString("<h1><font color=black>%1</font></h1>").arg("数据库保存中......!"));
                            CGlobal::instance()->InformationWindow()->show();
                            CGlobal::instance()->InformationWindow()->hideCloseButton();
                            CGlobal::instance()->setMSgColor(QColor(12, 180, 3));
                            CGlobal::instance()->setMsgText("数据库保存中......!");
                            CGlobal::instance()->m_isDelUnregLampFlag = true;
                            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",fileName);
                            db.setDatabaseName(fileName);
                            if(!db.open())
                                return;
                            QSqlQuery query(db);
                            QList<CDevice*> devices = loop->devices();
                            for(int m=0; m<devices.count(); m++){
                                CDevice* device = devices.at(m);
                                if(!device)
                                    continue;
                                if(!device->isDeviceLogin())
                                {
                                    int id = CGlobal::instance()->saveKeyId(OBJT_Device,device->keyId());
                                    query.exec(QString("DELETE FROM LampInfo WHERE ID=%1").arg(id));
                                    query.exec(QString("DELETE FROM Coordinate WHERE LampInfoID=%1").arg(id));
                                    query.exec(QString("DELETE FROM EvacuationPathItem WHERE LampInfoID=%1").arg(id));
                                    query.exec(QString("DELETE FROM LampConnection WHERE StartLampID=%1").arg(id));
                                    query.exec(QString("DELETE FROM LampConnection WHERE EndLampID=%1").arg(id));
                                    query.exec(QString("DELETE FROM LampDefaultDirection WHERE LampInfoID=%1").arg(id));
                                    query.exec(QString("DELETE FROM LampToFirePoint WHERE LampInfoID=%1").arg(id));
                                    query.exec(QString("DELETE FROM LampToLaunchGroup WHERE LampInfoID=%1").arg(id));
                                }
                            }
                            devices.clear();
                            if(!query.exec())
                            {
                                qDebug()<<query.lastError();
                            }
                            db.close();
                        }
                        else
                        {
                            // 备份失败，处理错误情况
                            QMessageBox::critical(this, "错误", "无法备份数据库文件！");
                            return;
                        }
                    }
                }
                else if(ret == QMessageBox::No)
                {
                    QList<CDevice*> devices = loop->devices();
                    for(int m=0; m<devices.count(); m++){
                        for(int n = m+1; n<devices.count(); n++)
                        {
                            if(devices.at(m)->deviceAdd() == devices.at(n)->deviceAdd())
                            {
                                qDebug() << m << n << devices.at(m)->deviceAdd();
                                // 在QMessageBox中设置字体
                                QMessageBox messageBox;
                                messageBox.setFont(font);
                                messageBox.setStyleSheet(styleSheet);
                                messageBox.setIcon(QMessageBox::NoIcon);
                                messageBox.setWindowTitle("信息");
                                messageBox.setStandardButtons(QMessageBox::Ok);
                                messageBox.button(QMessageBox::Ok)->setText("确定");
                                QString log;
                                log = "当前回路存在重码";
                                messageBox.setText(log);
                                messageBox.exec();
                                reflag = true;
                                return;
                            }
                        }
                    }
                    //存在重码，直接返回
                    if(reflag) return;
                    CGlobal::instance()->InformationWindow()->setDescription(QString("<h1><font color=black>%1</font></h1>").arg("数据库保存中......!"));
                    CGlobal::instance()->InformationWindow()->show();
                    CGlobal::instance()->InformationWindow()->hideCloseButton();
                    CGlobal::instance()->setMSgColor(QColor(12, 180, 3));
                    CGlobal::instance()->setMsgText("数据库保存中......!");
                }
                const QString &dbName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
                CDBSqlite db(dbName);
                // 在QMessageBox中设置字体
                QMessageBox messageSaveBox;
                messageSaveBox.setFont(font);
                messageSaveBox.setStyleSheet(styleSheet);
                messageSaveBox.setIcon(QMessageBox::NoIcon);
                messageSaveBox.setWindowTitle("信息");
                messageSaveBox.setStandardButtons(QMessageBox::Ok);
                messageSaveBox.button(QMessageBox::Ok)->setText("确定");
                if(!db.open())
                {
                    log = "保存失败！";
                    CGlobal::instance()->setMsgText("系统运行！");
                    CGlobal::instance()->InformationWindow()->hide();
                    messageSaveBox.setText(log);
                    messageSaveBox.exec();
                    return;
                }
                else
                {
                    if(loop->saveLoopdevice(&db) != 0)
                    {
                        log = "保存失败！";
                    }
                    else
                    {
                        log = "保存成功，请重启软件！(主菜单中“系统”->“重启”)";
                    }
                    CGlobal::instance()->setMsgText("系统运行！");
                    CGlobal::instance()->InformationWindow()->hide();
                    messageSaveBox.setText(log);
                    messageSaveBox.exec();
                }
                db.close();
                CGlobal::instance()->m_isDelUnregLampFlag = false;
            }
        }
    }
}
