#include "dlgusetime.h"
#include "ui_dlgusetime.h"
#include "dlgtimeinterval.h"
#include "widget/ctoptoolbar.h"
#include "cglobal.h"

#include <QMessageBox>
#include <QSqlQuery>

dlgUseTime::dlgUseTime(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgUseTime)
{
    ui->setupUi(this);
    ui->label_message->setWordWrap(true);
    m_useDay = 0;
    m_localNumber = qrand() % (1000000 - 100000 + 1) + 100000;
    ui->m_LocalNumber->setText(QString::number(m_localNumber));
    CGlobal::instance()->m_HaveDialog = true;
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    ui->label_message->setStyleSheet("color: rgb(255, 0, 0); font: bold 14pt;");
    setlabelMessage();
    ui->m_linkmanPhoneNumber->setText(CGlobal::instance()->m_LinkmanPhoneNumer);
    ui->m_rPowerCode->setFocus();
    CGlobal::instance()->m_lineEditwidget = dynamic_cast<QWidget*>(ui->m_rPowerCode);
    if(!CGlobal::instance()->m_selfCheck)
    {
        CGlobal::instance()->m_isdialogMousePress = true;
    }
}

dlgUseTime::~dlgUseTime()
{
    delete ui;
    CGlobal::instance()->m_HaveDialog = false;
}

void dlgUseTime::setlabelMessage()
{
    qDebug() << "dlgUseTime::setlabelMessage"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(CGlobal::instance()->m_LiftTime <= 15)
    {
        QString str = "您的软件使用期限剩余" +  QString::number(CGlobal::instance()->m_LiftTime) +
                "天，请及时联系服务提供商，并提供本机序号。获得权限序号后，点击确定。";
        ui->label_message->setText(str);

    }
    else
    {
        ui->label_message->setText("请输入权限序号");
    }
    ui->m_btnCancel->setText("取消");
    if(CGlobal::instance()->m_LiftTime == 0)
    {
        ui->label_message->setText("您的软件使用权限已到期，请联系服务提供商，并提供本机序号。获得权限序号后，点击确定。");
        ui->m_btnCancel->setText("复位");
    }
}


void dlgUseTime::on_m_btnOk_clicked()
{
    qDebug() << "dlgUseTime::on_m_btnOk_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    setlabelMessage();
    if(ui->m_rPowerCode->text() == "")
    {
        ui->m_rPowerCode->setPlaceholderText("请输入权限序号...");
        return;
    }
    if(ui->m_linkmanPhoneNumber->text().length() != 11)
    {
        QString str = ui->label_message->text() + "\n" + "联系电话输入位数错误！！！";
        ui->label_message->setText(str);
        return;
    }
    bool bVerified = VerifyPowerCode(ui->m_rPowerCode->text(),m_localNumber,m_useDay);
    if(bVerified)
    {
        CGlobal::instance()->m_LiftTime += m_useDay;
        CGlobal::instance()->TimeInterval()->setUseTime(CGlobal::instance()->m_LiftTime);
        CGlobal::instance()->m_LinkmanPhoneNumer = ui->m_linkmanPhoneNumber->text();
        QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(fileName);
        if(!db.open())
            return;
        QSqlQuery query(db);
        query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('LiftTime', '%1');").arg(CGlobal::instance()->m_LiftTime));
        query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('LinkmanPhoneNumber', '%1');").arg(CGlobal::instance()->m_LinkmanPhoneNumer));
        db.commit();
        db.close();
        CGlobal::instance()->m_isdialogMousePress = false;
        this->close();
    }
}

void dlgUseTime::on_m_btnCancel_clicked()
{
    qDebug() << "dlgUseTime::on_m_btnCancel_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(CGlobal::instance()->m_LiftTime == 0)
    {
        CGlobal::instance()->TopToolBar()->slotResetSystem();
        return;
    }
    CGlobal::instance()->m_isdialogMousePress = false;
    this->close();
}

