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

    void slot_dataLoaded(QTreeWidgetItem *rootItem);

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
    explicit LoadNodeDataThread(QObject *parent = NULL) :
        QThread(parent)
    {

    }

signals:
    void dataLoaded(QTreeWidgetItem *rootItem);
    void threadFinished();

protected:
    void run();

};

#endif // DLGHANDFIREVIEW_H
