
 /*fix （2023-02-06）修改月检查按钮 年检查按钮作为检查按钮的弹出菜单 */


#include "ctoptoolbar.h"
#include "ccenterframe.h"
#include "ccenterframe.h"
#include "model/cinformationmodel.h"
#include "business/cdataprocessing.h"
#include "dm/clayer.h"
#include "dm/ccontroller.h"
#include "dm/ccanport.h"
#include "dm/cdistribution.h"
#include "dm/cdevice.h"
#include "dm/cloop.h"
#include "graphics/configfilemanager.h"
#include "widget.h"
#include "cmyiconstyle.h"
#include "cglobal.h"
#include "dialog/dlgadministrator.h"
#include "dialog/dlglogin.h"
#include "dialog/dlgtimeinterval.h"
#include "dialog/dlgsetsystemtime.h"
#include "dialog/dlgadministrator.h"
#include "dialog/dlglogininformation.h"
#include "dialog/dlgsystemrecord.h"
#include "dialog/dlgsearchdevice.h"
#include "dialog/dlgdevices.h"
#include "widget/cprogramdistributionview.h"
#include "widget/cprogramloopview.h"

#include "cdesigntreeview.h"
#include "cdesignsplashview.h"
#include "cbottomtoolbar.h"

#include "widget/cstatustabs.h"

#include <QDebug>
#include <QList>
#include <QVariant>
#include <QMessageBox>
#include <QStackedWidget>
#include <QProcess>
#include <QDesktopWidget>
#include <QApplication>

CTopToolBar::CTopToolBar(QWidget *parent) :
    QWidget(parent)
{
    initMember();
    createToolBar();
    updateMenu();
//    m_time = new QTimer(this);
//    connect(m_time,SIGNAL(timeout()),this,SLOT(slottimeout()));
//    m_time->start(10000);
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
    QPixmap  image = QPixmap::grabWindow(widget.winId(), widget.x(), widget.y(), 800, 520);
    QString str = "/root/" + QString::number(i) + ".png";
    image.save(str);
}

CTopToolBar::~CTopToolBar()
{
    delete m_style;
}

