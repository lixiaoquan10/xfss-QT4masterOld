#ifndef CGLOBAL_H
#define CGLOBAL_H

#include <QApplication>
#include <QByteArray>
#include <QCloseEvent>
#include <QColor>
#include <QComboBox>
#include <QContextMenuEvent>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>
#include <QDesktopWidget>
#include <QDialog>
#include <QDir>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QFile>
#include <QFileInfo>
#include <QFont>
#include <QGridLayout>
#include <QHash>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QLineF>
#include <QList>
#include <QMenu>
#include <QMessageBox>
#include <QModelIndex>
#include <QMouseEvent>
#include <QMimeData>
#include <QNetworkInterface>
#include <QObject>
#include <QPixmap>
#include <QPoint>
#include <QPointF>
#include <QProcess>
#include <QPushButton>
#include <QRegExp>
#include <QScrollBar>
#include <QSet>
#include <QSettings>
#include <QSizePolicy>
#include <QShortcut>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSortFilterProxyModel>
#include <QState>
#include <QStackedLayout>
#include <QStackedWidget>
#include <QSvgRenderer>
#include <QTabWidget>
#include <QTableView>
#include <QTemporaryFile>
#include <QTextCodec>
#include <QTimer>
#include <QToolButton>
#include <QUuid>
#include <QVariant>
#include <QVector>
#include <QVBoxLayout>
#include <QWidget>


#include "widget.h"
#include "cdevicetype.h"
#include "cmyiconstyle.h"
#include "common.h"
#include "groupbox.h"
#include "qcustomplot.h"
#include "cfilterobject.h"

#include "business/cclientbusiness.h"
#include "business/cdataprocessing.h"
#include "communication/ccommunication.h"
#include "dialog/dlgadministrator.h"
#include "dialog/dlgchangeadd.h"
#include "dialog/dlgchangetype.h"
#include "dialog/dlgdevices.h"
#include "dialog/dlginformationwindow.h"
#include "dialog/dlglogininformation.h"
#include "dialog/dlgsystemrecord.h"
#include "dialog/dlgsystemrecordmodel.h"
#include "dialog/dlgtimeinterval.h"
#include "dialog/dlguploadlamp.h"
#include "dialog/dlgusetime.h"
#include "dialog/tablewidget.h"
#include "dialog/dlgsearchdevice.h"
#include "dm/cdbsqlite.h"
#include "dm/clayer.h"
#include "dm/ccontroller.h"
#include "dm/ccanport.h"
#include "dm/cdistribution.h"
#include "dm/cloop.h"
#include "dm/cdevice.h"
#include "dm/cfirepoint.h"
#include "dm/cobject.h"
#include "dm/cdm.h"
#include "graphics/cmath.h"
#include "graphics/configfilemanager.h"
#include "graphics/deviceControl.h"
#include "graphics/deviceForbidFault.h"
#include "model/ccanportmodel.h"
#include "model/ccontrollermodel.h"
#include "model/ccontrollertreemodel.h"
#include "model/cdesigntreemodel.h"
#include "model/cdevicelistmodel.h"
#include "model/cdevicemodel.h"
#include "model/cdevicesfindtablemodel.h"
#include "model/cdistributionmodel.h"
#include "model/cinformationmodel.h"
#include "model/cloopmodel.h"
#include "model/cmodel.h"
#include "model/ctablemodel.h"
#include "model/ctreeitem.h"

#include "struct/shardEnum.h"

#include "widget/cbottomtoolbar.h"
#include "widget/ccenterframe.h"
#include "widget/cdesigncontainer.h"
#include "widget/cdesignsplashview.h"
#include "widget/cdesigntreeview.h"
#include "widget/cdesignview.h"
#include "widget/cinformationlabel.h"
#include "widget/cminisplitter.h"
#include "widget/cprogramcanportview.h"
#include "widget/cprogramcontainer.h"
#include "widget/cprogramcontrollerview.h"
#include "widget/cprogramdeviceview.h"
#include "widget/cprogramdistributionview.h"
#include "widget/cprogramloopview.h"
#include "widget/cprogramtreeview.h"
#include "widget/cprogramview.h"
#include "widget/cstatustabs.h"
#include "widget/ctoptoolbar.h"

