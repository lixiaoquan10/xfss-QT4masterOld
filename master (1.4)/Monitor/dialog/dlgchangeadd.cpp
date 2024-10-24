#include "dlgchangeadd.h"
#include "ui_dlgchangeadd.h"
#include "dlgdevices.h"
#include "cglobal.h"
#include "widget/cprogramdeviceview.h"
#include "dm/cproject.h"
#include "dm/ccontroller.h"
#include "dm/cloop.h"
#include "dm/cdevice.h"
#include "dm/clayer.h"
#include "dm/cdistribution.h"
#include "business/cclientbusiness.h"
#include "struct/cmsgdeviceinfo.h"
#include "graphics/configfilemanager.h"

#include "QDebug"

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
        CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
        if(controller){
            CDistribution* distribution = controller->distributionByAddress(CGlobal::instance()->m_nDistriAddress);
            if(distribution){
                CLoop* loop = distribution->loopByAdd(CGlobal::instance()->m_nLoopAddress);
                if(loop){
                    CDevice* device = loop->deviceByAdd(i);
                    if(i == m_device->deviceAdd())
                    {
                        ui->comboBox->addItem(QString::number(i));
                    }
                    if(!device || (device && ((device->value(DEVICE_VALUE_ISUSED) != 1 && device->value(DEVICE_VALUE_ISUSED) != 2) ||
                                              device->getStatus(OBJS_LightCommunicationFault))))
                    {
                        ui->comboBox->addItem(QString::number(i));
                    }
                }
            }
        }
    }
//    int index = ui->comboBox->findText(QString::number(m_device->deviceAdd()));  // 查找文本在列表中的索引
//    if (index != -1) {
//        ui->comboBox->setCurrentIndex(index);  // 设置当前选项的索引
//    }
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
    CLoop* loop = m_device->loop();
    if(!loop)
        return;
    CDistribution* distribution = loop->distribution();
    if(!distribution)
        return;
    msgobjectstatus.nDisID = distribution->distributionAddress();
    msgobjectstatus.nLoopID = loop->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();
    msgobjectstatus.nValue = ui->comboBox->currentText().toInt();
    msgobjectstatus.strText = m_device->deviceProductionId();
    if(msgobjectstatus.nValue != msgobjectstatus.nDeviceID)
    {
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceAdd, &msgobjectstatus);
        m_device->setValue(DEVICE_VALUE_CHANGEADD,1);
        m_device->setValue(DEVICE_VALUE_ADDR,msgobjectstatus.nValue);
        CGlobal::instance()->DlgDevices()->getDeviceInfo();
        //上传灯具状态到云平台/客户端
        CGlobal::instance()->ClientBusiness()->uploadLampStatus(m_device);
//        ConfigFileManager cfg;
//        cfg.SaveConfigFile();//写入配置文件
    }
    this->close();
}
