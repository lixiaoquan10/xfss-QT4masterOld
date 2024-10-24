#ifndef CTABLEVIEW_H
#define CTABLEVIEW_H

#include <QTableView>
#include <QList>

class CTableView : public QTableView
{
    Q_OBJECT
public:
    explicit CTableView(QWidget *parent = 0);
    void addSelectEditColumn(const int i);
    void delSelectEditColumn(const int i);
    void setTrackRow(const bool enable) { m_trackRow = enable; setMouseTracking(enable);}
signals:
    void signalStartDrag(Qt::DropActions supportedActions);
    void rowChanged(const int row);
protected:
    void keyPressEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void leaveEvent(QEvent* event);
    void startDrag(Qt::DropActions supportedActions);
protected slots:
    void commitData(QWidget *editor);
private:
    QList<int> m_selectEditColumns;
    bool m_trackRow;
    QModelIndexList m_currentSelecteds;
    QModelIndex m_currentEditIndex;
};

#endif // CTABLEVIEW_H
