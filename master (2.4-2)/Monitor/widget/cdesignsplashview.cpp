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
#include "dialog/dlgdevices.h"
#include "cmyiconstyle.h"

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
    m_verticalSlider->setMinimum(50);
    m_verticalSlider->setMaximum(150);
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
    pLeftLayout->addWidget(m_enlarge);
    pLeftLayout->addLayout(sliderLayout);
    pLeftLayout->addWidget(m_reduction);
    pMainLayout->addLayout(pLeftLayout);
    isHideSlider(true);

    m_pView = new QLabel();
    m_pView->setPixmap(QPixmap(":/images/main.png"));
    m_pView->setStyleSheet("background-color: rgb(255, 255, 255);");
    m_pView->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_pView->setScaledContents(true);
    m_pView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_scrollArea = new QScrollArea;
    m_scrollArea->setWidgetResizable(true); // 让滚动区域自适应窗口大小
    m_scrollArea->setWidget(m_pView);

    // 连接滚动事件
    connect(m_scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onVerticalScroll(int)));
    connect(m_scrollArea->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onHorizontalScroll(int)));

    pMainLayout->addWidget(m_scrollArea);

    m_isEnblePageStateUpdate = false;

    //定时1s更新布局视图（更新标志使能情况下）
    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_setPrimitiveItem()));
    m_timer->start(1000);

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
}
//页面坐标跳转
void CDesignSplashView::ScrollBarChange(int valueX, int valueY)
{
    m_scrollArea->horizontalScrollBar()->setValue(valueX);
    m_scrollArea->verticalScrollBar()->setValue(valueY);
}

// 滑块滑动
void CDesignSplashView::slot_spliderValueChanged(int value)
{
    QString str = QString::number(value) + "%";
    m_label->setText(str);
    m_previousScrollX = m_previousScrollX*value/m_changeValue;
    m_previousScrollY = m_previousScrollY*value/m_changeValue;
    m_changeValue = value;
    m_isChanged = true;
    m_isEnblePageStateUpdate = true;
    slot_setPrimitiveItem();
    ScrollBarChange(m_previousScrollX, m_previousScrollY);
}

// 放大
void CDesignSplashView::slot_enlarge()
{
    if(m_verticalSlider->value() + 10 <= m_verticalSlider->maximum())
        m_verticalSlider->setValue(m_verticalSlider->value()+10);
    else
        m_verticalSlider->setValue(m_verticalSlider->maximum());
    slot_spliderValueChanged(m_verticalSlider->value());
}

//缩小
void CDesignSplashView::slot_reduction()
{
    if(m_verticalSlider->value() - 10 >= m_verticalSlider->minimum())
        m_verticalSlider->setValue(m_verticalSlider->value()-10);
    else
        m_verticalSlider->setValue(m_verticalSlider->minimum());
    slot_spliderValueChanged(m_verticalSlider->value());
}

// 滚动事件处理函数
void CDesignSplashView::onVerticalScroll(int value) {
    m_previousScrollY = value;
}

void CDesignSplashView::onHorizontalScroll(int value) {
    m_previousScrollX = value;
}

