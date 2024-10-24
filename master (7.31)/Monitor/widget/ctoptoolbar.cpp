
 /*fix （2023-02-06）修改月检查按钮 年检查按钮作为检查按钮的弹出菜单 */

#include "cglobal.h"

CTopToolBar::CTopToolBar(QWidget *parent) :
    QWidget(parent)
{
    initMember();
    createToolBar();
    updateMenu();
    m_process_unmountUSB = new QProcess(this);
//    m_MonthYearCheckTime = new CMsgCheckTime();
//    m_MonthYearCheckTime->systemStartTime = QDateTime::currentDateTime();
//    m_MonthYearCheckTime->systemMainPowerTime = m_MonthYearCheckTime->systemStartTime;
//    m_checktimer = new QTimer(this);
//    connect(m_checktimer, SIGNAL(timeout()), this, SLOT(slot_yearMonthCheck()));
//    m_checktimer->start(1000);


    m_time = new QTimer(this);
    connect(m_time,SIGNAL(timeout()),this,SLOT(slottimeout()));
    m_time->start(10000);
}
void CTopToolBar::slottimeout()
{
    static int i = 0;
    i++;
    // 获取主屏幕的几何信息
    QDesktopWidget *desktop = QApplication::desktop();
    QRect screenGeometry = desktop->screenGeometry();

    // 创建一个父级窗口并设置窗口大小为整个屏幕大小
    QWidget widget;
    widget.setGeometry(screenGeometry);

    // 截取窗口内容并保存为图片
    QPixmap  image = QPixmap::grabWindow(widget.winId(), widget.x(), widget.y(), 1366, 768);
    QString str = "/home/xfss/root/picture/" + QString::number(i) + ".png";
    image.save(str);
}

CTopToolBar::~CTopToolBar()
{
    delete m_style;
    delete m_process_unmountUSB;
//    delete m_MonthYearCheckTime;
}

void CTopToolBar::initMember()
{
    qDebug() << "CTopToolBar::initMember" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_menuBrowse = NULL;
    m_menuRegedit = NULL;
    m_menuManage = NULL;
    m_menuSystem = NULL;
    m_menuCheck = NULL;
    m_menuEmergency = NULL;
    m_menuList = NULL;
    m_speedup = false;
    m_style = new CMyIconStyle();
    m_informationModel = CGlobal::instance()->statusTabs()->m_informationModel;
    m_tabList = CGlobal::instance()->statusTabs()->m_tabList;
    m_tabWidget = CGlobal::instance()->statusTabs()->m_tabWidget;
    m_strSheet = "QMenu {background-color: white; margin: 3px; font-size: 14px;}\
            QMenu::item {padding: 3px 25px 3px 42px; border: 1px solid transparent; min-height: 40px; min-width: 70px; border-radius: 4px;}\
            QMenu::separator {height: 2px; background: #CCCCFF; margin-left: 5px; margin-right: 5px;}\
            QMenu::right-arrow { image: url(:/res/right-arrow.png); }\
            QMenu::indicator:exclusive:checked {image: url(:/res/symbol-check.png);}\
            QMenu::item:selected {background-color: qlineargradient(x1: 0.5, y1: 0, x2: 0.5, y2: 1, stop: 0 #99CCFF, stop: 0.5 #CCFFFF, stop: 1 #99CCFF); border-color: #6699CC;}";
}

void CTopToolBar::createCheckTime()
{
    qDebug() << "CTopToolBar::createCheckTime" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QFont font;
    font.setPointSize(10.5);
    font.setBold(true);
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::blue);
    m_checkTime = new QLabel(this);
    m_checkTime->setWordWrap(true);
    m_checkTime->setFont(font);
    m_checkTime->setPalette(palette);
    m_checkTime->setFixedSize(300, 70);
    m_checkTime->setAlignment(Qt::AlignBottom | Qt::AlignRight);
}

void CTopToolBar::createToolBar()
{
    qDebug() << "CTopToolBar::createToolBar" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    createCheckTime();
    QFont font;
    font.setPointSize(12);

//    CDesignTreeView* treeView = CGlobal::instance()->designTreeView();
    m_btnMuteTool = CGlobal::instance()->CreateToolButton(this, ":/images/play.png", "消音");
    connect(m_btnMuteTool,SIGNAL(clicked()),this,SLOT(slotMuteSystem()));
    m_btnMuteTool->setPopupMode(QToolButton::InstantPopup);
    m_btnMuteTool->setFocusPolicy(Qt::NoFocus);
    m_btnMuteTool->setCheckable(true);
    m_btnMuteTool->setChecked(true);
    m_btnMuteTool->setFont(font);

    m_btnResetTool = CGlobal::instance()->CreateToolButton(this, ":/images/reset.png", "复位");
    connect(m_btnResetTool,SIGNAL(clicked()),this,SLOT(slotResetSystem()));
    m_btnResetTool->setPopupMode(QToolButton::InstantPopup);
    m_btnResetTool->setFocusPolicy(Qt::NoFocus);
    m_btnResetTool->setCheckable(true);
    m_btnResetTool->setChecked(true);
    m_btnResetTool->setFont(font);

    m_btnEmergencyTool = CGlobal::instance()->CreateToolButton(this, ":/images/startinfo.png", "应急");
    connect(m_btnEmergencyTool, SIGNAL(clicked(bool)), this, SLOT(slotShowEmergencyMenu()));
    m_btnEmergencyTool->setPopupMode(QToolButton::InstantPopup);
    m_btnEmergencyTool->setFocusPolicy(Qt::NoFocus);
    m_btnEmergencyTool->setCheckable(true);
    m_btnEmergencyTool->setChecked(true);
    m_btnEmergencyTool->setFont(font);

    /*点击注册按钮显示设备信息*/
    m_btnLoginInfoTool = CGlobal::instance()->CreateToolButton(this, ":/images/regedit.png", "注册");
    connect(m_btnLoginInfoTool,SIGNAL(clicked()),this,SLOT(slotDeviceInfo()));
    m_btnLoginInfoTool->setPopupMode(QToolButton::InstantPopup);
    m_btnLoginInfoTool->setFocusPolicy(Qt::NoFocus);
    m_btnLoginInfoTool->setCheckable(true);
    m_btnLoginInfoTool->setChecked(true);
    m_btnLoginInfoTool->setFont(font);

    m_btnPlanTool = CGlobal::instance()->CreateToolButton(this, ":/images/plan.png", "疏散预案");
    connect(m_btnPlanTool,SIGNAL(clicked()),this,SLOT(slotHandFire()));
    m_btnPlanTool->setPopupMode(QToolButton::InstantPopup);
    m_btnPlanTool->setFocusPolicy(Qt::NoFocus);
    m_btnPlanTool->setCheckable(true);
    m_btnPlanTool->setChecked(true);
    m_btnPlanTool->setFont(font);

    m_btnCanInfoTool = CGlobal::instance()->CreateToolButton(this, ":/images/CanDeviceConfig.png", "CAN设备");
    connect(m_btnCanInfoTool,SIGNAL(clicked()),this,SLOT(slotCanInfo()));
    m_btnCanInfoTool->setPopupMode(QToolButton::InstantPopup);
    m_btnCanInfoTool->setFocusPolicy(Qt::NoFocus);
    m_btnCanInfoTool->setCheckable(true);
    m_btnCanInfoTool->setChecked(true);
    m_btnCanInfoTool->setFont(font);

    m_btnBrowseInfoTool = CGlobal::instance()->CreateToolButton(this, ":/images/browse.png", "信息");
    connect(m_btnBrowseInfoTool,SIGNAL(clicked()),this,SLOT(slotShowBrowseMenu()));
    m_btnBrowseInfoTool->setPopupMode(QToolButton::InstantPopup);
    m_btnBrowseInfoTool->setFocusPolicy(Qt::NoFocus);
    m_btnBrowseInfoTool->setCheckable(true);
    m_btnBrowseInfoTool->setChecked(true);
    m_btnBrowseInfoTool->setFont(font);

    m_btnTableShowTool = CGlobal::instance()->CreateToolButton(this, ":/images/HandEvacuate.png", "视图");
    connect(m_btnTableShowTool,SIGNAL(clicked()),this,SLOT(slotShowListMenu()));
    m_btnTableShowTool->setPopupMode(QToolButton::InstantPopup);
    m_btnTableShowTool->setFocusPolicy(Qt::NoFocus);
    m_btnTableShowTool->setCheckable(true);
    m_btnTableShowTool->setChecked(true);
    m_btnTableShowTool->setFont(font);

    m_btnManageTool = CGlobal::instance()->CreateToolButton(this, ":/images/set.png", "管理");
    connect(m_btnManageTool,SIGNAL(clicked(bool)), this, SLOT(slotShowManageMenu()));
    m_btnManageTool->setPopupMode(QToolButton::InstantPopup);
    m_btnManageTool->setFocusPolicy(Qt::NoFocus);
    m_btnManageTool->setCheckable(true);
    m_btnManageTool->setChecked(true);
    m_btnManageTool->setFont(font);

    m_btnSystemTool = CGlobal::instance()->CreateToolButton(this, ":/images/systempath.png", "系统");
    connect(m_btnSystemTool,SIGNAL(clicked()),this,SLOT(slotShowSystemMenu()));
    m_btnSystemTool->setPopupMode(QToolButton::InstantPopup);
    m_btnSystemTool->setFocusPolicy(Qt::NoFocus);
    m_btnSystemTool->setCheckable(true);
    m_btnSystemTool->setChecked(true);
    m_btnSystemTool->setFont(font);

    m_btnParamTool = CGlobal::instance()->CreateToolButton(this, ":/images/PartCheck.png", "参数");
    connect(m_btnParamTool,SIGNAL(clicked()),this,SLOT(slotSystemSetup()));
    m_btnParamTool->setPopupMode(QToolButton::InstantPopup);
    m_btnParamTool->setFocusPolicy(Qt::NoFocus);
    m_btnParamTool->setCheckable(true);
    m_btnParamTool->setChecked(true);
    m_btnParamTool->setFont(font);

    m_btnLoginTool = CGlobal::instance()->CreateToolButton(this, ":/images/Login.png", "登录");
    connect(m_btnLoginTool,SIGNAL(clicked()),this,SLOT(slotLogin()));
    m_btnLoginTool->setPopupMode(QToolButton::InstantPopup);
    m_btnLoginTool->setFocusPolicy(Qt::NoFocus);
    m_btnLoginTool->setCheckable(true);
    m_btnLoginTool->setChecked(true);
    m_btnLoginTool->setFont(font);

    m_btnLogoutTool = CGlobal::instance()->CreateToolButton(this, ":/images/Logout.png", "注销");
    connect(m_btnLogoutTool,SIGNAL(clicked()),this,SLOT(slotLogout()));
    m_btnLogoutTool->setPopupMode(QToolButton::InstantPopup);
    m_btnLogoutTool->setFocusPolicy(Qt::NoFocus);
    m_btnLogoutTool->setCheckable(true);
    m_btnLogoutTool->setChecked(true);
    m_btnLogoutTool->setFont(font);

    QSpacerItem *topSpacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    m_topToolBox = CGlobal::instance()->CreateHBoxLayout();
    m_topToolBox->setContentsMargins(0, 0, 0, 0);    
    m_topToolBox->addWidget(m_btnMuteTool);
    m_topToolBox->addWidget(m_btnResetTool);
    m_topToolBox->addWidget(m_btnEmergencyTool);
    m_topToolBox->addWidget(m_btnLoginInfoTool);
    m_topToolBox->addWidget(m_btnPlanTool);
    m_topToolBox->addWidget(m_btnCanInfoTool);
    m_topToolBox->addWidget(m_btnBrowseInfoTool);
    m_topToolBox->addWidget(m_btnTableShowTool);
    m_topToolBox->addWidget(m_btnManageTool);
    m_topToolBox->addWidget(m_btnSystemTool);
    m_topToolBox->addWidget(m_btnParamTool);
    m_topToolBox->addWidget(m_btnLoginTool);
    m_topToolBox->addWidget(m_btnLogoutTool);

    m_topToolBox->addWidget(m_checkTime);
    m_topToolBox->addItem(topSpacerItem);
    setLayout(m_topToolBox);
}

