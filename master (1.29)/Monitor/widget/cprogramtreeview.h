#ifndef CPROGRAMTREEVIEW_H
#define CPROGRAMTREEVIEW_H

#include <QWidget>
#include <QItemSelection>

class CLoop;
class QLabel;
class CProject;
class CStyledBar;
class QToolButton;
class QStackedLayout;
class CProjectTreeView;
class CProjectTreeModel;
class CProgramTreeView : public QWidget
{
    Q_OBJECT
public:
    explicit CProgramTreeView(QWidget *parent = 0);
    virtual ~CProgramTreeView();
    void resetProjectTreeView();
    void setProject(CProject* project);

    void newProject();
    void openProject();
    void closeProject();
    void saveProject();

public slots:
    void slotNewProject();
    void slotOpenProject();
    void slotCloseProject();
    void slotSaveProject();
    void slotExportProject();
    void slotSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);


signals:
    void signalRightSelectedLoop(CLoop* loop);

private:
    CProjectTreeView* m_projectTreeView;
    CProjectTreeModel* m_projectTreeModel;
};

#endif // CPROGRAMTREEVIEW_H