void CTopToolBar::initMember()
{
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
    QFont font;
    font.setPointSize(9.5);
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

    m_btnManageTool = CGlobal::instance()->CreateToolButton(this, ":/images/set.png", "管理");
    connect(m_btnManageTool,SIGNAL(clicked(bool)), this, SLOT(slotShowManageMenu()));
    m_btnManageTool->setPopupMode(QToolButton::InstantPopup);
    m_btnManageTool->setFocusPolicy(Qt::NoFocus);
    m_btnManageTool->setCheckable(true);
    m_btnManageTool->setChecked(true);
    m_btnManageTool->setFont(font);

    /*点击注册按钮显示设备信息*/
    m_btnLoginInfoTool = CGlobal::instance()->CreateToolButton(this, ":/images/regedit.png", "注册");
    connect(m_btnLoginInfoTool,SIGNAL(clicked()),this,SLOT(slotDeviceInfo()));
    m_btnLoginInfoTool->setPopupMode(QToolButton::InstantPopup);
    m_btnLoginInfoTool->setFocusPolicy(Qt::NoFocus);
    m_btnLoginInfoTool->setCheckable(true);
    m_btnLoginInfoTool->setChecked(true);
    m_btnLoginInfoTool->setFont(font);

    m_btnCanInfoTool = CGlobal::instance()->CreateToolButton(this, ":/images/CanDeviceConfig.png", "CAN设备");
    connect(m_btnCanInfoTool,SIGNAL(clicked()),this,SLOT(slotCanInfo()));
    m_btnCanInfoTool->setPopupMode(QToolButton::InstantPopup);
    m_btnCanInfoTool->setFocusPolicy(Qt::NoFocus);
    m_btnCanInfoTool->setCheckable(true);
    m_btnCanInfoTool->setChecked(true);
    m_btnCanInfoTool->setFont(font);


    m_btnSystemTool = CGlobal::instance()->CreateToolButton(this, ":/images/systempath.png", "系统");
    connect(m_btnSystemTool,SIGNAL(clicked()),this,SLOT(slotShowSystemMenu()));
    m_btnSystemTool->setPopupMode(QToolButton::InstantPopup);
    m_btnSystemTool->setFocusPolicy(Qt::NoFocus);
    m_btnSystemTool->setCheckable(true);
    m_btnSystemTool->setChecked(true);
    m_btnSystemTool->setFont(font);

    m_btnBrowseInfoTool = CGlobal::instance()->CreateToolButton(this, ":/images/browse.png", "信息");
    connect(m_btnBrowseInfoTool,SIGNAL(clicked()),this,SLOT(slotShowBrowseMenu()));
    m_btnBrowseInfoTool->setPopupMode(QToolButton::InstantPopup);
    m_btnBrowseInfoTool->setFocusPolicy(Qt::NoFocus);
    m_btnBrowseInfoTool->setCheckable(true);
    m_btnBrowseInfoTool->setChecked(true);
    m_btnBrowseInfoTool->setFont(font);


    m_btnPlanTool = CGlobal::instance()->CreateToolButton(this, ":/images/plan.png", "疏散预案");
//    connect(m_btnPlanTool,SIGNAL(clicked()),CGlobal::instance()->designMainView(),SLOT(slotEvacuationGroups()));
//    connect(treeView, SIGNAL(sigIsLayerView(bool)), m_btnPlanTool, SLOT(setEnabled(bool)));
    m_btnPlanTool->setEnabled(false);

    m_btnTableShowTool = CGlobal::instance()->CreateToolButton(this, ":/images/HandEvacuate.png", "视图");
    connect(m_btnTableShowTool,SIGNAL(clicked()),this,SLOT(slotShowListMenu()));
    m_btnTableShowTool->setPopupMode(QToolButton::InstantPopup);
    m_btnTableShowTool->setFocusPolicy(Qt::NoFocus);
    m_btnTableShowTool->setCheckable(true);
    m_btnTableShowTool->setChecked(true);
    m_btnTableShowTool->setFont(font);

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
    m_topToolBox->addWidget(m_btnPlanTool);
    m_topToolBox->addWidget(m_btnLoginInfoTool);
    m_topToolBox->addWidget(m_btnCanInfoTool);
    m_topToolBox->addWidget(m_btnBrowseInfoTool);
    m_topToolBox->addWidget(m_btnTableShowTool);
    m_topToolBox->addWidget(m_btnManageTool);

    m_topToolBox->addWidget(m_btnSystemTool);
    m_topToolBox->addWidget(m_btnLoginTool);
    m_topToolBox->addWidget(m_btnLogoutTool);

    m_topToolBox->addWidget(m_checkTime);
    m_topToolBox->addItem(topSpacerItem);
    setLayout(m_topToolBox);
}

QMenu* CTopToolBar::createCheckMenu()
{
    QMenu *checkMenu = new QMenu(this);
    checkMenu ->setStyleSheet(m_strSheet);
    checkMenu ->setStyle(m_style);
    checkMenu ->addAction(QIcon(":/images/monthcheck.png"), "系统月检", this, SLOT(slotMonthlyTest()));
    checkMenu ->addAction(QIcon(":/images/yearcheck.png"), "系统年检", this, SLOT(slotYearTest()));
    return checkMenu;
}

QMenu* CTopToolBar::createEmergencyMenu()
{
    QMenu *emergencyMenu = new QMenu(this);
    emergencyMenu ->setStyleSheet(m_strSheet);
    emergencyMenu ->setStyle(m_style);
    emergencyMenu->addAction(QIcon(":/images/start.png"), "应急启动", this, SLOT(slotStart()));
    emergencyMenu->addAction(QIcon(":/images/stop.png"), "应急停止", this, SLOT(slotResetSystem()));
    return emergencyMenu;
}

QMenu* CTopToolBar::createBrowseMenu()
{
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
    QMenu *regMenu = new QMenu(this);
    regMenu->setStyleSheet(m_strSheet);
    regMenu->setStyle(m_style);
    regMenu->addAction(QIcon(":/images/allregedit.png"), "全部注册", this, SLOT(slotLoginInfo()));
    regMenu->addAction(QIcon(":/images/loopregedit.png"), "单机注册", this, SLOT(slotLoginLoop()));
    return regMenu;
}

QMenu* CTopToolBar::createListMenu()
{
    QMenu *listMenu = new QMenu(this);
    listMenu->setStyleSheet(m_strSheet);
    listMenu->setStyle(m_style);
    listMenu->addAction(QIcon(":/images/List.png"), "列表视图", this, SLOT(slotListView()));
    listMenu->addAction(QIcon(":/images/Layout.png"), "布局视图", this, SLOT(slotLayoutView()));
    return listMenu;
}