#include "style/cstyledbar.h"
#include "style/cstylehelper.h"

#include "delegate/cdevicetypedelegate.h"
#include "view/ctableview.h"
#include "view/ccontrollertreeview.h"


#define DATETIME_FORMAT "yyyy-MM-dd HH:mm:ss"
#define GLOBAL_LINE_WIDTH 16
#define EVACUATION_HAS_DESIGNMAINVIEW
#define EVACUATION_HAS_DESIGNTREEVIEW
#define EVACUATION_HAS_MONITOR_STATUS_CHANGED
#define EVACUATION_HAS_MONITOR_UNDEFINED_OBJECT
#define EVACUATION_HAS_WMF
#define EVACUATION_HAS_DLGPATH
#define EVACUATION_HAS_DLGGROUPS
#define EVACUATION_HAS_DLGGROUP
#define EVACUATION_HAS_DLGFLOWS
#define EVACUATION_HAS_DLGFLOW
#define EVACUATION_HAS_DLGHAND
#define EVACUATION_HAS_DLGSEARCHDEVICE
#define EVACUATION_HAS_OBJECT_STATUS_TIME
#define EVACUATION_HAS_MANUAL_ALARMPOINT_MENU
#define EVACUATION_HAS_HIDE_ALARMPOINT_MENU
#define EVACUATION_NOT_SHOW_PATHNOMARL
#define EVACUATION_NOT_SHOW_ALARMPOINT
#define EVACUATION_HAS_LOGIN_DEVICE_STATUS
#define EVACUATION_HAS_LOGIN_DEVICE_TYPE
#define EVACUATION_HAS_LOGIN_DEVICE_NUMBER
#define EVACUATION_HAS_POWER_STATUS_INFO
class QStackedWidget;
class QTabWidget;
class CDM;
class CObject;
class CController;
class CCanport;
class CDeviceType;
class CProgramTreeView;
class CProgramContainer;
class CProgramControllerView;
class CProgramCanportView;
class CProgramLoopView;
class CProgramDistributionView;
class CProgramDeviceView;
class CDesignTreeView;
class CDesignContainer;
//class CDesignMainView;
class CDesignSplashView;
class dlgDevices;
class dlgPaths;
class dlgEvacuationGroups;
class dlgEvacuationGroup;
class dlgEvacuationFlow;
class dlgHandEvacuation;
class dlgSearchDevice;

class dlgTimeInterval;
class dlgSystemRecord;
class dlgInformationWindow;
class dlgLoginInformation;
class dlgSetSystemTime;
class dlgDevices;
class CLayer;
class Widget;
class CClientBusiness;
class QSound;
class CTopToolBar;
class CBottomToolBar;
class CDataProcessing;
class CCenterFrame;
class CStatusTabs;

class ConfigFileManager;


namespace  irrklang {
    class ISoundEngine;
}

enum ObjectType {
    OBJT_Controller = 0,
    OBJT_Canport = 1,
    OBJT_Distribution = 2,
    OBJT_Loop = 3,
    OBJT_Device = 4
};

//enum LampType {
//    LT_None = 0,
//    LT_DoubleArrow = 1,
//    LT_LeftArrow = 2,
//    LT_RightArrow = 3,
//    LT_Light = 4,
//    LT_LeftF = 5,
//    LT_Floor = 6,
//    LT_Exit = 7,
//    LT_ChanggeExit = 8,
//    LT_OneUnderground = 9,
//    LT_DoubleUnderground = 10
//};

