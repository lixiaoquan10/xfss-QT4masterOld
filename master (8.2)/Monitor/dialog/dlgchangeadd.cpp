#include "ui_dlgchangeadd.h"
#include "dlgchangeadd.h"
#include "cglobal.h"

dlgChangeAdd::dlgChangeAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgChangeAdd)
{
    ui->setupUi(this);
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
    ui->comboBox->view()->verticalScrollBar()->setStyleSheet(styleSheet);
    m_device = CGlobal::instance()->programDeviceView()->getDevice();
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
                    else if(!device || (device && (!device->isDeviceLogin() || device->getDeviceCommunicationFault())))
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
    qDebug() << "dlgChangeAdd::on_pushButton_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
        messageBox.button(QMessageBox::Yes)->setText("是");
        messageBox.button(QMessageBox::No)->setText("否");
        QString log;
        log = "是否清除注册并重新注册?";
        messageBox.setText(log);
        int ret = messageBox.exec();
        if(ret == QMessageBox::Yes)
        {
            //获取当前点击的回路地址
            CMsgObjectStatus msg;
            msg.nDisID = CGlobal::instance()->m_nDistriAddress;
            msg.nLoopID = CGlobal::instance()->m_nLoopAddress;
            msg.nCanportAdd = CGlobal::instance()->m_nCanportAddress;
            //清除注册
            CGlobal::instance()->ClientBusiness()->removeLoginObject(CGlobal::instance()->m_nCanportAddress,CGlobal::instance()->m_nDistriAddress,
                                                                     CGlobal::instance()->m_nLoopAddress);
            CGlobal::instance()->ClientBusiness()->exeCommand(NCT_UnLoginLoop, &msg);
            //重新注册
            CGlobal::instance()->ClientBusiness()->exeCommand(NCT_LoginInfoLoop, &msg);
        }
//        //上传灯具状态到云平台/客户端
//        CGlobal::instance()->ClientBusiness()->uploadLampStatus(m_device);
    }
    this->close();
}

void dlgChangeAdd::on_pushButton_cancel_clicked()
{
    qDebug() << "dlgChangeAdd::on_pushButton_cancel_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    this->close();
}