//设置默认图片
void CDesignSplashView::setMainPicture()
{
    m_pView->setPixmap(QPixmap(":/images/main.png"));
    isHideSlider(true);
}
//设置布局视图
void CDesignSplashView::setPixmap(QPixmap background)
{
    isHideSlider(false);
    int minValue = std::max(100*m_scrollArea->width()/background.width(), 100*m_scrollArea->height()/background.height());
    m_verticalSlider->setMinimum(minValue);
    QPixmap scaledPixmap;
    if(m_isChanged)
        scaledPixmap = background.scaled(background.width()*m_changeValue/100, background.width()*m_changeValue/100, Qt::KeepAspectRatio);
    else
        scaledPixmap = background;
    m_pView->setPixmap(scaledPixmap);
}
//
void CDesignSplashView::isHideSlider(bool istrue)
{
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
    if(!CGlobal::instance()->designTreeView()->CurrentLayer())
        return;
    int currentlayoutPageID = CGlobal::instance()->designTreeView()->CurrentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    // 加载main.png
    QString pictureName = CGlobal::instance()->designTreeView()->currentLayer()->layerValue(LAYER_VALUE_BACKGROUNDPICTURE).toString();
    QString filePath = CGlobal::instance()->workspaces() + "/" + "PageBackgroundFolder/" + pictureName;
    QPixmap background(filePath);

    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    CDBSqlite db(fileName);
    if(!db.open())
        return;
    QString str = QString("SELECT l.ID,l.LampInfoID,l.LayoutPageID,l.CX,l.CY,l.Height,l.Angle,m.Number,r.Address,c.Loop,a.CanPort,a.Address from Coordinate l "
                              "left join LampInfo r on l.LampInfoID=r.id "
                              "left join LampType m on r.LampTypeID=m.id "
                              "left join ChannelInfo c on r.ChannelInfoID = c.ID "
                              "left join CanDeviceInfo a on c.CanDeviceInfoID = a.ID "
                              "where l.LayoutPageID=%1 "
                              ).arg(layerID);

    QSqlQuery query = db.query(str);
    QSqlRecord record = query.record();
    int fieldLayoutPageID = record.indexOf("LayoutPageID");
    int fieldCX = record.indexOf("CX");
    int fieldCY = record.indexOf("CY");
    int fieldHeight = record.indexOf("Height");
    int fieldAngle = record.indexOf("Angle");
    int fieldNumber = record.indexOf("Number");
    while(query.next())
    {
        //8:r.Address灯具地址    9:c.Loop回路地址   10:a.CanPort CAN地址  11:a.Address集中电源地址
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(query.value(10).toInt());
        if(!canport)
            continue;
        CDistribution* distribution = canport->distributionByAddress(query.value(11).toInt());
        if(!distribution)
            continue;
        CLoop* loop = distribution->loopByAdd(query.value(9).toInt());
        if(!loop)
            continue;
        CDevice* device = loop->deviceByAdd(query.value(8).toInt());
        if(!device)
            continue;
//        int LampInfoID = query.value(fieldLampInfoID).toInt();
        int LayoutPageID = query.value(fieldLayoutPageID).toInt();
        double CX = query.value(fieldCX).toDouble();
        double CY = query.value(fieldCY).toDouble();
        double Height = query.value(fieldHeight).toDouble();
        double Angle = query.value(fieldAngle).toDouble();
        int Number = query.value(fieldNumber).toInt();
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
    if(CGlobal::instance()->m_isEmergency)
    {
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
            CCanport* canport1 = CGlobal::instance()->controller()->canportByAddress(query.value(6).toInt());
            if(!canport1)
                continue;
            CDistribution* distribution1 = canport1->distributionByAddress(queryConnection.value(7).toInt());
            if(!distribution1)
                continue;
            CLoop* loop1 = distribution1->loopByAdd(queryConnection.value(5).toInt());
            if(!loop1)
                continue;
            StartLamp = loop1->deviceByAdd(queryConnection.value(4).toInt());
            CCanport* canport2 = CGlobal::instance()->controller()->canportByAddress(query.value(10).toInt());
            if(!canport2)
                continue;
            CDistribution* distribution2 = canport2->distributionByAddress(queryConnection.value(11).toInt());
            if(!distribution2)
                continue;
            CLoop* loop2 = distribution2->loopByAdd(queryConnection.value(9).toInt());
            if(!loop2)
                continue;
            EndLamp = loop2->deviceByAdd(queryConnection.value(8).toInt());
            if(StartLamp->deviceValue(DEVICE_VALUE_LAYOUTPAGEID).toInt() == currentlayoutPageID)
                setLampConnectionArrow(&background, StartLamp, EndLamp, queryConnection.value(2).toInt());
        }
    }

    db.close();
    m_pView->clear();
    setPixmap(background);
}



QPointF CDesignSplashView::rotatePoint(const QPointF& point, const QPointF& center, double angle)
{
    double radian = angle * M_PI / 180.0;
    double x = center.x() + (point.x() - center.x()) * std::cos(radian) - (point.y() - center.y()) * std::sin(radian);
    double y = center.y() + (point.x() - center.x()) * std::sin(radian) + (point.y() - center.y()) * std::cos(radian);
    return QPointF(x, y);
}