class CGlobal : public QObject
{
    Q_OBJECT
public:
    enum UserType{OnDuty,Operator,Administrator,SuperAdministrator,User };
    enum EvacuationMode{ Manual, Auto };
    enum EmergencyOperation{ Start, Stop };

    static CGlobal* instance();
    static void destroy();
    static QByteArray loadRawData(const QString& str);
    static int colorToInt(const QColor& color);
    static QColor intToColor(const int n);
    static void initLineWidth() { m_lineWidth = GLOBAL_LINE_WIDTH; }
    static void setLineWidth(const qreal width) { if(m_lineWidth == width) return; m_lineWidth = width; }
    static qreal lineWidth() { return m_lineWidth; }

    void setPanelAddress(const int n) { m_nPanelAddress = n; }
    int panelAddress() const { return m_nPanelAddress; }
    int loadKeyId(int type,int Id);
    int saveKeyId(int type,int Id);
//    int LampTypeAnalysis(int deviceTypeId);
    void setMsgText(const QString &str);
    void setMSgColor(const QColor &color);
    const QList<CLayer *> getLayers(const QList<CController *> &controllerList);

    QGridLayout* CreateGridLayout(QWidget *parent=0, int nmargin=0, int nspacing=0);
    QVBoxLayout* CreateVBoxLayout(QWidget *parent=0, int nmargin=0, int nspacing=0);
    QHBoxLayout* CreateHBoxLayout(QWidget *parent=0, int nmargin=0, int nspacing=0);
    QPushButton* CreatePushButton(QWidget *parent, QString strText, QString strico, QString strtooltip, QSizePolicy::Policy horizontal=QSizePolicy::Expanding, QSizePolicy::Policy vertical=QSizePolicy::Expanding);
    QToolButton* CreateToolButton(QWidget *parent, QString strico, QString strtooltip, QSizePolicy::Policy horizontal=QSizePolicy::Expanding, QSizePolicy::Policy vertical=QSizePolicy::Expanding);
    QToolButton* CreateLedButton(QWidget *parent, QString strText, QString strico, QString strtooltip, QSizePolicy::Policy horizontal=QSizePolicy::Fixed, QSizePolicy::Policy vertical=QSizePolicy::Expanding);
    void setLayer(CLayer *layer);

    void delFileLater(const QString& fileName) { m_delFiles.insert(fileName); }

    void setWorkspaces(const QString path) { m_workspaces = path; }
    QString workspaces() const { return m_workspaces; }
    void setControllerName(const QString name) {m_controllerName = name;}
    QString controllerName() const { return m_controllerName; }


    void setMaxCacheLayers(const int n) { m_maxCacheLayers = n; }
    int maxCacheLayers() const { return m_maxCacheLayers; }
    void setIsShowDeviceToolTip(const bool enable) { m_isShowDeviceToolTip = enable; }
    bool isShowDeviceToolTip() const { return m_isShowDeviceToolTip; }
    void setAutoSwitchLayer(bool enable) { m_isAutoSwitchLayer = enable; }
    bool isAutoSwitchLayer() const { return m_isAutoSwitchLayer; }


    QPixmap* simulateFirePixmap() const { return m_simulateFirePixmap; }
    QPixmap* simulateFirstFirePixmap() const { return m_simulateFirstFirePixmap; }
    void saveSetting();
    void readSetting();
    void readConfig();

    CDM* dm() const { return m_dm; }
    CController* controller();
    CCanport* panel();

