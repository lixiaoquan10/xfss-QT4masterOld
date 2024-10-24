#ifndef CPROGRAMTREEVIEW_H
#define CPROGRAMTREEVIEW_H

#include <QWidget>
#include <QItemSelection>

class CLoop;
class QLabel;
class CController;
class CStyledBar;
class QToolButton;
class QStackedLayout;
class CControllerTreeView;
class CControllerTreeModel;
class CProgramTreeView : public QWidget
{
    Q_OBJECT
public:
    explicit CProgramTreeView(QWidget *parent = 0);
    virtual ~CProgramTreeView();
    void setController(CController* controller);
    void resetControllerTreeView();
    void newController();
    void openController();
    void closeController();
    void saveController();

public slots:
    void slotNewController();
    void slotOpenController();
    void slotCloseController();
    void slotSaveController();
    void slotExportController();
    void slotSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);


signals:
    void signalRightSelectedLoop(CLoop* loop);

private:
    CControllerTreeView* m_controllerTreeView;
    CControllerTreeModel* m_controllerTreeModel;
};

#endif // CPROGRAMTREEVIEW_H
