#include "cprogramtreeview.h"
#include "style/cstyledbar.h"
#include "cprogramcontainer.h"
#include "cprogramprojectview.h"
#include "cprogramcanportview.h"
#include "cprogramloopview.h"
#include "cprogramdistributionview.h"
#include "cprogramdeviceview.h"
#include "cdesigncontainer.h"
#include "view/cprojecttreeview.h"
#include "model/cprojecttreemodel.h"
#include "dm/cproject.h"
#include "dm/ccanport.h"
#include "dm/cdistribution.h"
#include "dm/cloop.h"
#include "dm/clayer.h"
#include "dm/cdm.h"
#include "model/ctreeitem.h"
#include "cglobal.h"
#include "dialog/dlgdevices.h"
#include "cdesigntreeview.h"
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QContextMenuEvent>
#include <QInputDialog>

CProgramTreeView::CProgramTreeView(QWidget *parent) :
    QWidget(parent)
{
    CGlobal::instance()->setProgramTreeView(this);
    setMinimumWidth(300);

    QVBoxLayout* pMainLayout = new QVBoxLayout();
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(0);
    m_controllerTreeView = new CControllerTreeView(this);
    QObject::connect(m_controllerTreeView, SIGNAL(sigSelectionChanged(QItemSelection,QItemSelection)), this, SLOT(slotSelectionChanged(QItemSelection,QItemSelection)));
    m_controllerTreeModel = new CControllerTreeModel();
    m_controllerTreeView->setModel(m_controllerTreeModel);
    pMainLayout->addWidget(m_controllerTreeView);
    setLayout(pMainLayout);
}

CProgramTreeView::~CProgramTreeView()
{
    CGlobal::instance()->setProgramTreeView(0);
    delete m_controllerTreeModel;
    delete m_controllerTreeView;
}

void CProgramTreeView::resetControllerTreeView()
{
    if(CGlobal::instance()->m_isSelectLoop)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportById(CGlobal::instance()->panelAddress());
        CDistribution* distribution = canport->distributionByAddress(CGlobal::instance()->m_nDistriAddress);
        CLoop* loop = distribution->loopByAdd(CGlobal::instance()->m_nLoopAddress);
        CGlobal::instance()->programDeviceView()->setUpdateEnble();
        CGlobal::instance()->programDeviceView()->setLoop(loop);
        CGlobal::instance()->programContainer()->setCurrentView(4);
    }
    // 保存当前选择的索引
    QModelIndex currentIndex = m_controllerTreeView->currentIndex();
    //重置树形结构
    m_controllerTreeView->reset();
    // 展开所有节点
    m_controllerTreeView->expandAll();
    // 恢复之前选择的索引
    m_controllerTreeView->setCurrentIndex(currentIndex);

}

void CProgramTreeView::setController(CController *controller)
{
    if(controller == NULL) return;
    m_controllerTreeModel->setController(controller);
    m_controllerTreeView->expandAll();
}

void CProgramTreeView::newController()
{
    CController* controller = new CController();
    CGlobal::instance()->dm()->addController(controller);
    CGlobal::instance()->dm()->setCreateTime(QDateTime::currentDateTime());
    m_controllerTreeModel->setController(controller);
    CGlobal::instance()->designTreeView()->setController(controller);
    CGlobal::instance()->designTreeView()->setCurrentView(1);
//    CGlobal::instance()->setControllerName("");
}

void CProgramTreeView::openController()
{
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    QFileInfo info(fileName);
    if(!info.exists())
        return;  
    if(CGlobal::instance()->dm()->load(fileName) != 0)
    {
        QMessageBox::information(NULL, tr("Information"), tr("Open controller failure"), QMessageBox::Ok);
        return;
    }
    CController* controller = CGlobal::instance()->dm()->controllerAt(0);
    setController(controller);
    CGlobal::instance()->designTreeView()->setController(controller);
    CCanport* canport = CGlobal::instance()->controller()->canportById(CGlobal::instance()->panelAddress());
    //导入数据库后获取灯具数量，此时灯具总数量等于在线数量
    canport->setDeviceTotal();
}

