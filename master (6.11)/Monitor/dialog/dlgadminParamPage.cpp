#include "dlgadminParamPage.h"
#include "ui_dlgadminParamPage.h"
#include "cglobal.h"

#include <QMessageBox>
#include <QSqlQuery>

dlgadminParamPage::dlgadminParamPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgadminParamPage)
{
    ui->setupUi(this);
    CGlobal::instance()->m_HaveDialog = true;
    ui->SoundFCheckBox->setChecked(CGlobal::instance()->m_forbidFaultSound);
    ui->SoundFCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->SoundECheckBox->setChecked(CGlobal::instance()->m_forbidEmergencySound);
    ui->SoundECheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->MonthCheckBox->setChecked(CGlobal::instance()->m_forbidMonthcheck);
    ui->MonthCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->YearCheckBox->setChecked(CGlobal::instance()->m_forbidYearcheck);
    ui->YearCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->lineEdit_month->setText(QString::number(CGlobal::instance()->m_MonthCheckSpanSecond));
    ui->lineEdit_year->setText(QString::number(CGlobal::instance()->m_YearCheckSpanSecond));
}

dlgadminParamPage::~dlgadminParamPage()
{
    delete ui;
    CGlobal::instance()->m_HaveDialog = false;
}

//更新数据库系统参数数据
void dlgadminParamPage::updateSqliteData()
{
    QString str = "高级参数显示截止时间：" + CGlobal::instance()->m_adminParamTime.toString(Qt::ISODate);
    str.replace("T", " "); // 将字符串中的 "T" 替换为空格
    ui->label->setText(str);
    ui->SoundFCheckBox->setChecked(CGlobal::instance()->m_forbidFaultSound);
    ui->SoundFCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->SoundECheckBox->setChecked(CGlobal::instance()->m_forbidEmergencySound);
    ui->SoundECheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->MonthCheckBox->setChecked(CGlobal::instance()->m_forbidMonthcheck);
    ui->MonthCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->YearCheckBox->setChecked(CGlobal::instance()->m_forbidYearcheck);
    ui->YearCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->lineEdit_month->setText(QString::number(CGlobal::instance()->m_MonthCheckSpanSecond));
    ui->lineEdit_year->setText(QString::number(CGlobal::instance()->m_YearCheckSpanSecond));
    ui->lineEdit_phoneNumber->setText(CGlobal::instance()->m_LinkmanPhoneNumer);
}

//系统参数变化
void dlgadminParamPage::systemChanged()
{
    CMsgObjectStatus m_MsgObjectStatus;
    QByteArray data;

    CGlobal::instance()->m_forbidFaultSound = ui->SoundFCheckBox->isChecked();
    CGlobal::instance()->m_forbidEmergencySound = ui->SoundECheckBox->isChecked();
    CGlobal::instance()->m_forbidMonthcheck = ui->MonthCheckBox->isChecked();
    CGlobal::instance()->m_forbidYearcheck = ui->YearCheckBox->isChecked();
    CGlobal::instance()->m_MonthCheckSpanSecond = ui->lineEdit_month->text().toInt();
    CGlobal::instance()->m_YearCheckSpanSecond = ui->lineEdit_year->text().toInt();
    data.append(CGlobal::instance()->m_forbidFaultSound);
    data.append(CGlobal::instance()->m_forbidEmergencySound);
    data.append(CGlobal::instance()->m_forbidMonthcheck);
    data.append(CGlobal::instance()->m_forbidYearcheck);
    data.append(CGlobal::instance()->m_SendDirectionAndTwinkleByOne);
    data.append(CGlobal::instance()->m_UCSendDirectionAndTwinkleByOne);
    data.append(CGlobal::instance()->m_nCanNumber);
    data.append(ui->lineEdit_month->text().toInt());
    data.append(ui->lineEdit_year->text().toInt());
    m_MsgObjectStatus.ndata = data;
    m_MsgObjectStatus.nDataServerIP = CGlobal::instance()->m_DataServerIP;
    m_MsgObjectStatus.nDataServerPort = CGlobal::instance()->m_DataServerPort;

    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_SystemSet, &m_MsgObjectStatus);
    CGlobal::instance()->TimeInterval()->saveSystemStatus();
}

void dlgadminParamPage::on_SoundFCheckBox_clicked()
{
    systemChanged();
}

void dlgadminParamPage::on_SoundECheckBox_clicked()
{
    systemChanged();
}

void dlgadminParamPage::on_MonthCheckBox_clicked()
{
    systemChanged();
}

void dlgadminParamPage::on_YearCheckBox_clicked()
{
    systemChanged();
}

void dlgadminParamPage::on_pushButton_month_clicked()
{
    systemChanged();
}

void dlgadminParamPage::on_pushButton_year_clicked()
{
    systemChanged();
}

void dlgadminParamPage::on_pushButton_phoneNumber_clicked()
{
    CGlobal::instance()->m_LinkmanPhoneNumer = ui->lineEdit_phoneNumber->text();
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fileName);
    if(!db.open())
        return;
    QSqlQuery query(db);
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('LinkmanPhoneNumer', '%1');").arg(CGlobal::instance()->m_LinkmanPhoneNumer));
    db.commit();
    db.close();
}