QMenu* CTopToolBar::createManageMenu()
{
    QMenu *manageMenu = new QMenu(this);
    manageMenu->setStyleSheet(m_strSheet);
    manageMenu->setStyle(m_style);
    manageMenu->addAction(QIcon(":/images/selfcheck.png"), "系统自检", this, SLOT(slotSelfTest()));
    manageMenu ->addAction(QIcon(":/images/monthcheck.png"), "系统月检", this, SLOT(slotMonthlyTest()));
    manageMenu ->addAction(QIcon(":/images/yearcheck.png"), "系统年检", this, SLOT(slotYearTest()));
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
    QMenu *sysMenu = new QMenu(this);
    sysMenu->setStyleSheet(m_strSheet);
    sysMenu->setStyle(m_style);
    sysMenu->addAction(QIcon(":/images/set_system.png"), "系统设置", this, SLOT(slotSystemSetup()));
    sysMenu->addSeparator();
    sysMenu->addAction(QIcon(":/images/simulatetime.png"), "模拟加速开始", this, SLOT(slotStartSimulateTime()));
    sysMenu->addAction(QIcon(":/images/Recovery.png"), "模拟加速停止", this, SLOT(slotStopSimulateTime()));

    sysMenu->addSeparator();
    sysMenu->addAction(QIcon(":/images/about.png"), "关于本机", this, SLOT(slotAbout()));
    sysMenu->addAction(QIcon(":/images/UserConfig.png"), "更新日志", this, SLOT(slotUpdateLog()));
    sysMenu->addAction(QIcon(":/images/ShutDown.png"), "重启", this, SLOT(slotReStart()));

    return sysMenu;
}

void CTopToolBar::slotShowCheckMenu()
{
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
    CMsgNull m_MsgNUll;
    m_btnMuteTool->setChecked(true);
    if(CGlobal::instance()->m_bFault || CGlobal::instance()->m_bStart)//当有状态信息时消音才有效
    {
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_Mute, &m_MsgNUll);
    }
}

void CTopToolBar::slotResetSystem()
{
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
    CGlobal::instance()->m_bInLogin = false;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_Reset, &m_MsgNUll);

    CGlobal::instance()->DataProcessing()->resetStatus();

//    CGlobal::instance()->ClientBusiness()->clearStatusObject(true);
//    CGlobal::instance()->ClientBusiness()->clearExpectObject();
    if(CGlobal::instance()->m_tcptype & 0X01)
    {
        //复位声明上传到云平台
        CGlobal::instance()->ClientBusiness()->XmlResetDeclare();
    }
    else if(CGlobal::instance()->m_tcptype & 0X02)
    {
        //复位声明上传到客户端
        CGlobal::instance()->ClientBusiness()->serverResetDeclareUpload();
    }

    //集中电源状态复位(包含回路)
    CGlobal::instance()->programDistributionView()->distributionResetStatus();

    //灯具状态复位
//    CGlobal::instance()->ClientBusiness()->DeviceResetStatus();
    CGlobal::instance()->DlgDevices()->getDeviceInfo();
    CGlobal::instance()->DlgDevices()->lampStatusUpload();
    //布局视图界面灯具状态更新
    CGlobal::instance()->designSplashView()->setPageStateUpdate(true);

    //复位后延迟1s对比检查数据库数据是否在线
    QTimer::singleShot(1000, this, SLOT(slotCheckLoginInfo()));
}

void CTopToolBar::resetSystem()
{
    CMsgNull m_MsgNUll;

    CGlobal::instance()->m_isEmergency = false;
    CGlobal::instance()->m_bInLogin = false;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_Reset, &m_MsgNUll);

    CGlobal::instance()->DataProcessing()->resetStatus();
//    CGlobal::instance()->ClientBusiness()->clearStatusObject(true);
//    CGlobal::instance()->ClientBusiness()->clearExpectObject();
    if(CGlobal::instance()->m_tcptype & 0X01)
    {
        //复位声明上传到云平台
        CGlobal::instance()->ClientBusiness()->XmlResetDeclare();
    }
    else if(CGlobal::instance()->m_tcptype & 0X02)
    {
        //复位声明上传到客户端
        CGlobal::instance()->ClientBusiness()->serverResetDeclareUpload();
    }
    //集中电源状态复位(包含回路)
    CGlobal::instance()->programDistributionView()->distributionResetStatus();

    //灯具状态复位