QMenu* CTopToolBar::createCheckMenu()
{
    qDebug() << "CTopToolBar::createCheckMenu" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QMenu *checkMenu = new QMenu(this);
    checkMenu ->setStyleSheet(m_strSheet);
    checkMenu ->setStyle(m_style);
    checkMenu ->addAction(QIcon(":/images/monthcheck.png"), "系统月检", this, SLOT(slotMonthTest()));
    checkMenu ->addAction(QIcon(":/images/yearcheck.png"), "系统年检", this, SLOT(slotYearTest()));
    return checkMenu;
}

QMenu* CTopToolBar::createEmergencyMenu()
{
    qDebug() << "CTopToolBar::createEmergencyMenu" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QMenu *emergencyMenu = new QMenu(this);
    emergencyMenu ->setStyleSheet(m_strSheet);
    emergencyMenu ->setStyle(m_style);
    emergencyMenu->addAction(QIcon(":/images/start.png"), "应急启动", this, SLOT(slotStart()));
    emergencyMenu->addAction(QIcon(":/images/stop.png"), "应急停止", this, SLOT(slotStop()));
    return emergencyMenu;
}

QMenu* CTopToolBar::createBrowseMenu()
{
    qDebug() << "CTopToolBar::createBrowseMenu" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QMenu *browseMenu = new QMenu(this);
    browseMenu->setStyleSheet(m_strSheet);
    browseMenu->setStyle(m_style);
    browseMenu->addAction(QIcon(":/images/One.png"), "当日记录", this, SLOT(slotDayInformation()));
    browseMenu->addAction(QIcon(":/images/Seven.png"), "七天内记录", this, SLOT(slotWeekInformation()));
    browseMenu->addAction(QIcon(":/images/Query.png"), "自定义查询", this, SLOT(slotLogging()));
//    browseMenu->addAction(QIcon(":/images/search.png"), tr("SearchDevice"), this, SLOT(slotSearchDevice()));
    return browseMenu;
}

/*obsolete*/
QMenu* CTopToolBar::createRegMenu()
{
    qDebug() << "CTopToolBar::createRegMenu" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QMenu *regMenu = new QMenu(this);
    regMenu->setStyleSheet(m_strSheet);
    regMenu->setStyle(m_style);
    regMenu->addAction(QIcon(":/images/allregedit.png"), "全部注册", this, SLOT(slotLoginInfo()));
    regMenu->addAction(QIcon(":/images/loopregedit.png"), "单机注册", this, SLOT(slotLoginLoop()));
    return regMenu;
}

QMenu* CTopToolBar::createListMenu()
{
    qDebug() << "CTopToolBar::createListMenu" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QMenu *listMenu = new QMenu(this);
    listMenu->setStyleSheet(m_strSheet);
    listMenu->setStyle(m_style);
    listMenu->addAction(QIcon(":/images/List.png"), "列表视图", this, SLOT(slotListView()));
    listMenu->addAction(QIcon(":/images/Layout.png"), "布局视图", this, SLOT(slotLayoutView()));
    return listMenu;
}

QMenu* CTopToolBar::createManageMenu()
{
    qDebug() << "CTopToolBar::createManageMenu" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QMenu *manageMenu = new QMenu(this);
    manageMenu->setStyleSheet(m_strSheet);
    manageMenu->setStyle(m_style);
    manageMenu->addAction(QIcon(":/images/selfcheck.png"), "系统自检", this, SLOT(slotSelfTest()));
    manageMenu->addAction(QIcon(":/images/monthcheck.png"), "系统月检", this, SLOT(slotMonthTest()));
    manageMenu->addAction(QIcon(":/images/yearcheck.png"), "系统年检", this, SLOT(slotYearTest()));
    manageMenu->addSeparator();
    QActionGroup *actionGroup1 = new QActionGroup(this);
    QAction *m_Hand = manageMenu->addAction("手动模式");
    connect(m_Hand, SIGNAL(triggered()), this, SLOT(slotManual()));
    QAction *m_Auto = manageMenu->addAction("自动模式");
    connect(m_Auto, SIGNAL(triggered()), this, SLOT(slotAuto()));
    actionGroup1->addAction(m_Hand);
    actionGroup1->addAction(m_Auto);
    m_Hand->setCheckable(true);
    m_Auto->setCheckable(true);
    if(CGlobal::instance()->m_EvacuationMode == CGlobal::Manual)
    {
        m_Hand->setChecked(true);
        m_Hand->setIcon(QIcon(":/images/ok.png"));
    }
    else if(CGlobal::instance()->m_EvacuationMode == CGlobal::Auto)
    {
        m_Auto->setChecked(true);
        m_Auto->setIcon(QIcon(":/images/ok.png"));
    }
    manageMenu->addSeparator();
    return manageMenu;
}

