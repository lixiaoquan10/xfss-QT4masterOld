#include "dlgproject.h"
#include "ui_dlgproject.h"
#include "model/cdlgprojecttablemodel.h"
#include "cglobal.h"
#include "ctrace.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QFileInfo>
#include <QDebug>

dlgProject::dlgProject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgProject)
{
    CTRACE_LOG;
    ui->setupUi(this);
    setWindowTitle(tr("Projects"));
    m_model = new CDlgProjectTableModel(CGlobal::instance()->workspaces());
    ui->tableView->setModel(m_model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setColumnWidth(DLGPROJECTTABLE_COLUMN_NAME, 350);
    ui->tableView->setColumnWidth(DLGPROJECTTABLE_COLUMN_CREATETIME, 130);
    ui->tableView->setColumnWidth(DLGPROJECTTABLE_COLUMN_UPDATETIME, 130);
    ui->tableView->setColumnWidth(DLGPROJECTTABLE_COLUMN_VERSION, 65);
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(dbclickTable(QModelIndex)));
}

dlgProject::~dlgProject()
{
    delete ui;
}

QString dlgProject::currentSelected() const
{
    CTRACE_LOG;
    return m_model->dirName(ui->tableView->currentIndex());
}

void dlgProject::on_btnImport_clicked()
{
    CTRACE_LOG;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import project"),
                                                     QDir::homePath(),
                                                     tr("TSConfig Project (*.tsp)"));
    if (fileName.isEmpty()) return;
    QFileInfo fileInfo(fileName);
    QString projectName = fileInfo.completeBaseName();
    while(m_model->hasProject(projectName))
    {
        bool ok;
        QString text = QInputDialog::getText(this, tr("The project name is exists, please reinput!"),
                                             tr("Please input project name:"), QLineEdit::Normal,
                                             projectName, &ok);
        if(!ok)
            return;
        if (!text.isEmpty())
            projectName = text;
        if(!m_model->hasProject(projectName))
            break;
    }
    qDebug()<<"Import:"<<projectName<<fileName;
    if(CGlobal::instance()->importProject(CGlobal::instance()->workspaces(), projectName, fileName))
    {
        QMessageBox::information(this, tr("Information"), tr("Import project success!"));
        m_model->reload();
    }
    else
        QMessageBox::information(this, tr("Information"), tr("Import project failure!"));
}

void dlgProject::on_btnDelete_clicked()
{
    CTRACE_LOG;
    QModelIndexList indexs = ui->tableView->selectionModel()->selectedRows();
    if(indexs.isEmpty())
        return;
    if (QMessageBox::information(this, tr("Information"), tr("Are you sure delete project?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;
    m_model->deleteProject(indexs);
}

void dlgProject::on_btnOpen_clicked()
{
    CTRACE_LOG;
    m_projectName = currentSelected();
    if(m_projectName.isEmpty())
    {
        QMessageBox::information(this, tr("Information"), tr("Please select a project"));
        return;
    }
    accept();
}

void dlgProject::on_btnExport_clicked()
{
    CTRACE_LOG;
    QString projectName = currentSelected();
    if(projectName.isEmpty())
        return;
    QString fileName  = QFileDialog::getSaveFileName(this, tr("Export project"),
                                                         QDir::homePath() + "/" + projectName,
                                                         tr("TSConfig Project (*.tsp)"));
    if(fileName.isEmpty()) return;
    qDebug()<<"Export:"<<projectName<<fileName;
    if(CGlobal::instance()->exportProject(CGlobal::instance()->workspaces(), projectName, fileName))
        QMessageBox::information(this, tr("Information"), tr("Export project success!"));
    else
        QMessageBox::information(this, tr("Information"), tr("Export project failure!"));
}

void dlgProject::dbclickTable(const QModelIndex &index)
{
    CTRACE_LOG;
    Q_UNUSED(index);
    on_btnOpen_clicked();
}
