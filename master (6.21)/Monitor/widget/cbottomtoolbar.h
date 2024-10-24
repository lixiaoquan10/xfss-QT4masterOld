#ifndef CBOTTOMTOOLBAR_H
#define CBOTTOMTOOLBAR_H

#include <QGroupBox>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>

class CBottomToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit CBottomToolBar(QWidget *parent = 0);
    ~CBottomToolBar();

    QHBoxLayout *m_bottomToolBox;//底层标题栏容器
    QLabel *m_labelCurrentZoneName;//当前分区
    QLabel *m_labelZoneName;//当前分区信息
    QLabel *m_labelUserLevel;//用户等级
    QLabel *m_labelEvacuationMode;//手自动状态
    QLabel *m_labelCurrentTime;//时间显示

    QString getCurrentDateTime();//获得日期时间
    void setCurrentDateTime();
    void setAdministrator();
    void setUser();
    void setManual();
    void setAuto();    

private:
    void createToolBar();

signals:

public slots:

};

#endif // CBOTTOMTOOLBOX_H
