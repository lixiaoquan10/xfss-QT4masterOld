#include "cprogramprojectview.h"
#include "ui_cprogramprojectview.h"
#include "cglobal.h"
#include "dm/ccontroller.h"
#include "dm/cproject.h"
#include "dm/cloop.h"
#include "model/cprojectmodel.h"

#include <QMessageBox>
#include <QToolButton>

CProgramProjectView::CProgramProjectView(QWidget *parent) :
    QWidget(parent), ui(new Ui::CProgramProjectView)
{
    ui->setupUi(this);
    CGlobal::instance()->setProgramProjectView(this);
    ui->m_pToolBar->hide();
    ui->groupBox->hide();

    m_model = new CProjectModel();
    ui->tableView->setModel(m_model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->addSelectEditColumn(PROJECT_COLUMN_NAME);
    ui->tableView->addSelectEditColumn(PROJECT_COLUMN_DESCRIPTION);
    ui->tableView->setColumnWidth(PROJECT_COLUMN_ADDRESS, 322);
    ui->tableView->setColumnWidth(PROJECT_COLUMN_NAME, 322);
    ui->tableView->setColumnWidth(PROJECT_COLUMN_DESCRIPTION, 322);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

CProgramProjectView::~CProgramProjectView()
{
    CGlobal::instance()->setProgramProjectView(0);
    ui->tableView->setModel(0);
    delete m_model;
    delete ui;
}

void CProgramProjectView::setProject(CProject *project)
{
    m_model->setProject(project);
}

CProject* CProgramProjectView::project()
{
    return m_model->project();
}

void CProgramProjectView::on_pushButtonSet_clicked()
{
    QString name = ui->lineEditName->text();
    if (name.isEmpty()) {
        QMessageBox::information(this, tr("Information"), tr("Please input the project name"), QMessageBox::Ok);
        return;
    }

    if(!(m_model->project()))
        return;
    m_model->project()->setValue(PROJECT_VALUE_NAME, name);
    m_model->project()->setValue(PROJECT_VALUE_ADDRESS, ui->lineEditCodeRule->text());
}

void CProgramProjectView::addController()
{
    CProject* project = m_model->project();
    if(!project)
        return;
    if(project->controllerCount() >= PROJECT_MAX_CONTROLLER_NUM)
        return;
    for(int i = 0; i < PROJECT_MAX_CONTROLLER_NUM; ++i)
    {
        if(!project->hasController(i + 1))
        {
            CController* controller = new CController();
            if(!controller)
                return;
            controller->setControllerId(i + 1);
            controller->setValue(CONTROLLER_VALUE_NAME, controller->value(CONTROLLER_VALUE_NAME).toString().append(QString::number(i + 1)));
            project->addController(controller);
            break;
        }
    }
}

void CProgramProjectView::delController()
{
    QModelIndexList indexs = ui->tableView->selectionModel()->selectedRows();
    for(int i = 0; i < indexs.size(); ++i)
    {
        QModelIndex index = indexs.at(i);
        CController* controller = static_cast<CController*>(m_model->getObject(index));
        if(!controller)
            return;
        controller->project()->removeController(controller);
    }
}