QMenu* CTopToolBar::createSystemMenu()
{
    qDebug() << "CTopToolBar::createSystemMenu" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QMenu *sysMenu = new QMenu(this);
    sysMenu->setStyleSheet(m_strSheet);
    sysMenu->setStyle(m_style);
//    sysMenu->addAction(QIcon(":/images/set_system.png"), "系统设置", this, SLOT(slotSystemSetup()));
//    sysMenu->addSeparator();
    sysMenu->addAction(QIcon(":/images/simulatetime.png"), "模拟加速开始", this, SLOT(slotStartSimulateTime()));
    sysMenu->addAction(QIcon(":/images/Recovery.png"), "模拟加速停止", this, SLOT(slotStopSimulateTime()));
//    sysMenu->addSeparator();
//    sysMenu->addAction(QIcon(":/images/Save.png"), "保存设备和灯具信息", this, SLOT(slotSaveDeviceData()));
//    sysMenu->addAction(QIcon(":/images/Save.png"), "保存运行状态", this, SLOT(slotSaveDeviceStatus()));
//    sysMenu->addAction(QIcon(":/images/Close.png"), "弹出U盘", this, SLOT(slotOutUSB()));
    sysMenu->addSeparator();
    sysMenu->addAction(QIcon(":/images/about.png"), "关于本机", this, SLOT(slotAbout()));
    sysMenu->addAction(QIcon(":/images/UserConfig.png"), "更新日志", this, SLOT(slotUpdateLog()));
    sysMenu->addSeparator();
    if(CGlobal::instance()->m_UserType == CGlobal::SuperAdministrator)
    {
        sysMenu->addAction(QIcon(":/images/set_system.png"), "屏幕校准", this, SLOT(slotScreenCalibration()));
        sysMenu->addSeparator();
    }
    sysMenu->addAction(QIcon(":/images/ShutDown.png"), "重启", this, SLOT(slotReStart()));

    return sysMenu;
}

void CTopToolBar::slotShowCheckMenu()
{
    qDebug() << "CTopToolBar::slotShowCheckMenu" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_menuCheck && !m_menuCheck->isHidden())
    {
        delete m_menuCheck;
        m_menuCheck = NULL;
    }
    else
    {
        if(m_menuCheck) delete m_menuCheck;
        m_menuCheck = createCheckMenu();
        if(m_menuCheck != NULL)//显示在鼠标点击点的上方
        {
            QToolButton *btn = qobject_cast<QToolButton*>(sender());
            btn->setChecked(true);
            QPoint pos;
            pos.setX(btn->pos().x() + 3);
            pos.setY(btn->height());
            pos = m_menuCheck->mapToGlobal(pos);
            m_menuCheck->popup(pos);
        }
    }

}

void CTopToolBar::slotShowEmergencyMenu()
{
    qDebug() << "CTopToolBar::slotShowEmergencyMenu" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_menuEmergency && !m_menuEmergency->isHidden())
    {
        delete m_menuEmergency;
        m_menuEmergency = NULL;
    }
    else
    {
        if(m_menuEmergency) delete m_menuEmergency;
        m_menuEmergency = createEmergencyMenu();
        if(m_menuEmergency != NULL)//显示在鼠标点击点的上方
        {
            QToolButton *btn = qobject_cast<QToolButton*>(sender());
            btn->setChecked(true);
            QPoint pos;
            pos.setX(btn->pos().x() + 3);
            pos.setY(btn->height());
            pos = m_menuEmergency->mapToGlobal(pos);
            m_menuEmergency->popup(pos);
        }
    }
}

void CTopToolBar::slotShowRegMenu()
{
    qDebug() << "CTopToolBar::slotShowRegMenu" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_menuRegedit && !m_menuRegedit->isHidden())
    {
        delete m_menuRegedit;
        m_menuRegedit = NULL;
    }
    else
    {
        delete m_menuRegedit;
        m_menuRegedit = createRegMenu();
        if(m_menuRegedit != NULL)//显示在鼠标点击点的上方
        {
            QToolButton *btn = qobject_cast<QToolButton*>(sender());
            btn->setChecked(true);
            QPoint pos;
            pos.setX(btn->pos().x() + 3);
            pos.setY(btn->height());
            pos = m_menuRegedit->mapToGlobal(pos);
            m_menuRegedit->popup(pos);
        }
    }
}

void CTopToolBar::slotShowListMenu()
{
    qDebug() << "CTopToolBar::slotShowListMenu" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_menuList && !m_menuList->isHidden())
    {
        delete m_menuList;
        m_menuList = NULL;
    }
    else
    {
        if(!m_menuList)delete m_menuList;
        m_menuList = createListMenu();
        if(m_menuList != NULL)//显示在鼠标点击点的上方
        {
            QToolButton *btn = qobject_cast<QToolButton*>(sender());
            btn->setChecked(true);
            QPoint pos;
            pos.setX(btn->pos().x() + 3);
            pos.setY(btn->height());
            pos = m_menuList->mapToGlobal(pos);
            m_menuList->popup(pos);
        }
    }
}


void CTopToolBar::slotShowBrowseMenu()
{
    qDebug() << "CTopToolBar::slotShowBrowseMenu" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_menuBrowse && !m_menuBrowse->isHidden())
    {
        delete m_menuBrowse;
        m_menuBrowse = NULL;
    }
    else
    {
        if(!m_menuBrowse)delete m_menuBrowse;
        m_menuBrowse = createBrowseMenu();
        if(m_menuBrowse != NULL)//显示在鼠标点击点的上方
        {
            QToolButton *btn = qobject_cast<QToolButton*>(sender());
            btn->setChecked(true);
            QPoint pos;
            pos.setX(btn->pos().x() + 3);
            pos.setY(btn->height());
            pos = m_menuBrowse->mapToGlobal(pos);
            m_menuBrowse->popup(pos);
        }
    }
}

void CTopToolBar::slotShowManageMenu()
{
    qDebug() << "CTopToolBar::slotShowManageMenu" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_menuManage && !m_menuManage->isHidden())
    {
        delete m_menuManage;
        m_menuManage = NULL;
    }
    else
    {
        delete m_menuManage;
        m_menuManage = createManageMenu();
        if(m_menuManage != NULL)//显示在鼠标点击点的上方
        {
            QToolButton *btn = qobject_cast<QToolButton*>(sender());
            btn->setChecked(true);
            QPoint pos;
            pos.setX(btn->pos().x() + 3);
            pos.setY(btn->height());
            pos = m_menuManage->mapToGlobal(pos);
            m_menuManage->popup(pos);
        }
    }
}

void CTopToolBar::slotShowSystemMenu()
{
    qDebug() << "CTopToolBar::slotShowSystemMenu" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_menuSystem && !m_menuSystem->isHidden())
    {
        delete m_menuSystem;
        m_menuSystem = NULL;
    }
    else
    {
        delete m_menuSystem;
        m_menuSystem = createSystemMenu();
        if(m_menuSystem != NULL)//显示在鼠标点击点的上方
        {
            QToolButton *btn = qobject_cast<QToolButton*>(sender());
            btn->setChecked(true);
            QPoint pos;
            pos.setX(btn->pos().x() + 3);
            pos.setY(btn->height());
            pos = m_menuSystem->mapToGlobal(pos);
            m_menuSystem->popup(pos);
        }
    }
}

void CTopToolBar::slotMuteSystem()
{
    qDebug() << "CTopToolBar::slotMuteSystem" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgNull m_MsgNUll;
    m_btnMuteTool->setChecked(true);
    if(CGlobal::instance()->m_bFault || CGlobal::instance()->m_bStart)//当有状态信息时消音才有效
    {
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_Mute, &m_MsgNUll);
        CGlobal::instance()->DataProcessing()->setSoundIcon(false);
    }
}

void CTopToolBar::slotResetSystem()
{
    qDebug() << "CTopToolBar::slotResetSystem" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgNull m_MsgNUll;

    dlgAdministrator m_Administrator;
    m_Administrator.setWindowTitle("用户验证");
    m_Administrator.setUserDisabled(true);
    if(QDialog::Accepted == m_Administrator.exec()) {
        m_btnResetTool->setChecked(true);
    } else {
        m_btnResetTool->setChecked(true);
        return;
    }
    CGlobal::instance()->m_isEmergency = false;
    CGlobal::instance()->m_isMonthEmergency = false;
    CGlobal::instance()->m_isYearEmergency = false;
    CGlobal::instance()->m_bInLogin = false;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_Reset, &m_MsgNUll);


//    CGlobal::instance()->ClientBusiness()->clearStatusObject(true);
//    CGlobal::instance()->ClientBusiness()->clearExpectObject();
//    if(CGlobal::instance()->m_tcptype & 0X01)
//    {
//        //复位声明上传到云平台
//        CGlobal::instance()->ClientBusiness()->XmlResetDeclare();
//    }
//    else if(CGlobal::instance()->m_tcptype & 0X02)
//    {
//        //复位声明上传到客户端
//        CGlobal::instance()->ClientBusiness()->serverResetDeclareUpload();
//    }
    //灯具状态复位
    CGlobal::instance()->ClientBusiness()->DeviceResetStatus();
    //报警的火警点状态复位(必须在清空故障列表前)
    CGlobal::instance()->ClientBusiness()->FirepointResetStatus();
    //清空故障列表
    CGlobal::instance()->DataProcessing()->resetStatus();
    //集中电源状态复位(包含回路)
    CGlobal::instance()->programDistributionView()->distributionResetStatus();


