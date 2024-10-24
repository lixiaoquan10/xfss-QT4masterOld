#include "cdesignsplashview.h"
#include "cdesigntreeview.h"
#include "cprogramdeviceview.h"
#include "style/cstyledbar.h"
#include "style/cstylehelper.h"
#include "cglobal.h"
#include "dm/cdbsqlite.h"
#include "dm/clayer.h"
#include "dm/ccontroller.h"
#include "dm/ccanport.h"
#include "dm/cdistribution.h"
#include "dm/cloop.h"
#include "dm/cdevice.h"
#include "dm/cfirepoint.h"
#include "dialog/dlgdevices.h"
#include "cmyiconstyle.h"
#include "graphics/deviceControl.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QPixmap>
#include <QPainter>
#include <cmath>
#include <QSqlQuery>
#include <iostream>
#include <QPoint>
#include <QMessageBox>
#include <QMouseEvent>
#include <QMenu>
#include <QSlider>
#include <QtCore>



CDesignSplashView::CDesignSplashView(QWidget *parent) :
    QWidget(parent)
{
    QFont font;
    font.setPointSize(12); // 设置字体大小为12像素
    CGlobal::instance()->setDesignSplashView(this);
    QHBoxLayout* pMainLayout = new QHBoxLayout(this);
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(0);

    QVBoxLayout* pLeftLayout = new QVBoxLayout(this);
    m_label = new QLabel();
    m_label->setFont(font);
    m_label->setText("100%");
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setMaximumHeight(30);
    m_label->setMaximumWidth(45);

    m_enlarge = new QToolButton(this);
    m_enlarge->setText("放大");
    m_enlarge->setFont(font);
    m_enlarge->setMinimumHeight(30);
    m_enlarge->setMaximumWidth(45);
    m_enlarge->setStyleSheet("background-color: rgb(84, 100, 245);border: none;");

    connect(m_enlarge,SIGNAL(clicked(bool)),this,SLOT(slot_enlarge()));
    QHBoxLayout* sliderLayout = new QHBoxLayout(this);
    m_verticalSlider = new QScrollBar();
    m_verticalSlider->setOrientation(Qt::Vertical);
//    m_verticalSlider->setStyleSheet("border:1px solid rgb(255,255,255); margin: 10px; width: 10px;");
    m_verticalSlider->setValue(100);
    m_sliderValue = 100;
    m_verticalSlider->setMinimum(20);
    m_verticalSlider->setMaximum(500);
//    m_verticalSlider->setStyleSheet("border: none;");
    connect(m_verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(slot_spliderValueChanged(int)));
    sliderLayout->addWidget(m_verticalSlider);
    m_reduction = new QToolButton(this);
    m_reduction->setText("缩小");
    m_reduction->setFont(font);
    m_reduction->setMinimumHeight(30);
    m_reduction->setMaximumWidth(45);
    m_reduction->setStyleSheet("background-color: rgb(84, 100, 245);border: none;");
    connect(m_reduction,SIGNAL(clicked(bool)),this,SLOT(slot_reduction()));
    pLeftLayout->addWidget(m_label);
    pLeftLayout->addWidget(m_reduction);
    pLeftLayout->addLayout(sliderLayout);
    pLeftLayout->addWidget(m_enlarge);
    pMainLayout->addLayout(pLeftLayout);
    isHideSlider(true);

    m_pView = new QLabel();
    m_pView->setPixmap(QPixmap("/home/xfss/root/PageBackgroundFolder/main.png"));
    m_pView->setStyleSheet("background-color: rgb(255, 255, 255);");
    m_pView->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_pView->setScaledContents(true);
    m_pView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_scrollArea = new QScrollArea;
    m_scrollArea->setWidgetResizable(true); // 让滚动区域自适应窗口大小
    m_scrollArea->setWidget(m_pView);

    QFont fontfirepoint;
    fontfirepoint.setPointSize(10); // 设置字体大小为10像素
    m_labelAddress = new QLabel(m_scrollArea);
    m_labelAddress->setFont(fontfirepoint);
    // 设置背景颜色
    m_labelAddress->setStyleSheet("background-color: rgb(255,250,240);border: 1px solid black;border-radius: 10px;"); // 设置样式表

    // 连接滚动事件
    connect(m_scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onVerticalScroll(int)));
    connect(m_scrollArea->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onHorizontalScroll(int)));

    pMainLayout->addWidget(m_scrollArea);
    m_isEnblePageStateUpdate = false;

    //定时1s更新布局视图（更新标志使能情况下）
    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_setPrimitiveItem()));
    m_timer->start(1000);

    m_slidertimer = new QTimer();
    connect(m_slidertimer, SIGNAL(timeout()), this, SLOT(slot_setSliderChanged()));

    m_device = NULL;
    m_previousScrollY = 0;
    m_previousScrollX = 0;
    m_menuTable = NULL;
    //布局视图移动上一光标位置默认为（0,0），默认为移动结束状态
    lastX = 0;
    lastY = 0;
    m_moveEnd = true;
    //放大/缩小布局视图，默认100%，未经过放大/缩小操作
    m_isChanged = false;
    m_changeValue = 100;
    m_style = new CMyIconStyle();
    m_strSheet = "QMenu {background-color: rgb(255, 246, 143); margin: 3px; font-size: 14px;}\
            QMenu::item {padding: 3px 15px 3px 15px; border: 1px solid transparent; min-height: 25px; min-width: 30px; border-radius: 4px;}\
            QMenu::separator {height: 2px; background: #CCCCFF; margin-left: 5px; margin-right: 5px;}\
            QMenu::right-arrow { image: url(:/res/right-arrow.png); }\
            QMenu::indicator:exclusive:checked {image: url(:/res/symbol-check.png);}\
            QMenu::item:selected {background-color: qlineargradient(x1: 0.5, y1: 0, x2: 0.5, y2: 1, stop: 0 #99CCFF, stop: 0.5 #CCFFFF, stop: 1 #99CCFF); border-color: #6699CC;}";

    m_strSheetCheck = "QMenu {background-color: rgb(205, 198, 115); margin: 3px; font-size: 14px;}\
            QMenu::item {padding: 3px 10px 3px 30px; border: 1px solid transparent; min-height: 25px; min-width: 30px; border-radius: 4px;}\
            QMenu::separator {height: 2px; background: #CCCCFF; margin-left: 5px; margin-right: 5px;}\
            QMenu::right-arrow { image: url(:/res/right-arrow.png); }\
            QMenu::indicator:exclusive:checked {image: url(:/res/symbol-check.png);}\
            QMenu::item:selected {background-color: qlineargradient(x1: 0.5, y1: 0, x2: 0.5, y2: 1, stop: 0 #99CCFF, stop: 0.5 #CCFFFF, stop: 1 #99CCFF); border-color: #6699CC;}";

}

CDesignSplashView::~CDesignSplashView()
{
    CGlobal::instance()->setDesignSplashView(0);
    delete m_pView;
    delete m_label;
    delete m_enlarge;
    delete m_verticalSlider;
    delete m_reduction;
    delete m_scrollArea;
    delete m_timer;
    delete m_labelAddress;
}
//页面坐标跳转
void CDesignSplashView::ScrollBarChange(int valueX, int valueY)
{
    qDebug() << "CDesignSplashView::ScrollBarChange" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_scrollArea->horizontalScrollBar()->setValue(valueX);
    m_scrollArea->verticalScrollBar()->setValue(valueY);
}

// 滑块滑动
void CDesignSplashView::slot_spliderValueChanged(int value)
{
    qDebug() << "CDesignSplashView::slot_spliderValueChanged" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_sliderValue = value;
    m_slidertimer->start(200);
}

void CDesignSplashView::slot_setSliderChanged()
{
    qDebug() << "CDesignSplashView::slot_setSliderChanged" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_slidertimer->stop();
    QString str = QString::number(m_sliderValue) + "%";
    m_label->setText(str);
    m_previousScrollX = m_previousScrollX*m_sliderValue/m_changeValue;
    m_previousScrollY = m_previousScrollY*m_sliderValue/m_changeValue;
    m_changeValue = m_sliderValue;
    m_isChanged = true;
    m_isEnblePageStateUpdate = true;
    slot_setPrimitiveItem();
    ScrollBarChange(m_previousScrollX, m_previousScrollY);
}

// 放大
void CDesignSplashView::slot_enlarge()
{
    qDebug() << "CDesignSplashView::slot_enlarge" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_verticalSlider->value() + 10 <= m_verticalSlider->maximum())
        m_verticalSlider->setValue(m_verticalSlider->value()+10);
    else
        m_verticalSlider->setValue(m_verticalSlider->maximum());
    slot_spliderValueChanged(m_verticalSlider->value());
}

//缩小
void CDesignSplashView::slot_reduction()
{
    qDebug() << "CDesignSplashView::slot_reduction" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_verticalSlider->value() - 10 >= m_verticalSlider->minimum())
        m_verticalSlider->setValue(m_verticalSlider->value()-10);
    else
        m_verticalSlider->setValue(m_verticalSlider->minimum());
    slot_spliderValueChanged(m_verticalSlider->value());
}

void CDesignSplashView::sliderReset()
{
    qDebug() << "CDesignSplashView::sliderReset" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_verticalSlider->setValue(100);
    slot_spliderValueChanged(m_verticalSlider->value());
}

// 滚动事件处理函数
void CDesignSplashView::onVerticalScroll(int value)
{
    qDebug() << "CDesignSplashView::onVerticalScroll" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_labelAddress->hide();
    m_previousScrollY = value;
}

void CDesignSplashView::onHorizontalScroll(int value)
{
    qDebug() << "CDesignSplashView::onHorizontalScroll" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_labelAddress->hide();
    m_previousScrollX = value;
}

//设置默认图片
void CDesignSplashView::setMainPicture()
{
    qDebug() << "CDesignSplashView::setMainPicture" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_pView->setPixmap(QPixmap("/home/xfss/root/PageBackgroundFolder/main.png"));
    isHideSlider(true);
}
//设置布局视图
void CDesignSplashView::setPixmap(QPixmap background)
{
    qDebug() << "CDesignSplashView::setPixmap" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 获取background图片尺寸
    double bgWidth = background.width();
    double bgHeight = background.height();

    // 获取m_pView尺寸
    double labelWidth = m_scrollArea->width();
    double labelHeight = m_scrollArea->height();

    // 如果background小于label尺寸
    if (bgWidth < labelWidth || bgHeight < labelHeight) {
        // 创建新的QPixmap，大小为label的尺寸
        QPixmap newPixmap(labelWidth, labelHeight);
        newPixmap.fill(Qt::white);  // 用白色填充

        // 创建QPainter，并在新的QPixmap上绘制background图片
        QPainter painter(&newPixmap);
        int x = (labelWidth - bgWidth) / 2;
        int y = (labelHeight - bgHeight) / 2;
        painter.drawPixmap(x, y, background);

        // 将新的QPixmap设置给m_pView
        m_pView->setPixmap(newPixmap);
    } else {
        // 如果background大于等于label尺寸，直接设置给m_pView
        m_pView->setPixmap(background);
    }
}
//
void CDesignSplashView::isHideSlider(bool istrue)
{
    qDebug() << "CDesignSplashView::isHideSlider" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(istrue)
    {
        m_label->hide();
        m_enlarge->hide();
        m_verticalSlider->hide();
        m_reduction->hide();
    }
    else
    {
        m_label->show();
        m_enlarge->show();
        m_verticalSlider->show();
        m_reduction->show();
    }
}

void CDesignSplashView::slot_setPrimitiveItem()
{
    qDebug() << "CDesignSplashView::slot_setPrimitiveItem" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_isEnblePageStateUpdate)
        return;
    if(CGlobal::instance()->m_bInLogin)
        return;
    if(!CGlobal::instance()->designTreeView()->CurrentLayer())
        return;
    int layoutPageID = CGlobal::instance()->designTreeView()->CurrentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    setPrimitiveItem(layoutPageID);
    m_isEnblePageStateUpdate = false;
}

void CDesignSplashView::setPrimitiveItem(int layerID)
{
    qDebug() << "CDesignSplashView::setPrimitiveItem" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!CGlobal::instance()->designTreeView()->CurrentLayer())
        return;
    int currentlayoutPageID = CGlobal::instance()->designTreeView()->CurrentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    // 加载main.png
    QString pictureName = CGlobal::instance()->designTreeView()->currentLayer()->layerValue(LAYER_VALUE_BACKGROUNDPICTURE).toString();
    QString filePath = CGlobal::instance()->workspaces() + "/" + "PageBackgroundFolder/" + pictureName;
    QPixmap background(filePath);

    isHideSlider(false);
    // 获取 m_pView 左上端点相对于该类界面的坐标
//    QPoint viewTopLeft = m_pView->mapTo(this, QPoint(0, 0));
//    int minValue = std::max(100*(m_scrollArea->width()-viewTopLeft.x())/background.width(),
//                                 100*(m_scrollArea->height()-viewTopLeft.y())/background.height());

//    m_verticalSlider->setMinimum(minValue + 3);  //避免缩放过多出现页面拉伸情况
    if(m_isChanged)
    {
        background = background.scaled(background.width()*m_changeValue/100,
                                         background.height()*m_changeValue/100, Qt::KeepAspectRatio);
    }

    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QString strfirepoint = QString("SELECT l.FirePointID,l.LayoutPageID,l.CX,l.CY,l.Width,l.Height,r.ID,r.Device,r.Loop,r.Address from FirePointCoordinate l "
                              "left join FirePoint r on l.FirePointID = r.ID "
                              "where l.LayoutPageID=%1 "
                              ).arg(layerID);

    QSqlQuery queryfirepoint = db.query(strfirepoint);
    QSqlRecord recordfirepoint = queryfirepoint.record();
    int fieldFirePointCX = recordfirepoint.indexOf("CX");
    int fieldFirePointCY = recordfirepoint.indexOf("CY");
    int fieldFirePointHeight = recordfirepoint.indexOf("Height");
    int fieldFirePointWidth = recordfirepoint.indexOf("Width");
    int fieldFirePointDevice = recordfirepoint.indexOf("Device");
    int fieldFirePointLoop = recordfirepoint.indexOf("Loop");
    int fieldFirePointAddress = recordfirepoint.indexOf("Address");
    int fieldFirePointLayoutPageID = recordfirepoint.indexOf("LayoutPageID");
    while(queryfirepoint.next())
    {

        double CX = queryfirepoint.value(fieldFirePointCX).toDouble();
        double CY = queryfirepoint.value(fieldFirePointCY).toDouble();
        double Height = queryfirepoint.value(fieldFirePointHeight).toDouble();
        double Width = queryfirepoint.value(fieldFirePointWidth).toDouble();
        int deviceAddress = queryfirepoint.value(fieldFirePointDevice).toInt();
        int loopAddress = queryfirepoint.value(fieldFirePointLoop).toInt();
        int address = queryfirepoint.value(fieldFirePointAddress).toInt();
        int layoutPageID = queryfirepoint.value(fieldFirePointLayoutPageID).toInt();

        CFirePoint* firepoint = CGlobal::instance()->controller()->firePointByAddress(deviceAddress,loopAddress,address);
        if(!firepoint)
            continue;
        firepoint->setFirePointValue(FIREPOINT_VALUE_CX, CX);
        firepoint->setFirePointValue(FIREPOINT_VALUE_CY, CY);
        firepoint->setFirePointValue(FIREPOINT_VALUE_LAYOUTPAGEID, layoutPageID);
        firepoint->setFirePointValue(FIREPOINT_VALUE_HEIGHT, Height);
        firepoint->setFirePointValue(FIREPOINT_VALUE_WIDTH, Width);
        if(!CGlobal::instance()->m_isHideFirePoint)
        {
            setFirePointPrimitiveItem(&background,firepoint);
        }
    }


    QString strlamp = QString("SELECT l.ID,l.LampInfoID,l.LayoutPageID,l.CX,l.CY,l.Height,l.Angle,m.Number,r.Address,c.Loop,a.CanPort,a.Address from Coordinate l "
                              "left join LampInfo r on l.LampInfoID=r.id "
                              "left join LampType m on r.LampTypeID=m.id "
                              "left join ChannelInfo c on r.ChannelInfoID = c.ID "
                              "left join CanDeviceInfo a on c.CanDeviceInfoID = a.ID "
                              "where l.LayoutPageID=%1 "
                              ).arg(layerID);

    QSqlQuery querylamp = db.query(strlamp);
    QSqlRecord recordlamp = querylamp.record();
    int fieldLayoutPageID = recordlamp.indexOf("LayoutPageID");
    int fieldCX = recordlamp.indexOf("CX");
    int fieldCY = recordlamp.indexOf("CY");
    int fieldHeight = recordlamp.indexOf("Height");
    int fieldAngle = recordlamp.indexOf("Angle");
    int fieldNumber = recordlamp.indexOf("Number");
    while(querylamp.next())
    {
        //8:r.Address灯具地址    9:c.Loop回路地址   10:a.CanPort CAN地址  11:a.Address集中电源地址
        int canAddress = querylamp.value(10).toInt()+2;
        int distributionAddress = querylamp.value(11).toInt();
        int loopAddress = querylamp.value(9).toInt();
        int deviceAddress = querylamp.value(8).toInt();

        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canAddress);
        if(!canport)
            continue;
        CDistribution* distribution = canport->distributionByAddress(distributionAddress);
        if(!distribution)
            continue;
        CLoop* loop = distribution->loopByAdd(loopAddress);
        if(!loop)
            continue;
        CDevice* device = loop->deviceByAdd(deviceAddress);
        if(!device)
            continue;
