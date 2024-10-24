#ifndef DLGTIMEINTERVAL_H
#define DLGTIMEINTERVAL_H

#include "cglobal.h"

namespace Ui {
class dlgTimeInterval;
}

class dlgTimeInterval : public QDialog
{
    Q_OBJECT

public:
    explicit dlgTimeInterval(QWidget *parent = 0);
    ~dlgTimeInterval();
    void systemChanged();
    void updateVersion();
    void updateSqliteData();
    void updateIPData();
    void saveSystemStatus();
    void setHostIpPort(QString ip, QString port);
    void setComStatus(int status);
    void setUseTime(int usetime);
    void setIpAndTimeHide(bool enable);
    bool isIpChanged();
    bool isCloudConnect();
    bool isMasterConnect();
    QString getGateway();
    void setIPData();

    QString m_getTmpip;
    QString m_getTmpmask;
    QString m_getTmpgateway;

    QString m_dateTimeStr;
    QString m_LinkmanPhoneNumer;
private slots:

    void on_MonthTimeSlider_valueChanged(int value);

    void on_pushButton_UseTime_clicked();

    void on_m_ButtonTimeSet_clicked();

    void on_pingButton_clicked();

    void slot_timeFinished();

    void slot_pingFinished(double successRate);

    void slot_ipsetFinished();


    void on_SoundFCheckBox_clicked();

    void on_SoundECheckBox_clicked();

    void on_MonthCheckBox_clicked();

    void on_YearCheckBox_clicked();

    void on_DirectionTwinkleCheckBox_clicked();

    void on_UCDirectionTwinkleCheckBox_clicked();

    void on_TwinkleCheckBox_clicked();

    void on_EnergyCheckBox_clicked();

    void on_pushButton_saveHostIP_clicked();

    void on_pushButton_saveCloudIP_clicked();

    void on_pushButton_can1can2_clicked();

    void on_pushButton_can3can4_clicked();

    void on_allStartCheckBox_clicked();

    void on_onlyStartPageCheckBox_clicked();

    void on_onlyStartGroupCheckBox_clicked();

    void on_Can4CheckBox_clicked();

    void on_Can8CheckBox_clicked();

    void on_onlyStartDistributionCheckBox_clicked();

private:
    Ui::dlgTimeInterval *ui;
    bool isThreadRunning;

signals:
    void m_Checked_signal(bool checked);
};

class timeThread : public QThread
{
    Q_OBJECT
public:
    explicit timeThread(QObject *parent = NULL)
        : QThread(parent)
    {
    }
signals:
    void timeFinished();
protected:
    void run();
};

class ipsetThread : public QThread
{
    Q_OBJECT
public:
    explicit ipsetThread(QObject *parent = NULL)
        : QThread(parent)
    {
    }
signals:
    void ipsetFinished();
protected:
    void run();
};

class pingThread : public QThread
{
    Q_OBJECT
public:
    explicit pingThread(QObject *parent = NULL)
        : QThread(parent)
    {
    }
    void setpingIP(QString ip) { m_pingIP = ip; }
signals:
    void pingFinished(double successRate);
protected:
    void run();
private:
    QString m_pingIP;
};

#endif // DLGTIMEINTERVAL_H
