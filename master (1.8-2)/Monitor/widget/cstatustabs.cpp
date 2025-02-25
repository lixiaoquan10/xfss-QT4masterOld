#include "cstatustabs.h"
#include "cglobal.h"
#include "model/cinformationmodel.h"
#include "view/ctableview.h"
#include "widget/cdesignsplashview.h"
#include "widget/cdesigntreeview.h"
#include "widget/ccenterframe.h"
#include "dm/cproject.h"
#include "dm/clayer.h"
#include "dm/ccontroller.h"
#include "dm/cdistribution.h"
#include "dm/cloop.h"
#include "dm/cdevice.h"
#include <qstackedwidget.h>

#include <QTabWidget>
#include <QTableView>
#include <QGridLayout>
#include <QHeaderView>
#include <QScrollBar>


#include <QMessageBox>

CStatusTabs::CStatusTabs(QWidget *parent) : QWidget(parent)
{
    m_tabWidget = new QTabWidget(this);
    createTab("启动");
    createTab("故障");
    createTab("应急");
}

CStatusTabs::~CStatusTabs()
{
    delete m_tabWidget;
}


void CStatusTabs::createTab(const QString &name)
{
    QFont font;
    font.setPointSize(12);  // 设置字号为12
    CTableView *tableView = new CTableView(this);
    m_tableView.insert(name, tableView);
    m_tabList.append(name);//按顺序保存状态页名，以便根据序号索引获取页面名

    m_informationModel.insert(name, new CInformationModel);
    tableView->setModel(m_informationModel.value(name));
    tableView->setColumnWidth(0, 180);
    tableView->setColumnWidth(1, 90);
    tableView->setColumnWidth(2, 60);
    tableView->setColumnWidth(3, 60);
    tableView->setColumnWidth(4, 250);
    tableView->setColumnWidth(5, 220);
    tableView->setColumnWidth(6, 220);
    tableView->setColumnWidth(7, 200);
    tableView->setAlternatingRowColors(true);
    tableView->horizontalHeader()->setStretchLastSection(true);  
    tableView->horizontalHeader()->setHighlightSections(false);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->verticalScrollBar()->setStyleSheet("QScrollBar{background: rgb(170, 170, 170); width: 20px;}");
    tableView->setStyleSheet("selection-color: rgb(0, 0, 0);");
    if (name == "故障") {
        connect(tableView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(slot_tableviewClicked(const QModelIndex&)));
    }

    QWidget *tab = new QWidget(this);
    QGridLayout *m_TabLayout = CGlobal::instance()->CreateGridLayout(tab);
    tableView->setFont(font);
    m_TabLayout->addWidget(tableView);
    m_tabWidget->addTab(tab, name);
    m_tabWidget->setFont(font);
    m_tabWidget->setGeometry(0,606,1366,130);
}
//跳转到布局视图界面位置
void CStatusTabs::slot_tableviewClicked(const QModelIndex &index)
{
    int distributionAddress = m_informationModel.value("故障")->index(index.row(),1).data().toInt();
    int loopAddress = m_informationModel.value("故障")->index(index.row(),2).data().toInt();
    int deviceAddress = m_informationModel.value("故障")->index(index.row(),3).data().toInt();
    if(!deviceAddress)
        return;
    CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
    if(!controller)
        return;
    CDistribution* distribution = controller->distributionByAddress(distributionAddress);
    if(distribution)
    {
        CLoop* loop = distribution->loopByAdd(loopAddress);
        if(loop)
        {
            CDevice* device = loop->deviceByAdd(deviceAddress);
            if(device)
            {
                int layoutID = device->deviceValue(DEVICE_VALUE_LAYOUTPAGEID).toInt();
                if(!device->isValueContain(DEVICE_VALUE_CX) || !device->isValueContain(DEVICE_VALUE_CY))
                    return;
                double valueX = device->deviceValue(DEVICE_VALUE_CX).toDouble();
                double valueY = device->deviceValue(DEVICE_VALUE_CY).toDouble();

                CGlobal::instance()->centerFrameContainer()->setCurrentIndex(0);
                CLayer* layer = CGlobal::instance()->project()->clayerById(layoutID);
                CGlobal::instance()->designTreeView()->setLayer(layer);
                //布局视图界面灯具状态更新
                CGlobal::instance()->designSplashView()->setPageStateUpdate(true);
                CGlobal::instance()->designSplashView()->ScrollBarChange(valueX,valueY);
            }
        }
    }

}