//        int LampInfoID = query.value(fieldLampInfoID).toInt();
        int LayoutPageID = querylamp.value(fieldLayoutPageID).toInt();
        double CX = querylamp.value(fieldCX).toDouble();
        double CY = querylamp.value(fieldCY).toDouble();
        double Height = querylamp.value(fieldHeight).toDouble();
        double Angle = querylamp.value(fieldAngle).toDouble();
        int Number = querylamp.value(fieldNumber).toInt();
        device->setDeviceValue(DEVICE_VALUE_CX,CX);
        device->setDeviceValue(DEVICE_VALUE_CY,CY);
        device->setDeviceValue(DEVICE_VALUE_HEIGHT,Height);
        device->setDeviceValue(DEVICE_VALUE_ANGEL,Angle);
        device->setDeviceValue(DEVICE_VALUE_LAYOUTPAGEID,LayoutPageID);
        //双向灯
        if(Number == 102 || Number == 106 || Number == 3 || Number == 9 ||
            Number == 17 || Number == 44 || Number == 117 || Number == 144 ||
            Number == 132 || Number == 136 || Number == 32 || Number == 36)
        {
            setArrowPrimitiveItem(&background,device);
        }
        //左向灯
        else if(Number == 101 || Number == 104 || Number == 2 || Number == 7 ||
            Number == 16 || Number == 42 || Number == 116 || Number == 142 ||
            Number == 131 || Number == 134 || Number == 31 || Number == 34)
        {
            setLeftArrowPrimitiveItem(&background,device);
        }
        //右向灯
        else if(Number == 105 || Number == 112 || Number == 8 || Number == 12 ||
            Number == 43 || Number == 47 || Number == 143 || Number == 147 || Number == 135 || Number == 35)
        {
            setRightArrowPrimitiveItem(&background,device);
        }
        //多信息复合标志灯
        else if(Number == 110 || Number == 4 || Number == 18 || Number == 118 ||
            Number == 137 || Number == 138 || Number == 37)
        {
            setLeftFPrimitiveItem(&background,device);
        }
        //楼层灯
        else if(Number == 107 || Number == 108 || Number == 10 || Number == 45 ||
            Number == 145)
        {
            setFloorPrimitiveItem(&background,device);
        }
        //安全出口灯
        else if(Number == 100 || Number == 103 || Number == 111 ||
                Number == 1 || Number == 5 || Number == 6 ||
                Number == 15 || Number == 40 || Number == 41 ||
                Number == 115 || Number == 140 || Number == 141 ||
                Number == 130 || Number == 133 || Number == 30 || Number == 33)
        {
            setExitPrimitiveItem(&background,device);
        }
        //可变状态灯
        else if(Number == 109 || Number == 11 || Number == 46 || Number == 146)
        {
            setChangeExitPrimitiveItem(&background,device);
        }
        //照明灯
        else if((Number >= 50 && Number < 100) || (Number >= 150 && Number < 200))
        {
            setLightPrimitiveItem(&background,device);
        }
        //单向地埋灯
        else if(Number == 120 || Number == 122)
        {
            setOneUndergroundPrimitiveItem(&background,device);
        }
        //双向地埋灯
        else if(Number == 121 || Number == 123)
        {
            setDoubleUndergroundPrimitiveItem(&background,device);
        }

    }
    if(!CGlobal::instance()->m_isHideLinkLine)
    {
        QString strlamptofirepoint = QString("SELECT l.FirePointID,l.LampInfoID,r.Address,c.Loop,a.CanPort,a.Address,m.LayoutPageID from LampToFirePoint l "
                                  "left join LampInfo r on l.LampInfoID=r.id "
                                  "left join ChannelInfo c on r.ChannelInfoID = c.ID "
                                  "left join CanDeviceInfo a on c.CanDeviceInfoID = a.ID "
                                  "left join FirePointCoordinate m on l.FirePointID=m.FirePointID "
                                  "where m.LayoutPageID=%1 "
                                  ).arg(layerID);

        QSqlQuery querylamptofirepoint = db.query(strlamptofirepoint);
        while(querylamptofirepoint.next())
        {
            //2:r.Address灯具地址    3:c.Loop回路地址   4:a.CanPort CAN地址  5:a.Address集中电源地址
            int canAddress = querylamptofirepoint.value(4).toInt()+2;
            int distributionAddress = querylamptofirepoint.value(5).toInt();
            int loopAddress = querylamptofirepoint.value(3).toInt();
            int deviceAddress = querylamptofirepoint.value(2).toInt();

            int firePointID = querylamptofirepoint.value(0).toInt();
            CFirePoint* firepoint = CGlobal::instance()->controller()->firePointByID(firePointID);

            CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canAddress);
            if(!canport)
                continue;
            CDistribution* distribution = canport->distributionByAddress(distributionAddress);
            if(!distribution)
                continue;
            CLoop* loop = distribution->loopByAdd(loopAddress);
            if(!loop)
                continue;
            CDevice* device = loop->deviceByAdd(deviceAddress);
            if(!device || !firepoint)
                continue;
            setLampToFirePoint(&background,firepoint,device);
        }
    }

    QString string = QString("SELECT l.ID,l.StartLampID,l.Joint,l.EndLampID,r1.Address,c1.Loop,a1.CanPort,a1.Address,r2.Address,c2.Loop,a2.CanPort,a2.Address from LampConnection l "
                             "left join LampInfo r1 on l.StartLampID=r1.id "
                             "left join ChannelInfo c1 on r1.ChannelInfoID = c1.ID "
                             "left join CanDeviceInfo a1 on c1.CanDeviceInfoID = a1.ID "
                             "left join LampInfo r2 on l.EndLampID=r2.id "
                             "left join ChannelInfo c2 on r2.ChannelInfoID = c2.ID "
                             "left join CanDeviceInfo a2 on c2.CanDeviceInfoID = a2.ID ");
    QSqlQuery queryConnection = db.query(string);
    CDevice* StartLamp;
    CDevice* EndLamp;
    while(queryConnection.next())
    {
        int canAddress1 = queryConnection.value(6).toInt()+2;
        int canAddress2 = queryConnection.value(10).toInt()+2;
        int distributionAddress1 = queryConnection.value(7).toInt();
        int distributionAddress2 = queryConnection.value(11).toInt();
        int loopAddress1 = queryConnection.value(5).toInt();
        int loopAddress2 = queryConnection.value(9).toInt();
        int deviceAddress1 = queryConnection.value(4).toInt();
        int deviceAddress2 = queryConnection.value(8).toInt();
        int joint = queryConnection.value(2).toInt();
        CCanport* canport1 = CGlobal::instance()->controller()->canportByAddress(canAddress1);
        if(!canport1)
            continue;
        CDistribution* distribution1 = canport1->distributionByAddress(distributionAddress1);
        if(!distribution1)
            continue;
        CLoop* loop1 = distribution1->loopByAdd(loopAddress1);
        if(!loop1)
            continue;
        StartLamp = loop1->deviceByAdd(deviceAddress1);
        CCanport* canport2 = CGlobal::instance()->controller()->canportByAddress(canAddress2);
        if(!canport2)
            continue;
        CDistribution* distribution2 = canport2->distributionByAddress(distributionAddress2);
        if(!distribution2)
            continue;
        CLoop* loop2 = distribution2->loopByAdd(loopAddress2);
        if(!loop2)
            continue;
        EndLamp = loop2->deviceByAdd(deviceAddress2);
        if(!StartLamp || !EndLamp)
            continue;
        if(StartLamp->deviceValue(DEVICE_VALUE_EMERGENCY) == "应急"
                || CGlobal::instance()->m_isEmergency)
        {
            if(StartLamp->deviceValue(DEVICE_VALUE_LAYOUTPAGEID).toInt() == currentlayoutPageID)
            {
                setLampConnectionArrow(&background, StartLamp, EndLamp, joint);
            }
        }
    }

    db.close();
    m_pView->clear();
    setPixmap(background);
}



QPointF CDesignSplashView::rotatePoint(const QPointF& point, const QPointF& center, double angle)
{
    qDebug() << "CDesignSplashView::rotatePoint" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    double radian = angle * M_PI / 180.0;
    double x = center.x() + (point.x() - center.x()) * std::cos(radian) - (point.y() - center.y()) * std::sin(radian);
    double y = center.y() + (point.x() - center.x()) * std::sin(radian) + (point.y() - center.y()) * std::cos(radian);
    return QPointF(x, y);
}

void CDesignSplashView::mouseMoveEvent(QMouseEvent* event)
{
    qDebug() << "CDesignSplashView::mouseMoveEvent" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_labelAddress->hide();
    QWidget::mousePressEvent(event);
    // 获取当前鼠标的位置
    int currentX = event->x();
    int currentY = event->y();
    // 如果在放大缩小滑块区域内则返回
    if(currentX < 35)
        return;
    // 计算移动的距离
    int dx = currentX - lastX;
    int dy = currentY - lastY;

    // 滑动操作结束
    if(m_moveEnd)
    {
        m_previousScrollX = -dx + m_previousScrollX;
        m_previousScrollY = -dy + m_previousScrollY;
        if(m_previousScrollX < 0)
            m_previousScrollX = 0;
        if(m_previousScrollY < 0)
            m_previousScrollY = 0;
        ScrollBarChange(m_previousScrollX, m_previousScrollY);
    }
    // 切换滑动状态（开始或结束）
    m_moveEnd = !m_moveEnd;

    // 更新上一次的事件位置
    lastX = currentX;
    lastY = currentY;
}

