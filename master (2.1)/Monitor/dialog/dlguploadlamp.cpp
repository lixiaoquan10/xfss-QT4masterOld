#include "dlguploadlamp.h"
#include "ui_dlguploadlamp.h"
#include "dlgdevices.h"
#include "cglobal.h"
#include "widget/cprogramdeviceview.h"
#include "dm/ccontroller.h"
#include "dm/ccanport.h"
#include "dm/cloop.h"
#include "dm/cdevice.h"
#include "dm/clayer.h"
#include "dm/cdistribution.h"
#include "business/cclientbusiness.h"
#include "struct/cmsgdeviceinfo.h"
#include "graphics/configfilemanager.h"

#include "QDebug"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

dlgUploadLamp::dlgUploadLamp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgUploadLamp)
{
    ui->setupUi(this);
    const QModelIndex *m_index;
    m_index = CGlobal::instance()->programDeviceView()->getDevice();
    m_device = CGlobal::instance()->programDeviceView()->getDevices(*m_index);
    if(!m_device)
        return;
    CLoop* loop = m_device->loop();
    if(loop){
        QList<CDevice*> devices = loop->devices();
        for(int i=0; i<devices.count(); i++){
            CDevice* lamp = devices.at(i);
            if(lamp)
            {
                if(((m_device->deviceTypeId() < 50 && lamp->deviceTypeId() < 50) ||
                        (m_device->deviceTypeId() >= 50 && m_device->deviceTypeId() < 100 && lamp->deviceTypeId() >= 50 && lamp->deviceTypeId() < 100) ||
                        (m_device->deviceTypeId() >= 100 && m_device->deviceTypeId() < 150 && lamp->deviceTypeId() >= 100 && lamp->deviceTypeId() < 150) ||
                        (m_device->deviceTypeId() > 150 && lamp->deviceTypeId() > 150))
                        && (lamp->getDeviceCommunicationFault() ||lamp->deviceValue(DEVICE_VALUE_ISUSED) == 0))
                    ui->comboBox->addItem(QString::number(lamp->deviceAdd()));;
            }
        }
        devices.clear();
    }
}

dlgUploadLamp::~dlgUploadLamp()
{
    delete ui;
}

void dlgUploadLamp::on_pushButton_clicked()
{
    CMsgObjectStatus msgobjectstatus;
    if(!m_device)
        return;
    CLoop* loop = m_device->loop();
    CDevice* device = loop->deviceByAdd(ui->comboBox->currentText().toInt());
    if(device)
    {
        device->setDeviceValue(DEVICE_VALUE_EMERGENCY, "正常");
        device->setDeviceValue(DEVICE_VALUE_LIGHT, "正常");
//        device->setStatus(OBJS_LightCommunicationFault, 0);
        QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(fileName);
        if(!db.open())
            return;
        QSqlQuery query(db);

        QString sql = QString("DELETE FROM LampInfo WHERE ID = %1").arg(CGlobal::instance()->saveKeyId(OBJT_Device,device->keyId()));
        query.prepare(sql);
        query.exec();

        if(!query.exec())
        {
            qDebug()<<query.lastError();
        }
        db.close();
        device->deleteLater();
    }
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
        m_device->setDeviceValue(DEVICE_VALUE_CHANGEADD,1);
        m_device->setDeviceValue(DEVICE_VALUE_ADDR,msgobjectstatus.nValue);
        CGlobal::instance()->DlgDevices()->getDeviceInfo();
        //上传灯具状态到云平台/客户端
        CGlobal::instance()->ClientBusiness()->uploadLampStatus(m_device);
//        ConfigFileManager cfg;
//        cfg.SaveConfigFile();//写入配置文件
    }
    this->close();
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
    log = "替换灯具成功，请保存灯具到数据库";
    messageBox.setText(log);
    messageBox.exec();

}
