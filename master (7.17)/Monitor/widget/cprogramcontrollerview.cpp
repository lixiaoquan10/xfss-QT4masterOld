#include "ui_cprogramprojectview.h"
#include "cglobal.h"


CProgramControllerView::CProgramControllerView(QWidget *parent) :
    QWidget(parent), ui(new Ui::CProgramControllerView)
{
    ui->setupUi(this);
    CGlobal::instance()->setProgramControllerView(this);
    ui->m_pToolBar->hide();
    ui->groupBox->hide();

    m_model = new CControllerModel();
    ui->tableView->setModel(m_model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->addSelectEditColumn(CONTROLLER_COLUMN_NAME);
    ui->tableView->addSelectEditColumn(CONTROLLER_COLUMN_DESCRIPTION);
    ui->tableView->setColumnWidth(CONTROLLER_COLUMN_ADDRESS, 322);
    ui->tableView->setColumnWidth(CONTROLLER_COLUMN_NAME, 322);
    ui->tableView->setColumnWidth(CONTROLLER_COLUMN_DESCRIPTION, 322);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
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
    qDebug() << "CProgramControllerView::on_pushButtonSet_clicked" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString name = ui->lineEditName->text();
    if (name.isEmpty()) {
        QMessageBox::information(this, tr("Information"), tr("Please input the controller name"), QMessageBox::Ok);
        return;
    }

    if(!(m_model->controller()))
        return;
    m_model->controller()->setValue(CONTROLLER_VALUE_NAME, name);
    m_model->controller()->setValue(CONTROLLER_VALUE_ADDRESS, ui->lineEditCodeRule->text());
}

void CProgramControllerView::addCanport()
{
    qDebug() << "CProgramControllerView::addCanport" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CController* controller = m_model->controller();
    if(!controller)
        return;
    if(controller->canportCount() >= CONTROLLER_MAX_CONTROLLER_NUM)
        return;
    for(int i = 0; i < CONTROLLER_MAX_CONTROLLER_NUM; ++i)
    {
        if(!controller->hasCanport(i + 1))
        {
            CCanport* canport = new CCanport();
            if(!canport)
                return;
            canport->setCanportId(i + 1);
            canport->setCanportValue(CANPORT_VALUE_NAME, canport->canportValue(CANPORT_VALUE_NAME).toString().append(QString::number(i + 1)));
            controller->addCanport(canport);
            break;
        }
    }
}

void CProgramControllerView::delCanport()
{
    qDebug() << "CProgramControllerView::delCanport" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QModelIndexList indexs = ui->tableView->selectionModel()->selectedRows();
    for(int i = 0; i < indexs.size(); ++i)
    {
        QModelIndex index = indexs.at(i);
        CCanport* canport = static_cast<CCanport*>(m_model->getObject(index));
        if(!canport)
            return;
        canport->controller()->removeCanport(canport);
    }
}
