#ifndef CTOPTOOLBAR_H
#define CTOPTOOLBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QHash>
#include <QMenu>
#include <QToolButton>
#include <QTabWidget>
#include <QDialog>
#include <QProcess>

class CMyIconStyle;
class CInformationModel;
class CTopToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit CTopToolBar(QWidget *parent = 0);
    ~CTopToolBar();
    void changeInformation(bool flag);//更改顶部工具栏信息提示
    void updateMenu();
    bool speedup() {return m_speedup;}
    void addIPChangeRecord(QString changeText);
    void emergencyStatusClear();
    void resetSystem();

    QHBoxLayout *m_topToolBox;//顶层工具栏容器
    QLabel *m_checkTime;
    QToolButton *m_btnMuteTool;//消音按钮
    QToolButton *m_btnResetTool;//复位按钮
    QToolButton *m_btnEmergencyTool; //应急按钮
    QToolButton *m_btnCheckTool;    // 增加检查按钮，按下后弹出年检或者月检按钮
    QToolButton *m_btnYCheckTool;//年检按钮
    QToolButton *m_btnMCheckTool;//月检按钮
    QToolButton *m_btnManageTool;//管理按钮
    QToolButton *m_btnLoginInfoTool;//注册信息按钮
    QToolButton *m_btnCanInfoTool;//Can信息按钮
    QToolButton *m_btnSystemTool;//系统按钮
    QToolButton *m_btnBrowseInfoTool;//信息浏览按钮
    QToolButton *m_btnPlanTool;//疏散预案按钮
    QToolButton *m_btnTableShowTool; // 表格显示灯具
    QToolButton *m_btnHandEvacuateTool;//手动疏散按钮
    QToolButton *m_btnLoginTool;//登录按钮
    QToolButton *m_btnLogoutTool;//注销按钮

private:
    QMenu *m_menuBrowse;
    QMenu *m_menuRegedit;
    QMenu *m_menuManage;
    QMenu *m_menuSystem;
    QMenu *m_menuList;
    QMenu *m_menuCheck;
    QMenu *m_menuEmergency;
    CMyIconStyle *m_style;
    QString m_strSheet;
    QHash<QString, CInformationModel*> m_informationModel;
    QStringList m_tabList;
    QTabWidget *m_tabWidget;
    bool m_speedup;
    QTimer * m_time;

private:
    void initMember();
    void createToolBar();
    void createCheckTime();
    QMenu *createCheckMenu();
    QMenu *createEmergencyMenu();
    QMenu *createBrowseMenu();
    QMenu *createRegMenu();
    QMenu *createManageMenu();
    QMenu *createSystemMenu();
    QMenu *createListMenu();

    QProcess *m_process_unmountUSB; // 声明一个 QProcess 成员变量用于执行外部脚本

signals:

public slots:
    //The top toolbar button to trigger the groove function
//    void InitMenuCheck();
    void slotShowCheckMenu();   /*fix:(2023-02-06) merge check button*/
    void slotShowListMenu();
    void slotShowRegMenu();
    void slotShowBrowseMenu();
    void slotShowManageMenu();
    void slotShowEmergencyMenu();
    void slottimeout();

    void slotShowSystemMenu();
    void slotMuteSystem();
    void slotResetSystem();
    void slotMonthlyTest();
    void slotYearTest();

    void slotDayInformation();
    void slotWeekInformation();
    void slotDeviceInfo();
    void slotCanInfo();
    void slotSearchDevice();
    void slotLogging();    

    void slotSelfTest();
    void slotStart();
    void slotStop();
    void slotHandFire();

    void slotListView();
    void slotLayoutView();
    void slotManual();
    void slotAuto();


    void slotSystemSetup();
    void slotStartSimulateTime();
    void slotStopSimulateTime();

    void slotSaveDeviceData();
    void slotSaveDeviceStatus();
    void slotOutUSB();

    void slotReStart();
    void slotAbout();
    void slotUpdateLog();

    void slotLogin();
    void slotLogout();
    void slotCheckLoginInfo();
};

#endif // CTOPTOOLBAR_H