// <summary>
// 这个函数检查用户输入的权限序号。权限序号由本机序号、使用天数和累加校验三部分组成。第一部分是将本机
// 序号的各个位相加得到的一个数，如：本机序号是123，那么这个数就是1+2+3=6，另要求第一部分站两个字符
// ，所以6应该写成06。第二部分是使用天数加上456后所得到的数字，如使用天数是100，那么第二部分的数字
// 就是556。第三部分位于最后两个数字，这两个数字是所有前面数字的累加和。
// 所以，如果本机序号是123，使用天数是100，那么得到的权限序号就是0655622。
// 注意，这个算法要求使用天数位数不能太多，所以建议使用天数不超过5位数。
// </summary>
// <param name="strPowerCode">需要验证的权限序号</param>
// <param name="nLocalNumber">本机序号</param>
// <param name="nDayCounts">如果验证成功，这个值表示新的使用天数</param>
// <returns></returns>
bool dlgUseTime::VerifyPowerCode(QString strPowerCode, int nLocalNumber, int &nDayCounts)
{
    qDebug() << "dlgUseTime::VerifyPowerCode"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QList<int> listPowerCode;
    // 首先把权限序号的各个数字放在一个链表中
    for (int i = 0; i < strPowerCode.length(); i++)
    {
        listPowerCode.append(strPowerCode.mid(i, 1).toInt());
    }

    QList<int> listLocalNumber;
    // 再把本机序号的各个数字放在一个链表中
    QString strLocalNumber = QString::number(nLocalNumber);
    for (int i = 0; i < strLocalNumber.length(); i++)
    {
        listLocalNumber.append(strLocalNumber.mid(i, 1).toInt());
    }

    // 开始累加和校验
    // 这个值是权限序号的后两位所表示的数字
    int nVerifySum = listPowerCode.at(listPowerCode.size() - 1) + listPowerCode.at(listPowerCode.size() - 2) * 10;
    // 这个值存放实际计算出的累加和
    int nSum = 0;
    for (int i = 0; i < listPowerCode.size() - 2; i++)
    {
        nSum += listPowerCode.at(i);
    }
    if (nSum != nVerifySum)
    {
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
        log = "权限序号错误!";
        messageBox.setText(log);
        int ret = messageBox.exec();
        if(ret == QMessageBox::Ok)
        {
            return false;
        }
    }

    // 开始本机序号校验
    // 这个值是权限需要的前两位所表示的数字
    nVerifySum = listPowerCode.at(0) * 10 + listPowerCode.at(1);
    // 开始计算本地序号的累加和
    nSum = 0;
    for (int i = 0; i < listLocalNumber.size(); i++)
    {
        nSum += listLocalNumber.at(i);
    }
    if (nSum != nVerifySum)
    {
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
        log = "本机序号错误!";
        messageBox.setText(log);
        int ret = messageBox.exec();
        if(ret == QMessageBox::Ok)
        {
            return false;
        }
    }

    // 校验都通过了，这里得到使用天数
    QList<int> listDayCounts;
    for (int i = 2; i < listPowerCode.size() - 2; i++)
    {
        listDayCounts.append(listPowerCode.at(i));
    }
    nDayCounts = 0;
    for (int i = 0; i < listDayCounts.size(); i++)
    {
        int n = listDayCounts.at(i) * MyPow(10, listDayCounts.size() - 1 - i);
        nDayCounts += n;
    }
    nDayCounts -= 456;

    return true;
}

// <summary>
// 乘幂函数
// </summary>
// <param name="dishu">底数</param>
// <param name="mi">幂</param>
// <returns>底数的幂次方</returns>
int dlgUseTime::MyPow(int dishu, int mi)
{
    qDebug() << "dlgUseTime::MyPow"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int nResult = 1;
    for (int i = 0; i < mi; i++)
    {
        nResult *= dishu;
    }
    return nResult;
}



