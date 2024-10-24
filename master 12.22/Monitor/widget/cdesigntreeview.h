#ifndef CDESIGNTREEVIEW_H
#define CDESIGNTREEVIEW_H

#include <QWidget>
#include <QItemSelection>

class QLabel;
class QToolButton;
class CStyledBar;
class QStackedLayout;
class CProjectTreeView;
class CDesignTreeModel;
class CProject;
class CLayer;
class CObject;
class CDesignTreeView : public QWidget
{
    Q_OBJECT
public:
    explicit CDesignTreeView(QWidget *parent = 0);
    virtual ~CDesignTreeView();
    void setCurrentView(int index = 0);
    void setProject(CProject* project);
    void setLayer(CLayer *layer);
    CLayer* currentLayer() const { return m_currentLayer; }
    void update(CObject* object);
    CLayer* CurrentLayer();

public slots:
    void slotNewProject();
    void slotOpenProject();
    void slotCloseProject();
    void slotSaveProject();
    void slotExportProject();
    void slotSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
    void slotStartDrag(Qt::DropActions supportedActions);
    void slotDragEnterEvent(QDragEnterEvent *event);
    void slotDragMoveEvent(QDragMoveEvent *event);
    void slotDropEvent(QDropEvent *event);

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
    CProjectTreeView* m_designTreeView;
    CDesignTreeModel* m_designTreeModel;
    CLayer* m_currentLayer;
};


#endif // CDESIGNTREEVIEW_H