//    CGlobal::instance()->ClientBusiness()->DeviceResetStatus();
    CGlobal::instance()->DlgDevices()->getDeviceInfo();
    CGlobal::instance()->DlgDevices()->lampStatusUpload();
    //布局视图界面灯具状态更新
    CGlobal::instance()->designSplashView()->setPageStateUpdate(true);

    //复位后延迟1s对比检查数据库数据是否在线
    QTimer::singleShot(1000, this, SLOT(slotCheckLoginInfo()));
}

void CTopToolBar::slotCheckLoginInfo()
{
    ConfigFileManager cfg;
    cfg.ParseConfigFile();//读取注册文件
    CGlobal::instance()->DataProcessing()->slotcheckLoginInfo();
}

void CTopToolBar::slotStart()
{
    dlgAdministrator m_Administrator;
    m_Administrator.setWindowTitle("用户验证");
    m_Administrator.setUserDisabled(true);
    if(QDialog::Accepted == m_Administrator.exec())
    {
        if(CGlobal::instance()->m_EmergencyOperation == CGlobal::Start)
            return;
        CGlobal::instance()->m_EmergencyOperation = CGlobal::Start;
        CMsgNull m_MsgNUll;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_EmergencyStart, &m_MsgNUll);
        CGlobal::instance()->ClientBusiness()->Emergency();//下发方向控制和闪烁
        if(CGlobal::instance()->m_tcptype & 0X01)
        {
            //上传手动应急到云平台
            CGlobal::instance()->ClientBusiness()->XmlManualLaunchUpload();
        }
        else if(CGlobal::instance()->m_tcptype & 0X02)
        {
            //服务端上传手动应急
            CGlobal::instance()->ClientBusiness()->serverManualLaunchUpload();
        }
    }
    m_btnManageTool->setChecked(true);
}

