#include "dlgdeviceForbidFault.h"
#include "ui_dlgdeviceForbidFault.h"
#include "model/cdeviceforbidfaultlistmodel.h"
#include "cglobal.h"


dlgdeviceForbidFault::dlgdeviceForbidFault(QWidget *parent) :
    QDialog(parent), ui(new Ui::dlgdeviceForbidFault)
{
    ui->setupUi(this);
    CGlobal::instance()->setDeviceForbidFault(this);
    m_model = new CDeviceForbidFaultlistmodel();
    m_count = 0;
    ui->tableView->setModel(m_model);
    ui->tableView->setColumnWidth(0, 90);
    ui->tableView->setColumnWidth(1, 60);
    ui->tableView->setColumnWidth(2, 60);
    ui->tableView->setColumnWidth(3, 250);
    ui->tableView->setColumnWidth(4, 170);

    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setHighlightSections(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setWordWrap(true);
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
    ui->tableView->setStyleSheet(styleSheet);
}

dlgdeviceForbidFault::~dlgdeviceForbidFault()
{
    delete ui;
}


void dlgdeviceForbidFault::on_pushButton_close_clicked()
{
    qDebug() << "dlgdeviceForbidFault::on_pushButton_close_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
   close();
}

void dlgdeviceForbidFault::addDeviceForbidFaultInfo(CDevice* device)
{
    qDebug() << "dlgdeviceForbidFault::getDeviceForbidFaultInfo"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(device)
    {
        DeviceForbidFaultStatusInfo tempInfo;
        tempInfo.keyId = m_count++;
        tempInfo.statusId = tempInfo.keyId;
        tempInfo.distributionId = "CAN" + QString::number(device->canportAdd()-2) + "-" + QString::number(device->distributionAdd());
        tempInfo.loopId = device->loopAdd();
        tempInfo.deviceId = device->deviceAdd();
        CDeviceType* devType;
        devType = CGlobal::instance()->deviceType(device->deviceValue(DEVICE_VALUE_TYPE_ID).toInt());
        if(!devType)
            devType = CGlobal::instance()->deviceType(0);
        tempInfo.productID = device->deviceProductionId();
        tempInfo.type = devType->name();
        m_model->addData(tempInfo);
    }
    ui->tableView->setCurrentIndex(ui->tableView->model()->index(-1, 0));
}

void dlgdeviceForbidFault::clearDeviceForbidFaultInfo()
{
    m_count = 0;
    m_model->clear();
}
