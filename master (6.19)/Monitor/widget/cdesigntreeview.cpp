#include "cglobal.h"


CDesignTreeView::CDesignTreeView(QWidget *parent) :
    QWidget(parent), m_currentLayer(NULL)
{
    CGlobal::instance()->setDesignTreeView(this);
    setMinimumWidth(300);

    QVBoxLayout* pMainLayout = new QVBoxLayout();
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(0);
    m_designTreeView = new CControllerTreeView(this);
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

    m_number = 0;
    //间隔10s自动轮询页面
    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_autoPage()));
}

CDesignTreeView::~CDesignTreeView()
{
    CGlobal::instance()->setDesignTreeView(0);
    delete m_designTreeModel;
    delete m_designTreeView;
}

void CDesignTreeView::slot_autoPage()
{
    CLayer *layer = CGlobal::instance()->controller()->layers().at(m_number);
    QModelIndex index = m_designTreeModel->getIndex(layer);
    m_designTreeView->setCurrentIndex(index);
    m_number++;
    if(m_number == CGlobal::instance()->controller()->layers().count())
    {
        m_number = 0;
    }
}

void CDesignTreeView::setAutoPage(bool enable)
{
    if(enable)
        m_timer->start(10000);
    else
        m_timer->stop();
}

void CDesignTreeView::setCurrentView(int index)
{
    Q_UNUSED(index);
}

void CDesignTreeView::setController(CController *controller)
{
    m_designTreeModel->setController(controller);
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

void CDesignTreeView::slotNewController()
{
    CGlobal::instance()->programTreeView()->slotNewController();
}

void CDesignTreeView::slotOpenController()
{
    CGlobal::instance()->programTreeView()->slotOpenController();
}

void CDesignTreeView::slotCloseController()
{
    CGlobal::instance()->programTreeView()->slotCloseController();
}

void CDesignTreeView::slotSaveController()
{
    CGlobal::instance()->programTreeView()->slotSaveController();
}

void CDesignTreeView::slotExportController()
{
    CGlobal::instance()->programTreeView()->slotExportController();
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
    if(type == CController::type_s)
    {
        setCurrentLayer(NULL);
        CGlobal::instance()->designSplashView()->setMainPicture();
    }
    else if(type == CLayer::type_s)
    {
        CLayer* layer = static_cast<CLayer*>(object);
        setCurrentLayer(layer);
        CGlobal::instance()->BottomToolBar()->m_labelZoneName->setText(layer->descriptionStr());
        CGlobal::instance()->designSplashView()->setPrimitiveItem(layer->layerValue(LAYER_VALUE_ID).toInt());
    }
    else
    {
        setCurrentLayer(NULL);
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
    CController* controller = m_designTreeModel->controller();
    if(controller)
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
            if(controller->moveLayer(dragLayer, destinationLayer))
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
    CController* controller = m_designTreeModel->controller();
    if(!controller)
        return;
    QModelIndexList indexs = m_designTreeView->selectionModel()->selectedRows();
    CLayer* layer = NULL;
    if(indexs.isEmpty())
    {
        layer = new CLayer(controller);
        m_designTreeModel->appendObject(layer, controller);
        return;
    }
    else
    {
        QModelIndex index = indexs.at(0);
        CObject* object = m_designTreeModel->getObject(index);
        Q_ASSERT(object);
        if(object->type() == CController::type_s)
        {
            layer = new CLayer(controller);
            m_designTreeModel->appendObject(layer, controller);
            return;
        }
        else
        {
            CLayer* parent = static_cast<CLayer*>(object);
            layer = new CLayer(controller, parent);
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
        layer->controller()->removeLayer(layer);
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
