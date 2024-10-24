#ifndef CCONTROLLERTREEVIEW_H
#define CCONTROLLERTREEVIEW_H

#include <QTreeView>
#include <QItemSelection>

class QModelIndex;
class CControllerTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit CControllerTreeView(QWidget *parent = 0);
    
signals:
    void sigSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void signalStartDrag(Qt::DropActions supportedActions);
    void signalDragEnterEvent(QDragEnterEvent *event);
    void signalDragMoveEvent(QDragMoveEvent *event);
    void signalDropEvent(QDropEvent *event);
    void signalContextMenuEvent(QContextMenuEvent* event);

protected:
    void resizeEvent(QResizeEvent *event);
    void dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
    void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
    void startDrag(Qt::DropActions supportedActions);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void contextMenuEvent(QContextMenuEvent* event);
};

#endif // CCONTROLLERTREEVIEW_H
