#include "ui_dlgsystemrecord.h"
#include "dlgsystemrecord.h"
#include "cglobal.h"

dlgSystemRecord::dlgSystemRecord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgSystemRecord)
{
    ui->setupUi(this);
    CGlobal::instance()->setSystemRecord(this);
    this->setWindowTitle("事件记录");
    this->setAttribute(Qt::WA_DeleteOnClose);
//    this->setFixedSize(800, 460);
    CGlobal::instance()->m_HaveDialog = true;

    m_Model = new dlgSystemRecordModel;
    ui->tableView->setModel(m_Model);
    ui->tableView->setColumnWidth(0, 100);
    ui->tableView->setColumnWidth(1, 60);
    ui->tableView->setColumnWidth(2, 180);
    ui->tableView->setColumnWidth(3, 90);
    ui->tableView->setColumnWidth(4, 60);
    ui->tableView->setColumnWidth(5, 60);
    ui->tableView->setColumnWidth(6, 220);
    ui->tableView->setColumnWidth(7, 220);
    ui->tableView->setColumnWidth(8, 250);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setHighlightSections(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setStyleSheet("selection-color: rgb(0, 0, 0);");

    QDate m_date;
    ui->m_begin_dateEdit->setDate(m_date.currentDate());
    ui->m_end_dateEdit->setDate(m_date.currentDate());
    m_page = 1;
    ui->lineEdit_page->setText(QString::number(m_page));
    ui->lineEdit_page_query->setText(QString::number(m_page));
}

void dlgSystemRecord::updateRecord()
{
    qDebug() << "dlgSystemRecord::updateRecord"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(CGlobal::instance()->centerFrameContainer()->currentIndex() != 2)
        return;
    if(m_mode == 0)
        dayrecord();
    else if(m_mode == 1)
        weekrecord();
    else if(m_mode == 2)
        customQueryrecord();
}

dlgSystemRecord::~dlgSystemRecord()
{
    delete m_Model;
    delete ui;
    CGlobal::instance()->setSystemRecord(NULL);
    CGlobal::instance()->m_HaveDialog = false;
}

void dlgSystemRecord::on_pushButton_clicked()
{
    qDebug() << "dlgSystemRecord::on_pushButton_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_Model->removeData();
    CMsgLogInfo m_MsgLogInfo;
    m_MsgLogInfo.beginTime = ui->m_begin_dateEdit->dateTime();
    m_MsgLogInfo.endTime = ui->m_end_dateEdit->dateTime();
    if(ui->m_WorklogType->currentText() != "全部")
    {
        m_MsgLogInfo.type = ui->m_WorklogType->currentText();
    }
    if(!CGlobal::instance()->ClientBusiness()->exeCommand(NCT_GetLog, &m_MsgLogInfo))
    {
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
        messageBox.button(QMessageBox::Ok)->setText("确定");
        QString log;
        log = "查询事件失败！";
        messageBox.setText(log);
        messageBox.exec();
    }
}

void dlgSystemRecord::setPage(int page)
{
    qDebug() << "dlgSystemRecord::setPage"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_page = page;
    ui->lineEdit_page->setText(QString::number(m_page));
    ui->lineEdit_page_query->setText(QString::number(m_page));
}


void dlgSystemRecord::dayrecord()
{
    qDebug() << "dlgSystemRecord::dayrecord"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_mode = 0;//日查询
    this->setWindowTitle("当日记录");
    //更新日期
    QDate m_date;
    ui->m_begin_dateEdit->setDate(m_date.currentDate());
    ui->m_end_dateEdit->setDate(m_date.currentDate());
    //更新页码
    ui->lineEdit_page->setText(QString::number(m_page));
    //隐藏自定义查询菜单
    ui->groupBox->setVisible(false);
    //显示下方翻页按钮
    ui->pushButton_before->setVisible(true);
    ui->pushButton_next->setVisible(true);
    ui->label_page->setVisible(true);
    ui->lineEdit_page->setVisible(true);
    ui->horizontalSpacer_page->changeSize(200,20,QSizePolicy::Expanding);
    ui->horizontalLayout_page->update();

    m_Model->removeData();

    QByteArray byte;
    int n = 0x00;
    byte.append(n); //0代表当日或七天内记录
    byte.append(CGlobal::instance()->DataProcessing()->isStatusTab());
    byte.append(m_page);
    CMsgObjectStatus m_MsgObjectStatus;
    m_MsgObjectStatus.ndata = byte;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_RecordPage, &m_MsgObjectStatus);

    CMsgLogInfo m_MsgLogInfo;
    m_MsgLogInfo.beginTime = ui->m_begin_dateEdit->dateTime();
    m_MsgLogInfo.endTime = ui->m_begin_dateEdit->dateTime();
    if(!CGlobal::instance()->ClientBusiness()->exeCommand(NCT_GetLog, &m_MsgLogInfo))
    {
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
        messageBox.button(QMessageBox::Ok)->setText("确定");
        QString log;
        log = "查询事件失败！";
        messageBox.setText(log);
        messageBox.exec();
    }
}

