#include "dlghandfireview.h"
#include "ui_dlghandfireview.h"
#include "cglobal.h"


dlgHandFireView::dlgHandFireView(QWidget *parent) :
    QDialog(parent), ui(new Ui::dlgHandFireView)
{
    ui->setupUi(this);
    ui->pushButton_fire->setStyleSheet("background-color: rgb(255,0,0);font: bold;");
    // 创建根节点
    QTreeWidgetItem *noneItem = new QTreeWidgetItem(ui->treeWidget);
    noneItem->setText(0, "无");
    // 将根节点添加到树形控件中
    ui->treeWidget->addTopLevelItem(noneItem);
    CController* controller = CGlobal::instance()->controller();
    // 创建加载节点数据的线程
    LoadNodeDataThread *thread = new LoadNodeDataThread(ui->treeWidget, controller, this);
    connect(thread, SIGNAL(dataLoaded()), this, SLOT(slot_threadFinished()));

    // 启动线程
    thread->start();

//    for(int i=0; i<=controller->maxFirePointDeviceAddress(); i++)
//    {
//        if(controller->isfirePointByDeviceAddress(i))
//        {
//            // 创建根节点
//            QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->treeWidget);
//            rootItem->setText(0, QString::number(i));
//            for(int j=0; j<=controller->maxFirePointLoopAddress(); j++)
//            {
//                if(controller->isfirePointByLoopAddress(i,j))
//                {
//                    // 添加第一层子节点
//                    QTreeWidgetItem *circuitNode = new QTreeWidgetItem(rootItem);
//                    circuitNode->setText(0, QString::number(j));
//                    for(int m=0; m<=controller->maxFirePointAddress(); m++)
//                    {
//                        CFirePoint* firepoint = controller->firePointByAddress(i,j,m);
//                        if(firepoint)
//                        {
//                            // 添加第二层子节点
//                            QTreeWidgetItem *addressNode = new QTreeWidgetItem(circuitNode);
//                            QString str = QString::number(i)+"-"+QString::number(j)+"-"+QString::number(m);
//                            addressNode->setText(0, str);
//                        }
//                    }

//                }
//            }
//            // 将根节点添加到树形控件中
//            ui->treeWidget->addTopLevelItem(rootItem);
//        }
//    }
}

dlgHandFireView::~dlgHandFireView()
{
    delete ui;
}

void dlgHandFireView::slot_threadFinished()
{
    QObject::sender()->deleteLater();  // 删除线程对象
}

void dlgHandFireView::on_pushButton_close_clicked()
{
   close();
}

void dlgHandFireView::on_pushButton_fire_clicked()
{
    CController* controller = CGlobal::instance()->controller();
    // 在这里处理按钮点击事件逻辑
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    QString str = item->text(0);
    QStringList list = str.split("-");
    if(list.size() == 3)
    {
        int i = list[0].toInt();
        int j = list[1].toInt();
        int m = list[2].toInt();
        CFirePoint* firepoint = controller->firePointByAddress(i,j,m);
        if(firepoint)
        {
            CGlobal::instance()->ClientBusiness()->firePointEmergency(firepoint->firePointId());
        }
    }
}
