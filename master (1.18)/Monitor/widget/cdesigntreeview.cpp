#include "cglobal.h"
#include "style/cstyledbar.h"
#include "cprogramtreeview.h"
#include "cdesigncontainer.h"
#include "view/cprojecttreeview.h"
#include "model/cdesigntreemodel.h"
#include "model/ctreeitem.h"
#include "dm/cproject.h"
#include "dm/clayer.h"
#include "dm/cdm.h"
#include "cdesigntreeview.h"
#include "cdesignsplashview.h"
#include "widget/cbottomtoolbar.h"

#include <QVBoxLayout>
#include <QStackedLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QMessageBox>
#include <QDrag>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QDebug>

CDesignTreeView::CDesignTreeView(QWidget *parent) :
    QWidget(parent), m_currentLayer(NULL)
{
    CGlobal::instance()->setDesignTreeView(this);
    setMinimumWidth(300);

    QVBoxLayout* pMainLayout = new QVBoxLayout();
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(0);
    m_designTreeView = new CProjectTreeView(this);
    m_designTreeView->setDragEnabled(true);
    m_designTreeView->setAcceptDrops(true);
    QObject::connect(m_designTreeView, SIGNAL(sigSelectionChanged(QItemSelection,QItemSelection)), this, SLOT(slotSelectionChanged(QItemSelection,QItemSelection)));
    QObject::connect(m_designTreeView, SIGNAL(signalStartDrag(Qt::DropActions)), this, SLOT(slotStartDrag(Qt::DropActions)));
    QObject::connect(m_designTreeView, SIGNAL(signalDragEnterEvent(QDragEnterEvent*)), this, SLOT(slotDragEnterEvent(QDragEnterEvent*)));
    QObject::connect(m_designTreeView, SIGNAL(signalDragMoveEvent(QDragMoveEvent*)), this, SLOT(slotDragMoveEvent(QDragMoveEvent*)));
    QObject::connect(m_designTreeView, SIGNAL(signalDropEvent(QDropEvent*)), this, SLOT(slotDropEvent(QDropEvent*)));
    m_designTreeModel = new CDesignTreeModel();
    m_designTreeModel->setTrack(false);
    m_designTreeView->setModel(m_designTreeModel);
    pMainLayout->addWidget(m_designTreeView);
    setLayout(pMainLayout);
}

CDesignTreeView::~CDesignTreeView()
{
    CGlobal::instance()->setDesignTreeView(0);
    delete m_designTreeModel;
    delete m_designTreeView;
}

void CDesignTreeView::setCurrentView(int index)
{
    Q_UNUSED(index);
}

void CDesignTreeView::setProject(CProject *project)
{
//    setCurrentLayer(NULL);
    m_designTreeModel->setProject(project);
    m_designTreeView->expandAll();
}

void CDesignTreeView::setLayer(CLayer *layer)
{
    QModelIndex index = m_designTreeModel->getIndex(layer);
    m_designTreeView->setCurrentIndex(index);
}

void CDesignTreeView::update(CObject *object)
{
    m_designTreeModel->update(object);
}

void CDesignTreeView::slotNewProject()
{
    CGlobal::instance()->programTreeView()->slotNewProject();
}

void CDesignTreeView::slotOpenProject()
{
    CGlobal::instance()->programTreeView()->slotOpenProject();
}

void CDesignTreeView::slotCloseProject()
{
    CGlobal::instance()->programTreeView()->slotCloseProject();
}

void CDesignTreeView::slotSaveProject()
{
    CGlobal::instance()->programTreeView()->slotSaveProject();
}

void CDesignTreeView::slotExportProject()
{
    CGlobal::instance()->programTreeView()->slotExportProject();
}

void CDesignTreeView::slotSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    if(selected.isEmpty())
        return;
    QModelIndex index = selected.indexes().at(0);
    CObject* object = m_designTreeModel->getObject(index);
    if(!object)
        return;
    QString type = object->type();
    if(type == CProject::type_s)
    {
        setCurrentLayer(NULL);
        CGlobal::instance()->designContainer()->setCurrentView(0);
        CGlobal::instance()->designSplashView()->setMainPicture();
    }
    else if(type == CLayer::type_s)
    {
        CLayer* layer = static_cast<CLayer*>(object);
        setCurrentLayer(layer);
//        CGlobal::instance()->designMainView()->setLayer(layer);
        CGlobal::instance()->BottomToolBar()->m_labelZoneName->setText(layer->descriptionStr());
        CGlobal::instance()->designContainer()->setCurrentView(0);
        CGlobal::instance()->designSplashView()->setPrimitiveItem(layer->layerValue(LAYER_VALUE_ID).toInt());
    }
    else
    {
        setCurrentLayer(NULL);
        CGlobal::instance()->designContainer()->setCurrentView(0);
    }
}

