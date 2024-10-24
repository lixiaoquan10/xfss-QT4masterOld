#include "cprogramtreeview.h"
#include "style/cstyledbar.h"
#include "cprogramview.h"
#include "cprogramcontainer.h"
#include "cprogramcontrollerview.h"
#include "cprogramcanportview.h"
#include "cprogramcandeviceview.h"
#include "cprogramdistributionview.h"
#include "cglobal.h"

CProgramTreeView::CProgramTreeView(QWidget *parent) :
    QWidget(parent)
{
    CGlobal::instance()->setProgramTreeView(this);
    setMinimumWidth(300);

    QVBoxLayout* pMainLayout = new QVBoxLayout();
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(0);
    m_controllerTreeView = new CControllerTreeView(this);
    connect(m_controllerTreeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(slot_TreeViewClicked(const QModelIndex &)));
    m_controllerTreeModel = new CControllerTreeModel();
    m_controllerTreeView->setModel(m_controllerTreeModel);
    pMainLayout->addWidget(m_controllerTreeView);
    setLayout(pMainLayout);
    isTableShow = false;
    m_style = new CMyIconStyle();
    m_strSheet = "QMenu {background-color: rgb(255, 246, 143); margin: 3px; font-size: 14px;}\
            QMenu::item {padding: 3px 15px 3px 15px; border: 1px solid transparent; min-height: 25px; min-width: 30px; border-radius: 4px;}\
            QMenu::separator {height: 2px; background: #CCCCFF; margin-left: 5px; margin-right: 5px;}\
            QMenu::right-arrow { image: url(:/res/right-arrow.png); }\
            QMenu::indicator:exclusive:checked {image: url(:/res/symbol-check.png);}\
            QMenu::item:selected {background-color: qlineargradient(x1: 0.5, y1: 0, x2: 0.5, y2: 1, stop: 0 #99CCFF, stop: 0.5 #CCFFFF, stop: 1 #99CCFF); border-color: #6699CC;}";

}

CProgramTreeView::~CProgramTreeView()
{
    CGlobal::instance()->setProgramTreeView(0);
    delete m_controllerTreeModel;
    delete m_controllerTreeView;
}

// 递归函数，保存节点及其子节点的展开状态
void CProgramTreeView::saveExpandState(const QModelIndex &parentIndex)
{
    expandStates[parentIndex] = m_controllerTreeView->isExpanded(parentIndex);

    // 递归保存子节点的展开状态
    int rows = m_controllerTreeModel->rowCount(parentIndex);
    for (int row = 0; row < rows; ++row) {
        QModelIndex childIndex = m_controllerTreeModel->index(row, 0, parentIndex);
        saveExpandState(childIndex);
    }
}

void CProgramTreeView::resetControllerTreeView()
{
    qDebug() << "CProgramTreeView::resetControllerTreeView" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 保存当前选择的索引
    QModelIndex currentIndex = m_controllerTreeView->currentIndex();
    // 保存滚动条的位置
    int verticalScrollBarValue = m_controllerTreeView->verticalScrollBar()->value();
    // 遍历所有节点，保存其展开状态
    QModelIndex rootIndex = m_controllerTreeView->rootIndex();
    saveExpandState(rootIndex);
    //重置树形结构
    m_controllerTreeView->reset();
//    // 展开所有节点
//    m_controllerTreeView->expandAll();
    // 恢复选择的索引
    if (currentIndex.isValid() && currentIndex.model() == m_controllerTreeView->model()) {
        // 索引仍然有效，恢复选择
        m_controllerTreeView->setCurrentIndex(currentIndex);
    } else {
        // 索引无效或者不再存在，清除选择
        m_controllerTreeView->clearSelection();
    }
    // 恢复节点的展开状态和选择
    QMap<QModelIndex, bool>::const_iterator it;
    for (it = expandStates.constBegin(); it != expandStates.constEnd(); ++it) {
        const QModelIndex &index = it.key();
        bool expanded = it.value();
        if (expanded) {
            m_controllerTreeView->setExpanded(index, true);
        }
    }
    // 恢复滚动条的位置
    m_controllerTreeView->verticalScrollBar()->setValue(verticalScrollBarValue);
}

void CProgramTreeView::setController(CController *controller)
{
    qDebug() << "CProgramTreeView::setController" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(controller == NULL) return;
    m_controllerTreeModel->setController(controller);
    m_controllerTreeView->expandAll();
}

void CProgramTreeView::newController()
{
    qDebug() << "CProgramTreeView::newController" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    qDebug() << "CProgramTreeView::openController" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
//    int deviceTotal = 0;
//    for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
//    {
//        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
//        //导入数据库后获取灯具数量，此时该CAN地址下灯具总数量等于在线数量
//        deviceTotal = deviceTotal + canport->getDeviceOnlineTotal();
//    }
//    CGlobal::instance()->controller()->setDeviceTotal(deviceTotal);
}

