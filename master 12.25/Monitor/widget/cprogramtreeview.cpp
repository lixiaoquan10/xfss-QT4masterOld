#include "cprogramtreeview.h"
#include "style/cstyledbar.h"
#include "cprogramcontainer.h"
#include "cprogramprojectview.h"
#include "cprogramcontrollerview.h"
#include "cprogramloopview.h"
#include "cprogramdistributionview.h"
#include "cprogramdeviceview.h"
#include "cdesigncontainer.h"
#include "view/cprojecttreeview.h"
#include "model/cprojecttreemodel.h"
#include "dm/cproject.h"
#include "dm/ccontroller.h"
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
    m_projectTreeView = new CProjectTreeView(this);
    QObject::connect(m_projectTreeView, SIGNAL(sigSelectionChanged(QItemSelection,QItemSelection)), this, SLOT(slotSelectionChanged(QItemSelection,QItemSelection)));
    m_projectTreeModel = new CProjectTreeModel();
    m_projectTreeView->setModel(m_projectTreeModel);
    pMainLayout->addWidget(m_projectTreeView);
    setLayout(pMainLayout);
}

CProgramTreeView::~CProgramTreeView()
{
    CGlobal::instance()->setProgramTreeView(0);
    delete m_projectTreeModel;
    delete m_projectTreeView;
}

void CProgramTreeView::setCurrentView(int index)
{
    Q_UNUSED(index);
//    m_layout->setCurrentIndex(index);
}

void CProgramTreeView::setProject(CProject *project)
{
    if(project == NULL) return;
    m_projectTreeModel->setProject(project);
    m_projectTreeView->expandAll();
}

void CProgramTreeView::newProject()
{
    CProject* project = new CProject();
    CGlobal::instance()->dm()->addProject(project);
    CGlobal::instance()->dm()->setCreateTime(QDateTime::currentDateTime());
    m_projectTreeModel->setProject(project);
    setCurrentView(1);
    CGlobal::instance()->designTreeView()->setProject(project);
    CGlobal::instance()->designTreeView()->setCurrentView(1);
//    CGlobal::instance()->setProjectName("");
}

void CProgramTreeView::openProject()
{
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    QFileInfo info(fileName);
    if(!info.exists())
        return;  
    if(CGlobal::instance()->dm()->load(fileName) != 0)
    {
        QMessageBox::information(NULL, tr("Information"), tr("Open project failure"), QMessageBox::Ok);
        return;
    }

    CProject* project = CGlobal::instance()->dm()->projectAt(0);
    setProject(project);
    setCurrentView(1);
    CGlobal::instance()->designTreeView()->setProject(project);
//    CGlobal::instance()->designTreeView()->setCurrentView(1);
}

void CProgramTreeView::closeProject()
{
    m_projectTreeModel->setProject(NULL);
    CGlobal::instance()->programProjectView()->setProject(NULL);
    CGlobal::instance()->programControllerView()->setController(NULL);
    CGlobal::instance()->programDistributionView()->setController(NULL);
    CGlobal::instance()->programLoopView()->setDistribution(NULL);
    CGlobal::instance()->programDeviceView()->setLoop(NULL);
    CGlobal::instance()->designTreeView()->setProject(NULL);
//    CGlobal::instance()->designMainView()->closeAll();
    CGlobal::instance()->dm()->clear();
//    CGlobal::instance()->designMainView()->clearViewScene();
    setCurrentView(0);
    CGlobal::instance()->programContainer()->setCurrentView(0);
    CGlobal::instance()->designTreeView()->setCurrentView(0);
    CGlobal::instance()->designContainer()->setCurrentView(0);
}

void CProgramTreeView::saveProject()
{
    m_projectTreeView->setFocus();      // Exit edit data
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

void CProgramTreeView::slotNewProject()
{
    newProject();
}

void CProgramTreeView::slotOpenProject()
{
    openProject();
    CGlobal::instance()->saveSetting();
}

void CProgramTreeView::slotCloseProject()
{
    if(CGlobal::instance()->dm()->isModify())
        if (QMessageBox::information(this, "信息", "工程已修改，是否保存？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            slotSaveProject();
    closeProject();
}

void CProgramTreeView::slotSaveProject()
{
    saveProject();
}

void CProgramTreeView::slotExportProject()
{

}

void CProgramTreeView::slotSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    if(selected.isEmpty())
        return;
    QModelIndex index = selected.indexes().at(0);
    CObject* object = m_projectTreeModel->getObject(index);
    if(!object)
        return;
    QString type = object->type();
    if(type == CProject::type_s)
    {
        //当前选中的不是集中电源也不是回路
        CGlobal::instance()->m_isSelectDistribution = false;
        CGlobal::instance()->m_isSelectLoop = false;
        CGlobal::instance()->programProjectView()->setProject(static_cast<CProject*>(object));
        CGlobal::instance()->programContainer()->setCurrentView(1);
    }
    else if(type == CController::type_s) /* just display loop*/
    {
        //当前选中的不是集中电源也不是回路
        CGlobal::instance()->m_isSelectDistribution = false;
        CGlobal::instance()->m_isSelectLoop = false;
        CGlobal::instance()->programControllerView()->setController(static_cast<CController*>(object));
        CGlobal::instance()->programContainer()->setCurrentView(2);
    }
    else if(type == CDistribution::type_s)
    {
        //当前选中的为集中电源
        CGlobal::instance()->m_isSelectDistribution = true;
        CGlobal::instance()->m_isSelectLoop = false;
        //获取集中电源地址
        CGlobal::instance()->m_nDistriAddress = object->value(DISTRIBUTION_VALUE_ADDR).toInt();
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
        CGlobal::instance()->programContainer()->setCurrentView(0);
}

