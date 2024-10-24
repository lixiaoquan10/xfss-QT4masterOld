#ifndef CPROGRAMCANDEVICEVIEW_H
#define CPROGRAMCANDEVICEVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QModelIndex>

namespace Ui {
class CProgramCanDeviceView;
}

class QToolButton;
class CCanport;
class CDistribution;
class CDistributionModel;
class CStyledBar;
class CTableView;
class CDistribution;
class CCanport;
class CProject;
class CClientbusiness;
class CProgramCanDeviceView : public QWidget
{
    Q_OBJECT
public:
    explicit CProgramCanDeviceView(QWidget *parent = 0);
    ~CProgramCanDeviceView();
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
    Ui::CProgramCanDeviceView *ui;
    CDistributionModel* m_model;
    QTimer* m_timer;
    QTimer* m_timerUpdateStatus;
    int m_canAddress;
    int m_distributionAddress;
    bool m_changeflag;   //集中电源初始化时不发送数据改变信号
};


#endif // CPROGRAMCANDEVICEVIEW_H
