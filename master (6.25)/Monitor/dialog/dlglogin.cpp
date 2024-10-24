#include "dlglogin.h"
#include "ui_dlglogin.h"
#include "global.h"
#include "cglobal.h"
#include "business/cclientbusiness.h"
#include "dialog/dlgremote.h"
#include "ctaprocess.h"

#include <QMessageBox>

#include <QDebug>

dlgLogin::dlgLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgLogin)
{
    ui->setupUi(this);
    setWindowTitle(tr("Login"));
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowModality(Qt::ApplicationModal);

    Global::instance()->m_HaveDialog = true;
    ui->m_UserName->setText(Global::instance()->m_strUserName);
    ui->m_PassWord->setText(Global::instance()->m_strPassWord);
    ui->m_btnLogin->setFocus();
}

dlgLogin::~dlgLogin()
{
    delete ui;
    Global::instance()->setLogin(NULL);
    Global::instance()->m_HaveDialog = false;
}

void dlgLogin::on_m_btnLogin_clicked()
{
    userName = ui->m_UserName->text();
    passWord = ui->m_PassWord->text();

    if(userName==tr("") || passWord==tr(""))
    {
        return;
    }
    else
    {
        Global::instance()->setMSgColor(Qt::black);
        Global::instance()->setMsgText(tr("Is connect to the server..."));
        Global::instance()->ClientBusiness()->login(Global::instance()->m_strRemoteAddress, userName, passWord);
        this->hide();

        Global::instance()->InformationWindow()->setMovie(":/res/login.gif");
        Global::instance()->InformationWindow()->setDescription(QString("<h1><font color=black>%1</font></h1>").arg(tr("Is in the login!")));
        Global::instance()->InformationWindow()->show();
    }
}

void dlgLogin::on_m_btnCancel_clicked()
{
    this->close();
}

void dlgLogin::on_m_btnNetSet_clicked()
{
    if(Global::instance()->NetRemote() == NULL)
    {
        Global::instance()->setNetRemote(new dlgRemote(this));
    }
    else
    {
        delete Global::instance()->NetRemote();
        Global::instance()->setNetRemote(NULL);
        Global::instance()->setNetRemote(new dlgRemote(this));
    }
    Global::instance()->NetRemote()->exec();
}
