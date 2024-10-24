#include "cprogramcontrollerview.h"
#include "ui_cprogramcontrollerview.h"
#include "cglobal.h"
#include "dm/ccontroller.h"
#include "dm/cproject.h"
#include "model/ccontrollermodel.h"

#include <QMessageBox>
#include <QToolButton>

CProgramCanportView::CProgramCanportView(QWidget *parent) :
    QWidget(parent), ui(new Ui::CProgramCanportView)
{
    ui->setupUi(this);
    CGlobal::instance()->setProgramCanportView(this);

    ui->m_pToolBar->hide();
    ui->groupBox->hide();

    m_model = new CCanportModel();
    ui->tableView->setModel(m_model);
    ui->tableView->setColumnWidth(CONTROLLER_COLUMN_NAME, 322);
    ui->tableView->setColumnWidth(CONTROLLER_COLUMN_NUMBER, 322);
    ui->tableView->setColumnWidth(CONTROLLER_COLUMN_ONLINE, 322);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

CProgramCanportView::~CProgramCanportView()
{
    CGlobal::instance()->setProgramCanportView(0);
    ui->tableView->setModel(0);
    delete m_model;
    delete ui;
}

void CProgramCanportView::setCanport(CCanport *canport)
{
    m_model->setCanport(canport);
}

CCanport* CProgramCanportView::canport()
{
    return m_model->canport();
}

void CProgramCanportView::on_pushButtonSet_clicked()
{
    QString name = ui->lineEditName->text();
    int id = ui->lineEditId->text().toInt();
    if (name.isEmpty()) {
        QMessageBox::information(this, tr("Information"), tr("Please input the canport name"), QMessageBox::Ok);
        return;
    }
    if(id <=0 ) {
        QMessageBox::information(this, tr("Information"), tr("Please input the valid canport id"), QMessageBox::Ok);
        return;
    }
    if(!(m_model->canport()))
        return;
    if((m_model->canport()->canportId() != id) && m_model->canport()->project()->hasCanport(id))
    {
        QMessageBox::information(this, tr("Information"), tr("The exists canport id"), QMessageBox::Ok);
        return;
    }
    m_model->canport()->setCanportValue(CANPORT_VALUE_NAME, name);
    m_model->canport()->setCanportValue(CANPORT_VALUE_ID, id);
}
