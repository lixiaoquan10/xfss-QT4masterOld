#include "dlgchangetype.h"
#include "ui_dlgchangetype.h"
#include "dlgdevices.h"
#include "cglobal.h"
#include "widget/cprogramdeviceview.h"
#include "cdevicetype.h"
#include "dm/cproject.h"
#include "dm/ccontroller.h"
#include "dm/cloop.h"
#include "dm/cdevice.h"
#include "dm/clayer.h"
#include "dm/cdistribution.h"
#include "dm/cdbsqlite.h"
#include "business/cclientbusiness.h"
#include "struct/cmsgdeviceinfo.h"
#include "graphics/configfilemanager.h"

#include <QSqlQuery>

dlgChangeType::dlgChangeType(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgChangeType)
{
    int count = 0;
    ui->setupUi(this);
    const QModelIndex *m_index;
    m_index = CGlobal::instance()->programDeviceView()->getDevice();
    device = CGlobal::instance()->programDeviceView()->getDevices(*m_index);
    if(!device)
        return;
    CDeviceType* devType;
    for (int i=0; i<256; ++i)
    {
        devType = CGlobal::instance()->deviceType(i);
        if(devType)
        {
            if(device->deviceTypeId() == 119 && (devType->type() < 115 || (devType->type() > 119 && devType->type() < 140) || devType->type() > 147))
                continue;
            if(device->deviceTypeId() == 124 && (devType->type() < 122 || devType->type() > 124))
                continue;
            if(device->deviceTypeId() == 19 && (devType->type() < 15 || (devType->type() > 19 && devType->type() < 40) || devType->type() > 47))
                continue;
            ui->comboBox->addItem(devType->name());
            if(devType->type() == device->deviceTypeId())
                ui->comboBox->setCurrentIndex(count);
            count++;
        }
    }
}

dlgChangeType::~dlgChangeType()
{
    delete ui;
}

void dlgChangeType::on_pushButton_clicked()
{
    CMsgObjectStatus msgobjectstatus;
    if(!device)
        return;
    CLoop* loop = device->loop();
    if(!loop)
        return;
    CDistribution* distribution = loop->distribution();
    if(!distribution)
        return;
    msgobjectstatus.nDisID = distribution->distributionAddress();
    msgobjectstatus.nLoopID = loop->loopAdd();
    msgobjectstatus.nDeviceID = device->deviceAdd();
    msgobjectstatus.nType = device->deviceTypeId();
    CDeviceType* devType;
    for (int i=0; i<255; ++i)
    {
        devType = CGlobal::instance()->deviceType(i);
        if(devType)
        {
            if(devType->name() == ui->comboBox->currentText())
                msgobjectstatus.nValue = devType->type();
        }
    }
    if(msgobjectstatus.nType != msgobjectstatus.nValue)
    {
        device->setValue(DEVICE_VALUE_TYPE_ID,msgobjectstatus.nValue);
        //更新列表视图
        CGlobal::instance()->DlgDevices()->getDeviceInfo();
        if(msgobjectstatus.nType == 119 || device->deviceTypeId() == 124 || device->deviceTypeId() == 19)
        {
            QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
            CDBSqlite db(fileName);
            if(!db.open())
                return;
            QSqlQuery query1 = db.query(QString("SELECT ID,number from lamptype where number=%1"
                                                ).arg(msgobjectstatus.nValue));
            QSqlRecord record = query1.record();
            int lampTypeId;
            while(query1.next())
            {
                lampTypeId = query1.value(record.indexOf("ID")).toInt();
            }
            // 绑定参数
            QSqlQuery query = db.query("UPDATE LampInfo SET LampTypeID=:LampTypeID WHERE ID=:id");
            query.bindValue(":LampTypeID", lampTypeId);
            query.bindValue(":id", CGlobal::instance()->saveKeyId(OBJT_Device,device->keyId()));
            // 执行 SQL 语句
            query.exec();
            db.close();
            this->close();
            return;
        }
        device->setValue(DEVICE_VALUE_CHANGETYPE,1);
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceType, &msgobjectstatus);
        //上传灯具状态到云平台/客户端
        CGlobal::instance()->ClientBusiness()->uploadLampStatus(device);
//        ConfigFileManager cfg;
//        cfg.SaveConfigFile();//写入配置文件
    }
    this->close();
}
