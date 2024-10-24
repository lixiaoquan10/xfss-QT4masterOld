#include "widget.h"
#include "cglobal.h"
#include "groupbox.h"
#include "dm/cdm.h"
#include "dm/clayer.h"
#include "dm/cdevice.h"
#include "dm/cloop.h"
#include "dm/cdistribution.h"

#include "dialog/dlgadministrator.h"
#include "dialog/dlgsystemrecord.h"
#include "dialog/dlglogininformation.h"
#include "dialog/dlginformationwindow.h"
#include "dialog/dlgdevices.h"
#include "dialog/dlgtimeinterval.h"
#include "business/cclientbusiness.h"
#include "business/cdataprocessing.h"
#include "widget/ccenterframe.h"
#include "widget/cbottomtoolbar.h"
#include "widget/ctoptoolbar.h"
#include "widget/cdesigntreeview.h"
#include "widget/cprogramloopview.h"
#include "dm/cproject.h"
#include "widget/cstatustabs.h"

#include <QSettings>
#include <QState>
#include <QDesktopWidget>
#include <QApplication>
#include <QProcess>
#include <QBoxLayout>
#include <QShortcut>
#include <QDebug>
#include <QStackedWidget>
#include <QHeaderView>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_iDelayCut = 0;
    CGlobal::instance()->m_widget = this;

    CGlobal::instance()->setLoginInformation(new dlgLoginInformation());
    CGlobal::instance()->loginInformation()->setWindowTitle(tr("The registered device information"));
    CGlobal::instance()->loginInformation()->hide();
    CGlobal::instance()->setStatusTabs(new CStatusTabs());
    CGlobal::instance()->setClientBusiness(new CClientBusiness);
    CGlobal::instance()->setInformationWindow(new dlgInformationWindow);
    CGlobal::instance()->setDlgDevices(new dlgDevices);
    CGlobal::instance()->setCenterFrameContainer(new QStackedWidget);
    CGlobal::instance()->setCenterFrame(new CCenterFrame);
    CGlobal::instance()->setSystemRecord(new dlgSystemRecord);
    CGlobal::instance()->setProgramLoopView(new CProgramLoopView);
    CGlobal::instance()->setTimeInterval(new dlgTimeInterval);
    CGlobal::instance()->centerFrameContainer()->insertWidget(0, CGlobal::instance()->CenterFrame());
    CGlobal::instance()->centerFrameContainer()->insertWidget(1,CGlobal::instance()->dlgDeviceInfomation());
    CGlobal::instance()->centerFrameContainer()->insertWidget(2,CGlobal::instance()->SystemRecord());
    CGlobal::instance()->centerFrameContainer()->insertWidget(3,CGlobal::instance()->programLoopView());
    CGlobal::instance()->centerFrameContainer()->insertWidget(4,CGlobal::instance()->loginInformation());
    CGlobal::instance()->centerFrameContainer()->insertWidget(5,CGlobal::instance()->TimeInterval());
    CGlobal::instance()->centerFrameContainer()->setCurrentIndex(0);
    CGlobal::instance()->setBottomToolBar(new CBottomToolBar);
    CGlobal::instance()->setTopToolBar(new CTopToolBar);
    CGlobal::instance()->setDataProcessing(new CDataProcessing());
    CGlobal::instance()->ClientBusiness()->InitProject();
    CGlobal::instance()->ClientBusiness()->startCommunication();
    CGlobal::instance()->ClientBusiness()->setNewPwd(CGlobal::instance()->m_strPassWord);


    topBox = new GroupBox;
    topBox->setMaximumHeight(75);
    topBox->setMinimumHeight(75);
    topLayout = new QGridLayout;
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->addWidget(CGlobal::instance()->TopToolBar());
    topBox->setLayout(topLayout);

    bottomBox = new GroupBox;
    bottomBox->setMaximumHeight(32);
    bottomBox->setMinimumHeight(32);
    bottomLayout = new QGridLayout;
    bottomLayout->setContentsMargins(0, 0, 0, 0);
    bottomLayout->addWidget(CGlobal::instance()->BottomToolBar());
    bottomBox->setLayout(bottomLayout);

    QVBoxLayout *mainLayout = CGlobal::instance()->CreateVBoxLayout(this);
    mainLayout->setContentsMargins(1, 1, 1, 1);
    mainLayout->setSpacing(1);
