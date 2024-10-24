#include "cprojecttreeview.h"
#include <QHeaderView>
#include <QContextMenuEvent>
#include <QDebug>

CProjectTreeView::CProjectTreeView(QWidget *parent) :
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

void CProjectTreeView::resizeEvent(QResizeEvent *event)
{
    header()->setMinimumSectionSize(viewport()->width());
    QTreeView::resizeEvent(event);
}

void CProjectTreeView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_UNUSED(bottomRight);
    expand(topLeft);
    expand(topLeft.parent());
}

void CProjectTreeView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QTreeView::selectionChanged(selected, deselected);
    emit sigSelectionChanged(selected, deselected);
}

void CProjectTreeView::startDrag(Qt::DropActions supportedActions)
{
    emit signalStartDrag(supportedActions);
}

void CProjectTreeView::dragEnterEvent(QDragEnterEvent *event)
{
    emit signalDragEnterEvent(event);
}

void CProjectTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    emit signalDragMoveEvent(event);
}

void CProjectTreeView::dropEvent(QDropEvent *event)
{
    emit signalDropEvent(event);
}

void CProjectTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    emit signalContextMenuEvent(event);
}