void CDesignSplashView::mousePressEvent(QMouseEvent* event)
{
    QWidget::mousePressEvent(event);
    qDebug() << "CDesignSplashView::mousePressEvent" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 点击事件，只能时滑动时间的开始
    m_moveEnd = false;

    m_labelAddress->hide();

    if(CGlobal::instance()->m_UserType == CGlobal::User)
        return;
    if(!CGlobal::instance()->designTreeView()->CurrentLayer())
        return;
    //当前页面
    int pageID = CGlobal::instance()->designTreeView()->currentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    // 获取鼠标点击位置
    QPoint pos = event->pos();

    // 获取 m_pView 左上端点相对于该类界面的坐标
    QPoint viewTopLeft = m_pView->mapTo(this, QPoint(0, 0));

    //实际坐标需要减去左上端点相对于界面的坐标
    double pos_x = pos.x() - viewTopLeft.x();
    double pos_y = pos.y() - viewTopLeft.y();


    //实际点击的坐标
    QPointF point(pos_x,pos_y);

    for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
        if(!canport)
            return;
        QList<CDistribution*> distributions = canport->distributions();
        for(int i=0; i<distributions.count(); i++){
            CDistribution* distribution = distributions.at(i);
            if(distribution){
                QList<CLoop*> loops = distribution->loops();
                for(int i=0; i<loops.count(); i++){
                    CLoop* loop = loops.at(i);
                    if(loop){
                        QList<CDevice*> devices = loop->devices();
                        for(int i=0; i<devices.count(); i++){
                            CDevice* device = devices.at(i);
                            if(device)
                            {
                                //device_x1  device_x2  device_y1  device_y2为4个边界点坐标的x y值，pointA-pointD为4个边界点坐标点
                                double angel = device->deviceValue(DEVICE_VALUE_ANGEL).toDouble();
                                double scale = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble()/40;
                                double device_x1 = device->deviceValue(DEVICE_VALUE_CX).toDouble();
                                double device_y1 = device->deviceValue(DEVICE_VALUE_CY).toDouble();
                                double device_x2 = scale*40*1.8 + device_x1;     //默认标志灯
                                double device_y2 = scale*40 + device_y1;

                                int type = device->deviceTypeId();
                                if((type >= 50 && type < 100) || (type >= 150 && type < 200))  //若为照明灯
                                    device_x2 = scale*40*4/3 + device_x1;
                                else if(type >= 120 && type <= 124)                            //若为地埋灯
                                    device_x2 = scale*40 + device_x1;
                                device_x1 = device_x1 * m_changeValue/100;
                                device_x2 = device_x2 * m_changeValue/100;
                                device_y1 = device_y1 * m_changeValue/100;
                                device_y2 = device_y2 * m_changeValue/100;
                                QPointF pointA(device_x1, device_y1);
                                QPointF pointB(device_x2, device_y1);
                                QPointF pointC(device_x2, device_y2);
                                QPointF pointD(device_x1, device_y2);
                                QPointF center((device_x1+device_x2)/2, (device_y1+device_y2)/2);
                                //以灯具中心点旋转4个点坐标
                                pointA = rotatePoint(pointA, center, angel);
                                pointB = rotatePoint(pointB, center, angel);
                                pointC = rotatePoint(pointC, center, angel);
                                pointD = rotatePoint(pointD, center, angel);

                                QPolygonF polygon;
                                polygon << pointA << pointB << pointC << pointD;


                                if(device->deviceValue(DEVICE_VALUE_LAYOUTPAGEID).toInt() != pageID)
                                    continue;
                                //点击的位置在灯具区域内
                                if(polygon.containsPoint(point, Qt::OddEvenFill))
                                {
                                    //建立控制列表
                                    if(m_menuTable && !m_menuTable->isHidden())
                                    {
                                        delete m_menuTable;
                                        m_menuTable = NULL;
                                    }
                                    else
                                    {
                                        if(m_menuTable) delete m_menuTable;
                                        m_menuTable = createTableMenu(device);
                                        if(m_menuTable != NULL)//显示在鼠标点击点的上方
                                        {
                                            QCursor cursor;
                                            QPoint pos;
                                            pos.setX(cursor.pos().x());
                                            pos.setY(cursor.pos().y());
                                            pos = m_menuTable->mapToGlobal(pos);
                                            m_menuTable->popup(pos);
                                        }
                                    }
                                    devices.clear();
                                    loops.clear();
                                    distributions.clear();
                                    return;
                                }
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
    QList<CFirePoint*> firepoints = CGlobal::instance()->controller()->firepoints();
    for(int i=0; i<firepoints.count(); i++)
    {
        CFirePoint* firepoint = firepoints.at(i);
        if(firepoint)
        {
            //firepoint_x1  firepoint_x2  firepoint_y1  firepoint_y2为4个边界点坐标的x y值，pointA-pointD为4个边界点坐标点
            double scale = firepoint->firePointValue(FIREPOINT_VALUE_HEIGHT).toDouble()/40;
            double firepoint_x1 = firepoint->firePointValue(FIREPOINT_VALUE_CX).toDouble();
            double firepoint_y1 = firepoint->firePointValue(FIREPOINT_VALUE_CY).toDouble();
            double firepoint_x2 = scale*40 + firepoint_x1;
            double firepoint_y2 = scale*40 + firepoint_y1;
            firepoint_x1 = firepoint_x1 * m_changeValue/100;
            firepoint_x2 = firepoint_x2 * m_changeValue/100;
            firepoint_y1 = firepoint_y1 * m_changeValue/100;
            firepoint_y2 = firepoint_y2 * m_changeValue/100;
            QPointF pointA(firepoint_x1, firepoint_y1);
            QPointF pointB(firepoint_x2, firepoint_y1);
            QPointF pointC(firepoint_x2, firepoint_y2);
            QPointF pointD(firepoint_x1, firepoint_y2);

            QPolygonF polygon;
            polygon << pointA << pointB << pointC << pointD;

            if(firepoint->firePointValue(FIREPOINT_VALUE_LAYOUTPAGEID).toInt() != pageID)
                continue;
            int DeviceAddress = firepoint->firePointValue(FIREPOINT_VALUE_DEVICEADDRESS).toInt();
            int loopAddress = firepoint->firePointValue(FIREPOINT_VALUE_LOOPADDRESS).toInt();
            int address = firepoint->firePointValue(FIREPOINT_VALUE_ADDRESS).toInt();
            QString str = QString("%1%2%3").arg(DeviceAddress, 3, 10, QChar('0'))
                                                .arg(loopAddress, 3, 10, QChar('0'))
                                                .arg(address, 3, 10, QChar('0'));
            //点击的位置在火警点区域内
            if(polygon.containsPoint(point, Qt::OddEvenFill))
            {
                // 在需要弹出信息的地方
                m_labelAddress->setGeometry(pos.x(), pos.y(), 90, 30); // 设置信息显示位置和大小
                m_labelAddress->setText(str);
                m_labelAddress->show();
            }
        }
    }
}

QMenu* CDesignSplashView::createTableMenu(CDevice* device)
{
    qDebug() << "CDesignSplashView::createTableMenu" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_device = device;
    QMenu *tableMenu = new QMenu(this);
    tableMenu->setStyleSheet(m_strSheet);
    tableMenu->setStyle(m_style);
    int typeID = device->deviceTypeId();
    if((typeID >= 50 && typeID < 100) || (typeID >= 150 && typeID < 200))
    {
    }
    else
    {
        // 创建控制子菜单
        QMenu* directionMenu = new QMenu("控制", this);
        directionMenu->setStyleSheet(m_strSheetCheck);
        directionMenu->setStyle(m_style);
        directionMenu->setIcon(QIcon(""));

    //    if((typeID >= 50 && typeID < 100) || (typeID >= 150 && typeID < 200))
    //    {
    //        directionMenu->addAction("点亮", this, SLOT(slot_Lighten()));
    //        directionMenu->addSeparator();
    //        directionMenu->addAction("熄灭", this, SLOT(slot_CloseLight()));
    //        tableMenu->addMenu(directionMenu);
    //        tableMenu->addSeparator();
    //    }
        if(typeID == 101 || typeID == 104 || typeID == 2 || typeID == 7 ||
                typeID == 16 || typeID == 42 || typeID == 116 || typeID == 142)
        {
            directionMenu->addAction("全灭", this, SLOT(slot_AllClose()));
            directionMenu->addSeparator();
            directionMenu->addAction("左向", this, SLOT(slot_Left()));
            directionMenu->addSeparator();
            directionMenu->addAction("全亮", this, SLOT(slot_StopTest()));
            tableMenu->addMenu(directionMenu);
            tableMenu->addSeparator();
        }
        else if(typeID == 105 || typeID == 8 || typeID == 43 || typeID == 143)
        {
            directionMenu->addAction("全灭", this, SLOT(slot_AllClose()));
            directionMenu->addSeparator();
            directionMenu->addAction("右向", this, SLOT(slot_Right()));
            directionMenu->addSeparator();
            directionMenu->addAction("全亮", this, SLOT(slot_StopTest()));
            tableMenu->addMenu(directionMenu);
            tableMenu->addSeparator();
        }
        else if(typeID == 102 || typeID == 106 || typeID == 3 || typeID == 9 ||
                typeID == 17 || typeID == 44 || typeID == 117 || typeID == 144 ||
                typeID == 121 || typeID == 123)
        {
            directionMenu->addAction("左向", this, SLOT(slot_Left()));
            directionMenu->addSeparator();
            directionMenu->addAction("右向", this, SLOT(slot_Right()));
            directionMenu->addSeparator();
            directionMenu->addAction("全灭", this, SLOT(slot_AllClose()));
            directionMenu->addSeparator();
            directionMenu->addAction("全亮", this, SLOT(slot_StopTest()));
            tableMenu->addMenu(directionMenu);
            tableMenu->addSeparator();
        }
    }

    if(typeID == 121 || typeID == 123)
    {
        // 创建默认指向子菜单
        QMenu* defaultDirectionMenu = new QMenu("默认指向", this);
        defaultDirectionMenu->setStyleSheet(m_strSheetCheck);
        defaultDirectionMenu->setStyle(m_style);
        QActionGroup *actionGroup1 = new QActionGroup(this);
        QAction *m_left = defaultDirectionMenu->addAction("左向");
        connect(m_left, SIGNAL(triggered()), this, SLOT(slot_DefaultLeft()));
        QAction *m_right = defaultDirectionMenu->addAction("右向");
        connect(m_right, SIGNAL(triggered()), this, SLOT(slot_DefaultRight()));
        QAction *m_all = defaultDirectionMenu->addAction("全亮");
        connect(m_all, SIGNAL(triggered()), this, SLOT(slot_DefaultAll()));
        QAction *m_off = defaultDirectionMenu->addAction("全灭");
        connect(m_off, SIGNAL(triggered()), this, SLOT(slot_DefaultOff()));
        actionGroup1->addAction(m_left);
        actionGroup1->addAction(m_right);
        actionGroup1->addAction(m_all);
        actionGroup1->addAction(m_off);
        if(m_device->deviceValue(DEVICE_VALUE_DEFAULTDIRECTION) == 2)
        {
            m_right->setIcon(QIcon(":/images/ok.png"));
        }
        else if(m_device->deviceValue(DEVICE_VALUE_DEFAULTDIRECTION) == 1)
        {
            m_left->setIcon(QIcon(":/images/ok.png"));
        }
        else if(m_device->deviceValue(DEVICE_VALUE_DEFAULTDIRECTION) == 0)
        {
            m_all->setIcon(QIcon(":/images/ok.png"));
        }
        else if(m_device->deviceValue(DEVICE_VALUE_DEFAULTDIRECTION) == 3)
        {
            m_off->setIcon(QIcon(":/images/ok.png"));
        }
        tableMenu->addMenu(defaultDirectionMenu);
        tableMenu->addSeparator();

        // 创建地埋灯反接子菜单
        QMenu* isReverseMenu = new QMenu("反接", this);
        isReverseMenu->setStyleSheet(m_strSheetCheck);
        isReverseMenu->setStyle(m_style);

        QActionGroup *actionGroup2 = new QActionGroup(this);
        QAction *m_yes = isReverseMenu->addAction("是");
        connect(m_yes, SIGNAL(triggered()), this, SLOT(slot_Reverse()));
        QAction *m_no = isReverseMenu->addAction("否");
        connect(m_no, SIGNAL(triggered()), this, SLOT(slot_NoReverse()));
        actionGroup2->addAction(m_yes);
        actionGroup2->addAction(m_no);
        if(device->deviceValue(DEVICE_VALUE_ISREVERSE).toInt() == 1)
        {
            m_yes->setIcon(QIcon(":/images/ok.png"));
        }
        else if(device->deviceValue(DEVICE_VALUE_ISREVERSE).toInt() == 0)
        {
            m_no->setIcon(QIcon(":/images/ok.png"));
        }

        tableMenu->addMenu(isReverseMenu);
        tableMenu->addSeparator();
    }

    tableMenu->addAction("开始测试", this, SLOT(slot_StartTest()));
    tableMenu->addAction("停止测试", this, SLOT(slot_StopTest()));
    tableMenu->addSeparator();
    tableMenu->addAction("开始应急", this, SLOT(slot_StartEmergency()));
    tableMenu->addAction("停止应急", this, SLOT(slot_StopEmergency()));
    tableMenu->addSeparator();
    if(device->deviceTypeId() < 100 && device->deviceTypeId() > 0)
    {
        QMenu* batteryMenu = new QMenu("电池输出设置", this);
        batteryMenu->setStyleSheet(m_strSheet);
        batteryMenu->setStyle(m_style);
        batteryMenu->setIcon(QIcon(""));
        batteryMenu->addAction("关闭电池输出", this, SLOT(slot_CloseBatteryOut()));
        batteryMenu->addSeparator();
        batteryMenu->addAction("电池输出保持", this, SLOT(slot_BatteryOutKeep()));
        tableMenu->addMenu(batteryMenu);
        tableMenu->addSeparator();
    }

    tableMenu->addAction("灯具信息", this, SLOT(slot_DeviceInfo()));
    tableMenu->addSeparator();

    return tableMenu;
}
//火警点
void CDesignSplashView::setFirePointPrimitiveItem(QPixmap* background,CFirePoint* firepoint)
{
    qDebug() << "CDesignSplashView::setFirePointPrimitiveItem" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //坐标、高度、宽度需要乘上放大倍数
    double x = firepoint->firePointValue(FIREPOINT_VALUE_CX).toDouble()*m_changeValue/100;
    double y = firepoint->firePointValue(FIREPOINT_VALUE_CY).toDouble()*m_changeValue/100;
//    double width = firepoint->firePointValue(FIREPOINT_VALUE_WIDTH).toDouble()*m_changeValue/100;
    double height = firepoint->firePointValue(FIREPOINT_VALUE_HEIGHT).toDouble()*m_changeValue/100;

    // 保存当前坐标系统
    painter.save();

    // 平移到圆形中心点位置
    painter.translate(x + 20*height/40, y + 20*height/40);

    // 缩放坐标系统
    painter.scale(height/40, height/40);

    // 绘制圆形
    QRect rect(-20, -20, 40, 40);  // 圆形位置和大小

    if(firepoint->isSimulateEmergency() || firepoint->isEmergency())
    {
        //设置填充颜色为Red
        painter.setBrush(QBrush(QColor(255,0,0)));
    }
    else
    {
        //设置填充颜色为Yellow1
        painter.setBrush(QBrush(QColor(255,255,0)));
    }

    QPen pen(Qt::black); // 创建一个黑色画笔对象
    pen.setWidth(2); // 设置边框宽度为2个像素

    painter.setPen(pen);  // 设置边框颜色为黑色
    painter.drawEllipse(rect);

    // 绘制圆形
    QRect rect1(-10, -10, 20, 20);  // 圆形位置和大小

    if(firepoint->isSimulateEmergency() || firepoint->isEmergency())
    {
        //设置填充颜色为Red
        painter.setBrush(QBrush(QColor(255,0,0)));
    }
    else
    {
        //设置填充颜色为Yellow1
        painter.setBrush(QBrush(QColor(255,255,0)));
    }

    painter.setPen(pen);  // 设置边框颜色为黑色
    painter.drawEllipse(rect1);

    // 恢复保存的坐标系统
    painter.restore();
}

//火警点与灯具关联线
void CDesignSplashView::setLampToFirePoint(QPixmap* background, CFirePoint* firepoint, CDevice* lamp)
{
    qDebug() << "CDesignSplashView::setLampToFirePoint" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    //坐标、高度、宽度需要乘上放大倍数
    double firepointX = firepoint->firePointValue(FIREPOINT_VALUE_CX).toDouble()*m_changeValue/100;
    double firepointY = firepoint->firePointValue(FIREPOINT_VALUE_CY).toDouble()*m_changeValue/100;
//    double firepointWidth = firepoint->firePointValue(FIREPOINT_VALUE_WIDTH).toDouble()*m_changeValue/100;
    double firepointHeight = firepoint->firePointValue(FIREPOINT_VALUE_HEIGHT).toDouble()*m_changeValue/100;
    double lampX = lamp->deviceValue(DEVICE_VALUE_CX).toDouble()*m_changeValue/100;
    double lampY = lamp->deviceValue(DEVICE_VALUE_CY).toDouble()*m_changeValue/100;
    double lampHeight = lamp->deviceValue(DEVICE_VALUE_HEIGHT).toDouble()*m_changeValue/100;
    double lampAngle = lamp->deviceValue(DEVICE_VALUE_ANGEL).toDouble();

    //灯具本身的缩放比例
    double multiple = lampHeight/40.0;

    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // 保存当前坐标系统
    painter.save();
    QPointF lampPoint[8];
    QPointF firepointCenter;
    QPointF lampCenter;
    // 绘制关联线
    firepointCenter = QPointF(firepointX + 20*firepointHeight/40, firepointY + 20*firepointHeight/40);
    //地埋灯取圆心作为比较点
    if(lamp->deviceTypeId() >= 120 && lamp->deviceTypeId() <= 123)
    {
        lampCenter = QPointF(lampX + 20*lampHeight/40, lampY + 20*lampHeight/40);
    }
    else
    {
        // 计算矩形中心点的坐标
        lampCenter = QPointF(lampX + 36*lampHeight/40, lampY + 20*lampHeight/40);
        // 标志灯取8个点作为比较点
        QPointF lamp[8] = {
            QPointF(lampX, lampY),
            QPointF(lampX + 36*lampHeight/40, lampY),
            QPointF(lampX + 72*lampHeight/40, lampY),
            QPointF(lampX + 72*lampHeight/40, lampY + 20*lampHeight/40),
            QPointF(lampX + 72*lampHeight/40, lampY + 40*lampHeight/40),
            QPointF(lampX + 36*lampHeight/40, lampY + 40*lampHeight/40),
            QPointF(lampX, lampY + 40*lampHeight/40),
            QPointF(lampX, lampY + 20*lampHeight/40)
        };
        for (int i = 0; i < 8; ++i) {
            lampPoint[i] = rotatePoint(lamp[i], lampCenter, lampAngle);
        }
    }
    //中心点连接线
    QLineF line(firepointCenter, lampCenter);
    //中心点连接线与灯具图形轮廓的交界点
    QPointF intersection;
    //关联线的起点和终点
    QPointF p1, p2;
    //偏移点
    QPointF lampOffset(0,0);
    //两个点距离
    double minDistance;
    //火警点取圆心作为关联线的起点
    p1 = firepointCenter;
    if(lamp->deviceTypeId() >= 120 && lamp->deviceTypeId() <= 123)
    {
        //地埋灯暂时取圆心作为关联线的终点
        p2 = lampCenter;
    }
    else
    {
        //标志灯计算8个比较点和(中心点连接线与标志灯轮廓的交点)的最短距离,得出关联线的终点
        QRectF lampRect = QRectF(lampX, lampY, 72*lampHeight/40, 40*lampHeight/40);

        QLineF lamptopEdge(lampRect.topLeft(), lampRect.topRight());
        QLineF lampbottomEdge(lampRect.bottomLeft(), lampRect.bottomRight());
        QLineF lampleftEdge(lampRect.topLeft(), lampRect.bottomLeft());
        QLineF lamprightEdge(lampRect.topRight(), lampRect.bottomRight());

        // 计算偏移点
        if (line.intersect(lamptopEdge, &intersection) == QLineF::BoundedIntersection)
        {
            lampOffset = QPointF(-5*m_changeValue/100.0,0);
        }
        else if(line.intersect(lampbottomEdge, &intersection) == QLineF::BoundedIntersection)
        {
            lampOffset = QPointF(5*m_changeValue/100.0,0);
        }
        else if(line.intersect(lampleftEdge, &intersection) == QLineF::BoundedIntersection)
        {
            lampOffset = QPointF(0,5*m_changeValue/100.0);
        }
        else if(line.intersect(lamprightEdge, &intersection) == QLineF::BoundedIntersection)
        {
            lampOffset = QPointF(0,-5*m_changeValue/100.0);
        }
        // 计算交点
        minDistance = std::max(72*firepointHeight/40,72*lampHeight/40);
        for (int i = 0; i < 8; ++i) {
            double deltaX = lampPoint[i].x() - intersection.x();
            double deltaY = lampPoint[i].y() - intersection.y();
            double distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);
            if (distance < minDistance) {
                minDistance = distance;
                p2 = lampPoint[i];
            }
        }
    }

    //更新关联线长度
    minDistance = std::sqrt((p1.x()-p2.x()) * (p1.x()-p2.x()) + (p1.y()-p2.y()) * (p1.y()-p2.y()));

    //关联线的起点取线段和圆周的交点,同时起点增加偏移量
    p1.setX(p1.x()-((p1.x()-p2.x())*20.0/minDistance)*firepointHeight/40);
    p1.setY(p1.y()-((p1.y()-p2.y())*20.0/minDistance)*firepointHeight/40);
    //若为地埋灯,关联线的终点取线段和圆周的交点,同时终点增加偏移量
    if(lamp->deviceTypeId() >= 120 && lamp->deviceTypeId() <= 123)
    {
        p2.setX(p2.x()-((p2.x()-p1.x())*20.0/minDistance)*lampHeight/40);
        p2.setY(p2.y()-((p2.y()-p1.y())*20.0/minDistance)*lampHeight/40);
    }
    //更新关联线长度
    minDistance = std::sqrt((p1.x()-p2.x()) * (p1.x()-p2.x()) + (p1.y()-p2.y()) * (p1.y()-p2.y()));
    double pointRotationAngle;
    pointRotationAngle = std::atan2(p2.y() - p1.y(), p2.x() - p1.x()) * 180 / M_PI;

    if(lampOffset.x() == 0 && lampOffset.y() == 0)
    {
        //灯具为地埋灯时的偏移量
        double xValue = std::sin(pointRotationAngle * M_PI / 180.0)*5*m_changeValue/100.0;
        double yValue = std::cos(pointRotationAngle * M_PI / 180.0)*5*m_changeValue/100.0;
        p1 = QPointF(p1.x()-xValue,p1.y()+yValue);
        p2 = QPointF(p2.x()-xValue,p2.y()+yValue);
    }
    else
    {
        p1 = QPointF(p1.x()+lampOffset.x(),p1.y()+lampOffset.y());
        p2 = QPointF(p2.x()+lampOffset.x(),p2.y()+lampOffset.y());
    }
    //线段为虚线，单个虚实线的长度，实20虚10
    double singleDistance = 30*multiple*m_changeValue/100.0;
    //线段为虚线，单个实线的长度
    double solidDistance = 20*multiple*m_changeValue/100.0;
    //单个虚实线的个数
    int number = minDistance/singleDistance;
    //绘制出number个虚实线
    for(int i=0; i<number; i++)
    {
        QPointF arrowline[5];
        double line_compare = 2.5*multiple*m_changeValue/100.0;
        QPointF arrowlinePoints[5] = {
            QPointF(p1.x() + line_compare + i*singleDistance, p1.y() + line_compare),
            QPointF(p1.x() + line_compare + i*singleDistance + solidDistance, p1.y() + line_compare),
            QPointF(p1.x() + line_compare + i*singleDistance + solidDistance, p1.y() - line_compare),
            QPointF(p1.x() + line_compare + i*singleDistance, p1.y() - line_compare),
            QPointF(p1.x() + line_compare + i*singleDistance, p1.y() + line_compare)
        };
        for (int i = 0; i < 5; ++i) {
            arrowline[i] = rotatePoint(arrowlinePoints[i], p1, pointRotationAngle);
        }

        painter.setBrush(QBrush(Qt::blue));  // 设置填充颜色为Blue
        //应急状态为红色，常规蓝色
        if(firepoint->isSimulateEmergency() || firepoint->isEmergency() || CGlobal::instance()->m_isEmergency)
            painter.setBrush(QBrush(Qt::red));  // 设置填充颜色为red
        else
            painter.setBrush(QBrush(Qt::blue));  // 设置填充颜色为Blue
        painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
        painter.drawPolygon(arrowline, 5);
    }
    //已经绘制完的虚实线
    double drawDistance = singleDistance*number;
    //未绘制完的虚实线
    double lastDistance = minDistance - drawDistance;
    //
    if(lastDistance > 0)
    {
        //未绘制完的虚实线小于单个实线的长度
        if(lastDistance <= solidDistance)
        {
            solidDistance = lastDistance;
        }
        QPointF arrowline[5];
        double line_compare = 2.5*multiple*m_changeValue/100.0;
        QPointF arrowlinePoints[5] = {
            QPointF(p1.x() + line_compare + drawDistance, p1.y() + line_compare),
            QPointF(p1.x() - line_compare + drawDistance + solidDistance, p1.y() + line_compare),
            QPointF(p1.x() - line_compare + drawDistance + solidDistance, p1.y() - line_compare),
            QPointF(p1.x() + line_compare + drawDistance, p1.y() - line_compare),
            QPointF(p1.x() + line_compare + drawDistance, p1.y() + line_compare)
        };
        for (int i = 0; i < 5; ++i) {
            arrowline[i] = rotatePoint(arrowlinePoints[i], p1, pointRotationAngle);
        }
        //应急状态为红色，常规蓝色
        if(firepoint->isSimulateEmergency() || firepoint->isEmergency())
            painter.setBrush(QBrush(Qt::red));  // 设置填充颜色为red
        else
            painter.setBrush(QBrush(Qt::blue));  // 设置填充颜色为Blue
        painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
        painter.drawPolygon(arrowline, 5);
    }

    // 恢复保存的坐标系统
    painter.restore();
}

//灯具连接线
void CDesignSplashView::setLampConnectionArrow(QPixmap* background, CDevice* startLamp, CDevice* endLamp, int joint)
{
    qDebug() << "CDesignSplashView::setLampConnectionArrow" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int Number = startLamp->deviceTypeId();
    //双向灯
    if(Number == 102 || Number == 106 || Number == 3 || Number == 9 ||
        Number == 17 || Number == 44 || Number == 117 || Number == 144 ||
        Number == 132 || Number == 136 || Number == 32 || Number == 36)
    {

    }
    //双向地埋灯
    else if(Number == 121 || Number == 123)
    {

    }
    //开始的灯具为左向灯，并且指向为左
    else if((Number == 101 || Number == 104 || Number == 2 || Number == 7 ||
        Number == 16 || Number == 42 || Number == 116 || Number == 142 ||
        Number == 131 || Number == 134 || Number == 31 || Number == 34) && (joint == 1))
    {

    }
    //开始的灯具为单向地埋灯，并且指向为左
    else if((Number == 120 || Number == 122) && (joint == 1))
    {

    }
    //开始的灯具为右向灯，并且指向为右
    else if((Number == 105 || Number == 112 || Number == 8 || Number == 12 ||
        Number == 43 || Number == 47 || Number == 143 || Number == 147 ||
             Number == 135 || Number == 35)  && (joint == 2))
    {

    }
    //非带箭头灯具或者单向灯具指向不对应直接返回
    else
    {
        return;
    }
    //坐标、高度、宽度需要乘上放大倍数
    double startX = startLamp->deviceValue(DEVICE_VALUE_CX).toDouble()*m_changeValue/100;
    double startY = startLamp->deviceValue(DEVICE_VALUE_CY).toDouble()*m_changeValue/100;
    double endX = endLamp->deviceValue(DEVICE_VALUE_CX).toDouble()*m_changeValue/100;
    double endY = endLamp->deviceValue(DEVICE_VALUE_CY).toDouble()*m_changeValue/100;
    double startHeight = startLamp->deviceValue(DEVICE_VALUE_HEIGHT).toDouble()*m_changeValue/100;
    double endHeight = endLamp->deviceValue(DEVICE_VALUE_HEIGHT).toDouble()*m_changeValue/100;
    double startAngle = startLamp->deviceValue(DEVICE_VALUE_ANGEL).toDouble();
    double endAngle = endLamp->deviceValue(DEVICE_VALUE_ANGEL).toDouble();

    //灯具本身的缩放比例
    double multiple = startHeight/40.0;
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // 保存当前坐标系统
    painter.save();
    QPointF startPoint[8];
    QPointF endPoint[8];
    QPointF startCenter;
    QPointF endCenter;
    // 绘制箭头
    // 地埋灯取圆心作为比较点
    if(startLamp->deviceTypeId() >= 120 && startLamp->deviceTypeId() <= 123)
    {
        startCenter = QPointF(startX + 20*startHeight/40, startY + 20*startHeight/40);
    }
    else
    {
        // 计算矩形中心点的坐标
        startCenter = QPointF(startX + 36*startHeight/40, startY + 20*startHeight/40);
        // 标志灯取8个点作为比较点
        QPointF start[8] = {
            QPointF(startX, startY),
            QPointF(startX + 36*startHeight/40, startY),
            QPointF(startX + 72*startHeight/40, startY),
            QPointF(startX + 72*startHeight/40, startY + 20*startHeight/40),
            QPointF(startX + 72*startHeight/40, startY + 40*startHeight/40),
            QPointF(startX + 36*startHeight/40, startY + 40*startHeight/40),
            QPointF(startX, startY + 40*startHeight/40),
            QPointF(startX, startY + 20*startHeight/40)
        };
        for (int i = 0; i < 8; ++i) {
            startPoint[i] = rotatePoint(start[i], startCenter, startAngle);
        }
    }
    //地埋灯取圆心作为比较点
    if(endLamp->deviceTypeId() >= 120 && endLamp->deviceTypeId() <= 123)
    {
        endCenter = QPointF(endX + 20*endHeight/40, endY + 20*endHeight/40);
    }
    else
    {
        // 计算矩形中心点的坐标
        endCenter = QPointF(endX + 36*endHeight/40, endY + 20*endHeight/40);
        // 标志灯取8个点作为比较点
        QPointF end[8] = {
            QPointF(endX, endY),
            QPointF(endX + 36*endHeight/40, endY),
            QPointF(endX + 72*endHeight/40, endY),
            QPointF(endX + 72*endHeight/40, endY + 20*endHeight/40),
            QPointF(endX + 72*endHeight/40, endY + 40*endHeight/40),
            QPointF(endX + 36*endHeight/40, endY + 40*endHeight/40),
            QPointF(endX, endY + 40*endHeight/40),
            QPointF(endX, endY + 20*endHeight/40)
        };
        for (int i = 0; i < 8; ++i) {
            endPoint[i] = rotatePoint(end[i], endCenter, endAngle);
        }
    }
    //中心点连接线
    QLineF line(startCenter, endCenter);
    //中心点连接线与两个灯具图形轮廓的交界点
    QPointF intersection1, intersection2;
    //疏散路径的起点和终点
    QPointF p1, p2;
    //偏移点
    QPointF startOffset(0,0);
    QPointF endOffset(0,0);
    //两个点距离
    double minDistance;
    if(startLamp->deviceTypeId() >= 120 && startLamp->deviceTypeId() <= 123)
    {
        //地埋灯暂时取圆心作为疏散路径的起点
        p1 = startCenter;
    }
    else
    {
        //标志灯计算8个比较点和(中心点连接线与标志灯轮廓的交点)的最短距离,得出疏散路径的起点
        QRectF startRect = QRectF(startX, startY, 72*startHeight/40, 40*startHeight/40);
        QLineF starttopEdge(startRect.topLeft(), startRect.topRight());
        QLineF startbottomEdge(startRect.bottomLeft(), startRect.bottomRight());
        QLineF startleftEdge(startRect.topLeft(), startRect.bottomLeft());
        QLineF startrightEdge(startRect.topRight(), startRect.bottomRight());
        // 计算偏移点
        if (line.intersect(starttopEdge, &intersection1) == QLineF::BoundedIntersection)
        {
            startOffset = QPointF(5*m_changeValue/100.0,0);
        }
        else if(line.intersect(startbottomEdge, &intersection1) == QLineF::BoundedIntersection)
        {
            startOffset = QPointF(-5*m_changeValue/100.0,0);
        }
        else if(line.intersect(startleftEdge, &intersection1) == QLineF::BoundedIntersection)
        {
            startOffset = QPointF(0,-5*m_changeValue/100.0);
        }
        else if(line.intersect(startrightEdge, &intersection1) == QLineF::BoundedIntersection)
        {
            startOffset = QPointF(0,5*m_changeValue/100.0);
        }
        // 计算交点
        minDistance = std::max(72*startHeight/40,72*endHeight/40);
        for (int i = 0; i < 8; ++i) {
            double deltaX = startPoint[i].x() - intersection1.x();
            double deltaY = startPoint[i].y() - intersection1.y();
            double distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);
            if (distance < minDistance) {
                minDistance = distance;
                p1 = startPoint[i];
            }
        }
    }
    if(endLamp->deviceTypeId() >= 120 && endLamp->deviceTypeId() <= 123)
    {
        //地埋灯暂时取圆心作为疏散路径的终点
        p2 = endCenter;
    }
    else
    {
        //标志灯计算8个比较点和(中心点连接线与标志灯轮廓的交点)的最短距离,得出疏散路径的终点
        QRectF endRect = QRectF(endX, endY, 72*endHeight/40, 40*endHeight/40);

        QLineF endtopEdge(endRect.topLeft(), endRect.topRight());
        QLineF endbottomEdge(endRect.bottomLeft(), endRect.bottomRight());
        QLineF endleftEdge(endRect.topLeft(), endRect.bottomLeft());
        QLineF endrightEdge(endRect.topRight(), endRect.bottomRight());

        // 计算偏移点
        if (line.intersect(endtopEdge, &intersection2) == QLineF::BoundedIntersection)
        {
            endOffset = QPointF(-5*m_changeValue/100.0,0);
        }
        else if(line.intersect(endbottomEdge, &intersection2) == QLineF::BoundedIntersection)
        {
            endOffset = QPointF(5*m_changeValue/100.0,0);
        }
        else if(line.intersect(endleftEdge, &intersection2) == QLineF::BoundedIntersection)
        {
            endOffset = QPointF(0,5*m_changeValue/100.0);
        }
        else if(line.intersect(endrightEdge, &intersection2) == QLineF::BoundedIntersection)
        {
            endOffset = QPointF(0,-5*m_changeValue/100.0);
        }
        // 计算交点
        minDistance = std::max(72*startHeight/40,72*endHeight/40);
        for (int i = 0; i < 8; ++i) {
            double deltaX = endPoint[i].x() - intersection2.x();
            double deltaY = endPoint[i].y() - intersection2.y();
            double distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);
            if (distance < minDistance) {
                minDistance = distance;
                p2 = endPoint[i];
            }
        }
    }

    //更新箭头长度
    minDistance = std::sqrt((p1.x()-p2.x()) * (p1.x()-p2.x()) + (p1.y()-p2.y()) * (p1.y()-p2.y()));

    //若为地埋灯,疏散路径的起点和终点取线段和圆周的交点,同时起点终点都增加偏移量
    if(startLamp->deviceTypeId() >= 120 && startLamp->deviceTypeId() <= 123)
    {
        p1.setX(p1.x()-((p1.x()-p2.x())*20.0/minDistance)*startHeight/40);
        p1.setY(p1.y()-((p1.y()-p2.y())*20.0/minDistance)*startHeight/40);
    }
    if(endLamp->deviceTypeId() >= 120 && endLamp->deviceTypeId() <= 123)
    {
        p2.setX(p2.x()-((p2.x()-p1.x())*20.0/minDistance)*endHeight/40);
        p2.setY(p2.y()-((p2.y()-p1.y())*20.0/minDistance)*endHeight/40);
    }
    //更新箭头长度
    minDistance = std::sqrt((p1.x()-p2.x()) * (p1.x()-p2.x()) + (p1.y()-p2.y()) * (p1.y()-p2.y()));
    double pointRotationAngle;
    pointRotationAngle = std::atan2(p2.y() - p1.y(), p2.x() - p1.x()) * 180 / M_PI;

    if(startOffset.x() == 0 && startOffset.y() == 0)
    {
        if(endOffset.x() == 0 && endOffset.y() == 0)
        {
            //都为地埋灯时的偏移量
            double xValue = std::sin(pointRotationAngle * M_PI / 180.0)*5*m_changeValue/100.0;
            double yValue = std::cos(pointRotationAngle * M_PI / 180.0)*5*m_changeValue/100.0;
            p1 = QPointF(p1.x()-xValue,p1.y()+yValue);
            p2 = QPointF(p2.x()-xValue,p2.y()+yValue);
        }
        else
        {
            p1 = QPointF(p1.x()+endOffset.x(),p1.y()+endOffset.y());
            p2 = QPointF(p2.x()+endOffset.x(),p2.y()+endOffset.y());
        }
    }
    else
    {
        p1 = QPointF(p1.x()+startOffset.x(),p1.y()+startOffset.y());
        p2 = QPointF(p2.x()+startOffset.x(),p2.y()+startOffset.y());
    }


    QPointF arrowline[8];
    double line_compare = 2.5*multiple*m_changeValue/100.0;
    double arrow_compare1 = 10*multiple*m_changeValue/100.0;
    double arrow_compare2 = 5*multiple*m_changeValue/100.0;
    QPointF arrowlinePoints[8] = {
        QPointF(p1.x(), p1.y() + line_compare),
        QPointF(p1.x() + minDistance - arrow_compare1, p1.y() + line_compare),
        QPointF(p1.x() + minDistance - std::sqrt(3.0) * arrow_compare1 * 0.5 - arrow_compare1,
               p1.y() + line_compare + arrow_compare1 * 0.5),
        QPointF(p1.x() + minDistance - std::sqrt(3.0) * (arrow_compare1 + arrow_compare2 * std::sqrt(3.0)) * 0.5,
               p1.y() + line_compare + (arrow_compare1 + arrow_compare2 * std::sqrt(3.0)) * 0.5),
        QPointF(p1.x() + minDistance, p1.y() + line_compare),
        QPointF(p1.x() + minDistance, p1.y() - line_compare),
        QPointF(p1.x(), p1.y() - line_compare),
        QPointF(p1.x(), p1.y() + line_compare)
    };
    for (int i = 0; i < 8; ++i) {
        arrowline[i] = rotatePoint(arrowlinePoints[i], p1, pointRotationAngle);
    }

    if(((startLamp->deviceValue(DEVICE_VALUE_LEFTCLOSE) == 1) && (joint == 1)) ||
        ((startLamp->deviceValue(DEVICE_VALUE_RIGHTCLOSE) == 1) && (joint == 2)))
    {
//        painter.setBrush(QBrush(QColor(255,255,255)));  // 设置填充颜色为透明
    }

    else
    {
        painter.setBrush(QBrush(QColor(205,51,51)));  // 设置填充颜色为Brown3
        painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
        painter.drawPolygon(arrowline, 8);
    }

    // 恢复保存的坐标系统
    painter.restore();
}

//双向标志灯
void CDesignSplashView::setArrowPrimitiveItem(QPixmap* background, CDevice* device)
{
    qDebug() << "CDesignSplashView::setArrowPrimitiveItem" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //坐标、高度、宽度需要乘上放大倍数
    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble()*m_changeValue/100;
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble()*m_changeValue/100;
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble()*m_changeValue/100;
    qreal rotation = device->deviceValue(DEVICE_VALUE_ANGEL).toDouble();

    // 保存当前坐标系统
    painter.save();

    // 平移到长方形中心点位置
    painter.translate(x + 36*height/40, y + 20*height/40);

    // 缩放坐标系统
    painter.scale(height/40, height/40);

    // 旋转坐标系统
    painter.rotate(rotation);

    // 绘制长方形
    QRect rect(-36, -20, 72, 40);  // 长方形位置和大小

    //通讯故障，设置填充颜色为gray41
    if(device->getDeviceCommunicationFault())
        painter.setBrush(QBrush(QColor(105,105,105)));
    //应急状态，设置填充颜色为Red
    else if(device->deviceValue(DEVICE_VALUE_EMERGENCY) == "应急" || CGlobal::instance()->m_isEmergency == true)
        painter.setBrush(QBrush(QColor(255,0,0)));
    //光源故障，设置填充颜色为Yellow1
    else if(device->deviceValue(DEVICE_VALUE_LIGHT) == "光源故障")
        painter.setBrush(QBrush(QColor(255,255,0)));
    //正常状态，设置填充颜色为LimeGreen
    else
        painter.setBrush(QBrush(QColor(50,205,50)));

    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawRect(rect);

    int leftClose = device->deviceValue(DEVICE_VALUE_LEFTCLOSE).toInt();
    int rightClose = device->deviceValue(DEVICE_VALUE_RIGHTCLOSE).toInt();
    int defaultDirection = device->deviceValue(DEVICE_VALUE_DEFAULTDIRECTION).toInt();
    QString emergency = device->deviceValue(DEVICE_VALUE_EMERGENCY).toString();
    // 绘制左箭头
    QPoint arrowPointsLeft[8] = {
        QPoint(rect.left() + 5, rect.center().y() + 2 - 2.5),
        QPoint(rect.left() + 32, rect.center().y() + 2 - 2.5),
        QPoint(rect.left() + 32, rect.center().y() + 2 + 2.5),
        QPoint(rect.left() + 5, rect.center().y() + 2 + 2.5),
        QPoint(rect.left() + 5, rect.center().y() + 2 - 2.5),
        QPoint(rect.left() + 5 + std::sqrt(3.0) * (5 + 5 * std::sqrt(3.0)) * 0.5,
               rect.center().y() + 2 - 2.5 - (5 + 5 * std::sqrt(3.0)) * 0.5),
        QPoint(rect.left() + 5 + std::sqrt(3.0) * 5 * 0.5 + 10,
               rect.center().y() + 2 - 2.5 - 5 * 0.5),
        QPoint(rect.left() + 5 + 10, rect.center().y() + 2 - 2.5)
    };
    if(leftClose == 1 || ((defaultDirection & 0x02) == 0x02 && emergency == "正常"))  //默认指向为右向或者全灭并且处于非应急状态
        painter.setBrush(QBrush(QColor(54,54,54)));  // 设置填充颜色为grey21
    else
        painter.setBrush(QBrush(QColor(173,255,47)));  // 设置填充颜色为GreenYellow
    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawPolygon(arrowPointsLeft, 8);

    // 绘制右箭头
    QPoint arrowPointsRight[8] = {
        QPoint(rect.right() - 5, rect.center().y() + 2 - 2.5),
        QPoint(rect.right() - 32, rect.center().y() + 2 - 2.5),
        QPoint(rect.right() - 32, rect.center().y() + 2 + 2.5),
        QPoint(rect.right() - 5, rect.center().y() + 2 + 2.5),
        QPoint(rect.right() - 5, rect.center().y() + 2 - 2.5),
        QPoint(rect.right() - 5 - std::sqrt(3.0) * (5 + 5 * std::sqrt(3.0)) * 0.5,
               rect.center().y() + 2 - 2.5 - (5 + 5 * std::sqrt(3.0)) * 0.5),
        QPoint(rect.right() - 5 - std::sqrt(3.0) * 5 * 0.5 - 10,
               rect.center().y() + 2 - 2.5 - 5 * 0.5),
        QPoint(rect.right() - 5 - 10, rect.center().y() + 2 - 2.5)
    };
    if(rightClose == 1 || ((defaultDirection & 0x01) == 0x01 && emergency == "正常"))  //默认指向为左向或者全灭并且处于非应急状态
        painter.setBrush(QBrush(QColor(54,54,54)));  // 设置填充颜色为grey21
    else
        painter.setBrush(QBrush(QColor(173,255,47)));  // 设置填充颜色为GreenYellow
    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawPolygon(arrowPointsRight, 8);

    // 恢复保存的坐标系统
    painter.restore();

}

//左向标志灯
void CDesignSplashView::setLeftArrowPrimitiveItem(QPixmap* background, CDevice* device)
{
    qDebug() << "CDesignSplashView::setLeftArrowPrimitiveItem" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //坐标、高度、宽度需要乘上放大倍数
    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble()*m_changeValue/100;
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble()*m_changeValue/100;
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble()*m_changeValue/100;
    qreal rotation = device->deviceValue(DEVICE_VALUE_ANGEL).toDouble();

    // 保存当前坐标系统
    painter.save();

    // 平移到长方形中心点位置
    painter.translate(x + 36*height/40, y + 20*height/40);

    // 缩放坐标系统
    painter.scale(height/40, height/40);

    // 旋转坐标系统
    painter.rotate(rotation);

    // 绘制长方形
    QRect rect(-36, -20, 72, 40);  // 长方形位置和大小

    //通讯故障，设置填充颜色为gray41
    if(device->getDeviceCommunicationFault())
        painter.setBrush(QBrush(QColor(105,105,105)));
    //应急状态，设置填充颜色为Red
    else if(device->deviceValue(DEVICE_VALUE_EMERGENCY) == "应急" || CGlobal::instance()->m_isEmergency == true)
        painter.setBrush(QBrush(QColor(255,0,0)));
    //光源故障，设置填充颜色为Yellow1
    else if(device->deviceValue(DEVICE_VALUE_LIGHT) == "光源故障")
        painter.setBrush(QBrush(QColor(255,255,0)));
    //正常状态，设置填充颜色为LimeGreen
    else
        painter.setBrush(QBrush(QColor(50,205,50)));

    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawRect(rect);

    // 绘制左箭头
    QPoint arrowPointsLeft[8] = {
        QPoint(rect.left() + 5, rect.center().y() + 2 - 2.5),
        QPoint(rect.left() + 32, rect.center().y() + 2 - 2.5),
        QPoint(rect.left() + 32, rect.center().y() + 2 + 2.5),
        QPoint(rect.left() + 5, rect.center().y() + 2 + 2.5),
        QPoint(rect.left() + 5, rect.center().y() + 2 - 2.5),
        QPoint(rect.left() + 5 + std::sqrt(3.0) * (5 + 5 * std::sqrt(3.0)) * 0.5,
               rect.center().y() + 2 - 2.5 - (5 + 5 * std::sqrt(3.0)) * 0.5),
        QPoint(rect.left() + 5 + std::sqrt(3.0) * 5 * 0.5 + 10,
               rect.center().y() + 2 - 2.5 - 5 * 0.5),
        QPoint(rect.left() + 5 + 10, rect.center().y() + 2 - 2.5)
    };
    painter.setBrush(QBrush(QColor(173,255,47)));  // 设置填充颜色为GreenYellow
    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawPolygon(arrowPointsLeft, 8);


    // 恢复保存的坐标系统
    painter.restore();

}

//右向标志灯
void CDesignSplashView::setRightArrowPrimitiveItem(QPixmap* background, CDevice* device)
{
    qDebug() << "CDesignSplashView::setRightArrowPrimitiveItem" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //坐标、高度、宽度需要乘上放大倍数
    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble()*m_changeValue/100;
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble()*m_changeValue/100;
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble()*m_changeValue/100;
    qreal rotation = device->deviceValue(DEVICE_VALUE_ANGEL).toDouble();

    // 保存当前坐标系统
    painter.save();

    // 平移到长方形中心点位置
    painter.translate(x + 36*height/40, y + 20*height/40);

    // 缩放坐标系统
    painter.scale(height/40, height/40);

    // 旋转坐标系统
    painter.rotate(rotation);

    // 绘制长方形
    QRect rect(-36, -20, 72, 40);  // 长方形位置和大小

    //通讯故障，设置填充颜色为gray41
    if(device->getDeviceCommunicationFault())
        painter.setBrush(QBrush(QColor(105,105,105)));
    //应急状态，设置填充颜色为Red
    else if(device->deviceValue(DEVICE_VALUE_EMERGENCY) == "应急" || CGlobal::instance()->m_isEmergency == true)
        painter.setBrush(QBrush(QColor(255,0,0)));
    //光源故障，设置填充颜色为Yellow1
    else if(device->deviceValue(DEVICE_VALUE_LIGHT) == "光源故障")
        painter.setBrush(QBrush(QColor(255,255,0)));
    //正常状态，设置填充颜色为LimeGreen
    else
        painter.setBrush(QBrush(QColor(50,205,50)));

    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawRect(rect);

    // 绘制右箭头
    QPoint arrowPointsRight[8] = {
        QPoint(rect.right() - 5, rect.center().y() + 2 - 2.5),
        QPoint(rect.right() - 32, rect.center().y() + 2 - 2.5),
        QPoint(rect.right() - 32, rect.center().y() + 2 + 2.5),
        QPoint(rect.right() - 5, rect.center().y() + 2 + 2.5),
        QPoint(rect.right() - 5, rect.center().y() + 2 - 2.5),
        QPoint(rect.right() - 5 - std::sqrt(3.0) * (5 + 5 * std::sqrt(3.0)) * 0.5,
               rect.center().y() + 2 - 2.5 - (5 + 5 * std::sqrt(3.0)) * 0.5),
        QPoint(rect.right() - 5 - std::sqrt(3.0) * 5 * 0.5 - 10,
               rect.center().y() + 2 - 2.5 - 5 * 0.5),
        QPoint(rect.right() - 5 - 10, rect.center().y() + 2 - 2.5)
    };
    painter.setBrush(QBrush(QColor(173,255,47)));  // 设置填充颜色为GreenYellow
    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawPolygon(arrowPointsRight, 8);

    // 恢复保存的坐标系统
    painter.restore();

}

//多信息复合标志灯
void CDesignSplashView::setLeftFPrimitiveItem(QPixmap* background, CDevice* device)
{
    qDebug() << "CDesignSplashView::setLeftFPrimitiveItem" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //坐标、高度、宽度需要乘上放大倍数
    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble()*m_changeValue/100;
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble()*m_changeValue/100;
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble()*m_changeValue/100;
    qreal rotation = device->deviceValue(DEVICE_VALUE_ANGEL).toDouble();

    // 保存当前坐标系统
    painter.save();

    // 平移到长方形中心点位置
    painter.translate(x + 36*height/40, y + 20*height/40);

    // 缩放坐标系统
    painter.scale(height/40, height/40);

    // 旋转坐标系统
    painter.rotate(rotation);

    // 绘制长方形
    QRect rect(-36, -20, 72, 40);  // 长方形位置和大小

    //通讯故障，设置填充颜色为gray41
    if(device->getDeviceCommunicationFault())
        painter.setBrush(QBrush(QColor(105,105,105)));
    //应急状态，设置填充颜色为Red
    else if(device->deviceValue(DEVICE_VALUE_EMERGENCY) == "应急" || CGlobal::instance()->m_isEmergency == true)
        painter.setBrush(QBrush(QColor(255,0,0)));
    //光源故障，设置填充颜色为Yellow1
    else if(device->deviceValue(DEVICE_VALUE_LIGHT) == "光源故障")
        painter.setBrush(QBrush(QColor(255,255,0)));
    //正常状态，设置填充颜色为LimeGreen
    else
        painter.setBrush(QBrush(QColor(50,205,50)));

    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawRect(rect);

    // 绘制左箭头
    QPoint arrowPointsLeft[8] = {
        QPoint(rect.left() + 5, rect.center().y() + 2 - 2.5),
        QPoint(rect.left() + 32, rect.center().y() + 2 - 2.5),
        QPoint(rect.left() + 32, rect.center().y() + 2 + 2.5),
        QPoint(rect.left() + 5, rect.center().y() + 2 + 2.5),
        QPoint(rect.left() + 5, rect.center().y() + 2 - 2.5),
        QPoint(rect.left() + 5 + std::sqrt(3.0) * (5 + 5 * std::sqrt(3.0)) * 0.5,
               rect.center().y() + 2 - 2.5 - (5 + 5 * std::sqrt(3.0)) * 0.5),
        QPoint(rect.left() + 5 + std::sqrt(3.0) * 5 * 0.5 + 10,
               rect.center().y() + 2 - 2.5 - 5 * 0.5),
        QPoint(rect.left() + 5 + 10, rect.center().y() + 2 - 2.5)
    };
    painter.setBrush(QBrush(QColor(173,255,47)));  // 设置填充颜色为GreenYellow
    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawPolygon(arrowPointsLeft, 8);

    // 绘制F
    QPoint arrowPointsF[10] = {
        QPoint(rect.right() - 28, rect.center().y() + 2 + 2.5 + 9),
        QPoint(rect.right() - 28 + 5, rect.center().y() + 2 + 2.5 + 9),
        QPoint(rect.right() - 28 + 5, rect.center().y() + 2 + 2.5),
        QPoint(rect.right() - 28 + 5 + 7, rect.center().y() + 2 + 2.5),
        QPoint(rect.right() - 28 + 5 + 7, rect.center().y() + 2 + 2.5 - 5),
        QPoint(rect.right() - 28 + 5, rect.center().y() + 2 + 2.5 - 5),
        QPoint(rect.right() - 28 + 5, rect.center().y() + 2 + 2.5 - 5 - 5),
        QPoint(rect.right() - 28 + 5 + 8, rect.center().y() + 2 + 2.5 - 5 - 5),
        QPoint(rect.right() - 28 + 5 + 8, rect.center().y() + 2 + 2.5 - 5 - 5 - 5),
        QPoint(rect.right() - 28, rect.center().y() + 2 + 2.5 - 5 - 5 - 5)
    };
    painter.setBrush(QBrush(QColor(173,255,47)));  // 设置填充颜色为GreenYellow
    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawPolygon(arrowPointsF, 10);


    // 恢复保存的坐标系统
    painter.restore();

}


//楼层标志灯
void CDesignSplashView::setFloorPrimitiveItem(QPixmap* background, CDevice* device)
{
    qDebug() << "CDesignSplashView::setFloorPrimitiveItem" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //坐标、高度、宽度需要乘上放大倍数
    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble()*m_changeValue/100;
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble()*m_changeValue/100;
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble()*m_changeValue/100;
    qreal rotation = device->deviceValue(DEVICE_VALUE_ANGEL).toDouble();

    // 保存当前坐标系统
    painter.save();

    // 平移到长方形中心点位置
    painter.translate(x + 36*height/40, y + 20*height/40);

    // 缩放坐标系统
    painter.scale(height/40, height/40);

    // 旋转坐标系统
    painter.rotate(rotation);

    // 绘制长方形
    QRect rect(-36, -20, 72, 40);  // 长方形位置和大小

    //通讯故障，设置填充颜色为gray41
    if(device->getDeviceCommunicationFault())
        painter.setBrush(QBrush(QColor(105,105,105)));
    //应急状态，设置填充颜色为Red
    else if(device->deviceValue(DEVICE_VALUE_EMERGENCY) == "应急" || CGlobal::instance()->m_isEmergency == true)
        painter.setBrush(QBrush(QColor(255,0,0)));
    //光源故障，设置填充颜色为Yellow1
    else if(device->deviceValue(DEVICE_VALUE_LIGHT) == "光源故障")
        painter.setBrush(QBrush(QColor(255,255,0)));
    //正常状态，设置填充颜色为LimeGreen
    else
        painter.setBrush(QBrush(QColor(50,205,50)));

    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawRect(rect);

    // 绘制F
    QPoint arrowPointsF[10] = {
        QPoint(rect.right() - 41, rect.center().y() + 2.5 + 9),
        QPoint(rect.right() - 41 + 5, rect.center().y() + 2.5 + 9),
        QPoint(rect.right() - 41 + 5, rect.center().y() + 2.5),
        QPoint(rect.right() - 41 + 5 + 7, rect.center().y() + 2.5),
        QPoint(rect.right() - 41 + 5 + 7, rect.center().y() + 2.5 - 5),
        QPoint(rect.right() - 41 + 5, rect.center().y() + 2.5 - 5),
        QPoint(rect.right() - 41 + 5, rect.center().y() + 2.5 - 5 - 5),
        QPoint(rect.right() - 41 + 5 + 8, rect.center().y() + 2.5 - 5 - 5),
        QPoint(rect.right() - 41 + 5 + 8, rect.center().y() + 2.5 - 5 - 5 - 5),
        QPoint(rect.right() - 41, rect.center().y() + 2.5 - 5 - 5 - 5)
    };
    painter.setBrush(QBrush(QColor(173,255,47)));  // 设置填充颜色为GreenYellow
    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawPolygon(arrowPointsF, 10);


    // 恢复保存的坐标系统
    painter.restore();

}

//安全出口标志灯
void CDesignSplashView::setExitPrimitiveItem(QPixmap* background, CDevice* device)
{
    qDebug() << "CDesignSplashView::setExitPrimitiveItem" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //坐标、高度、宽度需要乘上放大倍数
    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble()*m_changeValue/100;
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble()*m_changeValue/100;
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble()*m_changeValue/100;
    qreal rotation = device->deviceValue(DEVICE_VALUE_ANGEL).toDouble();

    // 保存当前坐标系统
    painter.save();
    // 平移到长方形中心点位置
    painter.translate(x + 36*height/40, y + 20*height/40);
    // 缩放坐标系统
    painter.scale(height/40, height/40);
    // 旋转坐标系统
    painter.rotate(rotation);

    // 绘制长方形
    QRect rect(-36, -20, 72, 40);  // 长方形位置和大小

    //通讯故障，设置填充颜色为gray41
    if(device->getDeviceCommunicationFault())
        painter.setBrush(QBrush(QColor(105,105,105)));
    //应急状态，设置填充颜色为Red
    else if(device->deviceValue(DEVICE_VALUE_EMERGENCY) == "应急" || CGlobal::instance()->m_isEmergency == true)
        painter.setBrush(QBrush(QColor(255,0,0)));
    //光源故障，设置填充颜色为Yellow1
    else if(device->deviceValue(DEVICE_VALUE_LIGHT) == "光源故障")
        painter.setBrush(QBrush(QColor(255,255,0)));
    //正常状态，设置填充颜色为LimeGreen
    else
        painter.setBrush(QBrush(QColor(50,205,50)));

    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawRect(rect);


    // 绘制文字
    QFont font;  // 设置字体和大小
    font.setPointSize(12);
    font.setBold(true);  // 设置为粗体
    painter.setFont(font);  // 应用字体设置
    painter.setPen(QColor(173,255,47));  // 设置填充颜色为GreenYellow
    // 限制绘制区域，只在长方形区域内绘制文字
    painter.setClipRect(rect);
    painter.drawText(rect, Qt::AlignCenter, "安全出口");  // 在长方形内居中绘制文字

    // 恢复保存的坐标系统
    painter.restore();
}

//可变状态灯
void CDesignSplashView::setChangeExitPrimitiveItem(QPixmap* background, CDevice* device)
{
    qDebug() << "CDesignSplashView::setChangeExitPrimitiveItem" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //坐标、高度、宽度需要乘上放大倍数
    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble()*m_changeValue/100;
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble()*m_changeValue/100;
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble()*m_changeValue/100;
    qreal rotation = device->deviceValue(DEVICE_VALUE_ANGEL).toDouble();

    // 保存当前坐标系统
    painter.save();
    // 平移到长方形中心点位置
    painter.translate(x + 36*height/40, y + 20*height/40);
    // 缩放坐标系统
    painter.scale(height/40, height/40);
    // 旋转坐标系统
    painter.rotate(rotation);

    // 绘制长方形
    QRect rect(-36, -20, 72, 40);  // 长方形位置和大小

    //通讯故障，设置填充颜色为gray41
    if(device->getDeviceCommunicationFault())
        painter.setBrush(QBrush(QColor(105,105,105)));
    //应急状态，设置填充颜色为Red
    else if(device->deviceValue(DEVICE_VALUE_EMERGENCY) == "应急" || CGlobal::instance()->m_isEmergency == true)
        painter.setBrush(QBrush(QColor(255,0,0)));
    //光源故障，设置填充颜色为Yellow1
    else if(device->deviceValue(DEVICE_VALUE_LIGHT) == "光源故障")
        painter.setBrush(QBrush(QColor(255,255,0)));
    //正常状态，设置填充颜色为LimeGreen
    else
        painter.setBrush(QBrush(QColor(50,205,50)));

    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawRect(rect);

    // 绘制圆形
    QRect circleRect(-36 + 2, -20 + 2, 8, 8);  // 圆形位置和大小
    painter.setBrush(QBrush(QColor(105,105,105)));  // 设置填充颜色为gray41
    painter.setPen(QPen(Qt::black));  // 设置边框颜色为黑色
    painter.drawEllipse(circleRect);


    // 绘制文字
    QFont font;  // 设置字体和大小
    font.setPointSize(12);
    font.setBold(true);  // 设置为粗体
    painter.setFont(font);  // 应用字体设置
    painter.setPen(QColor(173,255,47));  // 设置填充颜色为GreenYellow
    // 限制绘制区域，只在长方形区域内绘制文字
    painter.setClipRect(rect);
    painter.drawText(rect, Qt::AlignCenter, "禁止/出口");  // 在长方形内居中绘制文字

    // 恢复保存的坐标系统
    painter.restore();
}

//照明灯
void CDesignSplashView::setLightPrimitiveItem(QPixmap* background, CDevice* device)
{
    qDebug() << "CDesignSplashView::setLightPrimitiveItem" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //坐标、高度、宽度需要乘上放大倍数
    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble()*m_changeValue/100;
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble()*m_changeValue/100;
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble()*m_changeValue/100;
    qreal rotation = device->deviceValue(DEVICE_VALUE_ANGEL).toDouble();

    // 保存当前坐标系统
    painter.save();

    // 平移到长方形中心点位置
    painter.translate(x + (20.0*4/3)*height/40, y + 20*height/40);

    // 缩放坐标系统
    painter.scale(height/40, height/40);

    // 旋转坐标系统
    painter.rotate(rotation);

    // 绘制圆形
    QRect ellipseRect1(-20.0*4/3+4, -20, 16, 16);  // 圆形位置和大小
    painter.setBrush(QBrush(QColor(205, 205, 0)));  // 设置填充颜色为yellow3
    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawEllipse(ellipseRect1);

    // 绘制圆形
    QRect ellipseRect2(20.0*4/3-4-16, -20, 16, 16);  // 圆形位置和大小
    painter.setBrush(QBrush(QColor(205, 205, 0)));  // 设置填充颜色为yellow3
    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawEllipse(ellipseRect2);

    // 绘制长方形
    QRect rect(-20.0*4/3, -20+16, 40.0*4/3, 24);  // 长方形位置和大小

    //通讯故障，设置填充颜色为gray41
    if(device->getDeviceCommunicationFault())
        painter.setBrush(QBrush(QColor(105,105,105)));
    //应急状态，设置填充颜色为Red
    else if(device->deviceValue(DEVICE_VALUE_EMERGENCY) == "应急" || CGlobal::instance()->m_isEmergency == true)
        painter.setBrush(QBrush(QColor(255,0,0)));
    //光源故障，设置填充颜色为Yellow1
    else if(device->deviceValue(DEVICE_VALUE_LIGHT) == "光源故障")
        painter.setBrush(QBrush(QColor(255,255,0)));
    //正常状态，设置填充颜色为LimeGreen
    else
        painter.setBrush(QBrush(QColor(50,205,50)));

    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawRect(rect);

    // 绘制文字
    QFont font;  // 设置字体和大小
    font.setPointSize(8);
    font.setBold(true);  // 设置为粗体
    painter.setFont(font);  // 应用字体设置
    painter.setPen(QColor(173,255,47));  // 设置填充颜色为GreenYellow
    // 限制绘制区域，只在长方形区域内绘制文字
    painter.setClipRect(rect);
    painter.drawText(rect, Qt::AlignCenter, "应急照明灯");  // 在长方形内居中绘制文字

    // 恢复保存的坐标系统
    painter.restore();

}
//单向地埋灯
void CDesignSplashView::setOneUndergroundPrimitiveItem(QPixmap* background, CDevice* device)
{
    qDebug() << "CDesignSplashView::setOneUndergroundPrimitiveItem" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //坐标、高度、宽度需要乘上放大倍数
    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble()*m_changeValue/100;
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble()*m_changeValue/100;
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble()*m_changeValue/100;
    qreal rotation = device->deviceValue(DEVICE_VALUE_ANGEL).toDouble();

    // 保存当前坐标系统
    painter.save();

    // 平移到圆形中心点位置
    painter.translate(x + 20*height/40, y + 20*height/40);

    // 缩放坐标系统
    painter.scale(height/40, height/40);

    // 旋转坐标系统
    painter.rotate(rotation);

    // 绘制圆形
    QRect rect(-20, -20, 40, 40);  // 圆形位置和大小

    //通讯故障，设置填充颜色为gray41
    if(device->getDeviceCommunicationFault())
        painter.setBrush(QBrush(QColor(105,105,105)));
    //应急状态，设置填充颜色为Red
    else if(device->deviceValue(DEVICE_VALUE_EMERGENCY) == "应急" || CGlobal::instance()->m_isEmergency == true)
        painter.setBrush(QBrush(QColor(255,0,0)));
    //光源故障，设置填充颜色为Yellow1
    else if(device->deviceValue(DEVICE_VALUE_LIGHT) == "光源故障")
        painter.setBrush(QBrush(QColor(255,255,0)));
    //正常状态，设置填充颜色为LimeGreen
    else
        painter.setBrush(QBrush(QColor(50,205,50)));

    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawEllipse(rect);

    // 绘制左箭头
    QPoint arrowPointsLeft[8] = {
        QPoint(rect.left() + 8, rect.center().y() - 2 + 2),
        QPoint(rect.left() + 8, rect.center().y() - 2 - 2),
        QPoint(rect.left() + 32, rect.center().y() - 2 - 2),
        QPoint(rect.left() + 32, rect.center().y() - 2 + 2),
        QPoint(rect.left() + 8, rect.center().y() - 2 + 2),
        QPoint(rect.left() + 8 + std::sqrt(3.0) * (4 + 4 * std::sqrt(3.0)) * 0.5,
               rect.center().y() - 2 + 2 + (4 + 4 * std::sqrt(3.0)) * 0.5),
        QPoint(rect.left() + 8 + std::sqrt(3.0) * 4 * 0.5 + 8,
               rect.center().y() - 2 + 2 + 4 * 0.5),
        QPoint(rect.left() + 8 + 8, rect.center().y() - 2 + 2)

    };
    painter.setBrush(QBrush(QColor(173,255,47)));  // 设置填充颜色为GreenYellow
    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawPolygon(arrowPointsLeft, 8);

    // 恢复保存的坐标系统
    painter.restore();

}


//双向地埋灯
void CDesignSplashView::setDoubleUndergroundPrimitiveItem(QPixmap* background, CDevice* device)
{
    qDebug() << "CDesignSplashView::setDoubleUndergroundPrimitiveItem" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //坐标、高度、宽度需要乘上放大倍数
    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble()*m_changeValue/100;
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble()*m_changeValue/100;
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble()*m_changeValue/100;
    qreal rotation = device->deviceValue(DEVICE_VALUE_ANGEL).toDouble();

    // 保存当前坐标系统
    painter.save();

    // 平移到圆形中心点位置
    painter.translate(x + 20*height/40, y + 20*height/40);

    // 缩放坐标系统
    painter.scale(height/40, height/40);

    // 旋转坐标系统
    painter.rotate(rotation);

    // 绘制圆形
    QRect rect(-20, -20, 40, 40);  // 圆形位置和大小

    //通讯故障，设置填充颜色为gray41
    if(device->getDeviceCommunicationFault())
        painter.setBrush(QBrush(QColor(105,105,105)));
    //应急状态，设置填充颜色为Red
    else if(device->deviceValue(DEVICE_VALUE_EMERGENCY) == "应急" || CGlobal::instance()->m_isEmergency == true)
        painter.setBrush(QBrush(QColor(255,0,0)));
    //光源故障，设置填充颜色为Yellow1
    else if(device->deviceValue(DEVICE_VALUE_LIGHT) == "光源故障")
        painter.setBrush(QBrush(QColor(255,255,0)));
    //正常状态，设置填充颜色为LimeGreen
    else
        painter.setBrush(QBrush(QColor(50,205,50)));

    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawEllipse(rect);


    int leftClose = device->deviceValue(DEVICE_VALUE_LEFTCLOSE).toInt();
    int rightClose = device->deviceValue(DEVICE_VALUE_RIGHTCLOSE).toInt();
    int defaultDirection = device->deviceValue(DEVICE_VALUE_DEFAULTDIRECTION).toInt();
    QString emergency = device->deviceValue(DEVICE_VALUE_EMERGENCY).toString();
    // 绘制左箭头
    QPoint arrowPointsLeft[8] = {
        QPoint(rect.left() + 8, rect.center().y() + 4 + 2),
        QPoint(rect.left() + 8, rect.center().y() + 4 - 2),
        QPoint(rect.left() + 32, rect.center().y() + 4 - 2),
        QPoint(rect.left() + 32, rect.center().y() + 4 + 2),
        QPoint(rect.left() + 8, rect.center().y() + 4 + 2),
        QPoint(rect.left() + 8 + std::sqrt(3.0) * (4 + 4 * std::sqrt(3.0)) * 0.5,
               rect.center().y() + 4 + 2 + (4 + 4 * std::sqrt(3.0)) * 0.5),
        QPoint(rect.left() + 8 + std::sqrt(3.0) * 4 * 0.5 + 8,
               rect.center().y() + 4 + 2 + 4 * 0.5),
        QPoint(rect.left() + 8 + 8, rect.center().y() + 4 + 2)
    };
    if(leftClose == 1 || ((defaultDirection & 0x01) == 0x01 && emergency == "正常"))  //默认指向为右向或者全灭并且处于非应急状态
        painter.setBrush(QBrush(QColor(54,54,54)));  // 设置填充颜色为grey21
    else
        painter.setBrush(QBrush(QColor(173,255,47)));  // 设置填充颜色为GreenYellow
    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawPolygon(arrowPointsLeft, 8);


    // 绘制右箭头
    QPoint arrowPointsRight[8] = {
        QPoint(rect.left() + 32, rect.center().y() - 4 - 2),
        QPoint(rect.left() + 32, rect.center().y() - 4 + 2),
        QPoint(rect.left() + 8, rect.center().y() - 4 + 2),
        QPoint(rect.left() + 8, rect.center().y() - 4 - 2),
        QPoint(rect.left() + 32, rect.center().y() - 4 - 2),
        QPoint(rect.left() + 32 - std::sqrt(3.0) * (4 + 4 * std::sqrt(3.0)) * 0.5,
               rect.center().y() - 4 - 2 - (4 + 4 * std::sqrt(3.0)) * 0.5),
        QPoint(rect.left() + 32 - std::sqrt(3.0) * 4 * 0.5 - 8,
               rect.center().y() - 4 - 2 - 4 * 0.5),
        QPoint(rect.left() + 32 - 8, rect.center().y() - 4 - 2)
    };
    if(rightClose == 1 || ((defaultDirection & 0x02) == 0x02 && emergency == "正常"))  //默认指向为左向或者全灭并且处于非应急状态
        painter.setBrush(QBrush(QColor(54,54,54)));  // 设置填充颜色为grey21
    else
        painter.setBrush(QBrush(QColor(173,255,47)));  // 设置填充颜色为GreenYellow
    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawPolygon(arrowPointsRight, 8);

    // 恢复保存的坐标系统
    painter.restore();

}


//照明灯点亮
void CDesignSplashView::slot_Lighten()
{
    qDebug() << "CDesignSplashView::slot_Lighten" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nCanportAdd = m_device->canportAdd();
    msgobjectstatus.nDisID = m_device->distributionAdd();
    msgobjectstatus.nLoopID = m_device->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();

    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDefaultOn, &msgobjectstatus);

}

//照明灯灭灯
void CDesignSplashView::slot_CloseLight()
{
    qDebug() << "CDesignSplashView::slot_CloseLight" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nCanportAdd = m_device->canportAdd();
    msgobjectstatus.nDisID = m_device->distributionAdd();
    msgobjectstatus.nLoopID = m_device->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();

    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDefaultOff, &msgobjectstatus);

}

//标志灯左向
void CDesignSplashView::slot_Left()
{
    qDebug() << "CDesignSplashView::slot_Left" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int direction = 0x05;
    if(!m_device)
        return;
    m_device->setDeviceValue(DEVICE_VALUE_LEFTCLOSE,0);
    m_device->setDeviceValue(DEVICE_VALUE_RIGHTCLOSE,1);
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nCanportAdd = m_device->canportAdd();
    msgobjectstatus.nDisID = m_device->distributionAdd();
    msgobjectstatus.nLoopID = m_device->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();
    msgobjectstatus.nValue = direction;
//    //上传灯具指向到云平台
//    CGlobal::instance()->ClientBusiness()->XmlUploadLampDirection(
//                CGlobal::instance()->saveKeyId(OBJT_Device, m_device->keyId()),"左向");
//    //服务端上传灯具指向
//    CGlobal::instance()->ClientBusiness()->serverLampDirectionUpload(
//                CGlobal::instance()->saveKeyId(OBJT_Device, m_device->keyId()),"左向");
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDirection, &msgobjectstatus);

    //布局视图界面灯具状态更新
    int layoutPageID = CGlobal::instance()->designTreeView()->CurrentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    setPrimitiveItem(layoutPageID);

}

