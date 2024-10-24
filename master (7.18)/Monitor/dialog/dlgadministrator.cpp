#include "ui_dlgadministrator.h"
#include "dlgadministrator.h"
#include "cglobal.h"

dlgAdministrator::dlgAdministrator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgAdministrator)
{
    ui->setupUi(this);
    ui->comboBox->addItem("值班员");
    ui->comboBox->addItem("操作员");
    ui->comboBox->addItem("管理员");
    ui->comboBox->addItem("SuperAdmin");
    //登录后再出现此界面时默认选中当前用户等级
    if(CGlobal::instance()->m_loginStatus == true)
        ui->comboBox->setCurrentIndex(CGlobal::instance()->m_UserType);
    else
    {
        CGlobal::instance()->m_UserType = CGlobal::Administrator;
        QString fileName = CGlobal::instance()->workspaces() + "/" + "ESSQLiteCE100.db";
        CDBSqlite db(fileName);
        if(!db.open())
            return;
        QSqlQuery query = db.query(QString("SELECT ParamName,ParamValue FROM ParamInfo"));
        QSqlRecord record = query.record();
        int fieldParamName = record.indexOf("ParamName");
        int fieldParamValue = record.indexOf("ParamValue");
        while(query.next())
        {
            if(query.value(fieldParamName).toString() == "LastLoginName")
            {
                if(query.value(fieldParamValue).toString() == "值班员")
                    CGlobal::instance()->m_UserType = CGlobal::OnDuty;
                else if(query.value(fieldParamValue).toString() == "操作员")
                    CGlobal::instance()->m_UserType = CGlobal::Operator;
                else if(query.value(fieldParamValue).toString() == "管理员")
                    CGlobal::instance()->m_UserType = CGlobal::Administrator;
                else if(query.value(fieldParamValue).toString() == "SuperAdmin")
                    CGlobal::instance()->m_UserType = CGlobal::SuperAdministrator;
            }
        }
        ui->comboBox->setCurrentIndex(CGlobal::instance()->m_UserType);
    }
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    CGlobal::instance()->m_HaveDialog = true;
    m_password = "";
    m_count = 0;
}

dlgAdministrator::~dlgAdministrator()
{
    delete ui;
    CGlobal::instance()->m_HaveDialog = false;
}

void dlgAdministrator::setUserDisabled(bool enable)
{
    qDebug() << "dlgAdministrator::setUserDisabled"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    ui->comboBox->setDisabled(enable);
}


void dlgAdministrator::verifyPassword()
{
    qDebug() << "dlgAdministrator::verifyPassword"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(CGlobal::instance()->ClientBusiness()->verifyAdminUser(ui->comboBox->currentIndex(),ui->m_PassWord->text()))
    {
        accept();
    }
}



void dlgAdministrator::on_pushButton_1_clicked()
{
    qDebug() << "dlgAdministrator::on_pushButton_1_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_count++;
    if(m_count > 4)
        return;
    m_password += "1";
    ui->m_PassWord->setText(m_password);
    verifyPassword();
}

void dlgAdministrator::on_pushButton_2_clicked()
{
    qDebug() << "dlgAdministrator::on_pushButton_2_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_count++;
    if(m_count > 4)
        return;
    m_password += "2";
    ui->m_PassWord->setText(m_password);
    verifyPassword();
}

void dlgAdministrator::on_pushButton_3_clicked()
{
    qDebug() << "dlgAdministrator::on_pushButton_3_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_count++;
    if(m_count > 4)
        return;
    m_password += "3";
    ui->m_PassWord->setText(m_password);
    verifyPassword();
}

void dlgAdministrator::on_pushButton_4_clicked()
{
    qDebug() << "dlgAdministrator::on_pushButton_4_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_count++;
    if(m_count > 4)
        return;
    m_password += "4";
    ui->m_PassWord->setText(m_password);
    verifyPassword();
}

void dlgAdministrator::on_pushButton_5_clicked()
{
    qDebug() << "dlgAdministrator::on_pushButton_5_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_count++;
    if(m_count > 4)
        return;
    m_password += "5";
    ui->m_PassWord->setText(m_password);
    verifyPassword();
}

void dlgAdministrator::on_pushButton_6_clicked()
{
    qDebug() << "dlgAdministrator::on_pushButton_6_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_count++;
    if(m_count > 4)
        return;
    m_password += "6";
    ui->m_PassWord->setText(m_password);
    verifyPassword();
}

void dlgAdministrator::on_pushButton_7_clicked()
{
    qDebug() << "dlgAdministrator::on_pushButton_7_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_count++;
    if(m_count > 4)
        return;
    m_password += "7";
    ui->m_PassWord->setText(m_password);
    verifyPassword();
}

void dlgAdministrator::on_pushButton_8_clicked()
{
    qDebug() << "dlgAdministrator::on_pushButton_8_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_count++;
    if(m_count > 4)
        return;
    m_password += "8";
    ui->m_PassWord->setText(m_password);
    verifyPassword();
}

void dlgAdministrator::on_pushButton_9_clicked()
{
    qDebug() << "dlgAdministrator::on_pushButton_9_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_count++;
    if(m_count > 4)
        return;
    m_password += "9";
    ui->m_PassWord->setText(m_password);
    verifyPassword();
}

void dlgAdministrator::on_pushButton_cancel_clicked()
{
    qDebug() << "dlgAdministrator::on_pushButton_cancel_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    this->close();
}

void dlgAdministrator::on_pushButton_0_clicked()
{
    qDebug() << "dlgAdministrator::on_pushButton_0_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_count++;
    if(m_count > 4)
        return;
    m_password += "0";
    ui->m_PassWord->setText(m_password);
    verifyPassword();
}

void dlgAdministrator::on_pushButton_empty_clicked()
{
    qDebug() << "dlgAdministrator::on_pushButton_empty_clicked"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_count = 0;
    m_password = "";
    ui->m_PassWord->setText(m_password);
}