void CProgramTreeView::closeController()
{
    m_controllerTreeModel->setController(NULL);
    CGlobal::instance()->programControllerView()->setController(NULL);
    CGlobal::instance()->programCanportView()->setCanport(NULL);
    CGlobal::instance()->programDistributionView()->setCanport(NULL);
    CGlobal::instance()->programLoopView()->setDistribution(NULL);
    CGlobal::instance()->programDeviceView()->setLoop(NULL);
    CGlobal::instance()->designTreeView()->setController(NULL);
//    CGlobal::instance()->designMainView()->closeAll();
    CGlobal::instance()->dm()->clear();
//    CGlobal::instance()->designMainView()->clearViewScene();
    CGlobal::instance()->programContainer()->setCurrentView(0);
    CGlobal::instance()->designTreeView()->setCurrentView(0);
    CGlobal::instance()->designContainer()->setCurrentView(0);
}

void CProgramTreeView::saveController()
{
    m_controllerTreeView->setFocus();      // Exit edit data
    CGlobal::instance()->dm()->setUpdateTime(QDateTime::currentDateTime());
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
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
    if(CGlobal::instance()->dm()->save(fileName) != 0)
    {
        log = "保存失败！";
    }
    else
    {
        log = "保存成功，请重启软件！(主菜单中“系统”->“重启”)";
    }
    messageBox.setText(log);
    messageBox.exec();
}

void CProgramTreeView::slotNewController()
{
    newController();
}

void CProgramTreeView::slotOpenController()
{
    openController();
    CGlobal::instance()->saveSetting();
}

void CProgramTreeView::slotCloseController()
{
    if(CGlobal::instance()->dm()->isModify())
        if (QMessageBox::information(this, "信息", "工程已修改，是否保存？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            slotSaveController();
    closeController();
}

void CProgramTreeView::slotSaveController()
{
    saveController();
}

void CProgramTreeView::slotExportController()
{

}

void CProgramTreeView::slotSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    if(selected.isEmpty())
        return;
    QModelIndex index = selected.indexes().at(0);
    CObject* object = m_controllerTreeModel->getObject(index);
    if(!object)
        return;
    QString type = object->type();
    if(type == CController::type_s)
    {
        //当前选中的不是集中电源也不是回路
        CGlobal::instance()->m_isSelectDistribution = false;
        CGlobal::instance()->m_isSelectLoop = false;
        CGlobal::instance()->programControllerView()->setController(static_cast<CController*>(object));
        CGlobal::instance()->programContainer()->setCurrentView(1);
    }
    else if(type == CCanport::type_s) /* just display loop*/
    {
        //当前选中的不是集中电源也不是回路
        CGlobal::instance()->m_isSelectDistribution = false;
        CGlobal::instance()->m_isSelectLoop = false;
        CGlobal::instance()->programCanportView()->setCanport(static_cast<CCanport*>(object));
        CGlobal::instance()->programContainer()->setCurrentView(2);
    }
    else if(type == CDistribution::type_s)
    {
        CDistribution* distribution = static_cast<CDistribution*>(object);
        //当前选中的为集中电源
        CGlobal::instance()->m_isSelectDistribution = true;
        CGlobal::instance()->m_isSelectLoop = false;
        //获取集中电源地址
        CGlobal::instance()->m_nDistriAddress = distribution->distributionValue(DISTRIBUTION_VALUE_ADDR).toInt();
        CGlobal::instance()->m_nLoopAddress = 0;
        qDebug() << CGlobal::instance()->m_nDistriAddress;
        CGlobal::instance()->programDistributionView()->setDistribution(static_cast<CDistribution*>(object));
        CGlobal::instance()->programContainer()->setCurrentView(3);
    }
    else if(type == CLoop::type_s)
    {
        CGlobal::instance()->programDeviceView()->setLoop(static_cast<CLoop*>(object));
        //当前选中的为回路
        CGlobal::instance()->m_isSelectDistribution = false;
        CGlobal::instance()->m_isSelectLoop = true;
        //获取集中电源回路地址
        CGlobal::instance()->m_nDistriAddress = CGlobal::instance()->programDeviceView()->loop()->distribution()->distributionAddress();
        CGlobal::instance()->m_nLoopAddress = CGlobal::instance()->programDeviceView()->loop()->loopAdd();
        CGlobal::instance()->programContainer()->setCurrentView(4);
    }
    else
    {
        //当前选中的不是集中电源也不是回路
        CGlobal::instance()->m_isSelectDistribution = false;
        CGlobal::instance()->m_isSelectLoop = false;
        CGlobal::instance()->programContainer()->setCurrentView(0);
    }
}