//标志灯右向
void CDesignSplashView::slot_Right()
{
    qDebug() << "CDesignSplashView::slot_Right" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int direction = 0x06;
    if(!m_device)
        return;
    m_device->setDeviceValue(DEVICE_VALUE_LEFTCLOSE,1);
    m_device->setDeviceValue(DEVICE_VALUE_RIGHTCLOSE,0);
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nCanportAdd = m_device->canportAdd();
    msgobjectstatus.nDisID = m_device->distributionAdd();
    msgobjectstatus.nLoopID = m_device->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();
    msgobjectstatus.nValue = direction;
//    //上传灯具指向到云平台
//    CGlobal::instance()->ClientBusiness()->XmlUploadLampDirection(
//                CGlobal::instance()->saveKeyId(OBJT_Device, m_device->keyId()),"右向");
//    //服务端上传灯具指向
//    CGlobal::instance()->ClientBusiness()->serverLampDirectionUpload(
//                CGlobal::instance()->saveKeyId(OBJT_Device, m_device->keyId()),"右向");
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDirection, &msgobjectstatus);

    //布局视图界面灯具状态更新
    int layoutPageID = CGlobal::instance()->designTreeView()->CurrentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    setPrimitiveItem(layoutPageID);

}

//标志灯全亮
void CDesignSplashView::slot_AllBright()
{
    qDebug() << "CDesignSplashView::slot_AllBright" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int direction = 0x07;
    if(!m_device)
        return;
    m_device->setDeviceValue(DEVICE_VALUE_LEFTCLOSE,0);
    m_device->setDeviceValue(DEVICE_VALUE_RIGHTCLOSE,0);
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nCanportAdd = m_device->canportAdd();
    msgobjectstatus.nDisID = m_device->distributionAdd();
    msgobjectstatus.nLoopID = m_device->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();
    msgobjectstatus.nValue = direction;
//    //上传灯具指向到云平台
//    CGlobal::instance()->ClientBusiness()->XmlUploadLampDirection(
//                CGlobal::instance()->saveKeyId(OBJT_Device, m_device->keyId()),"全亮");
//    //服务端上传灯具指向
//    CGlobal::instance()->ClientBusiness()->serverLampDirectionUpload(
//                CGlobal::instance()->saveKeyId(OBJT_Device, m_device->keyId()),"全亮");
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDirection, &msgobjectstatus);

    //布局视图界面灯具状态更新
    int layoutPageID = CGlobal::instance()->designTreeView()->CurrentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    setPrimitiveItem(layoutPageID);
}

