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
    ui->MonthTimeSlider->setValue(CGlobal::instance()->m_MonthCheckSpanSecond);
    ui->label_monthtime->setText(QString::number(CGlobal::instance()->m_MonthCheckSpanSecond));
}

dlgadminParamPage::~dlgadminParamPage()
{
    delete ui;
    CGlobal::instance()->m_HaveDialog = false;
}

//更新数据库系统参数数据
void dlgadminParamPage::updateSqliteData()
{
    QString str = ui->label->text() + CGlobal::instance()->m_adminParamTime.toString(Qt::ISODate);
    ui->label->setText(str);
    ui->SoundFCheckBox->setChecked(CGlobal::instance()->m_forbidFaultSound);
    ui->SoundFCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->SoundECheckBox->setChecked(CGlobal::instance()->m_forbidEmergencySound);
    ui->SoundECheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->MonthCheckBox->setChecked(CGlobal::instance()->m_forbidMonthcheck);
    ui->MonthCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->YearCheckBox->setChecked(CGlobal::instance()->m_forbidYearcheck);
    ui->YearCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->MonthTimeSlider->setValue(CGlobal::instance()->m_MonthCheckSpanSecond);
    ui->label_monthtime->setText(QString::number(CGlobal::instance()->m_MonthCheckSpanSecond));
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
    CGlobal::instance()->m_MonthCheckSpanSecond = ui->MonthTimeSlider->value();
    data.append(CGlobal::instance()->m_forbidFaultSound);
    data.append(CGlobal::instance()->m_forbidEmergencySound);
    data.append(CGlobal::instance()->m_forbidMonthcheck);
    data.append(CGlobal::instance()->m_forbidYearcheck);
    data.append(CGlobal::instance()->m_SendDirectionAndTwinkleByOne);
    data.append(CGlobal::instance()->m_UCSendDirectionAndTwinkleByOne);
    data.append(CGlobal::instance()->m_nCanNumber);
    data.append(ui->MonthTimeSlider->value());
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

void dlgadminParamPage::on_MonthTimeSlider_valueChanged(int value)
{
    ui->label_monthtime->setText(QString::number(value));
    systemChanged();
}
