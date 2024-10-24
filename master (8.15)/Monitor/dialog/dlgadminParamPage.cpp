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
    updateSqliteData();
//    ui->SoundFCheckBox->setChecked(CGlobal::instance()->m_forbidFaultSound);
//    ui->SoundFCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
//    ui->SoundECheckBox->setChecked(CGlobal::instance()->m_forbidEmergencySound);
//    ui->SoundECheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
//    ui->MonthCheckBox->setChecked(CGlobal::instance()->m_forbidMonthcheck);
//    ui->MonthCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
//    ui->YearCheckBox->setChecked(CGlobal::instance()->m_forbidYearcheck);
//    ui->YearCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
//    ui->lineEdit_month->setText(QString::number(CGlobal::instance()->m_MonthCheckSpanSecond));
//    ui->lineEdit_year->setText(QString::number(CGlobal::instance()->m_YearCheckSpanSecond));
}

dlgadminParamPage::~dlgadminParamPage()
{
    delete ui;
    CGlobal::instance()->m_HaveDialog = false;
}

//更新数据库系统参数数据
void dlgadminParamPage::updateSqliteData()
{
    qDebug() << "dlgadminParamPage::updateSqliteData"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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

    ui->HideLinkLineCheckBox->setChecked(CGlobal::instance()->m_isHideLinkLine);
    ui->HideLinkLineCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->HideFirePointCheckBox->setChecked(CGlobal::instance()->m_isHideFirePoint);
    ui->HideFirePointCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->ShieldUnknownFirePointCheckBox->setChecked(CGlobal::instance()->m_isShieldUnknownFirePoint);
    ui->ShieldUnknownFirePointCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");

    ui->ShieldArrowLampLightFaultCheckBox->setChecked(CGlobal::instance()->m_isShieldArrowLampLightFault);
    ui->ShieldArrowLampLightFaultCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->ShieldLightLampLightFaultCheckBox->setChecked(CGlobal::instance()->m_isShieldLightLampLightFault);
    ui->ShieldLightLampLightFaultCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->ShieldUndergroundLampLightFaultCheckBox->setChecked(CGlobal::instance()->m_isShieldUndergroundLampLightFault);
    ui->ShieldUndergroundLampLightFaultCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");

    ui->ShieldLampComFaultCheckBox->setChecked(CGlobal::instance()->m_isShieldLampComFault);
    ui->ShieldLampComFaultCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->ShieldCanDeviceComFaultCheckBox->setChecked(CGlobal::instance()->m_isShieldCanDeviceComFault);
    ui->ShieldCanDeviceComFaultCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->ShieldLoginComFaultCheckBox->setChecked(CGlobal::instance()->m_isShieldLoginComFault);
    ui->ShieldLoginComFaultCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");

    ui->ShieldMainPowerFaultEmergencyCheckBox->setChecked(CGlobal::instance()->m_isShieldMainPowerFaultEmergency);
    ui->ShieldMainPowerFaultEmergencyCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
    ui->HideBatPowerFaultShowCheckBox->setChecked(CGlobal::instance()->m_isHideBatPowerFaultShow);
    ui->HideBatPowerFaultShowCheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");

    ui->OldLampcheckBox->setChecked(CGlobal::instance()->m_isOldLampEmergencyParam);
    ui->OldLampcheckBox->setStyleSheet("QCheckBox::indicator:checked {image:url(:/images/Check.png);}");
}

//系统参数变化
void dlgadminParamPage::systemChanged()
{
    qDebug() << "dlgadminParamPage::systemChanged"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CMsgObjectStatus m_MsgObjectStatus;
    QByteArray data;
    data.append(CGlobal::instance()->m_forbidFaultSound);
    data.append(CGlobal::instance()->m_forbidEmergencySound);
    data.append(CGlobal::instance()->m_forbidMonthcheck);
    data.append(CGlobal::instance()->m_forbidYearcheck);
    data.append(CGlobal::instance()->m_nCanNumber);
    data.append(CGlobal::instance()->m_MonthCheckSpanSecond);
    data.append(CGlobal::instance()->m_YearCheckSpanSecond);
    m_MsgObjectStatus.ndata = data;
    CGlobal::instance()->processSocket()->setip(CGlobal::instance()->m_DataServerIP);
    CGlobal::instance()->processSocket()->setport(CGlobal::instance()->m_DataServerPort);

    CGlobal::instance()->ClientBusiness()->exeCommand(NCT_SystemSet, &m_MsgObjectStatus);
}

