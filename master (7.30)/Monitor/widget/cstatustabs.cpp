#include "cglobal.h"


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
    tableView->setColumnWidth(1, 80);
    tableView->setColumnWidth(2, 60);
    tableView->setColumnWidth(3, 60);
    tableView->setColumnWidth(4, 220);
    tableView->setColumnWidth(5, 220);
    tableView->setColumnWidth(6, 220);
    tableView->setColumnWidth(7, 240);
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
    m_tabWidget->setGeometry(0,536,1366,175);
    m_tabWidget->setFixedHeight(175);
}
//跳转到布局视图界面位置
void CStatusTabs::slot_tableviewClicked(const QModelIndex &index)
{
    qDebug() << "CStatusTabs::slot_tableviewClicked" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString canData = m_informationModel.value("故障")->index(index.row(),1).data().toString();
    int dashIndex = canData.indexOf('-');
    int canAddress = canData.mid(3, dashIndex - 3).toInt() + 2;
    int distributionAddress = canData.mid(dashIndex + 1).toInt();
    int loopAddress = m_informationModel.value("故障")->index(index.row(),2).data().toInt();
    int deviceAddress = m_informationModel.value("故障")->index(index.row(),3).data().toInt();
    if(!deviceAddress)
        return;
    CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canAddress);
    if(!canport)
        return;
    CDistribution* distribution = canport->distributionByAddress(distributionAddress);
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

                // 替换界面
                CGlobal::instance()->TopToolBar()->changeCenterFrameWidget(CGlobal::instance()->CenterFrame());

                CLayer* layer = CGlobal::instance()->controller()->clayerById(layoutID);
                CGlobal::instance()->designTreeView()->setLayer(layer);
                //布局视图界面灯具状态更新
                CGlobal::instance()->designSplashView()->sliderReset();
                CGlobal::instance()->designSplashView()->ScrollBarChange(valueX,valueY);
//                CGlobal::instance()->designSplashView()->setPageStateUpdate(true);
            }
        }
    }

}