void CDesignSplashView::mouseMoveEvent(QMouseEvent* event)
{
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

void CDesignSplashView::mousePressEvent(QMouseEvent* event) {
    QWidget::mousePressEvent(event);
    // 点击事件，只能时滑动时间的开始
    m_moveEnd = false;

    if(CGlobal::instance()->m_UserType == CGlobal::User)
        return;
    if(!CGlobal::instance()->designTreeView()->CurrentLayer())
        return;
    //当前页面
    int pageID = CGlobal::instance()->designTreeView()->currentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    // 获取鼠标点击位置
    QPoint pos = event->pos();
    //实际坐标需要加上页面滑动距离  以及放大缩小滑块占用位置
    double pos_x = pos.x() + m_previousScrollX - 35;
    double pos_y = pos.y() + m_previousScrollY;
    //实际点击的坐标
    QPointF point(pos_x,pos_y);

    for(int i=3; i<=6; i++)
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
    //                            if(pos_x >= device_x1 && pos_x <= device_x2 && pos_y >= device_y1 && pos_y <= device_y2)
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
}

QMenu* CDesignSplashView::createTableMenu(CDevice* device)
{
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
            m_left->setIcon(QIcon(":/images/ok.png"));
        }
        else if(m_device->deviceValue(DEVICE_VALUE_DEFAULTDIRECTION) == 1)
        {
            m_right->setIcon(QIcon(":/images/ok.png"));
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

//灯具连接线
void CDesignSplashView::setLampConnectionArrow(QPixmap* background, CDevice* startLamp, CDevice* endLamp, int joint)
{
    double startX = startLamp->deviceValue(DEVICE_VALUE_CX).toDouble();
    double startY = startLamp->deviceValue(DEVICE_VALUE_CY).toDouble();
    double endX = endLamp->deviceValue(DEVICE_VALUE_CX).toDouble();
    double endY = endLamp->deviceValue(DEVICE_VALUE_CY).toDouble();
    double startHeight = startLamp->deviceValue(DEVICE_VALUE_HEIGHT).toDouble();
    double endHeight = endLamp->deviceValue(DEVICE_VALUE_HEIGHT).toDouble();
    double startAngle = startLamp->deviceValue(DEVICE_VALUE_ANGEL).toDouble();
    double endAngle = endLamp->deviceValue(DEVICE_VALUE_ANGEL).toDouble();

    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // 保存当前坐标系统
    painter.save();
//    // 缩放坐标系统
//    painter.scale(startHeight/40, startHeight/40);
    QPointF startPoint[8];
    QPointF endPoint[8];
    // 绘制箭头
    if(startLamp->deviceTypeId() >= 120 && startLamp->deviceTypeId() <= 123)
    {
        QPointF start[8] = {
            QPointF(startX + 20*startHeight/40, startY + 20*startHeight/40),
            QPointF(startX + 20*startHeight/40, startY + 20*startHeight/40),
            QPointF(startX + 20*startHeight/40, startY + 20*startHeight/40),
            QPointF(startX + 20*startHeight/40, startY + 20*startHeight/40),
            QPointF(startX + 20*startHeight/40, startY + 20*startHeight/40),
            QPointF(startX + 20*startHeight/40, startY + 20*startHeight/40),
            QPointF(startX + 20*startHeight/40, startY + 20*startHeight/40),
            QPointF(startX + 20*startHeight/40, startY + 20*startHeight/40)
        };
        for (int i = 0; i < 8; ++i) {
            startPoint[i] = start[i];
        }
    }
    else
    {
        // 计算矩形中心点的坐标
        QPointF center(startX + 36*startHeight/40, startY + 20*startHeight/40);
        QPointF start[8] = {
            QPointF(startX + 36*startHeight/40, startY),
            QPointF(startX + 72*startHeight/40, startY + 20*startHeight/40),
            QPointF(startX + 36*startHeight/40, startY + 40*startHeight/40),
            QPointF(startX, startY + 20*startHeight/40),
            QPointF(startX, startY),
            QPointF(startX + 72*startHeight/40, startY),
            QPointF(startX + 72*startHeight/40, startY + 40*startHeight/40),
            QPointF(startX, startY + 40*startHeight/40)
        };
        for (int i = 0; i < 8; ++i) {
            startPoint[i] = rotatePoint(start[i], center, startAngle);
        }
    }
    if(endLamp->deviceTypeId() >= 120 && endLamp->deviceTypeId() <= 123)
    {
        QPointF end[8] = {
            QPointF(endX + 20*endHeight/40, endY + 20*endHeight/40),
            QPointF(endX + 20*endHeight/40, endY + 20*endHeight/40),
            QPointF(endX + 20*endHeight/40, endY + 20*endHeight/40),
            QPointF(endX + 20*endHeight/40, endY + 20*endHeight/40),
            QPointF(endX + 20*endHeight/40, endY + 20*endHeight/40),
            QPointF(endX + 20*endHeight/40, endY + 20*endHeight/40),
            QPointF(endX + 20*endHeight/40, endY + 20*endHeight/40),
            QPointF(endX + 20*endHeight/40, endY + 20*endHeight/40)
        };
        for (int i = 0; i < 8; ++i) {
            endPoint[i] = end[i];
        }
    }
    else
    {
        // 计算矩形中心点的坐标
        QPointF center1(endX + 36*endHeight/40, endY + 20*endHeight/40);
        QPointF end[8] = {
            QPointF(endX + 36*endHeight/40, endY + 40*endHeight/40),
            QPointF(endX + 36*endHeight/40, endY),
            QPointF(endX + 72*endHeight/40, endY + 20*endHeight/40),
            QPointF(endX, endY + 20*endHeight/40),
            QPointF(endX + 72*endHeight/40, endY),
            QPointF(endX, endY),
            QPointF(endX + 72*endHeight/40, endY + 40*endHeight/40),
            QPointF(endX, endY + 40*endHeight/40)

        };
        for (int i = 0; i < 8; ++i) {
            endPoint[i] = rotatePoint(end[i], center1, endAngle);
        }
    }

    // 找到距离最短的两个点
    double minDistance = std::max(background->width(),background->height());
    QPointF p1, p2;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            double deltaX = startPoint[i].x() - endPoint[j].x();
            double deltaY = startPoint[i].y() - endPoint[j].y();
            double distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);
            if (distance < minDistance) {
                minDistance = distance;
                p1 = startPoint[i];
                p2 = endPoint[j];
            }
        }
    }
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
    double pointRotationAngle = std::atan2(p2.y() - p1.y(), p2.x() - p1.x()) * 180 / M_PI;

    QPointF line[5];

    QPointF arrowPoints[5] = {
        QPointF(p1.x() - 1 + minDistance, p1.y() + 2.5),
        QPointF(p1.x() + 1, p1.y() + 2.5),
        QPointF(p1.x() + 1, p1.y() - 2.5),
        QPointF(p1.x() - 1 + minDistance, p1.y() - 2.5),
        QPointF(p1.x() - 1 + minDistance, p1.y() + 2.5)
    };
    for (int i = 0; i < 5; ++i) {
        line[i] = rotatePoint(arrowPoints[i], p1, pointRotationAngle);
    }

    if(((startLamp->deviceValue(DEVICE_VALUE_LEFTCLOSE) == 1) && (joint == 1)) ||
        ((startLamp->deviceValue(DEVICE_VALUE_RIGHTCLOSE) == 1) && (joint == 2)))
        painter.setBrush(QBrush(QColor(255,255,255)));  // 设置填充颜色为透明
    else
        painter.setBrush(QBrush(QColor(205,51,51)));  // 设置填充颜色为Brown3
    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawPolygon(line, 5);

    //判断箭头方向
    QPointF arrow[5];
    if(joint == 2)
    {
        QPointF arrowPoints[5] = {
            QPointF(p1.x() - 1 + minDistance, p1.y() + 2.5),
            QPointF(p1.x() - 1 + minDistance - std::sqrt(3.0) * (10 + 5 * std::sqrt(3.0)) * 0.5,
                   p1.y() + 2.5 + (10 + 5 * std::sqrt(3.0)) * 0.5),
            QPointF(p1.x() - 1 + minDistance - std::sqrt(3.0) * 10 * 0.5 - 10,
                   p1.y() + 2.5 + 10 * 0.5),
            QPointF(p1.x() - 1 + minDistance - 10, p1.y() + 2.5),
            QPointF(p1.x() - 1 + minDistance, p1.y() + 2.5)
        };
        for (int i = 0; i < 5; ++i) {
            arrow[i] = rotatePoint(arrowPoints[i], p1, pointRotationAngle);
        }
    }
    else if(joint == 1)
    {
        QPointF arrowPoints[5] = {
            QPointF(p1.x() - 1 + minDistance, p1.y() - 2.5),
            QPointF(p1.x() - 1 + minDistance - std::sqrt(3.0) * (10 + 5 * std::sqrt(3.0)) * 0.5,
                   p1.y() - 2.5 - (10 + 5 * std::sqrt(3.0)) * 0.5),
            QPointF(p1.x() - 1 + minDistance - std::sqrt(3.0) * 10 * 0.5 - 10,
                   p1.y() - 2.5 - 10 * 0.5),
            QPointF(p1.x() - 1 + minDistance - 10, p1.y() - 2.5),
            QPointF(p1.x() - 1 + minDistance, p1.y() - 2.5)
        };
        for (int i = 0; i < 5; ++i) {
            arrow[i] = rotatePoint(arrowPoints[i], p1, pointRotationAngle);
        }
    }
    if(((startLamp->deviceValue(DEVICE_VALUE_LEFTCLOSE) == 1) && (joint == 1)) ||
        ((startLamp->deviceValue(DEVICE_VALUE_RIGHTCLOSE) == 1) && (joint == 2)))
        painter.setBrush(QBrush(QColor(255,255,255)));  // 设置填充颜色为透明
    else
