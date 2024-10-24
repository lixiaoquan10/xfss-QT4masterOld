#include "ctableview.h"
#include <QHeaderView>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>

CTableView::CTableView(QWidget *parent) :
    QTableView(parent), m_trackRow(false)
{
    setFrameStyle(QFrame::NoFrame);
    setTextElideMode(Qt::ElideNone);
    setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked | QAbstractItemView::EditKeyPressed);
    verticalHeader()->setResizeMode(QHeaderView::Fixed);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    QPalette pal = palette();
    pal.setColor(QPalette::Highlight, QColor(167, 205, 240));
    setPalette(pal);
    setAutoScroll(true);
}

void CTableView::addSelectEditColumn(const int i)
{
    if(!m_selectEditColumns.contains(i))
        m_selectEditColumns.append(i);
}

void CTableView::delSelectEditColumn(const int i)
{
    if(m_selectEditColumns.contains(i))
        m_selectEditColumns.removeOne(i);
}

void CTableView::keyPressEvent(QKeyEvent *event)
{
    QModelIndex index = currentIndex();
    if(model() && (model()->flags(index) & Qt::ItemIsEditable) && state() != QAbstractItemView::EditingState &&
            index.isValid() && (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return))
        edit(index);
    else
        QTableView::keyPressEvent(event);
    event->accept();
}

void CTableView::mousePressEvent(QMouseEvent *event)
{
    m_currentSelecteds.clear();
    m_currentEditIndex = QModelIndex();
    if (event->button() == Qt::LeftButton) {
        QModelIndex index = indexAt(event->pos());
        if(selectedIndexes().contains(index))
            selectionModel()->clear();
    }
    QTableView::mousePressEvent(event);
}

void CTableView::mouseReleaseEvent(QMouseEvent *event)
{
    QTableView::mouseReleaseEvent(event);
    QModelIndex index = indexAt(event->pos());
    if(!selectedIndexes().contains(index))
        selectionModel()->clear();
    if (event->button() == Qt::LeftButton) {
        m_currentSelecteds = selectedIndexes();
        int nSize = m_currentSelecteds.size();
        if (nSize > 1) {
            bool bSameCol = true;
            for (int i = 0; i < nSize - 1; ++i) {
                QModelIndex index = m_currentSelecteds.at(i);
                if (!m_selectEditColumns.contains(index.column()) || index.column() != m_currentSelecteds.at(i + 1).column()) {
                    bSameCol = false;
                    break;
                }
            }
            if (bSameCol && nSize > 1) {
                m_currentEditIndex = indexAt(event->pos());
                if(!m_currentSelecteds.contains(m_currentEditIndex))   // If not index then get the last index
                    m_currentEditIndex = m_currentSelecteds.last();
                if (m_currentEditIndex.flags() & Qt::ItemIsEditable) {
                    edit(m_currentEditIndex);
                }
            }
        }
    }
}

void CTableView::mouseMoveEvent(QMouseEvent *event)
{
    QTableView::mouseMoveEvent(event);
    if(m_trackRow)
    {
        if(state() == QAbstractItemView::EditingState)
            return;
        QModelIndex index = this->indexAt(event->pos());
        emit rowChanged(index.row());
    }
}

void CTableView::leaveEvent(QEvent *event)
{
    QTableView::leaveEvent(event);
    if(m_trackRow)
    {
        if(state() == QAbstractItemView::EditingState)
            return;
        emit rowChanged(-1);
    }
}

void CTableView::startDrag(Qt::DropActions supportedActions)
{
    emit signalStartDrag(supportedActions);
}

void CTableView::commitData(QWidget *editor)
{
    if(!m_selectEditColumns.contains(m_currentEditIndex.column()))
    {
        QTableView::commitData(editor);
    }
    else
    {
        QAbstractItemDelegate* delegate = itemDelegateForColumn(m_currentEditIndex.column());
        if(!delegate)
            delegate = itemDelegate();
        // If not changed then return.
        QAbstractItemModel* model = this->model();
        QVariant oldData = m_currentEditIndex.data(Qt::EditRole);
        delegate->setModelData(editor, model, m_currentEditIndex);
        QVariant newData = m_currentEditIndex.data(Qt::EditRole);
        if(newData == oldData)
            return;

        for(int i = 0; i < m_currentSelecteds.size(); ++i)
        {
            model->setData(m_currentSelecteds.at(i), newData);
            update(m_currentSelecteds.at(i));
        }
    }
}
