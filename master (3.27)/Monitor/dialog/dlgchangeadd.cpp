#include "ui_dlgchangeadd.h"
#include "dlgchangeadd.h"
#include "cglobal.h"

dlgChangeAdd::dlgChangeAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgChangeAdd)
{
    ui->setupUi(this);
    const QModelIndex *m_index;
    m_index = CGlobal::instance()->programDeviceView()->getDevice();
    m_device = CGlobal::instance()->programDeviceView()->getDevices(*m_index);
    if(!m_device)
        return;
    for (uint i=1; i<256; ++i)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(CGlobal::instance()->m_nCanportAddress);
        if(canport){
            CDistribution* distribution = canport->distributionByAddress(CGlobal::instance()->m_nDistriAddress);
            if(distribution){
                CLoop* loop = distribution->loopByAdd(CGlobal::instance()->m_nLoopAddress);
                if(loop){
                    CDevice* device = loop->deviceByAdd(i);
                    if(i == (m_device->deviceAdd()))
                    {
                        ui->comboBox->addItem(QString::number(i));
                    }
                    if(!device || (device && ((device->deviceValue(DEVICE_VALUE_ISUSED) != 1 && device->deviceValue(DEVICE_VALUE_ISUSED) != 2) ||
                                              device->getDeviceCommunicationFault())))
                    {
                        ui->comboBox->addItem(QString::number(i));
                    }
                }
            }
        }
    }
}

dlgChangeAdd::~dlgChangeAdd()
{
    delete ui;
}

void dlgChangeAdd::on_pushButton_clicked()
{
    CMsgObjectStatus msgobjectstatus;
    if(!m_device)
        return;
    msgobjectstatus.nCanportAdd = m_device->canportAdd();
    msgobjectstatus.nDisID = m_device->distributionAdd();
    msgobjectstatus.nLoopID = m_device->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();
    msgobjectstatus.nValue = ui->comboBox->currentText().toInt();
    msgobjectstatus.strText = m_device->deviceProductionId();
    if(msgobjectstatus.nValue != msgobjectstatus.nDeviceID)
    {
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceAdd, &msgobjectstatus);
        m_device->setDeviceValue(DEVICE_VALUE_CHANGEADD,1);
        m_device->setDeviceValue(DEVICE_VALUE_ADDR,msgobjectstatus.nValue);
        CGlobal::instance()->DlgDevices()->getDeviceInfo();
//        //上传灯具状态到云平台/客户端
//        CGlobal::instance()->ClientBusiness()->uploadLampStatus(m_device);
    }
    this->close();
}