//标志灯全灭
void CDesignSplashView::slot_AllClose()
{
    qDebug() << "CDesignSplashView::slot_AllClose" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int direction = 0x04;
    if(!m_device)
        return;
    m_device->setDeviceValue(DEVICE_VALUE_LEFTCLOSE,1);
    m_device->setDeviceValue(DEVICE_VALUE_RIGHTCLOSE,1);
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nCanportAdd = m_device->canportAdd();
    msgobjectstatus.nDisID = m_device->distributionAdd();
    msgobjectstatus.nLoopID = m_device->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();
    msgobjectstatus.nValue = direction;
//    //上传灯具指向到云平台
//    CGlobal::instance()->ClientBusiness()->XmlUploadLampDirection(
//                CGlobal::instance()->saveKeyId(OBJT_Device, m_device->keyId()),"全灭");
//    //服务端上传灯具指向
//    CGlobal::instance()->ClientBusiness()->serverLampDirectionUpload(
//                CGlobal::instance()->saveKeyId(OBJT_Device, m_device->keyId()),"全灭");
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDirection, &msgobjectstatus);

    //布局视图界面灯具状态更新
    int layoutPageID = CGlobal::instance()->designTreeView()->CurrentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    setPrimitiveItem(layoutPageID);
}
//灯具默认左向
void CDesignSplashView::slot_DefaultLeft()
{
    qDebug() << "CDesignSplashView::slot_DefaultLeft" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nCanportAdd = m_device->canportAdd();
    msgobjectstatus.nDisID = m_device->distributionAdd();
    msgobjectstatus.nLoopID = m_device->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();
    msgobjectstatus.nValue = 0X05;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDefaultDrection, &msgobjectstatus);
    m_device->setDeviceValue(DEVICE_VALUE_DEFAULTDIRECTION,1);

    int keyID = CGlobal::instance()->saveKeyId(OBJT_Device,m_device->keyId());
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSqlQuery query = db.query("REPLACE INTO LampDefaultDirection (ID, LampInfoID, LampDirection) "
                                "VALUES (:ID, :LampInfoID, :LampDirection)");
    query.bindValue(":ID", keyID);
    query.bindValue(":LampInfoID", keyID);
    query.bindValue(":LampDirection", 1);

    if(!query.exec())
    {
        qDebug()<<query.lastError().text();
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
        log = "灯具:" + QString::number(msgobjectstatus.nDisID) + tr("-") + QString::number(msgobjectstatus.nLoopID) + tr("-") +
                       QString::number(msgobjectstatus.nDeviceID) + tr("默认方向保存数据库失败！");
        messageBox.setText(log);
        messageBox.exec();
    }
    db.close();

    //布局视图界面灯具状态更新
    int layoutPageID = CGlobal::instance()->designTreeView()->CurrentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    setPrimitiveItem(layoutPageID);
}
//灯具默认右向
void CDesignSplashView::slot_DefaultRight()
{
    qDebug() << "CDesignSplashView::slot_DefaultRight" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nCanportAdd = m_device->canportAdd();
    msgobjectstatus.nDisID = m_device->distributionAdd();
    msgobjectstatus.nLoopID = m_device->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();
    msgobjectstatus.nValue = 0X06;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDefaultDrection, &msgobjectstatus);
    m_device->setDeviceValue(DEVICE_VALUE_DEFAULTDIRECTION,2);

    int keyID = CGlobal::instance()->saveKeyId(OBJT_Device,m_device->keyId());
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSqlQuery query = db.query("REPLACE INTO LampDefaultDirection (ID, LampInfoID, LampDirection) "
                                "VALUES (:ID, :LampInfoID, :LampDirection)");
    query.bindValue(":ID", keyID);
    query.bindValue(":LampInfoID", keyID);
    query.bindValue(":LampDirection", 2);

    if(!query.exec())
    {
        qDebug()<<query.lastError().text();
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
        log = "灯具:" + QString::number(msgobjectstatus.nDisID) + tr("-") + QString::number(msgobjectstatus.nLoopID) + tr("-") +
                       QString::number(msgobjectstatus.nDeviceID) + tr("默认方向保存数据库失败！");
        messageBox.setText(log);
        messageBox.exec();
    }
    db.close();

    //布局视图界面灯具状态更新
    int layoutPageID = CGlobal::instance()->designTreeView()->CurrentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    setPrimitiveItem(layoutPageID);
}
//灯具默认全亮
void CDesignSplashView::slot_DefaultAll()
{
    qDebug() << "CDesignSplashView::slot_DefaultAll" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nCanportAdd = m_device->canportAdd();
    msgobjectstatus.nDisID = m_device->distributionAdd();
    msgobjectstatus.nLoopID = m_device->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();
    msgobjectstatus.nValue = 0X07;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDefaultDrection, &msgobjectstatus);
    m_device->setDeviceValue(DEVICE_VALUE_DEFAULTDIRECTION,0);

    int keyID = CGlobal::instance()->saveKeyId(OBJT_Device,m_device->keyId());
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSqlQuery query = db.query("REPLACE INTO LampDefaultDirection (ID, LampInfoID, LampDirection) "
                                "VALUES (:ID, :LampInfoID, :LampDirection)");
    query.bindValue(":ID", keyID);
    query.bindValue(":LampInfoID", keyID);
    query.bindValue(":LampDirection", 0);

    if(!query.exec())
    {
        qDebug()<<query.lastError().text();
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
        log = "灯具:" + QString::number(msgobjectstatus.nDisID) + tr("-") + QString::number(msgobjectstatus.nLoopID) + tr("-") +
                       QString::number(msgobjectstatus.nDeviceID) + tr("默认方向保存数据库失败！");
        messageBox.setText(log);
        messageBox.exec();
    }
    db.close();

    //布局视图界面灯具状态更新
    int layoutPageID = CGlobal::instance()->designTreeView()->CurrentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    setPrimitiveItem(layoutPageID);
}
//灯具默认全灭
void CDesignSplashView::slot_DefaultOff()
{
    qDebug() << "CDesignSplashView::slot_DefaultOff" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nCanportAdd = m_device->canportAdd();
    msgobjectstatus.nDisID = m_device->distributionAdd();
    msgobjectstatus.nLoopID = m_device->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();
    msgobjectstatus.nValue = 0X04;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDefaultDrection, &msgobjectstatus);
    m_device->setDeviceValue(DEVICE_VALUE_DEFAULTDIRECTION,3);

    int keyID = CGlobal::instance()->saveKeyId(OBJT_Device,m_device->keyId());
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QSqlQuery query = db.query("REPLACE INTO LampDefaultDirection (ID, LampInfoID, LampDirection) "
                                "VALUES (:ID, :LampInfoID, :LampDirection)");
    query.bindValue(":ID", keyID);
    query.bindValue(":LampInfoID", keyID);
    query.bindValue(":LampDirection", 3);

    if(!query.exec())
    {
        qDebug()<<query.lastError().text();
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
        log = "灯具:" + QString::number(msgobjectstatus.nDisID) + tr("-") + QString::number(msgobjectstatus.nLoopID) + tr("-") +
                       QString::number(msgobjectstatus.nDeviceID) + tr("默认方向保存数据库失败！");
        messageBox.setText(log);
        messageBox.exec();
    }
    db.close();

    //布局视图界面灯具状态更新
    int layoutPageID = CGlobal::instance()->designTreeView()->CurrentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    setPrimitiveItem(layoutPageID);
}

