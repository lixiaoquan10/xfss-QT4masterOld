#ifndef CDESIGNTREEVIEW_H
#define CDESIGNTREEVIEW_H

#include <QWidget>
#include <QItemSelection>

class QLabel;
class QToolButton;
class CStyledBar;
class QStackedLayout;
class CControllerTreeView;
class CDesignTreeModel;
class CController;
class CLayer;
class CObject;
class CDesignTreeView : public QWidget
{
    Q_OBJECT
public:
    explicit CDesignTreeView(QWidget *parent = 0);
    virtual ~CDesignTreeView();
    void setCurrentView(int index = 0);
    void setController(CController* controller);
    void setLayer(CLayer *layer);
    CLayer* currentLayer() const { return m_currentLayer; }
    void update(CObject* object);
    CLayer* CurrentLayer();
    void setAutoPage(bool enable);

public slots:
    void slotNewController();
    void slotOpenController();
    void slotCloseController();
    void slotSaveController();
    void slotExportController();
    void slotSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
    void slotStartDrag(Qt::DropActions supportedActions);
    void slotDragEnterEvent(QDragEnterEvent *event);
    void slotDragMoveEvent(QDragMoveEvent *event);
    void slotDropEvent(QDropEvent *event);

    void slot_autoPage();

private:
    void setIsLayerView(bool enable);
    void setCurrentLayer(CLayer* layer);
private slots:
    void addLayer();
    void delLayer();

signals:
    void sigIsLayerView(bool enable);
    void layerChanged(CLayer* layer);
private:
    CControllerTreeView* m_designTreeView;
    CDesignTreeModel* m_designTreeModel;
    CLayer* m_currentLayer;
    QTimer* m_timer;
    int m_number;
};


#endif // CDESIGNTREEVIEW_H