void dlgSystemRecord::weekrecord()
{
    qDebug() << "dlgSystemRecord::weekrecord"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_mode = 1;//周查询
    this->setWindowTitle("七天內记录");
    //更新日期
    QDate m_date;
    ui->m_begin_dateEdit->setDate(m_date.currentDate());
    ui->m_end_dateEdit->setDate(m_date.currentDate());
    //更新页码
    ui->lineEdit_page->setText(QString::number(m_page));
    //隐藏自定义查询菜单
    ui->groupBox->setVisible(false);
    //显示下方翻页按钮
    ui->pushButton_before->setVisible(true);
    ui->pushButton_next->setVisible(true);
    ui->label_page->setVisible(true);
    ui->lineEdit_page->setVisible(true);
    ui->horizontalSpacer_page->changeSize(200,20,QSizePolicy::Expanding);
    ui->horizontalLayout_page->update();

    m_Model->removeData();

    QByteArray byte;
    int n = 0x00;
    byte.append(n); //0代表当日或七天内记录
    byte.append(CGlobal::instance()->DataProcessing()->isStatusTab());
    byte.append(m_page);
    CMsgObjectStatus m_MsgObjectStatus;
    m_MsgObjectStatus.ndata = byte;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_RecordPage, &m_MsgObjectStatus);

    CMsgLogInfo m_MsgLogInfo;
    m_MsgLogInfo.beginTime = ui->m_begin_dateEdit->dateTime().addDays(-7);
    m_MsgLogInfo.endTime = ui->m_end_dateEdit->dateTime();
    if(!CGlobal::instance()->ClientBusiness()->exeCommand(NCT_GetLog, &m_MsgLogInfo))
    {
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
        messageBox.button(QMessageBox::Ok)->setText("确定");
        QString log;
        log = "查询事件失败！";
        messageBox.setText(log);
        messageBox.exec();
    }
}

void dlgSystemRecord::customQueryrecord()
{
    qDebug() << "dlgSystemRecord::customQueryrecord"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    //切换到自定义查询时更新日期
    if(m_mode != 2)
    {
        QDate m_date;
        ui->m_begin_dateEdit->setDate(m_date.currentDate());
        ui->m_end_dateEdit->setDate(m_date.currentDate());
    }

    m_mode = 2;//自定义查询
    this->setWindowTitle("事件记录");

    //更新页码
    ui->lineEdit_page_query->setText(QString::number(m_page));
    //显示自定义查询菜单
    ui->groupBox->setVisible(true);
    //隐藏下方翻页按钮
    ui->pushButton_before->setVisible(false);
    ui->pushButton_next->setVisible(false);
    ui->label_page->setVisible(false);
    ui->lineEdit_page->setVisible(false);
    ui->horizontalSpacer_page->changeSize(0,0);

    m_Model->removeData();

    QByteArray byte;
    byte.append(1); //1代表自定义查询
    byte.append(CGlobal::instance()->DataProcessing()->isStatusTab());
    byte.append(m_page);
    CMsgObjectStatus m_MsgObjectStatus;
    m_MsgObjectStatus.ndata = byte;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_RecordPage, &m_MsgObjectStatus);

    CMsgLogInfo m_MsgLogInfo;
    m_MsgLogInfo.beginTime = ui->m_begin_dateEdit->dateTime();
    m_MsgLogInfo.endTime = ui->m_end_dateEdit->dateTime();
    if(ui->m_WorklogType->currentText() != "全部")
    {
        m_MsgLogInfo.type = ui->m_WorklogType->currentText();
    }
    if(!CGlobal::instance()->ClientBusiness()->exeCommand(NCT_GetLog, &m_MsgLogInfo))
    {
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
        messageBox.button(QMessageBox::Ok)->setText("确定");
        QString log;
        log = "查询事件失败！";
        messageBox.setText(log);
        messageBox.exec();
    }
}

void dlgSystemRecord::on_pushButton_before_clicked()
{
    qDebug() << "dlgSystemRecord::on_pushButton_before_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_page == 1)
        return;
    m_page = m_page - 1;
    QByteArray byte;
    int n = 0x00;
    byte.append(n); //0代表当日或七天内记录
    byte.append(m_page);
    CMsgObjectStatus m_MsgObjectStatus;
    m_MsgObjectStatus.ndata = byte;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_RecordPage, &m_MsgObjectStatus);
    if(m_mode == 0)
        dayrecord();
    else if(m_mode == 1)
        weekrecord();
}

void dlgSystemRecord::on_pushButton_next_clicked()
{
    qDebug() << "dlgSystemRecord::on_pushButton_next_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_page = m_page + 1;
    QByteArray byte;
    int n = 0x00;
    byte.append(n); //0代表当日或七天内记录
    byte.append(m_page);
    CMsgObjectStatus m_MsgObjectStatus;
    m_MsgObjectStatus.ndata = byte;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_RecordPage, &m_MsgObjectStatus);
    if(m_mode == 0)
        dayrecord();
    else if(m_mode == 1)
        weekrecord();
}

void dlgSystemRecord::on_pushButton_before_query_clicked()
{
    qDebug() << "dlgSystemRecord::on_pushButton_before_query_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_page == 1)
        return;
    m_page = m_page - 1;
    QByteArray byte;
    byte.append(1); //1代表自定义查询
    byte.append(m_page);
    CMsgObjectStatus m_MsgObjectStatus;
    m_MsgObjectStatus.ndata = byte;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_RecordPage, &m_MsgObjectStatus);
    customQueryrecord();
}

void dlgSystemRecord::on_pushButton_next_query_clicked()
{
    qDebug() << "dlgSystemRecord::on_pushButton_next_query_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
//    if(m_page == 1)
//        return;
    m_page = m_page + 1;
    QByteArray byte;
    byte.append(1); //1代表自定义查询
    byte.append(m_page);
    CMsgObjectStatus m_MsgObjectStatus;
    m_MsgObjectStatus.ndata = byte;
    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_RecordPage, &m_MsgObjectStatus);
    ui->lineEdit_page_query->setText(QString::number(m_page));
    customQueryrecord();
}