//        painter.setBrush(QBrush(QColor(173,255,47)));  // 设置填充颜色为GreenYellow
        painter.setBrush(QBrush(QColor(205,51,51)));  // 设置填充颜色为Brown3
    painter.setPen(QPen(Qt::NoPen));  // 设置边框颜色为透明
    painter.drawPolygon(arrow, 5);
    // 恢复保存的坐标系统
    painter.restore();
}

//双向标志灯
void CDesignSplashView::setArrowPrimitiveItem(QPixmap* background, CDevice* device)
{
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble();
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble();
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble();
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
    if(leftClose == 1 || ((defaultDirection & 0x01) == 0x01 && emergency == "正常"))  //默认指向为右向或者全灭并且处于非应急状态
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
    if(rightClose == 1 || ((defaultDirection & 0x02) == 0x02 && emergency == "正常"))  //默认指向为左向或者全灭并且处于非应急状态
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
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble();
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble();
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble();
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
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble();
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble();
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble();
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
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble();
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble();
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble();
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
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble();
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble();
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble();
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
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble();
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble();
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble();
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
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble();
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble();
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble();
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
    painter.drawText(rect, Qt::AlignCenter, "安全出口");  // 在长方形内居中绘制文字

    // 恢复保存的坐标系统
    painter.restore();
}

