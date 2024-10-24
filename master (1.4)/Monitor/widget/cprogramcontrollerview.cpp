#include "cprogramcontrollerview.h"
#include "ui_cprogramcontrollerview.h"
#include "cglobal.h"
#include "dm/ccontroller.h"
#include "dm/cproject.h"
#include "model/ccontrollermodel.h"

#include <QMessageBox>
#include <QToolButton>

CProgramControllerView::CProgramControllerView(QWidget *parent) :
    QWidget(parent), ui(new Ui::CProgramControllerView)
{
    ui->setupUi(this);
    CGlobal::instance()->setProgramControllerView(this);

    ui->m_pToolBar->hide();
    ui->groupBox->hide();

    m_model = new CControllerModel();
    ui->tableView->setModel(m_model);
    ui->tableView->setColumnWidth(CONTROLLER_COLUMN_NAME, 322);
    ui->tableView->setColumnWidth(CONTROLLER_COLUMN_NUMBER, 322);
    ui->tableView->setColumnWidth(CONTROLLER_COLUMN_ONLINE, 322);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

CProgramControllerView::~CProgramControllerView()
{
    CGlobal::instance()->setProgramControllerView(0);
    ui->tableView->setModel(0);
    delete m_model;
    delete ui;
}

void CProgramControllerView::setController(CController *controller)
{
    m_model->setController(controller);
}

CController* CProgramControllerView::controller()
{
    return m_model->controller();
}

void CProgramControllerView::on_pushButtonSet_clicked()
{
    QString name = ui->lineEditName->text();
    int id = ui->lineEditId->text().toInt();
    if (name.isEmpty()) {
        QMessageBox::information(this, tr("Information"), tr("Please input the controller name"), QMessageBox::Ok);
        return;
    }
    if(id <=0 ) {
        QMessageBox::information(this, tr("Information"), tr("Please input the valid controller id"), QMessageBox::Ok);
        return;
    }
    if(!(m_model->controller()))
        return;
    if((m_model->controller()->controllerId() != id) && m_model->controller()->project()->hasController(id))
    {
        QMessageBox::information(this, tr("Information"), tr("The exists controller id"), QMessageBox::Ok);
        return;
    }
    m_model->controller()->setValue(CONTROLLER_VALUE_NAME, name);
    m_model->controller()->setValue(CONTROLLER_VALUE_ID, id);
}