    QString getObjectStatusStr(short status) const { return m_objectStatusStr.value(status); }
    QString getObjectStatusSuffix(short status, unsigned char value) const;
    QString getObjectStatusDescription(const CObject* object, short status, unsigned char value) const;
    QSet<short> startStatusList() const { return m_startStatusList; }
    bool isStartStatus(short status) const { return m_startStatusList.contains(status); }
    QSet<short> faultStatusList() const { return m_faultStatusList; }
    bool isFaultStatus(short status) const { return m_faultStatusList.contains(status); }
    QSet<short> emergencyStatusList() const { return m_emergencyStatusList; }
    bool isEmergencyStatus(short status) const { return m_emergencyStatusList.contains(status); }
    inline dlgEvacuationFlow* DlgEvacuationFlow() const { return m_dlgEvacuationFlow; }
    inline void setDlgEvacuationFlow(dlgEvacuationFlow* dlg) { m_dlgEvacuationFlow = dlg; }
    inline dlgSearchDevice* DlgSearchDevice() const { return m_dlgSearchDevice; }
    inline void setDlgSearchDevice(dlgSearchDevice* searchDevice){ m_dlgSearchDevice = searchDevice; }

    void initObjectStatusList();
    void initDeviceType();
    CDeviceType* deviceType(int i) const;
    QVector<CDeviceType*> deviceTypes() const { return m_deviceTypes; }

    CProgramTreeView* programTreeView() const { return m_pProgramTreeView; }
    void setProgramTreeView(CProgramTreeView* programTreeView) { m_pProgramTreeView = programTreeView; }
    CProgramControllerView* programControllerView() const { return m_pProgramControllerView; }
    void setProgramControllerView(CProgramControllerView* programControllerView) { m_pProgramControllerView = programControllerView; }
    CProgramContainer* programContainer() const { return m_pProgramContainer; }
    void setProgramContainer(CProgramContainer* programContainer) { m_pProgramContainer = programContainer; }
    CProgramCanportView* programCanportView() const { return m_pProgramCanportView; }
    void setProgramCanportView(CProgramCanportView* programCanportView) { m_pProgramCanportView = programCanportView; }
    CProgramLoopView* programLoopView() const { return m_pProgramLoopView; }
    void setProgramLoopView(CProgramLoopView* programLoopView) { m_pProgramLoopView = programLoopView; }
    CProgramDistributionView* programDistributionView() const { return m_pProgramDistributionView; }
    void setProgramDistributionView(CProgramDistributionView* programDistributionView) { m_pProgramDistributionView = programDistributionView; }
    CProgramDeviceView* programDeviceView() const { return m_pProgramDeviceView; }
    void setProgramDeviceView(CProgramDeviceView* programDeviceView) { m_pProgramDeviceView = programDeviceView; }
    CDesignTreeView* designTreeView() const { return m_pDesignTreeView; }
    void setDesignTreeView(CDesignTreeView* designTreeView) { m_pDesignTreeView = designTreeView; }
    CDesignContainer* designContainer() const { return m_pDesignContainer; }
    void setDesignContainer(CDesignContainer* designContainer) { m_pDesignContainer = designContainer; }
//    CDesignMainView* designMainView() const { return m_pDesignMainView; }
//    void setDesignMainView(CDesignMainView* designMainView) { m_pDesignMainView = designMainView; }
    CDesignSplashView* designSplashView() const { return m_pDesignSplashView; }
    void setDesignSplashView(CDesignSplashView* designSplashView) { m_pDesignSplashView = designSplashView; }
    dlgDevices* DlgDevices() const { return m_dlgDevices; }
    void setDlgDevices(dlgDevices* dlg) { m_dlgDevices = dlg; }