//双向地埋灯反接
void CDesignSplashView::slot_Reverse()
{
    qDebug() << "CDesignSplashView::slot_Reverse" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_device->deviceValue(DEVICE_VALUE_ISREVERSE).toInt() == 1)
        return;
    m_device->setDeviceValue(DEVICE_VALUE_ISREVERSE,1);
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    int lampTypeId = 0;
    QString cmd = QString("SELECT ID, Number, Type "
                          "FROM LampType WHERE Number = %1"
                          ).arg(m_device->deviceValue(DEVICE_VALUE_TYPE_ID).toInt());

    QSqlQuery querydata = db.query(cmd);
    while (querydata.next())
    {
        lampTypeId = querydata.value(0).toInt();
    }
    QSqlQuery query = db.query("REPLACE INTO LampInfo (ID, ChannelInfoID, Address, LampTypeID, Area, Location, Remark, IsReverse, IsUsed) "
                                "VALUES (:ID, :ChannelInfoID, :Address, :LampTypeID, :Area, :Location, :remark, :isreverse, :isused)");
    query.bindValue(":ID", CGlobal::instance()->saveKeyId(OBJT_Device, m_device->keyId()));
    query.bindValue(":ChannelInfoID", CGlobal::instance()->saveKeyId(OBJT_Loop, m_device->loop()->keyId()));
    query.bindValue(":Address", m_device->deviceAdd());
    query.bindValue(":LampTypeID", lampTypeId);
    if(m_device->deviceValue(DEVICE_VALUE_AREA).toString() == "")
        query.bindValue(":Area", " ");
    else
        query.bindValue(":Area", m_device->deviceValue(DEVICE_VALUE_AREA).toString());
    if(m_device->deviceValue(DEVICE_VALUE_LOCATION).toString() == "")
        query.bindValue(":Location", " ");
    else
        query.bindValue(":Location", m_device->deviceValue(DEVICE_VALUE_LOCATION).toString());
    query.bindValue(":remark", m_device->deviceValue(DEVICE_VALUE_DESCRIPTION).toString());
    query.bindValue(":isreverse", m_device->deviceValue(DEVICE_VALUE_ISREVERSE).toString());
