#include "ui_dlgchangetype.h"
#include "dlgchangetype.h"
#include "cglobal.h"

dlgChangeType::dlgChangeType(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgChangeType)
{
    int count = 0;
    ui->setupUi(this);
    m_device = CGlobal::instance()->programDeviceView()->getDevice();
    if(!m_device)
        return;
    CDeviceType* devType;
    for (int i=0; i<256; ++i)
    {
        devType = CGlobal::instance()->deviceType(i);
        if(devType)
        {
            if(m_device->deviceTypeId() == 119 && (devType->type() < 115 || (devType->type() > 119 && devType->type() < 140) || devType->type() > 147))
                continue;
            if(m_device->deviceTypeId() == 124 && (devType->type() < 122 || devType->type() > 124))
                continue;
            if(m_device->deviceTypeId() == 19 && (devType->type() < 15 || (devType->type() > 19 && devType->type() < 40) || devType->type() > 47))
                continue;
            ui->comboBox->addItem(devType->name());
            if(devType->type() == m_device->deviceTypeId())
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
    if(!m_device)
        return;
    msgobjectstatus.nCanportAdd = m_device->canportAdd();
    msgobjectstatus.nDisID = m_device->distributionAdd();
    msgobjectstatus.nLoopID = m_device->loopAdd();
    msgobjectstatus.nDeviceID = m_device->deviceAdd();
    msgobjectstatus.nType = m_device->deviceTypeId();
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
        m_device->setDeviceValue(DEVICE_VALUE_TYPE_ID,msgobjectstatus.nValue);
        //更新列表视图
        CGlobal::instance()->DlgDevices()->getDeviceInfo();
        if(msgobjectstatus.nType == 119 || m_device->deviceTypeId() == 124 || m_device->deviceTypeId() == 19)
        {
            QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
            CDBSqlite db(fileName);
            if(!db.open())
                return;
            QSqlQuery queryTypeId = db.query(QString("SELECT ID,number from lamptype where number=%1"
                                                ).arg(msgobjectstatus.nValue));
            QSqlRecord record = queryTypeId.record();
            int lampTypeId;
            while(queryTypeId.next())
            {
                lampTypeId = queryTypeId.value(record.indexOf("ID")).toInt();
            }
            // 绑定参数
            QSqlQuery query = db.query("UPDATE LampInfo SET LampTypeID=:LampTypeID WHERE ID=:id");
            query.bindValue(":LampTypeID", lampTypeId);
            query.bindValue(":id", CGlobal::instance()->saveKeyId(OBJT_Device,m_device->keyId()));
            // 执行 SQL 语句
            query.exec();
            db.close();
        }
        else
        {
            m_device->setDeviceValue(DEVICE_VALUE_CHANGETYPE,1);
            CGlobal::instance()->ClientBusiness()->exeCommand(NCT_DeviceType, &msgobjectstatus);
//            //上传灯具状态到云平台/客户端
//            CGlobal::instance()->ClientBusiness()->uploadLampStatus(device);

            QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");;
            db.setDatabaseName(fileName);
            if(!db.open())
                return;
            QSqlQuery query(db);

            int id = CGlobal::instance()->saveKeyId(OBJT_Device,m_device->keyId());
            query.exec(QString("DELETE FROM EvacuationPathItem WHERE LampInfoID=%1").arg(id));
            query.exec(QString("DELETE FROM LampConnection WHERE StartLampID=%1").arg(id));
            query.exec(QString("DELETE FROM LampConnection WHERE EndLampID=%1").arg(id));
            query.exec(QString("DELETE FROM LampDefaultDirection WHERE LampInfoID=%1").arg(id));
            query.exec(QString("DELETE FROM LampToFirePoint WHERE LampInfoID=%1").arg(id));
            query.exec(QString("DELETE FROM LampToLaunchGroup WHERE LampInfoID=%1").arg(id));
            if(!query.exec())
            {
                qDebug() << query.lastError();
            }
            db.close();
        }
        QFont font;
        font.setPointSize(12); // 设置字体大小为12像素
        // 在QMessageBox中设置字体
        QMessageBox messageBox;
        messageBox.setFont(font);
        QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
        messageBox.setStyleSheet(styleSheet);
        messageBox.setIcon(QMessageBox::NoIcon);
        messageBox.setWindowTitle("信息");
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        messageBox.setDefaultButton(QMessageBox::No);
        QString log;
        log = "是否清除注册并重新注册?" + tr("\n") + "Yes:是" + tr("\n") + "No:否";
        messageBox.setText(log);
        int ret = messageBox.exec();
        if(ret == QMessageBox::No)
        {
            ;
        }
        if(ret == QMessageBox::Yes)
        {
            CMsgObjectStatus msg;
            //获取当前点击的回路地址
            msg.nDisID = CGlobal::instance()->m_nDistriAddress;
            msg.nLoopID = CGlobal::instance()->m_nLoopAddress;
            msg.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
            CGlobal::instance()->ClientBusiness()->exeCommand(NCT_LoginInfoLoop, &msg);
        }
    }
    this->close();
}

void dlgChangeType::on_pushButton_cancel_clicked()
{
    this->close();
}
