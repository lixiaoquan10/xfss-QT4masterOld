#ifndef DLGHANDFIREVIEW_H
#define DLGHANDFIREVIEW_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class dlgHandFireView;
}

class dlgHandFireView : public QDialog
{
    Q_OBJECT
public:
    explicit dlgHandFireView(QWidget *parent = 0);
    ~dlgHandFireView();
private slots:
    void on_pushButton_close_clicked();

    void on_pushButton_fire_clicked();

    void slot_threadFinished();

private:
    Ui::dlgHandFireView *ui;
};


#include <QTreeWidgetItem>
#include "cglobal.h"
#include "dm/ccontroller.h"
class CGlobal;
class CController;
class LoadNodeDataThread : public QThread
{
    Q_OBJECT

public:
    explicit LoadNodeDataThread(QTreeWidget *treeWidget, CController* controller, QObject *parent = NULL) :
        QThread(parent), treeWidget(treeWidget), controller(controller) {}

signals:
    void dataLoaded();

protected:
    void run()
    {
        for(int i=0; i<=controller->maxFirePointDeviceAddress(); i++)
        {
            if(controller->isfirePointByDeviceAddress(i))
            {
                // 创建根节点
                QTreeWidgetItem *rootItem = new QTreeWidgetItem(treeWidget);
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
                                QString str = QString::number(i)+"-"+QString::number(j)+"-"+QString::number(m);
                                addressNode->setText(0, str);
                            }
                        }
                    }
                }
                // 将根节点添加到树形控件中
                treeWidget->addTopLevelItem(rootItem);
            }
        }
        // 发送信号，将节点传递给主线程
        emit dataLoaded();
    }

private:
    QTreeWidget *treeWidget;
    CController* controller;
};

#endif // DLGHANDFIREVIEW_H