//    query.bindValue(":isused", m_device->deviceValue(DEVICE_VALUE_ISUSED));
    query.bindValue(":isused", 1);

    if(!query.exec())
    {
        qDebug()<<query.lastError().text();
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
        log = "反接保存数据库失败！";
        messageBox.setText(log);
        messageBox.exec();
    }
    db.close();
}
//双向地埋灯未反接
void CDesignSplashView::slot_NoReverse()
{
    qDebug() << "CDesignSplashView::slot_NoReverse" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_device->deviceValue(DEVICE_VALUE_ISREVERSE).toInt() == 0)
        return;
    m_device->setDeviceValue(DEVICE_VALUE_ISREVERSE,0);
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    int lampTypeId = 0;
    QString cmd = QString("SELECT ID, Number, Type "
                          "FROM LampType WHERE Number = %1"
                          ).arg(m_device->deviceValue(DEVICE_VALUE_TYPE_ID).toInt());

    QSqlQuery querydata = db.query(cmd);
    while (querydata.next())
    {
        lampTypeId = querydata.value(0).toInt();
    }
    QSqlQuery query = db.query("REPLACE INTO LampInfo (ID, ChannelInfoID, Address, LampTypeID, Area, Location, Remark, IsReverse, IsUsed) "
                                "VALUES (:ID, :ChannelInfoID, :Address, :LampTypeID, :Area, :Location, :remark, :isreverse, :isused)");
    query.bindValue(":ID", CGlobal::instance()->saveKeyId(OBJT_Device, m_device->keyId()));
    query.bindValue(":ChannelInfoID", CGlobal::instance()->saveKeyId(OBJT_Loop, m_device->loop()->keyId()));
    query.bindValue(":Address", m_device->deviceAdd());
    query.bindValue(":LampTypeID", lampTypeId);
    if(m_device->deviceValue(DEVICE_VALUE_AREA).toString() == "")
        query.bindValue(":Area", " ");
    else
        query.bindValue(":Area", m_device->deviceValue(DEVICE_VALUE_AREA).toString());
    if(m_device->deviceValue(DEVICE_VALUE_LOCATION).toString() == "")
        query.bindValue(":Location", " ");
    else
        query.bindValue(":Location", m_device->deviceValue(DEVICE_VALUE_LOCATION).toString());
    query.bindValue(":remark", m_device->deviceValue(DEVICE_VALUE_DESCRIPTION).toString());
    query.bindValue(":isreverse", m_device->deviceValue(DEVICE_VALUE_ISREVERSE).toString());