//    //所有设备状态复位
//    resetAllDevice();
    //列表视图界面灯具状态更新
    CGlobal::instance()->DlgDevices()->getDeviceInfo();
    CGlobal::instance()->DlgDevices()->lampBatStatusUpload();
    //布局视图界面灯具状态更新
    CGlobal::instance()->designSplashView()->setPageStateUpdate(true);

    //复位后延迟1s对比检查数据库数据是否在线
    QTimer::singleShot(10000, this, SLOT(slotCheckLoginInfo()));
}

void CTopToolBar::resetSystem()
{
    qDebug() << "CTopToolBar::resetSystem" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgNull m_MsgNUll;

    CGlobal::instance()->m_isEmergency = false;
    CGlobal::instance()->m_isMonthEmergency = false;
    CGlobal::instance()->m_isYearEmergency = false;
    CGlobal::instance()->m_bInLogin = false;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_Reset, &m_MsgNUll);

//    CGlobal::instance()->ClientBusiness()->clearStatusObject(true);
//    CGlobal::instance()->ClientBusiness()->clearExpectObject();
//    if(CGlobal::instance()->m_tcptype & 0X01)
//    {
//        //复位声明上传到云平台
//        CGlobal::instance()->ClientBusiness()->XmlResetDeclare();
//    }
//    else if(CGlobal::instance()->m_tcptype & 0X02)
//    {
//        //复位声明上传到客户端
//        CGlobal::instance()->ClientBusiness()->serverResetDeclareUpload();
//    }

    //清空故障列表
    CGlobal::instance()->DataProcessing()->resetStatus();
    //集中电源状态复位(包含回路)
    CGlobal::instance()->programDistributionView()->distributionResetStatus();
    //灯具状态复位
    CGlobal::instance()->ClientBusiness()->DeviceResetStatus();
    //报警的火警点状态复位
    CGlobal::instance()->ClientBusiness()->FirepointResetStatus();

    //所有设备状态复位
//    resetAllDevice();
    //列表视图界面灯具状态更新
    CGlobal::instance()->DlgDevices()->getDeviceInfo();
    CGlobal::instance()->DlgDevices()->lampBatStatusUpload();
    //布局视图界面灯具状态更新
    CGlobal::instance()->designSplashView()->setPageStateUpdate(true);

    //复位后延迟1s对比检查数据库数据是否在线
    QTimer::singleShot(1000, this, SLOT(slotCheckLoginInfo()));
}

void CTopToolBar::resetAllDevice()
{
    qDebug() << "CTopToolBar::resetAllDevice" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    resetThread *thread = new resetThread(this);
    connect(thread, SIGNAL(threadFinished()), this, SLOT(slot_threadFinished()), Qt::QueuedConnection);
    connect(thread, SIGNAL(updateStatus(CObject*, short, unsigned char)), this, SLOT(slot_updateStatus(CObject*, short, unsigned char)));
    thread->start();
}

void resetThread::run()
{
    qDebug() << "resetThread::run" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->ClientBusiness()->clearExpectObject(SRT_ALL);
    CGlobal::instance()->controller()->clearStatus();
    emit updateStatus(CGlobal::instance()->controller(),OBJS_Online,1);
    QList<CFirePoint*> firepoints = CGlobal::instance()->controller()->firepoints();
    for(int i=0; i<firepoints.count(); i++)
    {
        CFirePoint* firepoint = firepoints.at(i);
        if(firepoint)
        {
            if(firepoint->isSimulateEmergency())
            {
                firepoint->setSimulateEmergency(false);
                emit updateStatus(firepoint,OBJS_SimulateFireEmergency,0);
            }
            if(firepoint->isEmergency())
            {
                firepoint->setEmergency(false);
                emit updateStatus(firepoint,OBJS_FireEmergency,0);
            }
        }
    }
    for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
        if(!canport)
            return;
        QList<CDistribution*> distributions = canport->distributions();
        for(int i=0; i<distributions.count(); i++)
        {
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
                distribution->clearStatus();
                if(!distribution->isDistributionOnline())
                {
                    distribution->setDistributionOnline(true);
//                    distribution->setStatus(OBJS_Online, 1);
                    emit updateStatus(distribution,OBJS_Online,1);
                }
                QList<CLoop*> loops = distribution->loops();
                for(int i=0; i<loops.count(); i++){
                    CLoop* loop = loops.at(i);
                    if(!loop)
                        continue;
                    if(!loop->isLoopOnline())
                    {
                        loop->setLoopOnline(true);
                        loop->setDeviceOnlineTotal(loop->deviceCount());
                    }
                    if(!loop->isLoopLogin())
                    {
                        loop->setLoopLogin(true);
                    }
                    loop->clearStatus();
                    QList<CDevice*> devices = loop->devices();
                    for(int i=0; i<devices.count(); i++){
                        CDevice* device = devices.at(i);
                        if(device)
                        {
                            device->clearStatus();
                            device->setDeviceControl(false);
                            //数据库中为启用状态或者为已注册过灯具
                            if(device->isDatabaseLogin())
                            {
                               device->setDeviceLogin(true);
                            }
                            if(!device->isDeviceOnline())
                            {
                                device->setEnableNumberChange(true);
                                device->setDeviceOnline(true);
                            }
                            if(device->deviceValue(DEVICE_VALUE_EMERGENCY) != "正常")
                            {
                                device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"正常");
                            }
                            if(device->deviceValue(DEVICE_VALUE_TEST) != "正常")
                            {
                                device->setDeviceValue(DEVICE_VALUE_TEST, "正常");
                            }
                            if(device->deviceValue(DEVICE_VALUE_LEFTCLOSE) == 1)
                            {
                                device->setDeviceValue(DEVICE_VALUE_LEFTCLOSE,0);
                            }
                            if(device->deviceValue(DEVICE_VALUE_RIGHTCLOSE) == 1)
                            {
                                device->setDeviceValue(DEVICE_VALUE_RIGHTCLOSE,0);
                            }
                        }
                    }
                    devices.clear();
                }
                loops.clear();
            }
        }
        distributions.clear();
    }
    emit threadFinished();
}

void CTopToolBar::slot_threadFinished()
{
    qDebug() << "CTopToolBar::slot_threadFinished" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 获取发送信号的线程对象
    QThread *finishedThread = qobject_cast<QThread*>(sender());
    if (finishedThread) {
        // 等待线程结束，确保线程执行已完成
        finishedThread->wait();
        // 删除线程对象
        finishedThread->deleteLater(); // 推荐使用 deleteLater()，安全删除线程对象
    }
}

void CTopToolBar::slot_updateStatus(CObject* object, short status, unsigned char value)
{
    qDebug() << "CTopToolBar::slot_updateStatus" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    object->setStatus(status, value);
}

void CTopToolBar::slotCheckLoginInfo()
{
    qDebug() << "CTopToolBar::slotCheckLoginInfo" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");

    ConfigFileManager cfg;
    cfg.ParseConfigFile();//读取注册文件
    CGlobal::instance()->DataProcessing()->slotcheckLoginInfo();
}

void CTopToolBar::slotStart()
{
    qDebug() << "CTopToolBar::slotStart" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    dlgAdministrator m_Administrator;
    m_Administrator.setWindowTitle("用户验证");
    m_Administrator.setUserDisabled(true);
    if(QDialog::Accepted == m_Administrator.exec())
    {
        CGlobal::instance()->m_isEmergency = true;
        CMsgNull m_MsgNUll;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_CancelMute, &m_MsgNUll);
        CGlobal::instance()->DataProcessing()->setSoundIcon(true);
        if(CGlobal::instance()->m_EmergencyOperation == CGlobal::Start)
            return;
        CGlobal::instance()->m_EmergencyOperation = CGlobal::Start;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_EmergencyStart, &m_MsgNUll);

        CGlobal::instance()->ClientBusiness()->Emergency();//下发方向控制和闪烁
//        if(CGlobal::instance()->m_tcptype & 0X01)
//        {
//            //上传手动应急到云平台
//            CGlobal::instance()->ClientBusiness()->XmlManualLaunchUpload();
//        }
//        else if(CGlobal::instance()->m_tcptype & 0X02)
//        {
//            //服务端上传手动应急
//            CGlobal::instance()->ClientBusiness()->serverManualLaunchUpload();
//        }
    }
    m_btnManageTool->setChecked(true);
}

void CTopToolBar::slotStop()
{
    qDebug() << "CTopToolBar::slotStop" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgNull m_MsgNUll;
    dlgAdministrator m_Administrator;
    m_Administrator.setWindowTitle("用户验证");
    m_Administrator.setUserDisabled(true);
    if(QDialog::Accepted == m_Administrator.exec()) {
        if(CGlobal::instance()->ClientBusiness()->exeCommand(NCT_EmergencyStop, &m_MsgNUll))
        {
            CGlobal::instance()->m_EmergencyOperation = CGlobal::Stop;
//            emergencyStatusClear();
            CGlobal::instance()->m_isEmergency = false;
            CGlobal::instance()->DataProcessing()->resetStatus("启动");
            //布局视图界面灯具状态更新
            CGlobal::instance()->designSplashView()->setPageStateUpdate(true);
        }
        else
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
            log = "系统应急停止失败！";
            messageBox.setText(log);
            messageBox.exec();
            CGlobal::instance()->m_HaveDialog = false;
        }
    } else {

    }
    m_btnManageTool->setChecked(true);
}