    dlgDevices *dlgDeviceInfomation() {return m_dlgDevices;}
    void setDeviceInformation(dlgDevices* deviceInformation){m_dlgDevices = deviceInformation;}
    dlgLoginInformation* loginInformation() { return m_pLoginInformation; }
    void setLoginInformation(dlgLoginInformation *loginInformation){ m_pLoginInformation = loginInformation; }
    dlgInformationWindow* InformationWindow() const { return m_dlgInforWindow; }
    void setInformationWindow(dlgInformationWindow* InforWindow){ m_dlgInforWindow = InforWindow; }
    dlgSystemRecord* SystemRecord() const { return m_SystemRecord; }
    void setSystemRecord(dlgSystemRecord* systemRecord){ m_SystemRecord = systemRecord; }
    dlgTimeInterval* TimeInterval() const { return m_dlgTimeInterval; }
    void setTimeInterval(dlgTimeInterval* timeInterval){ m_dlgTimeInterval = timeInterval; }
    CClientBusiness* ClientBusiness() const { return m_ClientBusiness; }
    void setClientBusiness(CClientBusiness* clientBusiness){ m_ClientBusiness = clientBusiness; }
    CDataProcessing *DataProcessing() const { return m_cDataProcessing; }
    void setDataProcessing(CDataProcessing* dataProcessing){ m_cDataProcessing = dataProcessing; }
    CTopToolBar *TopToolBar() const { return m_cTopToolBar; }
    void setTopToolBar(CTopToolBar *topToolBar) { m_cTopToolBar = topToolBar; }
    CBottomToolBar *BottomToolBar() const { return m_cBottomToolBar; }
    void setBottomToolBar(CBottomToolBar *bottomToolBar) { m_cBottomToolBar = bottomToolBar; }
    CCenterFrame *CenterFrame() const { return m_cCenterFrame; }
    void setCenterFrame(CCenterFrame *centerFrame) { m_cCenterFrame = centerFrame; }

    QStackedWidget *centerFrameContainer() {return m__cCenterFrameContainer;}
    void setCenterFrameContainer(QStackedWidget* stackedWidget) {m__cCenterFrameContainer = stackedWidget;}
    CStatusTabs *statusTabs() {return m_statusTabs;}
    void setStatusTabs(CStatusTabs *statusTabs){m_statusTabs = statusTabs;}

    ConfigFileManager* configFileManager() const { return m_cConfigFileManager; }
    void setConfigFileManager(ConfigFileManager* configFileManager) { m_cConfigFileManager = configFileManager; }
private:
    explicit CGlobal(QObject *parent = 0);
    ~CGlobal();
    void soundType(SoundType iType, QString sType);

public:
    Widget* m_widget;
    QTabWidget* m_tabWidget;
    QString m_strApplicationTitle;
    bool m_bSwitch_LEDButton;
    bool m_bCommOk;
    bool m_bMainPowerFault;
    bool m_bStandbyPowerFault;
    bool m_bMove_Mouse;
    bool m_bCutLayer;
    bool m_bFullScreen;
    bool m_HaveDialog;
    bool m_bFull;
    bool m_bStartComm;
    bool m_bInLogin;
    bool m_bWriteLog;

    bool m_SystemMonthCheck;
    bool m_SystemYearCheck;

    bool m_bCharge;

    bool m_loginStatus;
    UserType m_UserType;
    EvacuationMode m_EvacuationMode;
    EmergencyOperation m_EmergencyOperation;

    int m_nTimeCutLayer;//Switch Partition Space of Time

    QString m_strRemoteAddress;
    QString m_strUserName;
    QString m_strPassWord;

    QString m_information;
    QColor m_infoColor;
    QLabel *m_labelSystemMsg;

    //sounds flag
    irrklang::ISoundEngine* m_audioOutput;

    int m_soundName;//current soundName
    int m_scSoundName;//The sound paly when system selfcheck;
    bool m_scbStart;//The start sound paly when system selfcheck;
    bool m_scbFault;//The fault sound paly when system selfcheck;
    bool m_soundPlay;
    bool m_selfCheck;
    bool m_bStart;
    bool m_bFault;
    QHash<QString, RouteConfig*> m_hashCard;
private:
    static CGlobal* m_instance;
    static qreal m_lineWidth;
    static QHash<short, QString> m_objectStatusStr;
    static QSet<short> m_startStatusList;
    static QSet<short> m_faultStatusList;
    static QSet<short> m_emergencyStatusList;

