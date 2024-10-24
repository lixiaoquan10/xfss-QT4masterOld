#include "dlgsearchdevice.h"
#include "ui_dlgsearchdevice.h"
#include "cglobal.h"


#include <QRegExpValidator>
#include <QDebug>
dlgSearchDevice::dlgSearchDevice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgSearchDevice)
{
    ui->setupUi(this);
    CGlobal::instance()->setDlgSearchDevice(this);
    setWindowTitle(tr("Search Device"));
    setAttribute(Qt::WA_DeleteOnClose);

    this->setWindowModality(Qt::NonModal);
    m_model = new CDevicesFindTableModel(CGlobal::instance()->dm());
    ui->tableView->setModel(m_model);
    ui->tableView->setColumnWidth(0, 50);
    ui->tableView->setColumnWidth(1, 50);
    ui->tableView->setColumnWidth(2, 50);
    ui->tableView->setColumnWidth(3, 80);
    ui->tableView->setColumnWidth(4, 110);
    ui->tableView->setColumnWidth(5, 260);
    ui->tableView->setColumnHidden(3, true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slot_on_dClicked(QModelIndex)));

    ui->comboBox->addItem(tr("All distribution"), 0);
    ui->comboBox_2->addItem(tr("All Loop"), 0);
    ui->comboBox->setCurrentIndex(0);
    CCanport * canport = CGlobal::instance()->ClientBusiness()->currentPanel();
    if(canport){
        for (int i=0; i<canport->distributionCount(); ++i)
        {
            CDistribution* distribution = canport->distribution(i);
            ui->comboBox->addItem(distribution->distributionValue(DISTRIBUTION_VALUE_NAME).toString(), distribution->distributionId());
        }
    }
}

dlgSearchDevice::~dlgSearchDevice()
{
    CGlobal::instance()->setDlgSearchDevice(NULL);
    delete ui;
    delete m_model;
}

void dlgSearchDevice::slot_on_dClicked(QModelIndex index)
{
    QString addrs = index.child(index.row(), 3).data().toString();

    CDevice* deviceAddrs = CGlobal::instance()->dm()->device(addrs);
    if(deviceAddrs)
    {
//        CPointDeviceItem* devicePointItem = reinterpret_cast<CPointDeviceItem*>(deviceAddrs->value(DEVICE_VALUE_ITEM_POINTER).toLongLong());
//        CGlobal::instance()->designMainView()->searchDevice(devicePointItem);
    }

    this->close();
}

void dlgSearchDevice::on_comboBox_currentIndexChanged(int index)
{
    CCanport * canport = CGlobal::instance()->ClientBusiness()->currentPanel();
    int nID = ui->comboBox->itemData(index).toInt();
    CDistribution* distribution = canport->distributionById(nID);
    ui->comboBox_2->clear();
    ui->comboBox_2->addItem(tr("All Loop"), 0);
    if(distribution)
    {
        for (int i=0; i<distribution->loopCount(); ++i)
        {
            CLoop* loop = distribution->loopById(i);
            if(loop)
                ui->comboBox_2->addItem(loop->loopValue(LOOP_VALUE_NAME).toString(), loop->loopId());
        }
    }
    QString arg1 = "**" + QString::number(nID).rightJustified(3, '0') + "***";
    if(nID == 0) arg1 = "********";
    m_model->setFilter(arg1);
}

void dlgSearchDevice::on_comboBox_2_currentIndexChanged(int index)
{
    int nDID = ui->comboBox->itemData(ui->comboBox->currentIndex()).toInt();
    int nLID = ui->comboBox_2->itemData(index).toInt();
    QString arg1 = "**";
    if(nDID == 0){
        arg1 += "***";
    }else {
        arg1 += QString::number(nDID).rightJustified(3, '0');
    }
    if(nLID == 0){
        arg1 += "***";
    }else {
        arg1 += QString::number(nLID) + "**";
    }
    m_model->setFilter(arg1);
}