void dlgadminParamPage::on_SoundFCheckBox_clicked()
{
    qDebug() << "dlgadminParamPage::on_SoundFCheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_forbidFaultSound = ui->SoundFCheckBox->isChecked();
    systemChanged();
    setParamDatabase();
}

void dlgadminParamPage::on_SoundECheckBox_clicked()
{
    qDebug() << "dlgadminParamPage::on_SoundECheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_forbidEmergencySound = ui->SoundECheckBox->isChecked();
    systemChanged();
    setParamDatabase();
}

void dlgadminParamPage::on_MonthCheckBox_clicked()
{
    qDebug() << "dlgadminParamPage::on_MonthCheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_forbidMonthcheck = ui->MonthCheckBox->isChecked();
    systemChanged();
    setParamDatabase();
}

void dlgadminParamPage::on_YearCheckBox_clicked()
{
    qDebug() << "dlgadminParamPage::on_YearCheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_forbidYearcheck = ui->YearCheckBox->isChecked();
    systemChanged();
    setParamDatabase();
}

void dlgadminParamPage::on_pushButton_month_clicked()
{
    qDebug() << "dlgadminParamPage::on_pushButton_month_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_MonthCheckSpanSecond = ui->lineEdit_month->text().toInt();
    systemChanged();
    setParamDatabase();
}

void dlgadminParamPage::on_pushButton_year_clicked()
{
    qDebug() << "dlgadminParamPage::on_pushButton_year_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_YearCheckSpanSecond = ui->lineEdit_year->text().toInt();
    systemChanged();
    setParamDatabase();
}

void dlgadminParamPage::on_HideLinkLineCheckBox_clicked()
{
    qDebug() << "dlgadminParamPage::on_HideLinkLineCheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_isHideLinkLine = ui->HideLinkLineCheckBox->isChecked();
    setParamDatabase();
    //布局视图界面灯具状态更新
    CGlobal::instance()->designSplashView()->setPageStateUpdate(true);
}

void dlgadminParamPage::on_HideFirePointCheckBox_clicked()
{
    qDebug() << "dlgadminParamPage::on_HideFirePointCheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_isHideFirePoint = ui->HideFirePointCheckBox->isChecked();
    setParamDatabase();
    //布局视图界面灯具状态更新
    CGlobal::instance()->designSplashView()->setPageStateUpdate(true);
}

void dlgadminParamPage::on_ShieldUnknownFirePointCheckBox_clicked()
{
    qDebug() << "dlgadminParamPage::on_ShieldUnknownFirePointCheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_isShieldUnknownFirePoint = ui->ShieldUnknownFirePointCheckBox->isChecked();
    setParamDatabase();
}

void dlgadminParamPage::on_ShieldArrowLampLightFaultCheckBox_clicked()
{
    qDebug() << "dlgadminParamPage::on_ShieldArrowLampLightFaultCheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_isShieldArrowLampLightFault = ui->ShieldArrowLampLightFaultCheckBox->isChecked();
    setParamDatabase();
}

void dlgadminParamPage::on_ShieldLightLampLightFaultCheckBox_clicked()
{
    qDebug() << "dlgadminParamPage::on_ShieldLightLampLightFaultCheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_isShieldLightLampLightFault = ui->ShieldLightLampLightFaultCheckBox->isChecked();
    setParamDatabase();
}

void dlgadminParamPage::on_ShieldUndergroundLampLightFaultCheckBox_clicked()
{
    qDebug() << "dlgadminParamPage::on_ShieldUndergroundLampLightFaultCheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_isShieldUndergroundLampLightFault = ui->ShieldUndergroundLampLightFaultCheckBox->isChecked();
    setParamDatabase();
}

