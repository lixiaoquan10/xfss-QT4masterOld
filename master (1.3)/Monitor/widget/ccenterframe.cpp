#include "ccenterframe.h"
#include "cglobal.h"
#include "cprogramview.h"
#include "dm/cdm.h"
#include "dm/cdevice.h"
#include "widget/cdesigntreeview.h"
#include "widget/cdesigncontainer.h"
#include "model/cinformationmodel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QToolButton>

CCenterFrame::CCenterFrame(QFrame *parent) :
    QSplitter(parent)
{
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_vSplitter = new QSplitter(Qt::Vertical, this);

    createLeftBox();
    createRightBox();

    addWidget(m_leftBox);
    setHandleWidth(2);
    addWidget(m_vSplitter);
}

CCenterFrame::~CCenterFrame()
{
    delete m_vSplitter;
    delete m_topLabel;
    delete m_leftBox;
    delete m_leftLayout;
    delete m_leftWidget;
    delete m_tabWidget;

}

void CCenterFrame::createLeftBox()
{
    QHBoxLayout *topLayout = new QHBoxLayout;
    m_topLabel = new QLabel;
    m_topLabel->setFixedHeight(30);
    m_topLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);


    m_topButton = createToolButton();
    m_topButton->setFixedHeight(30);
    m_topButton->setFixedWidth(30);
    m_topButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_topButton->setArrowType(Qt::LeftArrow);
    m_leftIsVisable = true;
    connect(m_topButton, SIGNAL(clicked(bool)),this,SLOT(slotToggleLeftBox()));

    topLayout->addWidget(m_topLabel);
    topLayout->addWidget(m_topButton);

    QVBoxLayout *layout = new QVBoxLayout;
    CDesignTreeView* treeView = new CDesignTreeView(this);
    treeView->setFixedWidth(treeView->sizeHint().width());
    treeView->setStyleSheet("QTreeView {background-color: rgb(215, 220, 225);}");
    layout->addWidget(treeView);
//    layout->addWidget(CGlobal::instance()->taProcess());

    m_leftBox  = new QGroupBox;
//    m_leftBox->setMaximumWidth(260);
    m_leftBox->setFixedWidth(CGlobal::instance()->m_screenWidth * 0.25);
    m_leftBox->setMouseTracking(true);
    m_leftBox->setStyleSheet("QGroupBox {background-color: rgb(215, 220, 225);}");
    m_leftBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_leftBox->setLayout(layout);

//    m_leftWidget = new QWidget(this);
//    m_leftLayout = new QVBoxLayout;
//    m_leftLayout->addLayout(topLayout);
//    m_leftLayout->addWidget(m_leftBox);
//    m_leftWidget->setLayout(m_leftLayout);

}

void CCenterFrame::createRightBox()
{/*
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->hide();

    createTab(tr("Start"));
    createTab(tr("Failure"));
    createTab(tr("Undefined"));*/

    CDesignContainer* designContainer = new CDesignContainer;
    m_vSplitter->addWidget(designContainer);
    m_vSplitter->setHandleWidth(1);
//    m_vSplitter->addWidget(m_tabWidget);
    QList<int> lsSize;
    lsSize<<this->width()*2;lsSize<<this->width()/2;
    m_vSplitter->setSizes(lsSize);
}

void CCenterFrame::createTab(const QString &name)
{
    QTableView *tableView = new QTableView(this);
    m_tableView.insert(name, tableView);
    m_tabList.append(name);//按顺序保存状态页名，以便根据序号索引获取页面名

    m_informationModel.insert(name, new CInformationModel);
    tableView->setModel(m_informationModel.value(name));
    tableView->setColumnWidth(0, 130);
    tableView->setColumnWidth(1, 300);
    tableView->setColumnWidth(2, 115);
    tableView->setColumnWidth(3, 245);
    tableView->setColumnWidth(4, 130);
    tableView->setColumnWidth(5, 100);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotDoubleClicked(QModelIndex)));

    QWidget *tab = new QWidget(this);
    QGridLayout *m_TabLayout = CGlobal::instance()->CreateGridLayout(tab);
    m_TabLayout->addWidget(tableView);
    m_tabWidget->addTab(tab, name);
}

void CCenterFrame::slotDoubleClicked(QModelIndex index)
{
    QString tabName = m_tabWidget->tabText(m_tabWidget->currentIndex());
    tabName = tabName.section('(', 0, 0);
    int keyId = m_informationModel.value(tabName)->getStatusInfo(index.row()).keyId;
    CObject* m_Object = CGlobal::instance()->dm()->object(keyId);

    if(m_Object->type() == CDevice::type_s)
    {
        CDevice* device = static_cast<CDevice*>(m_Object);
        if(device)
        {
//            CPointDeviceItem* devicePointItem = reinterpret_cast<CPointDeviceItem*>(device->value(DEVICE_VALUE_ITEM_POINTER).toLongLong());
//            CGlobal::instance()->designMainView()->searchDevice(devicePointItem);
        }
    }
//    else if(m_Object->type() == CAlarmPoint::type_s)
//    {
//        CAlarmPoint* alarm = static_cast<CAlarmPoint*>(m_Object);
//        if(alarm)
//        {
////            CPointAlarmPointItem* alarmPoint = reinterpret_cast<CPointAlarmPointItem*>(alarm->value(ALARMPOINT_VALUE_ITEM_POINTER).toLongLong());
////            CGlobal::instance()->designMainView()->searchDevice(alarmPoint);
//        }
//    }
}

void CCenterFrame::slotToggleLeftBox()
{
    if (m_leftIsVisable)    /*hide*/
    {

        m_leftBox->hide();

        m_leftIsVisable = false;
    }
    else
    {
        m_leftBox->show();
        m_leftIsVisable = true;
    }
}



QToolButton* CCenterFrame::createToolButton()
{

    QToolButton *toolbutton = new QToolButton;
    toolbutton->setFixedSize(30,30);
    toolbutton->setArrowType(Qt::LeftArrow);
    return toolbutton;
}