void CTopToolBar::slotStop()
{
    CMsgNull m_MsgNUll;
    dlgAdministrator m_Administrator;
    m_Administrator.setWindowTitle("用户验证");
    m_Administrator.setUserDisabled(true);
    if(QDialog::Accepted == m_Administrator.exec()) {
        if(CGlobal::instance()->ClientBusiness()->exeCommand(NCT_EmergencyStop, &m_MsgNUll))
        {
            CGlobal::instance()->m_EmergencyOperation = CGlobal::Stop;
            emergencyStatusClear();
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

void CTopToolBar::slotDeviceInfo()
{
//    CGlobal::instance()->m_HaveDialog = true;
//    CGlobal::instance()->loginInformation()->exec();
    CGlobal::instance()->centerFrameContainer()->setCurrentIndex(4);
    m_btnLoginInfoTool->setChecked(true);
//    CGlobal::instance()->m_HaveDialog = false;
}

void CTopToolBar::slotCanInfo()
{
    CGlobal::instance()->programLoopView()->updateDistribution();
    CGlobal::instance()->centerFrameContainer()->setCurrentIndex(3);
    m_btnCanInfoTool->setChecked(true);
}

void CTopToolBar::slotDayInformation()
{
    CGlobal::instance()->SystemRecord()->setPage(1);
    CGlobal::instance()->SystemRecord()->dayrecord();
    CGlobal::instance()->centerFrameContainer()->setCurrentIndex(2);
    m_btnBrowseInfoTool->setChecked(true);
}

void CTopToolBar::slotWeekInformation()
{
    CGlobal::instance()->SystemRecord()->setPage(1);
    CGlobal::instance()->SystemRecord()->weekrecord();
    CGlobal::instance()->centerFrameContainer()->setCurrentIndex(2);
    m_btnBrowseInfoTool->setChecked(true);
}

void CTopToolBar::slotLogging()
{
    CGlobal::instance()->SystemRecord()->setPage(1);
    CGlobal::instance()->centerFrameContainer()->setCurrentIndex(2);
    CGlobal::instance()->SystemRecord()->customQueryrecord();
    m_btnBrowseInfoTool->setChecked(true);
}

void CTopToolBar::slotSearchDevice()
{
    if(!CGlobal::instance()->DlgSearchDevice())
        new dlgSearchDevice(this);
    CGlobal::instance()->DlgSearchDevice()->show();
    m_btnBrowseInfoTool->setChecked(true);
}

void CTopToolBar::slotListView()
{
     CGlobal::instance()->centerFrameContainer()->setCurrentIndex(1);
     CGlobal::instance()->DlgDevices()->getDeviceInfo();
     m_btnTableShowTool->setChecked(true);
}

void CTopToolBar::slotLayoutView()
{
    CGlobal::instance()->centerFrameContainer()->setCurrentIndex(0);
    m_btnTableShowTool->setChecked(true);
}

void CTopToolBar::slotSelfTest()
{
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
        QString log;
        log = "系统自检失败！";
        messageBox.setText(log);
        messageBox.exec();
        CGlobal::instance()->m_HaveDialog = false;
    }  
}

void CTopToolBar::slotMonthlyTest()
{
    CMsgNull m_MsgNUll;
    dlgAdministrator m_Administrator;
    m_Administrator.setWindowTitle("用户验证");
    m_Administrator.setUserDisabled(true);
    if(QDialog::Accepted == m_Administrator.exec()) {
        if(!CGlobal::instance()->ClientBusiness()->exeCommand(NCT_SystemMonthCheck, &m_MsgNUll))
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
    CMsgNull m_MsgNUll;
    dlgAdministrator m_Administrator;
    m_Administrator.setWindowTitle("用户验证");
    m_Administrator.setUserDisabled(true);
    if(QDialog::Accepted == m_Administrator.exec()) {
        if(!CGlobal::instance()->ClientBusiness()->exeCommand(NCT_SystemYearCheck, &m_MsgNUll))
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
    CGlobal::instance()->TimeInterval()->updateIPData();
    CGlobal::instance()->TimeInterval()->updateSqliteData();
    CGlobal::instance()->centerFrameContainer()->setCurrentIndex(5);
    m_btnSystemTool->setChecked(true);
}



void CTopToolBar::slotStartSimulateTime()
{
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
    CMsgNull m_MsgNUll;
    m_speedup = false;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_StopSpeedUp, &m_MsgNUll);
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::blue);
    CGlobal::instance()->TopToolBar()->m_checkTime->setPalette(palette);
    m_btnSystemTool->setChecked(true);
}

//void CTopToolBar::slotRestoreFactory()
//{
//    CMsgNull m_MsgNUll;
//    dlgAdministrator m_Administrator;
//    if(QDialog::Accepted == m_Administrator.exec()) {
        
//    } else {
//        return;
//    }

//    if(QMessageBox::Yes == QMessageBox::question(this, CGlobal::instance()->m_strApplicationTitle, tr("Are you sure want to recover the canport, this operate will be clear all the data in project?"), QMessageBox::Yes, QMessageBox::No))
//    {
//        if(CGlobal::instance()->ClientBusiness()->exeCommand(NCT_FactoryReset, &m_MsgNUll))
//            QMessageBox::information(this, CGlobal::instance()->m_strApplicationTitle,
//                                     tr("The canport recover success."));
//    }
//}

//void CTopToolBar::slotStartCharge()
//{
//    InitMenuCheck();
//    CGlobal::instance()->m_bCharge = true;
//    CMsgNull m_MsgNUll;

//    dlgAdministrator m_Administrator;
//    if(QDialog::Accepted == m_Administrator.exec()) {
        
//    } else {
//        return;
//    }

//    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_StartCharge, &m_MsgNUll);
//}

//void CTopToolBar::slotStopCharge()
//{
//    InitMenuCheck();
//    CGlobal::instance()->m_bCharge = false;
//    CMsgNull m_MsgNUll;

//    dlgAdministrator m_Administrator;
//    if(QDialog::Accepted == m_Administrator.exec()) {
        
//    } else {
//        return;
//    }

//    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_StopCharge, &m_MsgNUll);
//}


//void CTopToolBar::slotHelp()
//{
//    InitMenuCheck();
////    m_btnSystemTool->setChecked(false);
//}

void CTopToolBar::slotAbout()
{
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
    messageBox.setText("名称:" + CGlobal::instance()->m_strApplicationTitle +
                   tr("\n") + "版本:" + "V1.01CA");
    messageBox.exec();
    CGlobal::instance()->m_HaveDialog = false;
    m_btnSystemTool->setChecked(true);
}

void CTopToolBar::slotUpdateLog()
{
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
    QString updateLog;
    updateLog = "2023.08 V1.00CA" + tr("\n") + "1、新版本发布" + tr("\n")
            + "2023.11 V1.01CA" + tr("\n") + "1、增加平面图显示功能;" + tr("\n") + "2、上传琴台主机数据增加平面图;";
    messageBox.setText(updateLog);
    messageBox.exec();
    CGlobal::instance()->m_HaveDialog = false;
    m_btnSystemTool->setChecked(true);
}

void CTopToolBar::slotReStart()
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
//    messageBox.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
    messageBox.setWindowTitle("重启");
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Cancel);
    QString log;
    log = "是否重启程序？";
    messageBox.setText(log);
    int ret = messageBox.exec();
//    int ret = QMessageBox::information(NULL, "重启",
//                             "是否重启程序？", QMessageBox::Yes, QMessageBox::No);
    if(ret == QMessageBox::Yes)
    {
        qApp->quit();
        slotMuteSystem();
//        QProcess::execute(QString("reboot"));

//        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
    CGlobal::instance()->m_HaveDialog = false;
    m_btnSystemTool->setChecked(true);
}

void CTopToolBar::slotLogin()
{
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
    CGlobal::instance()->m_UserType = CGlobal::User;
    CGlobal::instance()->m_loginStatus = false;
    CGlobal::instance()->BottomToolBar()->setUser();
    CGlobal::instance()->centerFrameContainer()->setCurrentIndex(0);
    updateMenu();
    m_btnLogoutTool->setChecked(true);
}

void CTopToolBar::changeInformation(bool flag)
{
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
        m_btnSystemTool->setVisible(false);
        m_btnLoginTool->setVisible(false);
        m_btnLogoutTool->setVisible(true);
        CGlobal::instance()->TimeInterval()->setIpAndTimeHide(true);
    }
    else if(CGlobal::instance()->m_UserType == CGlobal::Operator)
    {
        m_btnResetTool->setVisible(true);
        m_btnEmergencyTool->setVisible(true);
        m_btnPlanTool->setVisible(false);
        m_btnCanInfoTool->setVisible(true);
        m_btnLoginInfoTool->setVisible(false);
        m_btnBrowseInfoTool->setVisible(true);
        m_btnTableShowTool->setVisible(true);
        m_btnManageTool->setVisible(true);
        m_btnSystemTool->setVisible(false);
        m_btnLoginTool->setVisible(false);
        m_btnLogoutTool->setVisible(true);
        CGlobal::instance()->TimeInterval()->setIpAndTimeHide(true);
    }
    else if(CGlobal::instance()->m_UserType == CGlobal::Administrator)
    {
        m_btnResetTool->setVisible(true);
        m_btnEmergencyTool->setVisible(true);
        m_btnPlanTool->setVisible(false);
        m_btnCanInfoTool->setVisible(true);
        m_btnLoginInfoTool->setVisible(false);
        m_btnBrowseInfoTool->setVisible(true);
        m_btnTableShowTool->setVisible(true);
        m_btnManageTool->setVisible(true);
        m_btnSystemTool->setVisible(true);
        m_btnLoginTool->setVisible(false);
        m_btnLogoutTool->setVisible(true);
        CGlobal::instance()->TimeInterval()->setIpAndTimeHide(true);
    }
    else if(CGlobal::instance()->m_UserType == CGlobal::SuperAdministrator)
    {
        m_btnResetTool->setVisible(true);
        m_btnEmergencyTool->setVisible(true);
        m_btnPlanTool->setVisible(false);
        m_btnCanInfoTool->setVisible(true);
        m_btnLoginInfoTool->setVisible(true);
        m_btnBrowseInfoTool->setVisible(true);
        m_btnTableShowTool->setVisible(true);
        m_btnManageTool->setVisible(true);
        m_btnSystemTool->setVisible(true);
        m_btnLoginTool->setVisible(false);
        m_btnLogoutTool->setVisible(true);
        CGlobal::instance()->TimeInterval()->setIpAndTimeHide(false);
    }
}

void CTopToolBar::emergencyStatusClear()
{
    for(int i=0; i<4; i++)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i+3);
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
                    QList<CDevice*> devices = loop->devices();
                    for(int m=0; m<devices.count(); m++){
                        CDevice* device = devices.at(m);
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