//照明灯
void CDesignSplashView::setLightPrimitiveItem(QPixmap* background, CDevice* device)
{
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble();
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble();
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble();
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
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble();
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble();
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble();
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
    // 在背景上绘制图案
    QPainter painter(background);
    painter.setRenderHint(QPainter::Antialiasing, true);

    double x = device->deviceValue(DEVICE_VALUE_CX).toDouble();
    double y = device->deviceValue(DEVICE_VALUE_CY).toDouble();
    double height = device->deviceValue(DEVICE_VALUE_HEIGHT).toDouble();
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
    //上传灯具指向到云平台
    CGlobal::instance()->ClientBusiness()->XmlUploadLampDirection(
                CGlobal::instance()->saveKeyId(OBJT_Device, m_device->keyId()),"左向");
    //服务端上传灯具指向
    CGlobal::instance()->ClientBusiness()->serverLampDirectionUpload(
                CGlobal::instance()->saveKeyId(OBJT_Device, m_device->keyId()),"左向");
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDirection, &msgobjectstatus);

    //布局视图界面灯具状态更新
    int layoutPageID = CGlobal::instance()->designTreeView()->CurrentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    setPrimitiveItem(layoutPageID);

}

//标志灯右向
void CDesignSplashView::slot_Right()
{
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
    //上传灯具指向到云平台
    CGlobal::instance()->ClientBusiness()->XmlUploadLampDirection(
                CGlobal::instance()->saveKeyId(OBJT_Device, m_device->keyId()),"右向");
    //服务端上传灯具指向
    CGlobal::instance()->ClientBusiness()->serverLampDirectionUpload(
                CGlobal::instance()->saveKeyId(OBJT_Device, m_device->keyId()),"右向");
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDirection, &msgobjectstatus);

    //布局视图界面灯具状态更新
    int layoutPageID = CGlobal::instance()->designTreeView()->CurrentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    setPrimitiveItem(layoutPageID);

}

