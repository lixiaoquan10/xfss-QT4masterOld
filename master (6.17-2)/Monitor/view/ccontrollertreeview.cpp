#include "ccontrollertreeview.h"
#include <QHeaderView>
#include <QContextMenuEvent>
#include <QDebug>

CControllerTreeView::CControllerTreeView(QWidget *parent) :
    QTreeView(parent)
{
    setFrameStyle(QFrame::NoFrame);
    setIndentation(indentation() * 9/10);
    setUniformRowHeights(true);
    setTextElideMode(Qt::ElideNone);

    setHeaderHidden(true);
    header()->setResizeMode(QHeaderView::ResizeToContents);
    header()->setStretchLastSection(false);
}

void CControllerTreeView::resizeEvent(QResizeEvent *event)
{
    header()->setMinimumSectionSize(viewport()->width());
    QTreeView::resizeEvent(event);
}

void CControllerTreeView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_UNUSED(bottomRight);
    expand(topLeft);
    expand(topLeft.parent());
}

void CControllerTreeView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QTreeView::selectionChanged(selected, deselected);
    emit sigSelectionChanged(selected, deselected);
}

void CControllerTreeView::startDrag(Qt::DropActions supportedActions)
{
    emit signalStartDrag(supportedActions);
}

void CControllerTreeView::dragEnterEvent(QDragEnterEvent *event)
{
    emit signalDragEnterEvent(event);
}

void CControllerTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    emit signalDragMoveEvent(event);
}

void CControllerTreeView::dropEvent(QDropEvent *event)
{
    emit signalDropEvent(event);
}

void CControllerTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    emit signalContextMenuEvent(event);
}