void dlgadminParamPage::on_ShieldLampComFaultCheckBox_clicked()
{
    qDebug() << "dlgadminParamPage::on_ShieldLampComFaultCheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_isShieldLampComFault = ui->ShieldLampComFaultCheckBox->isChecked();
    setParamDatabase();
}

void dlgadminParamPage::on_ShieldCanDeviceComFaultCheckBox_clicked()
{
    qDebug() << "dlgadminParamPage::on_ShieldCanDeviceComFaultCheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_isShieldCanDeviceComFault = ui->ShieldCanDeviceComFaultCheckBox->isChecked();
    setParamDatabase();
}

void dlgadminParamPage::on_ShieldLoginComFaultCheckBox_clicked()
{
    qDebug() << "dlgadminParamPage::on_ShieldLoginComFaultCheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_isShieldLoginComFault = ui->ShieldLoginComFaultCheckBox->isChecked();
    setParamDatabase();
}

void dlgadminParamPage::on_ShieldMainPowerFaultEmergencyCheckBox_clicked()
{
    qDebug() << "dlgadminParamPage::on_ShieldMainPowerFaultEmergencyCheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_isShieldMainPowerFaultEmergency = ui->ShieldMainPowerFaultEmergencyCheckBox->isChecked();
    setParamDatabase();
}

void dlgadminParamPage::on_HideBatPowerFaultShowCheckBox_clicked()
{
    qDebug() << "dlgadminParamPage::on_HideBatPowerFaultShowCheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_isHideBatPowerFaultShow = ui->HideBatPowerFaultShowCheckBox->isChecked();
    setParamDatabase();
}

void dlgadminParamPage::on_OldLampcheckBox_clicked()
{
    qDebug() << "dlgadminParamPage::on_OldLampcheckBox_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_isOldLampEmergencyParam = ui->OldLampcheckBox->isChecked();
    setParamDatabase();
}

void dlgadminParamPage::setParamDatabase()
{
    qDebug() << "dlgadminParamPage::setParamDatabase"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fileName);
    if(!db.open())
        return;
    QSqlQuery query(db);
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('ShieldFaultSound', '%1');").arg(!CGlobal::instance()->m_forbidFaultSound));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('ShieldWarningSound', '%1');").arg(!CGlobal::instance()->m_forbidEmergencySound));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('MonthCheckSpanSecond', '%1');").arg(CGlobal::instance()->m_MonthCheckSpanSecond));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('YearCheckSpanSecond', '%1');").arg(CGlobal::instance()->m_YearCheckSpanSecond));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('HideLinkLine', '%1');").arg(CGlobal::instance()->m_isHideLinkLine));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('HideFirePoint', '%1');").arg(CGlobal::instance()->m_isHideFirePoint));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('ShieldUnknownFirePoint', '%1');").arg(CGlobal::instance()->m_isShieldUnknownFirePoint));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('ShieldArrowLampLightFault', '%1');").arg(CGlobal::instance()->m_isShieldArrowLampLightFault));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('ShieldLightLampLightFault', '%1');").arg(CGlobal::instance()->m_isShieldLightLampLightFault));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('ShieldUndergroundLampLightFault', '%1');").arg(CGlobal::instance()->m_isShieldUndergroundLampLightFault));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('ShieldLampComFault', '%1');").arg(CGlobal::instance()->m_isShieldLampComFault));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('ShieldCanDeviceComFault', '%1');").arg(CGlobal::instance()->m_isShieldCanDeviceComFault));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('ShieldLoginComFault', '%1');").arg(CGlobal::instance()->m_isShieldLoginComFault));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('ShieldMainPowerFaultEmergency', '%1');").arg(CGlobal::instance()->m_isShieldMainPowerFaultEmergency));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('HideBatPowerShow', '%1');").arg(CGlobal::instance()->m_isHideBatPowerFaultShow));
    query.exec(QString("INSERT OR REPLACE INTO ParamInfo (ParamName, ParamValue) VALUES ('OldLampEmergencyParam', '%1');").arg(CGlobal::instance()->m_isOldLampEmergencyParam));

    db.commit();
    db.close();
}
