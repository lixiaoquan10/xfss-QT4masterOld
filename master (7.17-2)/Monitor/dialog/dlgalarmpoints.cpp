#include "dlgalarmpoints.h"
#include "ui_dlgalarmpoints.h"
#include "model/calarmpointsfiltertablemodel.h"
#include "dm/cproject.h"
#include "dm/calarmpoint.h"
#include "dm/ccontroller.h"
#include "dm/cdm.h"
#include "graphics/cpointalarmpointitem.h"
#include "cglobal.h"
#include "ctrace.h"

#include <QPainter>
#include <QRadialGradient>
#include <QMimeData>
#include <QDrag>

QPixmap* dlgAlarmPoints::m_pixmap = NULL;

dlgAlarmPoints::dlgAlarmPoints(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgAlarmPoints)
{
    ui->setupUi(this);
    CGlobal::instance()->setDlgAlarmPoints(this);
    setAttribute(Qt::WA_DeleteOnClose);
    if(!m_pixmap)
    {
        int size = 32;
        m_pixmap = new QPixmap(size, size);
        m_pixmap->fill(Qt::transparent);
        QPainter painter(m_pixmap);
        QRadialGradient radialGradient;
        radialGradient.setColorAt(0, Qt::red);
        radialGradient.setColorAt(1, Qt::yellow);
        QPointF point(size / 2, size / 2);
        radialGradient.setCenter(point);
        radialGradient.setFocalPoint(point);
        radialGradient.setRadius(size / 2);
        painter.setBrush(radialGradient);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(0, 0, size, size);
    }
    m_model = new CAlarmPointsFilterTableModel(CGlobal::instance()->dm());
    ui->tableView->setDragEnabled(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setModel(m_model);
    ui->tableView->setColumnWidth(ALARMPOINTSFILTERTABLE_COLUMN_CONTROLLER_ID, 80);
    ui->tableView->setColumnWidth(ALARMPOINTSFILTERTABLE_COLUMN_ADDRESS, 120);
    ui->tableView->setColumnWidth(ALARMPOINTSFILTERTABLE_COLUMN_DESCRIPTION, 200);
    initData();
    connect(ui->comboBoxController, SIGNAL(currentIndexChanged(int)), this, SLOT(controllerChanged(int)));
    connect(ui->lineEditAlarmPointAddress, SIGNAL(textChanged(QString)), this, SLOT(addressChanged(QString)));
    connect(ui->tableView, SIGNAL(signalStartDrag(Qt::DropActions)), this, SLOT(startDrag(Qt::DropActions)));
    connect(CGlobal::instance()->dm(), SIGNAL(removedObject(CObject*)), this, SLOT(addRemovedObject(CObject*)));
    connect(CGlobal::instance()->dm(), SIGNAL(addedObject(CObject*)), this, SLOT(removeAddObject(CObject*)));
}

dlgAlarmPoints::~dlgAlarmPoints()
{
    CGlobal::instance()->setDlgAlarmPoints(NULL);
    delete ui;
    delete m_model;
}

void dlgAlarmPoints::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    deleteLater();
}

QList<CAlarmPoint*> dlgAlarmPoints::currentSelected()
{
    QList<CAlarmPoint*> objects;
    QModelIndexList indexs = ui->tableView->selectionModel()->selectedRows();
    for(int i = 0; i < indexs.size(); ++i)
    {
        CAlarmPoint* object = m_model->getAlarmPoint(indexs.at(i).row());
        if(object)
            objects.append(object);
    }
    return objects;
}

void dlgAlarmPoints::initData()
{
    CTRACE_LOG;
    CProject* project = CGlobal::instance()->dm()->projectAt(0);
    loadController(project);
}

void dlgAlarmPoints::loadController(CProject *project)
{
    CTRACE_LOG;
    ui->comboBoxController->clear();
    ui->comboBoxController->addItem(tr("All"), 0);
    if(!project)
        return;
    for(int i = 0; i < project->controllerCount(); ++i)
    {
        CController* controller = project->controller(i);
        ui->comboBoxController->addItem(controller->value(CONTROLLER_VALUE_NAME).toString(), controller->keyId());
    }
}

void dlgAlarmPoints::on_pushButtonOk_clicked()
{
    CTRACE_LOG;
    if(isModal())
        QDialog::accept();
    else
        close();
}

void dlgAlarmPoints::on_pushButtonCancel_clicked()
{
    CTRACE_LOG;
    if(isModal())
        QDialog::reject();
    else
        close();
}

void dlgAlarmPoints::controllerChanged(const int i)
{
    CTRACE_LOG;
    int keyId = ui->comboBoxController->itemData(i).toInt();
    CController* controller = static_cast<CController*>(CGlobal::instance()->dm()->object(keyId));
    m_model->setFilter(controller);
    m_model->reload();
}

void dlgAlarmPoints::addressChanged(const QString address)
{
    CTRACE_LOG;
    m_model->setFilter(address);
}

void dlgAlarmPoints::startDrag(Qt::DropActions supportedActions)
{
    CTRACE_LOG;
    Q_UNUSED(supportedActions);
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) return;
    CAlarmPoint* alarmPoint = m_model->getAlarmPoint(index.row());
    if(!alarmPoint) return;

    QMimeData *mimeData = new QMimeData;
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << reinterpret_cast<qlonglong>(alarmPoint);
    mimeData->setData("device/ts-alarmpoint", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(m_pixmap->width()/2, m_pixmap->height()/2));
    drag->setPixmap(*m_pixmap);
    if (drag->exec(Qt::MoveAction) == Qt::MoveAction) {
        ;
    }
}

void dlgAlarmPoints::addRemovedObject(CObject *object)
{
    if(object->type() == CPointAlarmPointItem::type_s)
    {
        CPointAlarmPointItem* item = static_cast<CPointAlarmPointItem*>(object);
        CAlarmPoint* alarmPoint = static_cast<CAlarmPoint*>(item->alarmPoint());
        item->setAlarmPoint(NULL);
        m_model->addAlarmPoint(alarmPoint);
        if(currentSelected().isEmpty())
            ui->tableView->selectRow(0);
    }
}

void dlgAlarmPoints::removeAddObject(CObject *object)
{
    if(object->type() == CPointAlarmPointItem::type_s)
    {
        CPointAlarmPointItem* item = static_cast<CPointAlarmPointItem*>(object);
        CAlarmPoint* alarmPoint = static_cast<CAlarmPoint*>(item->alarmPoint());
        m_model->removeAlarmPoint(alarmPoint);
    }
}

