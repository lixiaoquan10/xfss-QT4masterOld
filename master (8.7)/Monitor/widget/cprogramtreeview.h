#ifndef CPROGRAMTREEVIEW_H
#define CPROGRAMTREEVIEW_H

#include <QWidget>
#include <QItemSelection>
#include <QMenu>

class CLoop;
class QLabel;
class CController;
class CStyledBar;
class QToolButton;
class QStackedLayout;
class CControllerTreeView;
class CControllerTreeModel;
class CMyIconStyle;
class CProgramTreeView : public QWidget
{
    Q_OBJECT
public:
    explicit CProgramTreeView(QWidget *parent = 0);
    virtual ~CProgramTreeView();
    void setController(CController* controller);
    void saveExpandState(const QModelIndex &parentIndex);
    void resetControllerTreeView();
    void newController();
    void openController();
    void closeController();
    void saveController();
    QMenu* createTableMenu(bool isDistribution);

public slots:
    void slotNewController();
    void slotOpenController();
    void slotCloseController();
    void slotSaveController();
    void slotExportController();

    void slot_TreeViewClicked(const QModelIndex &index) ;

    void slot_emergencyStart();
    void slot_emergencyStop();
    void slot_ReadLampVersion();
    void slot_removeLampLogin();

signals:
    void signalRightSelectedLoop(CLoop* loop);

private:
    QMenu *m_menuTable;
    CControllerTreeView* m_controllerTreeView;
    CControllerTreeModel* m_controllerTreeModel;
    CMyIconStyle *m_style;
    QString m_strSheet;
    bool isTableShow;
    // 保存每个节点的展开状态
    QMap<QModelIndex, bool> expandStates;
};

#endif // CPROGRAMTREEVIEW_H
