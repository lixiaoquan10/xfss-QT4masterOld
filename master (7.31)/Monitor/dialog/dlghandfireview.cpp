#include "dlghandfireview.h"
#include "ui_dlghandfireview.h"
#include "cglobal.h"


dlgHandFireView::dlgHandFireView(QWidget *parent) :
    QDialog(parent), ui(new Ui::dlgHandFireView)
{
    ui->setupUi(this);
    QString styleSheet =
            "QScrollBar:vertical {"
            "    border: 1px solid #999999;"
            "    background: #f0f0f0;"
            "    width: 20px; /* 设置滚动条的宽度 */"
            "    margin: 0px 0px 0px 0px;"
            "}"
            "QScrollBar::handle:vertical {"
            "    background: #666666;"
            "    min-height: 20px; /* 设置滚动条手柄的最小高度 */"
            "}"
            "QScrollBar::add-line:vertical {"
            "    border: 1px solid #999999;"
            "    background: #e0e0e0; /* 按钮的背景颜色 */"
            "    height: 20px; /* 按钮的高度 */"
            "    subcontrol-position: bottom;"
            "    subcontrol-origin: margin;"
            "}"
            "QScrollBar::sub-line:vertical {"
            "    border: 1px solid #999999;"
            "    background: #e0e0e0; /* 按钮的背景颜色 */"
            "    height: 20px; /* 按钮的高度 */"
            "    subcontrol-position: top;"
            "    subcontrol-origin: margin;"
            "}";
    ui->treeWidget->verticalScrollBar()->setStyleSheet(styleSheet);
    ui->pushButton_fire->setStyleSheet("background-color: rgb(255,0,0);font: bold;");
    // 创建根节点
    QTreeWidgetItem *noneItem = new QTreeWidgetItem(ui->treeWidget);
    noneItem->setText(0, "无");
    // 将根节点添加到树形控件中
    ui->treeWidget->addTopLevelItem(noneItem);
    // 创建加载节点数据的线程
    LoadNodeDataThread *thread = new LoadNodeDataThread(this);
    connect(thread, SIGNAL(dataLoaded(QTreeWidgetItem*)), this, SLOT(slot_dataLoaded(QTreeWidgetItem*)));
//    connect(thread, SIGNAL(threadFinished()), this, SLOT(slot_threadFinished()));

    // 启动线程
    thread->start();
}

dlgHandFireView::~dlgHandFireView()
{
    delete ui;
}

void LoadNodeDataThread::run()
{
    qDebug() << "LoadNodeDataThread::run()"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CController* controller = CGlobal::instance()->controller();
    for(int i=0; i<=controller->maxFirePointDeviceAddress(); i++)
    {
        if(controller->isfirePointByDeviceAddress(i))
        {
            // 创建根节点
            QTreeWidgetItem *rootItem = new QTreeWidgetItem();
            rootItem->setText(0, QString::number(i));
            for (int j = 0; j <= controller->maxFirePointLoopAddress(); j++)
            {
                if(controller->isfirePointByLoopAddress(i,j))
                {
                    // 加载第一层子节点
                    QTreeWidgetItem *circuitNode = new QTreeWidgetItem(rootItem);
                    circuitNode->setText(0, QString::number(j));

                    for (int m = 0; m <= controller->maxFirePointAddress(); m++)
                    {
                        CFirePoint* firepoint = controller->firePointByAddress(i,j,m);
                        if(firepoint)
                        {
                            // 添加第二层子节点
                            QTreeWidgetItem *addressNode = new QTreeWidgetItem(circuitNode);
                            QString str = QString::number(i) + "-" + QString::number(j) + "-" + QString::number(m);
                            QString area = firepoint->firePointValue(FIREPOINT_VALUE_AREA).toString();
                            QString location = firepoint->firePointValue(FIREPOINT_VALUE_LOCATION).toString();
                            if(area != "" || location != "")
                            {
                                str = str + "(" + area + location + ")";
                            }
                            addressNode->setText(0, str);
                        }
                    }
                }
            }
            // 将根节点添加到树形控件中
            emit dataLoaded(rootItem);
        }
    }
    // 发送信号，将节点传递给主线程
//    emit threadFinished();
}

void dlgHandFireView::slot_dataLoaded(QTreeWidgetItem *rootItem)
{
    qDebug() << "dlgHandFireView::slot_dataLoaded"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    ui->treeWidget->addTopLevelItem(rootItem);
}

void dlgHandFireView::slot_threadFinished()
{
    qDebug() << "dlgHandFireView::slot_threadFinished"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    // 展开所有节点
    ui->treeWidget->expandAll();
}

void dlgHandFireView::on_pushButton_close_clicked()
{
    qDebug() << "dlgHandFireView::on_pushButton_close_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
   close();
}

void dlgHandFireView::on_pushButton_fire_clicked()
{
    qDebug() << "dlgHandFireView::on_pushButton_fire_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CController* controller = CGlobal::instance()->controller();
    // 在这里处理按钮点击事件逻辑
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(!item)
        return;
    QString str = item->text(0);
    if(str == "无")
    {
        CGlobal::instance()->m_isEmergency = true;
        CMsgNull m_MsgNUll;
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_CancelMute, &m_MsgNUll);
        CGlobal::instance()->DataProcessing()->setSoundIcon(true);
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_SimulateFire, &m_MsgNUll);
        CGlobal::instance()->ClientBusiness()->Emergency();
    }
    else
    {
        // 找到括号的位置
        int startIndex = str.indexOf('(');
        int endIndex = str.indexOf(')');

        // 如果找到了括号，就去除括号及其内部的内容
        if (startIndex != -1 && endIndex != -1 && startIndex < endIndex) {
            str = str.left(startIndex) + str.mid(endIndex + 1);
        }
        QStringList list = str.split("-");
        if(list.size() == 3)
        {
            int i = list[0].toInt();
            int j = list[1].toInt();
            int m = list[2].toInt();
            CFirePoint* firepoint = controller->firePointByAddress(i,j,m);
            if(firepoint)
            {
                CMsgNull m_MsgNUll;
                CGlobal::instance()->ClientBusiness()->exeCommand(NCT_CancelMute, &m_MsgNUll);
                CGlobal::instance()->DataProcessing()->setSoundIcon(true);
                CGlobal::instance()->ClientBusiness()->exeCommand(NCT_SimulateFire, &m_MsgNUll);
                if(CGlobal::instance()->m_emergencyOnlyDistribution)
                {
                    CGlobal::instance()->ClientBusiness()->firePointEmergencyByDistribution(firepoint->firePointId());
                }
                else
                {
                    if(CGlobal::instance()->m_nStartRule == RULE_OnlyGroup)
                        CGlobal::instance()->ClientBusiness()->firePointEmergencyByGroup(firepoint->firePointId());
                    else if(CGlobal::instance()->m_nStartRule == RULE_OnlyPage)
                        CGlobal::instance()->ClientBusiness()->firePointEmergencyByLayoutPage(firepoint->firePointId());
                    else
                    {
                        CGlobal::instance()->m_isEmergency = true;
                        CMsgObjectStatus msgObjectStatus;
                        msgObjectStatus.nValue = true;
                        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_Emergency, &msgObjectStatus);
                        CGlobal::instance()->ClientBusiness()->Emergency();
                    }
                }

                firepoint->setSimulateEmergency(true);
                firepoint->setStatus(OBJS_SimulateFireEmergency,1);
            }
        }
    }
    CGlobal::instance()->TopToolBar()->changeCenterFrameWidget(CGlobal::instance()->CenterFrame());
    close();
}
