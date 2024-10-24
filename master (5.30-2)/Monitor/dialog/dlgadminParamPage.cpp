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
}

dlgadminParamPage::~dlgadminParamPage()
{
    delete ui;
    CGlobal::instance()->m_HaveDialog = false;
}