//标志灯全亮
void CDesignSplashView::slot_AllBright()
{
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
    //上传灯具指向到云平台
    CGlobal::instance()->ClientBusiness()->XmlUploadLampDirection(
                CGlobal::instance()->saveKeyId(OBJT_Device, m_device->keyId()),"全亮");
    //服务端上传灯具指向
    CGlobal::instance()->ClientBusiness()->serverLampDirectionUpload(
                CGlobal::instance()->saveKeyId(OBJT_Device, m_device->keyId()),"全亮");
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDirection, &msgobjectstatus);

    //布局视图界面灯具状态更新
    int layoutPageID = CGlobal::instance()->designTreeView()->CurrentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    setPrimitiveItem(layoutPageID);
}

//标志灯全灭
void CDesignSplashView::slot_AllClose()
{
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
    //上传灯具指向到云平台
    CGlobal::instance()->ClientBusiness()->XmlUploadLampDirection(
                CGlobal::instance()->saveKeyId(OBJT_Device, m_device->keyId()),"全灭");
    //服务端上传灯具指向
    CGlobal::instance()->ClientBusiness()->serverLampDirectionUpload(
                CGlobal::instance()->saveKeyId(OBJT_Device, m_device->keyId()),"全灭");
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceDirection, &msgobjectstatus);

    //布局视图界面灯具状态更新
    int layoutPageID = CGlobal::instance()->designTreeView()->CurrentLayer()->layerValue(LAYER_VALUE_ID).toInt();
    setPrimitiveItem(layoutPageID);
}
//灯具默认左向
void CDesignSplashView::slot_DefaultLeft()
{
    if(!m_device)
        return;
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nCanportAdd = m_device->canportAdd();
    msgobjectstatus.nDisID = m_device->distributionAdd();
    msgobjectstatus.nLoopID = m_device->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();
    msgobjectstatus.nValue = 0X05;
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
    if(!m_device)
        return;
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nCanportAdd = m_device->canportAdd();
    msgobjectstatus.nDisID = m_device->distributionAdd();
    msgobjectstatus.nLoopID = m_device->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();
    msgobjectstatus.nValue = 0X06;
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
    query.bindValue(":isused", m_device->deviceValue(DEVICE_VALUE_ISUSED));

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
    query.bindValue(":isused", m_device->deviceValue(DEVICE_VALUE_ISUSED));

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
    if(!m_device)
        return;
    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = m_device->canportAdd();
    msgObjectStatus.nDisID = m_device->distributionAdd();
    msgObjectStatus.nLoopID = m_device->loopAdd();
    msgObjectStatus.nDeviceID = m_device->deviceAdd();

    m_device->setDeviceValue(DEVICE_VALUE_TEST,"测试");
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceTestStart, &msgObjectStatus);

}
//灯具测试停止
void CDesignSplashView::slot_StopTest()
{
    if(!m_device)
        return;
    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = m_device->canportAdd();
    msgObjectStatus.nDisID = m_device->distributionAdd();
    msgObjectStatus.nLoopID = m_device->loopAdd();
    msgObjectStatus.nDeviceID = m_device->deviceAdd();

    m_device->setDeviceValue(DEVICE_VALUE_TEST,"正常");
    m_device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"正常");
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
    if(!m_device)
        return;
    if(m_device->getDeviceCommunicationFault())
        return;

    m_device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"应急");
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
    //上传灯具状态到云平台/客户端
    CGlobal::instance()->ClientBusiness()->uploadLampStatus(m_device);
}
//灯具应急停止
void CDesignSplashView::slot_StopEmergency()
{
    if(!m_device)
        return;
    if(m_device->getDeviceCommunicationFault())
        return;
    m_device->setDeviceValue(DEVICE_VALUE_EMERGENCY,"正常");
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
    //上传灯具状态到云平台/客户端
    CGlobal::instance()->ClientBusiness()->uploadLampStatus(m_device);
}

void CDesignSplashView::slot_CloseBatteryOut()
{
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
    QString log;
    log = QString::number(distribution->distributionAddress()) + tr("-") + QString::number(loop->loopAdd()) + tr("-") +
            QString::number(m_device->deviceAdd()) + tr("\n") + m_device->deviceValue(DEVICE_VALUE_TYPE).toString() + tr("\n") +
           "区域:" + m_device->deviceValue(DEVICE_VALUE_AREA).toString() + tr("\n") +
           "位置:" + m_device->deviceValue(DEVICE_VALUE_LOCATION).toString();
    messageBox.setText(log);
    messageBox.exec();
    CGlobal::instance()->m_HaveDialog = false;

}
