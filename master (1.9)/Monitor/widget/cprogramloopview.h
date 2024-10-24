#ifndef CPROGRAMLOOPVIEW_H
#define CPROGRAMLOOPVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QModelIndex>

namespace Ui {
class CProgramLoopView;
}

class QToolButton;
class CController;
class CDistribution;
class CDistributionModel;
class CStyledBar;
class CTableView;
class CDistribution;
class CController;
class CProject;
class CClientbusiness;
class CProgramLoopView : public QWidget
{
    Q_OBJECT
public:
    explicit CProgramLoopView(QWidget *parent = 0);
    ~CProgramLoopView();
    void setDistribution(CDistribution* distribution);
    void setLoopStatus(int loopCommunication, int loopOpen);
    void setDistributionStatus(CDistribution *distribution);
    void setDistributionParameter(CDistribution *distribution);
    void updateDistribution();
    QString batteryTStatus(u_int16_t t);

private slots:
    void readPowerInfo();
    void updateStatus();
    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::CProgramLoopView *ui;
    CDistributionModel* m_model;
    QTimer* m_timer;
    QTimer* m_timerUpdateStatus;
    int m_distributionAddress;
    bool m_changeflag;   //集中电源初始化时不发送数据改变信号
};


#endif // CPROGRAMLOOPVIEW_H
