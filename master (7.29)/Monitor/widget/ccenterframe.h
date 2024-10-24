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
    QHash<QString, QTableView*> m_tableView;//<页面名, 信息表>
    //  one tool button
    QToolButton* m_topButton;
    QLabel *m_topLabel;
    bool m_leftIsVisable;
    QWidget *m_leftWidget;
    QVBoxLayout *m_leftLayout;
    void createLeftBox();//创建左边界面

    // add toolbutton function
    QToolButton *createToolButton();
public slots:
    void slotToggleLeftBox();
private:
    QStackedWidget *m_centerContainer;
};

#endif // CCENTERFRAME_H