void CTopToolBar::slotHandFire()
{
    qDebug() << "CTopToolBar::slotHandFire" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    dlgHandFireView m_handfire;
    if(QDialog::Accepted == m_handfire.exec()) {

    }
    m_btnPlanTool->setChecked(true);
}

void CTopToolBar::slotDeviceInfo()
{
    qDebug() << "CTopToolBar::slotDeviceInfo" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 替换界面
    changeCenterFrameWidget(CGlobal::instance()->loginInformation());
    m_btnLoginInfoTool->setChecked(true);
}

void CTopToolBar::slotCanInfo()
{
    qDebug() << "CTopToolBar::slotCanInfo" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->programCanDeviceView()->updateDistribution();
    // 替换界面
    changeCenterFrameWidget(CGlobal::instance()->programCanDeviceView());
    m_btnCanInfoTool->setChecked(true);
}

void CTopToolBar::slotDayInformation()
{
    qDebug() << "CTopToolBar::slotDayInformation" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->SystemRecord()->setPage(1);
    CGlobal::instance()->SystemRecord()->dayrecord();
    // 替换界面
    changeCenterFrameWidget(CGlobal::instance()->SystemRecord());
    m_btnBrowseInfoTool->setChecked(true);
}

void CTopToolBar::slotWeekInformation()
{
    qDebug() << "CTopToolBar::slotWeekInformation" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->SystemRecord()->setPage(1);
    CGlobal::instance()->SystemRecord()->weekrecord();
    // 替换界面
    changeCenterFrameWidget(CGlobal::instance()->SystemRecord());
    m_btnBrowseInfoTool->setChecked(true);
}

void CTopToolBar::slotLogging()
{
    qDebug() << "CTopToolBar::slotLogging" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->SystemRecord()->setPage(1);
    // 替换界面
    changeCenterFrameWidget(CGlobal::instance()->SystemRecord());
    CGlobal::instance()->SystemRecord()->customQueryrecord();
    m_btnBrowseInfoTool->setChecked(true);
}

void CTopToolBar::slotSearchDevice()
{
    qDebug() << "CTopToolBar::slotSearchDevice" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!CGlobal::instance()->DlgSearchDevice())
        new dlgSearchDevice(this);
    CGlobal::instance()->DlgSearchDevice()->show();
    m_btnBrowseInfoTool->setChecked(true);
}

void CTopToolBar::slotListView()
{
    qDebug() << "CTopToolBar::slotListView" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 替换界面
    changeCenterFrameWidget(CGlobal::instance()->dlgDeviceInfomation());
    CGlobal::instance()->DlgDevices()->getDeviceInfo();
    m_btnTableShowTool->setChecked(true);
}

void CTopToolBar::slotLayoutView()
{
    qDebug() << "CTopToolBar::slotLayoutView" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 替换界面
    changeCenterFrameWidget(CGlobal::instance()->CenterFrame());
    m_btnTableShowTool->setChecked(true);
}

void CTopToolBar::slotSelfTest()
{
    qDebug() << "CTopToolBar::slotSelfTest" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgNull m_MsgNUll;
    m_btnManageTool->setChecked(true);
    CGlobal::instance()->m_widget->hide();
    if(!CGlobal::instance()->ClientBusiness()->exeCommand(NCT_SystemSelfCheck, &m_MsgNUll))
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
        log = "系统自检失败！";
        messageBox.setText(log);
        messageBox.exec();
        CGlobal::instance()->m_HaveDialog = false;
    }  
}

void CTopToolBar::slotMonthTest()
{
    qDebug() << "CTopToolBar::slotMonthlyTest" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgNull m_MsgNUll;
    dlgAdministrator m_Administrator;
    m_Administrator.setWindowTitle("用户验证");
    m_Administrator.setUserDisabled(true);
    if(QDialog::Accepted == m_Administrator.exec()) {
        if(!CGlobal::instance()->ClientBusiness()->exeCommand(NCT_SystemMonthCheck, &m_MsgNUll))
        {
//            m_MonthCheckTime = QDateTime::currentDateTime().addSecs(CGlobal::instance()->m_MonthCheckSpanSecond);
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
            log = "系统月检失败！";
            messageBox.setText(log);
            messageBox.exec();
            CGlobal::instance()->m_HaveDialog = false;
        }
    } else {

    }
    m_btnManageTool->setChecked(true);

}

void CTopToolBar::slotYearTest()
{
    qDebug() << "CTopToolBar::slotYearTest" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgNull m_MsgNUll;
    dlgAdministrator m_Administrator;
    m_Administrator.setWindowTitle("用户验证");
    m_Administrator.setUserDisabled(true);
    if(QDialog::Accepted == m_Administrator.exec()) {
        if(!CGlobal::instance()->ClientBusiness()->exeCommand(NCT_SystemYearCheck, &m_MsgNUll))
        {
//            m_YearCheckTime = QDateTime::currentDateTime().addSecs(CGlobal::instance()->m_YearCheckSpanSecond * 60);
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
            log = "系统年检失败！";
            messageBox.setText(log);
            messageBox.exec();
            CGlobal::instance()->m_HaveDialog = false;
        }
    } else {

    }
    m_btnManageTool->setChecked(true);

}


void CTopToolBar::slotManual()
{
    qDebug() << "CTopToolBar::slotManual" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgNull m_MsgNUll;
    if(CGlobal::instance()->m_HaveDialog)
        return;
    dlgAdministrator m_Administrator;
    m_Administrator.setWindowTitle("用户验证");
    m_Administrator.setUserDisabled(true);
    if(QDialog::Accepted == m_Administrator.exec()) {
        if(!CGlobal::instance()->ClientBusiness()->exeCommand(NCT_ManualEvacuation, &m_MsgNUll))
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
            log = "手动疏散设置失败！";
            messageBox.setText(log);
            messageBox.exec();
            CGlobal::instance()->m_HaveDialog = false;
        }
        CGlobal::instance()->BottomToolBar()->setManual();
    } else {

    }
    m_btnManageTool->setChecked(true);
}

void CTopToolBar::slotAuto()
{
    qDebug() << "CTopToolBar::slotAuto" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgNull m_MsgNUll;
    if(CGlobal::instance()->m_HaveDialog)
        return;
    dlgAdministrator m_Administrator;
    m_Administrator.setWindowTitle("用户验证");
    m_Administrator.setUserDisabled(true);
    if(QDialog::Accepted == m_Administrator.exec()) {
        if(!CGlobal::instance()->ClientBusiness()->exeCommand(NCT_AutomaticEvacuation, &m_MsgNUll))
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
            log = "自动疏散设置失败！";
            messageBox.setText(log);
            messageBox.exec();
            CGlobal::instance()->m_HaveDialog = false;
        }
        CGlobal::instance()->BottomToolBar()->setAuto();
    } else {

    }
    m_btnManageTool->setChecked(true);   
}


void CTopToolBar::slotSystemSetup()
{
    qDebug() << "CTopToolBar::slotSystemSetup" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->TimeInterval()->updateWLANData();
    CGlobal::instance()->TimeInterval()->updateSqliteData();
    // 替换界面
    changeCenterFrameWidget(CGlobal::instance()->TimeInterval());
    m_btnParamTool->setChecked(true);
}



void CTopToolBar::slotStartSimulateTime()
{
    qDebug() << "CTopToolBar::slotStartSimulateTime" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgNull m_MsgNUll;
    dlgAdministrator m_Administrator;
    m_Administrator.setWindowTitle("用户验证");
    m_Administrator.setUserDisabled(true);
    if(QDialog::Accepted == m_Administrator.exec()) {
        m_speedup = true;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_StartSpeedUp, &m_MsgNUll);
        QPalette palette;
        palette.setColor(QPalette::WindowText, Qt::red);
        CGlobal::instance()->TopToolBar()->m_checkTime->setPalette(palette);
    } else {
    }
    m_btnSystemTool->setChecked(true);
}

void CTopToolBar::slotStopSimulateTime()
{
    qDebug() << "CTopToolBar::slotStopSimulateTime" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgNull m_MsgNUll;
    m_speedup = false;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_StopSpeedUp, &m_MsgNUll);
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::blue);
    CGlobal::instance()->TopToolBar()->m_checkTime->setPalette(palette);
    m_btnSystemTool->setChecked(true);
}