    int m_nPanelAddress;
    QString m_workspaces;
    QString m_controllerName;
    int m_interval;
    int m_maxCacheLayers;
    QPixmap* m_simulateFirePixmap;
    QPixmap* m_simulateFirstFirePixmap;
    bool m_isShowDeviceToolTip;
    QSet<QString> m_delFiles;
    bool m_hideNormalAlarmPoint;
    bool m_isAutoSwitchLayer;
    bool m_allowHandFire;
    bool m_allowHandEvacuation;
    bool m_allowHandFlow;

    QVector<CDeviceType*> m_deviceTypes;
    QList<int> m_defaultDeviceType;

    CDM* m_dm;
    CProgramTreeView* m_pProgramTreeView;
    CProgramControllerView* m_pProgramControllerView;
    CProgramContainer* m_pProgramContainer;
    CProgramCanportView* m_pProgramCanportView;
    CProgramLoopView* m_pProgramLoopView;
    CProgramDistributionView* m_pProgramDistributionView;
    CProgramDeviceView* m_pProgramDeviceView;
    CDesignTreeView* m_pDesignTreeView;
    CDesignContainer* m_pDesignContainer;
//    CDesignMainView* m_pDesignMainView;
    CDesignSplashView* m_pDesignSplashView;
    dlgDevices* m_dlgDevices;
    dlgEvacuationFlow* m_dlgEvacuationFlow;
    dlgSearchDevice* m_dlgSearchDevice;
  //  dlgDevices *m_dlgDevices;

    QSettings *m_Settings;

    dlgLoginInformation *m_pLoginInformation;
    dlgInformationWindow *m_dlgInforWindow;
    dlgSystemRecord *m_SystemRecord;
    dlgTimeInterval *m_dlgTimeInterval;

    CClientBusiness *m_ClientBusiness;
    CDataProcessing *m_cDataProcessing;//data processing
    CTopToolBar *m_cTopToolBar;
    CBottomToolBar *m_cBottomToolBar;
    CCenterFrame *m_cCenterFrame;
    QStackedWidget *m__cCenterFrameContainer;
    CStatusTabs *m_statusTabs;
    ConfigFileManager *m_cConfigFileManager;
public:
    bool m_ShieldFaultSound;
    bool m_ShieldWarningSound;
    bool m_AutoSwitchToDangerPage;
    QString LastLoginName;
    int m_MonthCheckSpanSecond;
    int m_YearCheckSpanSecond;
    bool m_PrintWarning;
    bool m_PrintFault;
    bool m_PrintOperation;
    int m_TwinkleWhenLaunch;
    int m_SendDirectionAndTwinkleByOne;
    int m_LaunchRule;
    int m_ProcessSwitchln;
    qint16 m_LiftTime;

    bool m_forbidFaultSound;
    bool m_forbidEmergencySound;
    bool m_forbidMonthcheck;
    bool m_forbidYearcheck;
    bool m_energymode;
    QString m_version;
    bool m_TwinkleControl;
    bool m_UCSendDirectionAndTwinkleByOne;
    QString m_DataServerIP;
    int m_DataServerPort;
    QString m_province;
    QString m_city;
    QString m_country;
    QString m_addressDetail;
    QString m_ControllerID;
    int m_tcptype;

    int m_nCanportAddress;         //?????CAN??   CAN1-CAN4(3-6)
    int m_nDistriAddress;          //???????????
    int m_nLoopAddress;            //?????????
    bool m_isSelectDistribution;   //??????????
    bool m_isSelectLoop;           //????????
    bool m_isDelUnregLampFlag;     //?????????????
    int m_FireEmergencyTime;
    int m_noFireEmergencyTime;
    int m_OpenA;
    bool m_PowerMode;
    bool m_lightTestSwitch;
    bool m_actionInSwitch;
    bool m_CANFaultSwitch;

    bool m_isxmlNull;
    bool m_isReNumeDevice;
    QString m_isReNumeProductId;

    int m_recordPage;

    int m_screenWidth;
    int m_screenHeight;

    bool m_isEmergency;
};

#endif // CGLOBAL_H