void CProgramTreeView::closeController()
{
    qDebug() << "CProgramTreeView::closeController" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_controllerTreeModel->setController(NULL);
    CGlobal::instance()->programControllerView()->setController(NULL);
    CGlobal::instance()->programCanportView()->setCanport(NULL);
    CGlobal::instance()->programCanDeviceView()->setDistribution(NULL);
    CGlobal::instance()->programDeviceView()->setLoop(NULL);
    CGlobal::instance()->designTreeView()->setController(NULL);
//    CGlobal::instance()->designMainView()->closeAll();
    CGlobal::instance()->dm()->clear();
//    CGlobal::instance()->designMainView()->clearViewScene();
    CGlobal::instance()->programContainer()->setCurrentView(0);
    CGlobal::instance()->designTreeView()->setCurrentView(0);
//    CGlobal::instance()->designContainer()->setCurrentView(0);
}

void CProgramTreeView::saveController()
{
    qDebug() << "CProgramTreeView::saveController" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    CGlobal::instance()->setMsgText("系统运行！");
    CGlobal::instance()->InformationWindow()->hide();
    messageBox.setText(log);
    messageBox.exec();
}

void CProgramTreeView::slotNewController()
{
    qDebug() << "CProgramTreeView::slotNewController" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    newController();
}

void CProgramTreeView::slotOpenController()
{
    qDebug() << "CProgramTreeView::slotOpenController" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    openController();
    CGlobal::instance()->saveSetting();
}

