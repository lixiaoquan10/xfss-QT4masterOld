#ifndef CCENTERFRAME_H
#define CCENTERFRAME_H

#include <QSplitter>
#include <QTabWidget>
#include <QTableView>
#include <QGroupBox>

class CInformationModel;
class QToolButton;
class QLabel;
class QVBoxLayout;
class QStackedWidget;
class CCenterFrame : public QSplitter
{
    Q_OBJECT
public:
    explicit CCenterFrame(QFrame *parent = 0);
    ~CCenterFrame();
    QGroupBox *m_leftBox;
    QStringList m_tabList;//信息页名链表
    QTabWidget *m_tabWidget;//信息页
    QHash<QString, CInformationModel*> m_informationModel;

private:
    QSplitter *m_vSplitter;//垂直分流器
    QHash<QString, QTableView*> m_tableView;//<页面名, 信息表>
    //  one tool button
    QToolButton* m_topButton;
    QLabel *m_topLabel;
    bool m_leftIsVisable;
    QWidget *m_leftWidget;
    QVBoxLayout *m_leftLayout;
    void createLeftBox();//创建左边界面
    void createRightBox();//创建右边界面`
    void createTab(const QString &name);//创建信息页面

    // add toolbutton function
    QToolButton *createToolButton();


    // add toolbar
   // QToolBar *createToolbar(const QString& name );

signals:

public slots:
    void slotDoubleClicked(QModelIndex index);
    void slotToggleLeftBox();
private:
    QStackedWidget *m_centerContainer;
};

#endif // CCENTERFRAME_H