//    mainLayout->addWidget(topBox, 2);
//    mainLayout->addWidget(CGlobal::instance()->centerFrameContainer(), 30);
//    mainLayout->addWidget(CGlobal::instance()->statusTabs()->m_tabWidget, 5);
//    mainLayout->addWidget(bottomBox, 1);
    mainLayout->addWidget(topBox);
    mainLayout->addWidget(CGlobal::instance()->centerFrameContainer());
    mainLayout->addWidget(CGlobal::instance()->statusTabs()->m_tabWidget);
    mainLayout->addWidget(bottomBox);
    createTimer();
    setGeometry(0,0,QApplication::desktop()->screen()->width(), QApplication::desktop()->screen()->height());
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint); // ??????
//    setWindowState(Qt::WindowFullScreen);
    CGlobal::instance()->m_widget->hide();
}

Widget::~Widget()
{
    delete CGlobal::instance()->InformationWindow();
    CGlobal::instance()->setClientBusiness(NULL);
    CGlobal::instance()->m_widget = NULL;
    CGlobal::destroy();
}

void Widget::closeEvent(QCloseEvent *event)
{
    event->ignore();
}

void Widget::createTimer()
{
    m_timerLED = new QTimer(this);
    connect(m_timerLED, SIGNAL(timeout()), this, SLOT(slotLEDTimer()));
    m_timerLED->start(500);
}


void Widget::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_F)
    {
        CGlobal::instance()->TopToolBar()->slotSearchDevice();
    }
    if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_L)
    {
        if(CGlobal::instance()->SystemRecord() == NULL)
        {
            CGlobal::instance()->setSystemRecord(new dlgSystemRecord(this));
            CGlobal::instance()->SystemRecord()->exec();
        }
    }
}


void Widget::slotLEDTimer()
{
    //?????????
    CGlobal::instance()->BottomToolBar()->setCurrentDateTime();
    if(CGlobal::instance()->m_HaveDialog)
    {
        CGlobal::instance()->m_bMove_Mouse = true;
    }
    //???????§Ý????
    if(CGlobal::instance()->m_bCutLayer != CGlobal::instance()->isAutoSwitchLayer())
    {
        CGlobal::instance()->m_bCutLayer = CGlobal::instance()->isAutoSwitchLayer();
    }
    if(!CGlobal::instance()->m_bMove_Mouse) {
        ++m_iDelayCut;
//        exitAdmin();
//        switchLayer();
    } else {
        m_iDelayCut = 0;
    }
    CGlobal::instance()->m_bMove_Mouse = false;
}


void Widget::switchLayer()
{
    if(m_iDelayCut > 2*CGlobal::instance()->m_nTimeCutLayer)
    {
        m_iDelayCut = 0;
        if(CGlobal::instance()->m_bCutLayer)
        {
            static int m_nLayerNum = -1;
            //??????????
            QList<CLayer*> tempLayer = CGlobal::instance()->getLayers(CGlobal::instance()->dm()->projects());

            if(tempLayer.count()>0)
            {
                ++m_nLayerNum;

                //????§Ý????
                if(m_nLayerNum >= tempLayer.count())
                {
                    m_nLayerNum = 0;
                }

                //????????????????§Ý?
                for(int i=0; i<tempLayer.count(); ++i)
                {
                    //?§Ø????????§Ý????????
                    if(tempLayer.at(m_nLayerNum)->getStatus(OBJS_Fire))
                    {
                        //????????????????????§Ý?
                        if(CGlobal::instance()->designTreeView()->currentLayer() != tempLayer.at(m_nLayerNum))
                        {
                            CGlobal::instance()->setLayer(tempLayer.at(m_nLayerNum));
                        }
                        return;
                    }

                    ++m_nLayerNum;
                    if(m_nLayerNum >= tempLayer.count())
                    {
                        m_nLayerNum = 0;
                    }
                }
                //???????????????????????§Ý?,????????????
                m_nLayerNum = normalSwitch(tempLayer, m_nLayerNum);
            }
        }
    }
}
void Widget::slotEventChange()
{
    CGlobal::instance()->m_bMove_Mouse = true;

}

int Widget::normalSwitch(QList<CLayer*> &listLayer, int num)
{
    CLayer* m_Lay = CGlobal::instance()->designTreeView()->currentLayer();//?????????
    if(m_Lay)//???????????????????§Ö?¦Ë??
    {
        int layNum = listLayer.indexOf(m_Lay);
        if(layNum >= 0)
        {
            num = layNum + 1;
            if(num >= listLayer.count())
            {
                num = 0;
            }
        }
    }
    CGlobal::instance()->setLayer(listLayer.at(num));
    return num;
}


