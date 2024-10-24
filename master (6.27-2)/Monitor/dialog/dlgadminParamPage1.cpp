#include "ui_dlgadminParamPage.h"
#include "dlgadminParamPage.h"
#include "cglobal.h"

dlgadminParamPage::dlgadminParamPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgadminParamPage)
{
    ui->setupUi(this);
    this->setWindowTitle("系统设置");
    setWindowFlags(Qt::FramelessWindowHint);
    CGlobal::instance()->m_HaveDialog = true;
}


dlgadminParamPage::~dlgadminParamPage()
{
    delete ui;
}


