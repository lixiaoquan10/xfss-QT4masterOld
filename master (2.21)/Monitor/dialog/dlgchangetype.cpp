#include "ui_dlgchangetype.h"
#include "cglobal.h"

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
    msgobjectstatus.nCanportAdd = device->canportAdd();
    msgobjectstatus.nDisID = device->distributionAdd();
    msgobjectstatus.nLoopID = device->loopAdd();
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
        device->setDeviceValue(DEVICE_VALUE_TYPE_ID,msgobjectstatus.nValue);
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
        device->setDeviceValue(DEVICE_VALUE_CHANGETYPE,1);
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceType, &msgobjectstatus);
        //上传灯具状态到云平台/客户端
        CGlobal::instance()->ClientBusiness()->uploadLampStatus(device);
    }
    this->close();
}