void CTopToolBar::slotSaveDeviceData()
{
    qDebug() << "CTopToolBar::slotSaveDeviceData" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString  data = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "\n";
    QString fileName = "/mnt/usb/DeviceData.txt";
    QFile file(fileName);
    // 清空文件内容
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.close();
    }

    for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
        if(!canport)
            continue;
        data = data + "<CAN回路  名称:" + canport->codeStr() + ">\n";
        QList<CDistribution*> distributions = canport->distributions();
        for(int i=0; i<distributions.count(); i++){
            CDistribution* distribution = distributions.at(i);
            if(distribution){
                data = data + "    <CAN设备  地址:" + QString::number(distribution->distributionAddress()) +
                        "  设备类型:" + QString::number(distribution->distributionTypeId()) +
                        "  产品编号:" + distribution->distributionProductId() +
                        "  名称:" + distribution->typeStr() +">\n";
                QList<CLoop*> loops = distribution->loops();
                for(int i=0; i<loops.count(); i++){
                    CLoop* loop = loops.at(i);
                    if(loop){
                        data = data + "        <回路  名称:" + loop->typeStr() +
                                "  地址:" + QString::number(loop->loopAdd()) + ">\n";
                        QList<CDevice*> devices = loop->devices();
                        for(int m=0; m<devices.count(); m++){
                            CDevice* device = devices.at(m);
                            if(device)
                            {
                                data = data + "            <灯具  名称:" + device->typeStr() +
                                        "  地址:" + QString::number(device->deviceAdd()) +
                                        "  设备类型:" + QString::number(device->deviceTypeId()) +
                                        "  产品编号:" + device->deviceProductionId() + ">\n";
                            }
                        }
                        devices.clear();
                    }
                }
                loops.clear();
            }
        }
        distributions.clear();
    }
    CGlobal::instance()->m_HaveDialog = true;
    // 创建一个QFont对象并设置字体属性
    QFont font;
    font.setPointSize(12); // 设置字体大小为12像素

    // 在QMessageBox中设置字体
    QMessageBox messageBox;
    messageBox.setFont(font);
    QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
    messageBox.setStyleSheet(styleSheet);
    messageBox.setWindowTitle("信息");
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << data << '\n';
        file.close();
        messageBox.setText("保存成功！\n拔出U盘前请点击弹出U盘，否则保存失败！！！");
        messageBox.exec();
        CGlobal::instance()->m_HaveDialog = false;
    }
    else
    {
        messageBox.setText("保存失败!");
        messageBox.exec();
        CGlobal::instance()->m_HaveDialog = false;
    }
}

void CTopToolBar::slotSaveDeviceStatus()
{
    qDebug() << "CTopToolBar::slotSaveDeviceStatus" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString  data = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "\n";
    QString fileName = "/mnt/usb/DeviceStatus.txt";
    QFile file(fileName);

    // 清空文件内容
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.close();
    }
    QString communicationStatus;
    for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
        if(!canport)
            continue;
        QList<CDistribution*> distributions = canport->distributions();
        for(int i=0; i<distributions.count(); i++){
            CDistribution* distribution = distributions.at(i);
            if(distribution){
                QList<CLoop*> loops = distribution->loops();
                for(int i=0; i<loops.count(); i++){
                    CLoop* loop = loops.at(i);
                    if(loop){
                        QList<CDevice*> devices = loop->devices();
                        for(int m=0; m<devices.count(); m++){
                            CDevice* device = devices.at(m);
                            if(device)
                            {
                                if(device->getDeviceCommunicationFault() == false)
                                {
                                    communicationStatus = "正常";
                                }
                                else
                                    communicationStatus = "通讯故障";
                                data = data + "灯具  " + canport->codeStr() +
                                        "-" + QString::number(distribution->distributionAddress()) +
                                        "  回路:" + QString::number(loop->loopAdd()) +
                                        "  地址:" + QString::number(device->deviceAdd()) +
                                        "  设备类型:" + QString::number(device->deviceTypeId()) +
                                        "  产品编号:" + device->deviceProductionId() +
                                        "  通讯状态:" +  communicationStatus +
                                        "  光源状态:" + device->deviceValue(DEVICE_VALUE_LIGHT).toString() +
                                        "  应急状态:" + device->deviceValue(DEVICE_VALUE_EMERGENCY).toString() +
                                        "  名称:" + device->typeStr() + "\n";
                            }
                        }
                        devices.clear();
                    }
                }
                loops.clear();
            }
        }
        distributions.clear();
    }
    CGlobal::instance()->m_HaveDialog = true;
    // 创建一个QFont对象并设置字体属性
    QFont font;
    font.setPointSize(12); // 设置字体大小为12像素

    // 在QMessageBox中设置字体
    QMessageBox messageBox;
    messageBox.setFont(font);
    QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
    messageBox.setStyleSheet(styleSheet);
    messageBox.setWindowTitle("信息");
    messageBox.setIcon(QMessageBox::NoIcon);
    messageBox.setStandardButtons(QMessageBox::Ok);
    messageBox.button(QMessageBox::Ok)->setText("确定");
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << data << '\n';
        file.close();
        messageBox.setText("保存成功！\n拔出U盘前请点击弹出U盘，否则保存失败！！！");
        messageBox.exec();
        CGlobal::instance()->m_HaveDialog = false;
    }
    else
    {
        messageBox.setText("保存失败!");
        messageBox.exec();
        CGlobal::instance()->m_HaveDialog = false;
    }
}

void CTopToolBar::slotOutUSB()
{
    qDebug() << "CTopToolBar::slotOutUSB" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString program_unmountUSB = "/home/xfss/root/USBprogramUpdate/unmontUSB.sh";  // 脚本的路径
    m_process_unmountUSB->start(program_unmountUSB);
    CGlobal::instance()->m_HaveDialog = true;
    // 创建一个QFont对象并设置字体属性
    QFont font;
    font.setPointSize(12); // 设置字体大小为12像素

    // 在QMessageBox中设置字体
    QMessageBox messageBox;
    messageBox.setFont(font);
    QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
    messageBox.setStyleSheet(styleSheet);
    messageBox.setWindowTitle("信息");
    messageBox.setIcon(QMessageBox::NoIcon);
    messageBox.setStandardButtons(QMessageBox::Ok);
    messageBox.button(QMessageBox::Ok)->setText("确定");
    messageBox.setText("弹出U盘成功！请拔出U盘！");
    messageBox.exec();
    CGlobal::instance()->m_HaveDialog = false;
}

void CTopToolBar::slotAbout()
{
    qDebug() << "CTopToolBar::slotAbout" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_HaveDialog = true;
    // 创建一个QFont对象并设置字体属性
    QFont font;
    font.setPointSize(12); // 设置字体大小为12像素

    // 在QMessageBox中设置字体
    QMessageBox messageBox;
    messageBox.setFont(font);
    QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
    messageBox.setStyleSheet(styleSheet);
    messageBox.setWindowTitle("软件信息");
    messageBox.setIcon(QMessageBox::NoIcon);
    messageBox.setStandardButtons(QMessageBox::Ok);
    messageBox.button(QMessageBox::Ok)->setText("确定");
    messageBox.setText("名称:" + CGlobal::instance()->m_strApplicationTitle +
                   tr("\n") + "版本:" + "TSJY0198  V1.00CA");
    messageBox.exec();
    CGlobal::instance()->m_HaveDialog = false;
    m_btnSystemTool->setChecked(true);
}

void CTopToolBar::slotScreenCalibration()
{
    qDebug() << "CTopToolBar::slotScreenCalibration" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_HaveDialog = true;
    QFont font;
    font.setPointSize(12); // 设置字体大小为12像素
    // 在QMessageBox中设置字体
    QMessageBox messageBox;
    messageBox.setFont(font);
    QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
    messageBox.setStyleSheet(styleSheet);
    messageBox.setIcon(QMessageBox::NoIcon);
    messageBox.setWindowTitle("屏幕校准");
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Cancel);
    // 设置按钮显示文字
    messageBox.button(QMessageBox::Yes)->setText("是");
    messageBox.button(QMessageBox::Cancel)->setText("取消");
    QString log;
    log = "确定要进行屏幕校准吗？";
    messageBox.setText(log);
    int ret = messageBox.exec();
    if(ret == QMessageBox::Yes)
    {
        QProcess *process = new QProcess(this);
        QString program = "/home/xfss/root/SystemSet/screenCalibration.sh";  // 脚本的路径
        process->start(program);
    }
    CGlobal::instance()->m_HaveDialog = false;
}

void CTopToolBar::slotUpdateLog()
{
    qDebug() << "CTopToolBar::slotUpdateLog" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_HaveDialog = true;
    // 创建一个QFont对象并设置字体属性
    QFont font;
    font.setPointSize(12); // 设置字体大小为12像素
    // 在QMessageBox中设置字体
    QMessageBox messageBox;
    messageBox.setFont(font);
    QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
    messageBox.setStyleSheet(styleSheet);
    messageBox.setIcon(QMessageBox::NoIcon);
    messageBox.setWindowTitle("更新日志");
    messageBox.setStandardButtons(QMessageBox::Ok);
    messageBox.button(QMessageBox::Ok)->setText("确定");
    QString updateLog;
    updateLog = "2024.05 V1.00CA" + tr("\n") + "1、新版本发布";
    messageBox.setText(updateLog);
    messageBox.exec();
    CGlobal::instance()->m_HaveDialog = false;
    m_btnSystemTool->setChecked(true);
}