//    query.bindValue(":isused", m_device->deviceValue(DEVICE_VALUE_ISUSED));
    query.bindValue(":isused", 1);

    if(!query.exec())
    {
        qDebug()<<query.lastError().text();
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
        log = "反接保存数据库失败！";
        messageBox.setText(log);
        messageBox.exec();
    }
    db.close();
}

//灯具测试开始
void CDesignSplashView::slot_StartTest()
{
    qDebug() << "CDesignSplashView::slot_StartTest" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = m_device->canportAdd();
    msgObjectStatus.nDisID = m_device->distributionAdd();
    msgObjectStatus.nLoopID = m_device->loopAdd();
    msgObjectStatus.nDeviceID = m_device->deviceAdd();

    m_device->setDeviceValue(DEVICE_VALUE_TEST,"测试");
    m_device->setDeviceControl(true);
    deviceControl devicecontrol;
    devicecontrol.saveDeviceControlFile();//保存灯具控制表
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceTestStart, &msgObjectStatus);

}
//灯具测试停止
void CDesignSplashView::slot_StopTest()
{
    qDebug() << "CDesignSplashView::slot_StopTest" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = m_device->canportAdd();
    msgObjectStatus.nDisID = m_device->distributionAdd();
    msgObjectStatus.nLoopID = m_device->loopAdd();
    msgObjectStatus.nDeviceID = m_device->deviceAdd();

    m_device->setDeviceValue(DEVICE_VALUE_TEST,"正常");
    m_device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"正常");
    m_device->setDeviceControl(false);
    deviceControl devicecontrol;
    devicecontrol.saveDeviceControlFile();//保存灯具控制表
    CGlobal::instance()->DlgDevices()->getDeviceInfo();

    m_device->setDeviceValue(DEVICE_VALUE_LEFTCLOSE,0);
    m_device->setDeviceValue(DEVICE_VALUE_RIGHTCLOSE,0);

    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceReset, &msgObjectStatus);

    //布局视图界面灯具状态更新
    int layoutPageID = CGlobal::instance()->designTreeView()->CurrentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    setPrimitiveItem(layoutPageID);
}
//灯具应急开始
void CDesignSplashView::slot_StartEmergency()
{
    qDebug() << "CDesignSplashView::slot_StartEmergency" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    if(m_device->getDeviceCommunicationFault())
        return;

    m_device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"应急");
    m_device->setDeviceControl(true);
    deviceControl devicecontrol;
    devicecontrol.saveDeviceControlFile();//保存灯具控制表
    CGlobal::instance()->DlgDevices()->getDeviceInfo();

    //布局视图界面灯具状态更新
    int layoutPageID = CGlobal::instance()->designTreeView()->CurrentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    setPrimitiveItem(layoutPageID);

    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = m_device->canportAdd();
    msgObjectStatus.nDisID = m_device->distributionAdd();
    msgObjectStatus.nLoopID = m_device->loopAdd();
    msgObjectStatus.nDeviceID = m_device->deviceAdd();

    //标志灯开始闪烁
    if(CGlobal::instance()->programDeviceView()->type(m_device->deviceTypeId()) != NCT_LightType)
    {
        msgObjectStatus.nValue = 1;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_FlashControl, &msgObjectStatus);
    }
    else
    {
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceEmergencyStart, &msgObjectStatus);
    }
//    //上传灯具状态到云平台/客户端
//    CGlobal::instance()->ClientBusiness()->uploadLampStatus(m_device);
}
//灯具应急停止
void CDesignSplashView::slot_StopEmergency()
{
    qDebug() << "CDesignSplashView::slot_StopEmergency" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    if(m_device->getDeviceCommunicationFault())
        return;
    m_device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"正常");
    if(m_device->deviceValue(DEVICE_VALUE_TEST) == "正常")
    {
        m_device->setDeviceControl(false);
        deviceControl devicecontrol;
        devicecontrol.saveDeviceControlFile();//保存灯具控制表
    }

    CGlobal::instance()->DlgDevices()->getDeviceInfo();
    //布局视图界面灯具状态更新
    int layoutPageID = CGlobal::instance()->designTreeView()->CurrentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    setPrimitiveItem(layoutPageID);

    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = m_device->canportAdd();
    msgObjectStatus.nDisID = m_device->distributionAdd();
    msgObjectStatus.nLoopID = m_device->loopAdd();
    msgObjectStatus.nDeviceID = m_device->deviceAdd();

    //标志灯停止闪烁
    if(CGlobal::instance()->programDeviceView()->type(m_device->deviceTypeId()) != NCT_LightType)
    {
        msgObjectStatus.nValue = 0;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_FlashControl, &msgObjectStatus);
    }
    else
    {
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceEmergencyStop, &msgObjectStatus);
    }
//    //上传灯具状态到云平台/客户端
//    CGlobal::instance()->ClientBusiness()->uploadLampStatus(m_device);
}

void CDesignSplashView::slot_CloseBatteryOut()
{
    qDebug() << "CDesignSplashView::slot_CloseBatteryOut" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nCanportAdd = m_device->canportAdd();
    msgobjectstatus.nDisID = m_device->distributionAdd();
    msgobjectstatus.nLoopID = m_device->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();
    msgobjectstatus.nValue = true;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_CloseBatteryOut, &msgobjectstatus);
}

void CDesignSplashView::slot_BatteryOutKeep()
{
    qDebug() << "CDesignSplashView::slot_BatteryOutKeep" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nCanportAdd = m_device->canportAdd();
    msgobjectstatus.nDisID = m_device->distributionAdd();
    msgobjectstatus.nLoopID = m_device->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();
    msgobjectstatus.nValue = false;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_CloseBatteryOut, &msgobjectstatus);
}

void CDesignSplashView::slot_DeviceInfo()
{
    qDebug() << "CDesignSplashView::slot_DeviceInfo" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_device)
        return;
    CLoop* loop = m_device->loop();
    if(!loop)
        return;
    CDistribution* distribution = loop->distribution();
    if(!distribution)
        return;

    CGlobal::instance()->m_HaveDialog = true;
    QFont font;
    font.setPointSize(12); // 设置字体大小为12像素
    // 在QMessageBox中设置字体
    QMessageBox messageBox;
    messageBox.setFont(font);
    QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
    messageBox.setStyleSheet(styleSheet);
    messageBox.setIcon(QMessageBox::NoIcon);
    messageBox.setWindowTitle("灯具信息");
    messageBox.setStandardButtons(QMessageBox::Ok);
    messageBox.button(QMessageBox::Ok)->setText("确定");
    QString log;
    log = QString::number(distribution->distributionAddress()) + tr("-") + QString::number(loop->loopAdd()) + tr("-") +
            QString::number(m_device->deviceAdd()) + tr("\n") + m_device->deviceValue(DEVICE_VALUE_TYPE).toString() + tr("\n") +
           "区域:" + m_device->deviceValue(DEVICE_VALUE_AREA).toString() + tr("\n") +
           "位置:" + m_device->deviceValue(DEVICE_VALUE_LOCATION).toString();
    messageBox.setText(log);
    messageBox.exec();
    CGlobal::instance()->m_HaveDialog = false;

}
