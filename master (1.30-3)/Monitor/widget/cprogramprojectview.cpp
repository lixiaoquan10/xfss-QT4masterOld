#include "cprogramprojectview.h"
#include "ui_cprogramprojectview.h"
#include "cglobal.h"
#include "dm/ccanport.h"
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

void CProgramProjectView::addCanport()
{
    CProject* project = m_model->project();
    if(!project)
        return;
    if(project->canportCount() >= PROJECT_MAX_CONTROLLER_NUM)
        return;
    for(int i = 0; i < PROJECT_MAX_CONTROLLER_NUM; ++i)
    {
        if(!project->hasCanport(i + 1))
        {
            CCanport* canport = new CCanport();
            if(!canport)
                return;
            canport->setCanportId(i + 1);
            canport->setCanportValue(CANPORT_VALUE_NAME, canport->canportValue(CANPORT_VALUE_NAME).toString().append(QString::number(i + 1)));
            project->addCanport(canport);
            break;
        }
    }
}

void CProgramProjectView::delCanport()
{
    QModelIndexList indexs = ui->tableView->selectionModel()->selectedRows();
    for(int i = 0; i < indexs.size(); ++i)
    {
        QModelIndex index = indexs.at(i);
        CCanport* canport = static_cast<CCanport*>(m_model->getObject(index));
        if(!canport)
            return;
        canport->project()->removeCanport(canport);
    }
}