void CTopToolBar::slotReStart()
{
    qDebug() << "CTopToolBar::slotReStart" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_HaveDialog = true;
    QFont font;
    font.setPointSize(12); // 设置字体大小为12像素
    // 在QMessageBox中设置字体
    QMessageBox messageBox;
    messageBox.setFont(font);
    QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
    messageBox.setStyleSheet(styleSheet);
    messageBox.setIcon(QMessageBox::NoIcon);
//    messageBox.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
    messageBox.setWindowTitle("重启");
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Cancel);
    // 设置按钮显示文字
    messageBox.button(QMessageBox::Yes)->setText("是");
    messageBox.button(QMessageBox::Cancel)->setText("取消");
    QString log;
    log = "是否重启程序？";
    messageBox.setText(log);
    int ret = messageBox.exec();
    if(ret == QMessageBox::Yes)
    {
//        qApp->quit();
        slotMuteSystem();
        QProcess::execute(QString("reboot"));

//        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
    CGlobal::instance()->m_HaveDialog = false;
    m_btnSystemTool->setChecked(true);
}

void CTopToolBar::slotLogin()
{
    qDebug() << "CTopToolBar::slotLogin" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    dlgAdministrator m_Administrator;
    m_Administrator.setWindowTitle("用户登录");
    m_Administrator.setUserDisabled(false);
    if(QDialog::Accepted == m_Administrator.exec())
    {
        updateMenu();
        qDebug() << CGlobal::instance()->m_UserType;
    }
    m_btnLoginTool->setChecked(true);
}

void CTopToolBar::slotLogout()
{
    qDebug() << "CTopToolBar::slotLogout" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    dlgAdministrator m_Administrator;
    m_Administrator.setWindowTitle("用户验证");
    m_Administrator.setUserDisabled(true);
    if(QDialog::Accepted == m_Administrator.exec())
    {
        CGlobal::instance()->m_UserType = CGlobal::User;
        CGlobal::instance()->m_loginStatus = false;
        CGlobal::instance()->BottomToolBar()->setUser();
        changeCenterFrameWidget(CGlobal::instance()->CenterFrame());
        updateMenu();
    }
    m_btnLogoutTool->setChecked(true);
}

void CTopToolBar::changeInformation(bool flag)
{
    qDebug() << "CTopToolBar::changeInformation" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if (flag)
    {
        CGlobal::instance()->setMsgText("模拟加速！");
        CGlobal::instance()->setMSgColor(Qt::red);
    }
    else
    {
        CGlobal::instance()->setMsgText("系统运行！");
        CGlobal::instance()->setMSgColor(QColor(12, 180, 3));
    }
}

void CTopToolBar::updateMenu()
{
    qDebug() << "CTopToolBar::updateMenu" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(CGlobal::instance()->m_UserType == CGlobal::User)
    {
        m_btnResetTool->setVisible(false);
        m_btnEmergencyTool->setVisible(false);
        m_btnPlanTool->setVisible(false);
        m_btnCanInfoTool->setVisible(false);
        m_btnLoginInfoTool->setVisible(false);
        m_btnBrowseInfoTool->setVisible(false);
        m_btnTableShowTool->setVisible(false);
        m_btnManageTool->setVisible(false);
        m_btnParamTool->setVisible(false);
        m_btnSystemTool->setVisible(false);
        m_btnLoginTool->setVisible(true);
        m_btnLogoutTool->setVisible(false);
    }
    else if(CGlobal::instance()->m_UserType == CGlobal::OnDuty)
    {
        m_btnResetTool->setVisible(false);
        m_btnEmergencyTool->setVisible(false);
        m_btnPlanTool->setVisible(false);
        m_btnCanInfoTool->setVisible(true);
        m_btnLoginInfoTool->setVisible(false);
        m_btnBrowseInfoTool->setVisible(true);
        m_btnTableShowTool->setVisible(true);
        m_btnManageTool->setVisible(false);
        m_btnParamTool->setVisible(false);
        m_btnSystemTool->setVisible(false);
        m_btnLoginTool->setVisible(false);
        m_btnLogoutTool->setVisible(true);
//        CGlobal::instance()->TimeInterval()->setIpAndTimeHide(true);
    }
    else if(CGlobal::instance()->m_UserType == CGlobal::Operator)
    {
        m_btnResetTool->setVisible(true);
        m_btnEmergencyTool->setVisible(true);
        m_btnPlanTool->setVisible(true);
        m_btnCanInfoTool->setVisible(true);
        m_btnLoginInfoTool->setVisible(false);
        m_btnBrowseInfoTool->setVisible(true);
        m_btnTableShowTool->setVisible(true);
        m_btnManageTool->setVisible(true);
        m_btnParamTool->setVisible(false);
        m_btnSystemTool->setVisible(false);
        m_btnLoginTool->setVisible(false);
        m_btnLogoutTool->setVisible(true);
//        CGlobal::instance()->TimeInterval()->setIpAndTimeHide(true);
    }
    else if(CGlobal::instance()->m_UserType == CGlobal::Administrator)
    {
        m_btnResetTool->setVisible(true);
        m_btnEmergencyTool->setVisible(true);
        m_btnPlanTool->setVisible(true);
        m_btnCanInfoTool->setVisible(true);
        m_btnLoginInfoTool->setVisible(false);
        m_btnBrowseInfoTool->setVisible(true);
        m_btnTableShowTool->setVisible(true);
        m_btnManageTool->setVisible(true);
        m_btnParamTool->setVisible(true);
        m_btnSystemTool->setVisible(true);
        m_btnLoginTool->setVisible(false);
        m_btnLogoutTool->setVisible(true);
//        CGlobal::instance()->TimeInterval()->setIpAndTimeHide(true);
    }
    else if(CGlobal::instance()->m_UserType == CGlobal::SuperAdministrator)
    {
        m_btnResetTool->setVisible(true);
        m_btnEmergencyTool->setVisible(true);
        m_btnPlanTool->setVisible(true);
        m_btnCanInfoTool->setVisible(true);
        m_btnLoginInfoTool->setVisible(true);
        m_btnBrowseInfoTool->setVisible(true);
        m_btnTableShowTool->setVisible(true);
        m_btnManageTool->setVisible(true);
        m_btnParamTool->setVisible(true);
        m_btnSystemTool->setVisible(true);
        m_btnLoginTool->setVisible(false);
        m_btnLogoutTool->setVisible(true);
//        CGlobal::instance()->TimeInterval()->setIpAndTimeHide(false);
    }
}

void CTopToolBar::changeCenterFrameWidget(QWidget* widget)
{
    qDebug() << "CTopToolBar::changeCenterFrameWidget" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->centerFrameContainer()->removeWidget(CGlobal::instance()->centerFrameContainer()->widget(0)); // 移除原来的小部件
    CGlobal::instance()->centerFrameContainer()->insertWidget(0, widget); // 插入新的小部件
    CGlobal::instance()->centerFrameContainer()->setCurrentIndex(0);
}

void CTopToolBar::emergencyStatusClear()
{
    qDebug() << "CTopToolBar::emergencyStatusClear" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
        if(!canport)
            return;
        QList<CDistribution*> distributions = canport->distributions();
        for(int i=0; i<distributions.count(); i++)
        {
            CDistribution* distribution = distributions.at(i);
            if(distribution)
            {
                distribution->setemergencyStatus(false);
                QList<CLoop*> loops = distribution->loops();
                for(int i=0; i<loops.count(); i++){
                    CLoop* loop = loops.at(i);
                    if(!loop)
                        continue;
                    QList<CDevice*> devices = loop->devices();
                    for(int m=0; m<devices.count(); m++){
                        CDevice* device = devices.at(m);
                        if(!device)
                            continue;
                        device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"正常");
                    }
                    devices.clear();
                }
                loops.clear();
            }

        }
        distributions.clear();
    }
//    QList<QVariant> data;
//    CGlobal::instance()->DataProcessing()->slotNotifyInformation(NIT_Reset, data);
//    data.clear();
    CGlobal::instance()->DlgDevices()->getDeviceInfo();
    //布局视图界面灯具状态更新
    CGlobal::instance()->designSplashView()->setPageStateUpdate(true);
}

//void CTopToolBar::slot_yearMonthCheck()
//{
//    static int count = 0;