void CProgramTreeView::slotCloseController()
{
    qDebug() << "CProgramTreeView::slotCloseController" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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

void CProgramTreeView::slot_TreeViewClicked(const QModelIndex &index)
{
    qDebug() << "CProgramTreeView::slotTreeViewClicked" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
        CCanport* canport = static_cast<CCanport*>(object);
        CGlobal::instance()->programCanportView()->setCanport(static_cast<CCanport*>(object));
        //当前选中的不是集中电源也不是回路
        CGlobal::instance()->m_isSelectDistribution = false;
        CGlobal::instance()->m_isSelectLoop = false;
        CGlobal::instance()->m_nCanportAddress = canport->canportAdd();
        CGlobal::instance()->programContainer()->setCurrentView(2);
    }
    else if(type == CDistribution::type_s)
    {
        CDistribution* distribution = static_cast<CDistribution*>(object);
        if(CGlobal::instance()->m_isSelectDistribution && (CGlobal::instance()->m_nDistriAddress == distribution->distributionAddress()))
        {
            if(m_menuTable && !m_menuTable->isHidden())
            {
                delete m_menuTable;
                m_menuTable = NULL;
            }
            else
            {
                if(m_menuTable) delete m_menuTable;
                m_menuTable = createTableMenu(true);
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
        else
        {
            CGlobal::instance()->programDistributionView()->setDistribution(distribution);
            //当前选中的为集中电源
            CGlobal::instance()->m_isSelectDistribution = true;
            CGlobal::instance()->m_isSelectLoop = false;
            //获取集中电源地址
            CGlobal::instance()->m_nDistriAddress = distribution->distributionAddress();
            CGlobal::instance()->m_nLoopAddress = 0;
            CGlobal::instance()->m_nCanportAddress = distribution->canportAddress();
            CGlobal::instance()->programContainer()->setCurrentView(3);
        }
    }
    else if(type == CLoop::type_s)
    {
        CLoop* loop = static_cast<CLoop*>(object);
        if(CGlobal::instance()->m_isSelectLoop && (CGlobal::instance()->m_nLoopAddress == loop->loopAdd()))
        {
            if(m_menuTable && !m_menuTable->isHidden())
            {
                delete m_menuTable;
                m_menuTable = NULL;
            }
            else
            {
                if(m_menuTable) delete m_menuTable;
                m_menuTable = createTableMenu(false);
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
        else
        {
            CGlobal::instance()->programDeviceView()->setLoop(static_cast<CLoop*>(object));
            //当前选中的为回路
            CGlobal::instance()->m_isSelectDistribution = false;
            CGlobal::instance()->m_isSelectLoop = true;
            //获取集中电源回路地址
            CGlobal::instance()->m_nDistriAddress = loop->distribution()->distributionAddress();
            CGlobal::instance()->m_nLoopAddress = loop->loopAdd();
            CGlobal::instance()->m_nCanportAddress = loop->canportAdd();
            CGlobal::instance()->programContainer()->setCurrentView(4);
        }
    }
    else
    {
        //当前选中的不是集中电源也不是回路
        CGlobal::instance()->m_isSelectDistribution = false;
        CGlobal::instance()->m_isSelectLoop = false;
        CGlobal::instance()->programContainer()->setCurrentView(0);
    }
}


QMenu* CProgramTreeView::createTableMenu(bool isDistribution)
{
    qDebug() << "CProgramTreeView::createTableMenu" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    isTableShow = true;
    QMenu *tableMenu = new QMenu(this);
    tableMenu->setStyleSheet(m_strSheet);
    tableMenu->setStyle(m_style);
    if(isDistribution)
    {
        tableMenu->addAction("设备单个注册", CGlobal::instance()->programview(), SLOT(slot_regLoop()));
        tableMenu->addAction("设备单个清除注册", CGlobal::instance()->programview(), SLOT(slot_unLoginLoop()));
        tableMenu->addSeparator();
        tableMenu->addAction("应急启动", this, SLOT(slot_emergencyStart()));
        tableMenu->addAction("应急停止", this, SLOT(slot_emergencyStop()));
        tableMenu->addSeparator();
        tableMenu->addAction("读取设备下灯具的软件版本", this, SLOT(slot_ReadLampVersion()));
        tableMenu->addSeparator();
        tableMenu->addAction("从注册文件中删除该设备下灯具", this, SLOT(slot_removeLampLogin()));
        tableMenu->addSeparator();
    }
    else
    {
        tableMenu->addAction("回路单个注册", CGlobal::instance()->programview(), SLOT(slot_regLoop()));
        tableMenu->addAction("回路单个清除注册", CGlobal::instance()->programview(), SLOT(slot_unLoginLoop()));
        tableMenu->addSeparator();
        tableMenu->addAction("应急启动", this, SLOT(slot_emergencyStart()));
        tableMenu->addAction("应急停止", this, SLOT(slot_emergencyStop()));
        tableMenu->addSeparator();
        tableMenu->addAction("读取回路下灯具的软件版本", this, SLOT(slot_ReadLampVersion()));
        tableMenu->addSeparator();
    }
    return tableMenu;
}
void CProgramTreeView::slot_emergencyStart()
{
    qDebug() << "CProgramTreeView::slot_emergencyStart" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nDisID = CGlobal::instance()->m_nDistriAddress;
    msgobjectstatus.nLoopID = CGlobal::instance()->m_nLoopAddress;
    msgobjectstatus.nValue = true;
    msgobjectstatus.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_Emergency, &msgobjectstatus);
}

void CProgramTreeView::slot_emergencyStop()
{
    qDebug() << "CProgramTreeView::slot_emergencyStart" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgObjectStatus msgobjectstatus;
    msgobjectstatus.nDisID = CGlobal::instance()->m_nDistriAddress;
    msgobjectstatus.nLoopID = CGlobal::instance()->m_nLoopAddress;
    msgobjectstatus.nValue = false;
    msgobjectstatus.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_Emergency, &msgobjectstatus);
}

//读软件版本
void CProgramTreeView::slot_ReadLampVersion()
{
    qDebug() << "CProgramTreeView::slot_ReadLampVersion" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgObjectStatus msgObjectStatus;
    msgObjectStatus.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
    msgObjectStatus.nDisID = CGlobal::instance()->m_nDistriAddress;
    msgObjectStatus.nLoopID = CGlobal::instance()->m_nLoopAddress;
    msgObjectStatus.nDeviceID = 0;

    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_ReadDeviceInfo, &msgObjectStatus);
}

void CProgramTreeView::slot_removeLampLogin()
{
    qDebug() << "CProgramTreeView::slot_removeLampLogin" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QFont font;
    font.setPointSize(12); // 设置字体大小为12像素
    // 在QMessageBox中设置字体
    QMessageBox messageBox;
    messageBox.setFont(font);
    QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
    messageBox.setStyleSheet(styleSheet);
    messageBox.setIcon(QMessageBox::NoIcon);
    messageBox.setWindowTitle("提示");
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Cancel);
    // 设置按钮显示文字
    messageBox.button(QMessageBox::Yes)->setText("是");
    messageBox.button(QMessageBox::Cancel)->setText("取消");
    QString log;
    log = "确定从注册信息中删除该集中电源下的灯具信息吗?";
    messageBox.setText(log);
    int ret = messageBox.exec();
    CMsgObjectStatus msg;;
    //获取当前点击的集中电源地址
    msg.nDisID = CGlobal::instance()->m_nDistriAddress;
    msg.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
    msg.nLoopID = 0;
    if(msg.nDisID == 0){
        return;
    }
    if(ret == QMessageBox::Cancel)
    {
        return;
    }
    else if(ret == QMessageBox::Yes)
    {
        QMessageBox messageBox;
        messageBox.setFont(font);
        QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
        messageBox.setStyleSheet(styleSheet);
        messageBox.setIcon(QMessageBox::NoIcon);
        messageBox.setWindowTitle("提示");
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        messageBox.setDefaultButton(QMessageBox::Cancel);
        // 设置按钮显示文字
        messageBox.button(QMessageBox::Yes)->setText("是");
        messageBox.button(QMessageBox::No)->setText("否");
        QString log;
        log = "是否要给集中电源发送命令，清除回路板中的离线灯具？";
        messageBox.setText(log);
        int ret = messageBox.exec();
        if(ret == QMessageBox::Yes)
        {
            CGlobal::instance()->ClientBusiness()->exeCommand(NCT_UnLoginLoop, &msg);
        }
        CGlobal::instance()->ClientBusiness()->removeLoginObject(CGlobal::instance()->m_nCanportAddress, CGlobal::instance()->m_nDistriAddress,0);
    }
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_RemoveFaultLamp, &msg);
    ConfigFileManager cfg;
    cfg.SaveConfigFile();//写入配置文件
}

