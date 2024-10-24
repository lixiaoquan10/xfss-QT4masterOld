#ifndef CPROGRAMDISTRIBUTIONVIEW_H
#define CPROGRAMDISTRIBUTIONVIEW_H

#include <QWidget>
#include <QVBoxLayout>

namespace Ui {
class CProgramdistributionView;
}

//class QToolButton;
//class CCanport;
//class CDistributionModel;
//class CProgramDistributionView : public QWidget
//{
//    Q_OBJECT
//public:
//    explicit CProgramDistributionView(QWidget *parent = 0);
//    ~CProgramDistributionView();
//    void setCanport(CCanport* canport);
//    CCanport* canport();
//private slots:
////    void addDistribution();
//    void delDistribution();
//private:
//    Ui::CProgramDistributionView *ui;
//    CDistributionModel* m_model;
//};


class QToolButton;
class CCanport;
class CDistribution;
class CLoopModel;
class CStyledBar;
class CTableView;
class CDistribution;
class CCanport;
class CProject;
class CClientbusiness;
class CDistributionModel;
class CProgramDistributionView : public QWidget
{
    Q_OBJECT
public:
    explicit CProgramDistributionView(QWidget *parent = 0);
    ~CProgramDistributionView();
    void setCanport(CCanport* canport);
    void setLoopStatus(int loopCommunication, int loopOpen);
    void setDistributionType(int typeID);
    void setDistributionStatus(CDistribution *distribution);
    void distributionResetStatus();
    void setDistribution(CDistribution *distribution);

private slots:
    void readProtectInfo();
    void updateStatus();
    void on_m_opencurrent_slider_valueChanged(int value);

    void on_m_firetime_slider_valueChanged(int value);

    void on_m_unfiretime_slider_valueChanged(int value);

    void on_CAN_communication_clicked(bool checked);

    void on_light_detection_clicked(bool checked);

    void on_comboBox_powermode_currentIndexChanged(int index);

    void on_comboBox_linkageInput_currentIndexChanged(int index);

    void on_comboBox_loopoutstatus_currentIndexChanged(int index);

    void on_closepowerout_button_clicked();

    void on_poweroutkeep_button_clicked();

    void on_readloopoutstatus_button_clicked();

    void on_emergencyStart_button_clicked();

    void on_emergencyStop_button_clicked();

    void on_removeLampLogin_button_clicked();

private:
    Ui::CProgramdistributionView *ui;
    CDistributionModel* m_model;
    QTimer* m_timer;
    QTimer* m_timerUpdateStatus;
    bool m_changeflag;   //集中电源初始化时不发送数据改变信号
};

#endif // CPROGRAMDISTRIBUTIONVIEW_H