//    if(CGlobal::instance()->m_isMonthEmergency)
//    {
//        if(QDateTime::currentDateTime() >= m_MonthCheckTime)
//        {
////            stopMonthCheck();
////            uploadStatus(1, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_MonthEmergency, 0, QDateTime::currentDateTime().toTime_t());
//        }
//        else
//        {
//            QString str = QString("%1秒").arg(QDateTime::currentDateTime().secsTo(m_MonthCheckTime), 2, 10, QChar('0')); // 格式化秒数，宽度2，用'0'填充
//            CGlobal::instance()->InformationWindow()->setLiftTime(str);
//        }
//        return;
//    }
//    if(CGlobal::instance()->m_isYearEmergency)
//    {
//        if(QDateTime::currentDateTime() >= m_YearCheckTime)
//        {
////            stopYearCheck();
////            uploadStatus(1, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_YearEmergency, 0, QDateTime::currentDateTime().toTime_t());
//        }
//        else
//        {
//            int min = QDateTime::currentDateTime().secsTo(m_MonthCheckTime)/60;
//            int sec = QDateTime::currentDateTime().secsTo(m_MonthCheckTime)%60;
//            QString str = QString("%1分 %2秒")
//                    .arg(min, 2, 10, QChar('0'))  // 格式化分钟数，宽度2，用'0'填充
//                    .arg(sec, 2, 10, QChar('0')); // 格式化秒数，宽度2，用'0'填充
//            CGlobal::instance()->InformationWindow()->setLiftTime(str);
//        }
//        return;
//    }
//    m_MonthYearCheckTime->systemCurrentTime = QDateTime::currentDateTime();


//    if(!m_speedup)//正常月检年检
//    {
//        // 进入计时状态后的计时天数
//        int days = m_MonthYearCheckTime->systemMainPowerTime.daysTo(m_MonthYearCheckTime->systemCurrentTime);
//        if(days < 30)// 小于月检周期
//        {// 更新界面显示下次月检、年检提示信息
//            m_MonthYearCheckTime->yearCheckTime = m_MonthYearCheckTime->systemMainPowerTime.addDays(365);
//            m_MonthYearCheckTime->monthCheckTime = m_MonthYearCheckTime->systemMainPowerTime.addDays(30);
//        }
//        else
//        {
//            if(days % 365 == 0)// 满足年检条件
//            {
//                QDateTime tmpYearTime = m_MonthYearCheckTime->systemMainPowerTime.addDays(365 * ((int)(days / 365)));
//                if(m_MonthYearCheckTime->yearCheckTime <= m_MonthYearCheckTime->systemCurrentTime)
//                {
//                    if(m_MonthYearCheckTime->lastYearCheckTime.date() < m_MonthYearCheckTime->systemCurrentTime.date())
//                    {
//                        if(!CGlobal::instance()->m_isMonthEmergency)// 当前不存在月检，则进行年检
//                        {
//                            m_MonthYearCheckTime->lastYearCheckTime = m_MonthYearCheckTime->systemCurrentTime;
//                            m_MonthYearCheckTime->yearCheckTime = m_MonthYearCheckTime->systemMainPowerTime.addDays(365 * ((int)(days / 365) + 1));
////                            autoYearCheck();
//                        }
//                    }
//                }
//                else
//                {
//                    if(!CGlobal::instance()->m_isYearEmergency && (tmpYearTime >= m_MonthYearCheckTime->systemCurrentTime)) m_MonthYearCheckTime->yearCheckTime = tmpYearTime;
//                }
//            }
//            else
//            {
//                if(m_MonthYearCheckTime->yearCheckTime <= m_MonthYearCheckTime->systemCurrentTime){//更新下次年检时间
//                    m_MonthYearCheckTime->yearCheckTime = m_MonthYearCheckTime->systemMainPowerTime.addDays(365 * ((int)(days / 365) + 1));
//                }
//            }

//            if(days % 30 == 0)//满足月检条件
//            {
//                QDateTime tmpMonthTime = m_MonthYearCheckTime->systemMainPowerTime.addDays(30 * (int)(days / 30));
//                if(m_MonthYearCheckTime->monthCheckTime <= m_MonthYearCheckTime->systemCurrentTime)
//                {
//                    if(m_MonthYearCheckTime->lastMonthCheckTime < m_MonthYearCheckTime->systemCurrentTime)
//                    {
//                        if(!CGlobal::instance()->m_isYearEmergency)//当前不存在年检，则进行月检
//                        {
//                            m_MonthYearCheckTime->lastMonthCheckTime = m_MonthYearCheckTime->systemCurrentTime;
//                            m_MonthYearCheckTime->monthCheckTime = m_MonthYearCheckTime->systemMainPowerTime.addDays(30 * ((int)(days / 30) + 1));
////                            autoMonthCheck();
//                        }
//                    }
//                }
//                else
//                {
//                    if(!CGlobal::instance()->m_isMonthEmergency && (tmpMonthTime >= m_MonthYearCheckTime->systemCurrentTime)) {
//                        m_MonthYearCheckTime->monthCheckTime = tmpMonthTime;
//                    }
//                }
//            }
//            else
//            {
//                if(m_MonthYearCheckTime->monthCheckTime <= m_MonthYearCheckTime->systemCurrentTime){//更新下次月检时间
//                    m_MonthYearCheckTime->monthCheckTime = m_MonthYearCheckTime->systemMainPowerTime.addDays(30 * ((int)(days / 30) + 1));
//                }
//            }
//        }
//        m_MonthYearCheckTime->speedMonthCheckTime = m_MonthYearCheckTime->monthCheckTime;
//        m_MonthYearCheckTime->speedYearCheckTime = m_MonthYearCheckTime->yearCheckTime;
//        m_CheckTick = 0;
//    }
//    else//模拟加速
//    {
//        if(!CGlobal::instance()->m_isMonthEmergency && !CGlobal::instance()->m_isYearEmergency)
//        {
//            m_CheckTick++;
//            m_MonthYearCheckTime->systemCurrentTime = QDateTime::currentDateTime().addDays(m_CheckTick);
//        }
//        int days = m_MonthYearCheckTime->systemMainPowerTime.daysTo(m_MonthYearCheckTime->systemCurrentTime);
//        if(days % 365 == 0)// 满足年检条件
//        {
//            QDateTime tmpYearTime = m_MonthYearCheckTime->systemMainPowerTime.addDays(365 * ((int)(days / 365)));
//            if(m_MonthYearCheckTime->speedYearCheckTime <= m_MonthYearCheckTime->systemCurrentTime)
//            {
//                if(m_MonthYearCheckTime->speedLastYearCheckTime.date() < m_MonthYearCheckTime->systemCurrentTime.date())
//                {
//                    if(!CGlobal::instance()->m_isMonthEmergency)// 当前不存在月检，则进行年检
//                    {
//                        m_MonthYearCheckTime->speedLastYearCheckTime = m_MonthYearCheckTime->systemCurrentTime;
//                        m_MonthYearCheckTime->speedYearCheckTime = m_MonthYearCheckTime->systemMainPowerTime.addDays(365 * ((int)(days / 365) + 1));
////                        autoYearCheck();
//                    }
//                }
//            }
//            else
//            {
//                if(!CGlobal::instance()->m_isYearEmergency && (tmpYearTime >= m_MonthYearCheckTime->systemCurrentTime)) m_MonthYearCheckTime->speedYearCheckTime = tmpYearTime;
//            }
//        }
//        else
//        {
//            if(m_MonthYearCheckTime->speedYearCheckTime <= m_MonthYearCheckTime->systemCurrentTime){//更新下次年检时间
//                m_MonthYearCheckTime->speedYearCheckTime = m_MonthYearCheckTime->systemMainPowerTime.addDays(365 * ((int)(days / 365) + 1));
//            }
//        }

//        if(days % 30 == 0)//满足月检条件
//        {
//            QDateTime tmpMonthTime = m_MonthYearCheckTime->systemMainPowerTime.addDays(30 * (int)(days / 30));
//            if(m_MonthYearCheckTime->speedMonthCheckTime <= m_MonthYearCheckTime->systemCurrentTime)
//            {
//                if(m_MonthYearCheckTime->speedLastMonthCheckTime < m_MonthYearCheckTime->systemCurrentTime)
//                {
//                    if(!CGlobal::instance()->m_isYearEmergency)//当前不存在年检，则进行月检
//                    {
//                        m_MonthYearCheckTime->speedLastMonthCheckTime = m_MonthYearCheckTime->systemCurrentTime;
//                        m_MonthYearCheckTime->speedMonthCheckTime = m_MonthYearCheckTime->systemMainPowerTime.addDays(30 * ((int)(days / 30) + 1));
////                        autoMonthCheck();
//                    }
//                }
//            }
//            else
//            {
//                if(!CGlobal::instance()->m_isMonthEmergency && (tmpMonthTime >= m_MonthYearCheckTime->systemCurrentTime)) {
//                    m_MonthYearCheckTime->speedMonthCheckTime = tmpMonthTime;
//                }
//            }
//        }

//    }

//    if(++count % 1 == 0)
//    {
//        if(CGlobal::instance()->m_isMonthEmergency || CGlobal::instance()->m_isYearEmergency)
//            return;
////        QByteArray data = m_checkTime->data(NCT_CheckTimeSync);
////        CGlobal::instance()->DealEvent(NCT_CheckTimeSync, data);
//    }
//}

