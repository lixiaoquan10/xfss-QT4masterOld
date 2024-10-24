#include "dlgadminparam.h"
#include "ui_dlgadminparam.h"
#include "cglobal.h"
#include "keyboard/ckeyboard.h"

#include <QMessageBox>
#include <QSqlQuery>

dlgadminParam::dlgadminParam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgadminParam)
{
    ui->setupUi(this);
    ui->label_message->setWordWrap(true);
    m_useDay = 0;
    m_localNumber = qrand() % (1000000 - 100000 + 1) + 100000;
    ui->m_LocalNumber->setText(QString::number(m_localNumber));
    CGlobal::instance()->m_HaveDialog = true;
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    ui->m_rPowerCode->setFocus();
    ui->label_message->setStyleSheet("font: bold 14pt;");
    CGlobal::instance()->m_lineEditwidget = dynamic_cast<QWidget*>(ui->m_rPowerCode);
    if(!CGlobal::instance()->m_selfCheck)
    {
        CGlobal::instance()->m_isdialogMousePress = true;
    }
}

dlgadminParam::~dlgadminParam()
{
    delete ui;
    CGlobal::instance()->m_HaveDialog = false;
}


void dlgadminParam::on_m_btnOk_clicked()
{
    qDebug() << "dlgadminParam::on_m_btnOk_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(ui->m_rPowerCode->text() == "")
    {
        ui->m_rPowerCode->setPlaceholderText("请输入权限代码...");
        return;
    }
    bool bVerified = VerifyPowerCode(ui->m_rPowerCode->text(),m_localNumber,m_useDay);
    if(bVerified)
    {
        CGlobal::instance()->m_adminParamTime = QDateTime::currentDateTime().addDays(m_useDay);
        QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(fileName);
        if(!db.open())
            return;
        QSqlQuery query(db);
        query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('AdminParamTime', '%1');").arg(CGlobal::instance()->m_adminParamTime.toString(Qt::ISODate)));
        db.commit();
        db.close();
        CGlobal::instance()->AdminParamPage()->updateSqliteData();
        // 替换界面
        CGlobal::instance()->centerFrameContainer()->removeWidget(CGlobal::instance()->centerFrameContainer()->widget(0)); // 移除原来的小部件
        CGlobal::instance()->centerFrameContainer()->insertWidget(0, CGlobal::instance()->AdminParamPage()); // 插入新的小部件
        CGlobal::instance()->centerFrameContainer()->setCurrentIndex(0);
        CGlobal::instance()->m_isdialogMousePress = false;
        this->close();
    }
}

void dlgadminParam::on_m_btnCancel_clicked()
{
    qDebug() << "dlgadminParam::on_m_btnCancel_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_isdialogMousePress = false;
    this->close();
}

//验证权限码，如果正确，返回天数，否则返回-1
//权限码分为2个部分，第一部分是本机码的累加和，用两位十进制数表示。
//第二部分是天数乘以15643
//如果本机码是123，天数是1。那么权限码就是0615643
bool dlgadminParam::VerifyPowerCode(QString strPowerCode, int nLocalNumber, int &nDayCounts)
{
    qDebug() << "dlgadminParam::VerifyPowerCode"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QFont font;
    font.setPointSize(12); // 设置字体大小为12像素
    // 在QMessageBox中设置字体
    QMessageBox messageBox;
    messageBox.setFont(font);
    QString styleSheet = "QMessageBox { border: 1px solid #333333; }";
    messageBox.setStyleSheet(styleSheet);
    messageBox.setIcon(QMessageBox::NoIcon);
    messageBox.setWindowTitle("使用期限");
    messageBox.setStandardButtons(QMessageBox::Ok);
    messageBox.button(QMessageBox::Ok)->setText("确定");
    QString log;

    QList<int> listLocalNumber;
    // 再把本机码的各个数字放在一个链表中
    QString strLocalNumber = QString::number(nLocalNumber);
    for (int i = 0; i < strLocalNumber.length(); i++)
    {
        listLocalNumber.append(strLocalNumber.mid(i, 1).toInt());
    }

    // 开始计算本机码的累加和
    int nSum = 0;
    for (int i = 0; i < listLocalNumber.size(); i++)
    {
        nSum += listLocalNumber.at(i);
    }
    // 开始累加和校验
    // 这个值是权限码的前两位所表示的数字
    int nVerifySum = strPowerCode.left(2).toInt();

    if (nSum != nVerifySum)
    {
        log = "权限代码错误!";
        messageBox.setText(log);
        int ret = messageBox.exec();
        if(ret == QMessageBox::Ok)
        {
            return false;
        }
    }

    int nRawDayCount = strPowerCode.mid(2).toInt();

    if(nRawDayCount % 15643 != 0)	//一定是15643的倍数
    {
        log = "权限代码错误!";
        messageBox.setText(log);
        int ret = messageBox.exec();
        if(ret == QMessageBox::Ok)
        {
            return false;
        }
    }
    nDayCounts = nRawDayCount / 15643;
    return true;
}