void CDesignTreeView::slotStartDrag(Qt::DropActions supportedActions)
{
    Q_UNUSED(supportedActions);
    if(!m_designTreeModel)
        return;
    QModelIndex index = m_designTreeView->currentIndex();
    if (!index.isValid()) return;

    QMimeData *mimeData = new QMimeData;
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << reinterpret_cast<qlonglong>(&index);
    mimeData->setData("device/ts-layer", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    QPixmap pixmap(":/images/layer.png");
    drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
    drag->setPixmap(pixmap);
    if (drag->exec(Qt::MoveAction) == Qt::MoveAction) {

    }
}

void CDesignTreeView::slotDragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("device/ts-layer"))
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void CDesignTreeView::slotDragMoveEvent(QDragMoveEvent *event)
{
    QModelIndex index = m_designTreeView->indexAt(event->pos());
    if(!index.isValid())
    {
        event->ignore();
        return;
    }
    if (event->mimeData()->hasFormat("device/ts-layer"))
    {
        m_designTreeView->setCurrentIndex(index);
        QByteArray itemData = event->mimeData()->data("device/ts-layer");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        qlonglong nTemp;
        dataStream >> nTemp;
        QModelIndex dragIndex = *reinterpret_cast<QModelIndex*>(nTemp);
        QModelIndex tmpIndex = index;
        if(dragIndex.parent() == tmpIndex)
        {
            event->ignore();
            return;
        }
        while(tmpIndex.isValid())
        {
            if(tmpIndex == dragIndex)
            {
                event->ignore();
                return;
            }
            tmpIndex = tmpIndex.parent();
        }
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void CDesignTreeView::slotDropEvent(QDropEvent *event)
{
    CProject* project = m_designTreeModel->project();
    if(project)
    {
        if (event->mimeData()->hasFormat("device/ts-layer"))
        {
            QByteArray itemData = event->mimeData()->data("device/ts-layer");
            QDataStream dataStream(&itemData, QIODevice::ReadOnly);
            qlonglong nTemp;
            dataStream >> nTemp;
            QModelIndex dragIndex = *reinterpret_cast<QModelIndex*>(nTemp);
            QModelIndex destinationIndex = m_designTreeView->currentIndex();
            CLayer* dragLayer = static_cast<CLayer*>(m_designTreeModel->getObject(dragIndex));
            CLayer* destinationLayer = static_cast<CLayer*>(m_designTreeModel->getObject(destinationIndex));
            if(project->moveLayer(dragLayer, destinationLayer))
            {
                m_designTreeModel->reloadData();
                m_designTreeView->expandAll();
            }
            event->setDropAction(Qt::MoveAction);
            event->accept();
            return;
        }
    }
    event->setDropAction(Qt::IgnoreAction);
    event->ignore();
}

void CDesignTreeView::addLayer()
{
    CProject* project = m_designTreeModel->project();
    if(!project)
        return;
    QModelIndexList indexs = m_designTreeView->selectionModel()->selectedRows();
    CLayer* layer = NULL;
    if(indexs.isEmpty())
    {
        layer = new CLayer(project);
        m_designTreeModel->appendObject(layer, project);
        return;
    }
    else
    {
        QModelIndex index = indexs.at(0);
        CObject* object = m_designTreeModel->getObject(index);
        Q_ASSERT(object);
        if(object->type() == CProject::type_s)
        {
            layer = new CLayer(project);
            m_designTreeModel->appendObject(layer, project);
            return;
        }
        else
        {
            CLayer* parent = static_cast<CLayer*>(object);
            layer = new CLayer(project, parent);
            m_designTreeModel->appendObject(layer, parent);
            m_designTreeView->expand(m_designTreeModel->getIndex(parent));
            return;
        }
    }
}

void CDesignTreeView::delLayer()
{
    QModelIndexList indexs = m_designTreeView->selectionModel()->selectedRows();
    for(int i = 0; i < indexs.size(); ++i)
    {
        QModelIndex index = indexs.at(i);
        CObject* object = m_designTreeModel->getObject(index);
        Q_ASSERT(object);
        if(object->type() != CLayer::type_s)
            return;
        CLayer* layer = static_cast<CLayer*>(object);
        layer->project()->removeLayer(layer);
        m_designTreeModel->removeObject(layer);
    }
}

void CDesignTreeView::setCurrentLayer(CLayer *layer)
{
//    if(!layer || m_currentLayer == layer)
    if(m_currentLayer == layer)
        return;
    m_currentLayer = layer;
//    emit sigIsLayerView(layer ? true : false);
//    emit layerChanged(layer);
}
CLayer* CDesignTreeView::CurrentLayer()
{
    return m_currentLayer;
}
