#ifndef DLGTIMEINTERVAL_H
#define DLGTIMEINTERVAL_H

#include "cglobal.h"

namespace Ui {
class dlgTimeInterval;
}

class TriangleButton : public QPushButton
{
public:
    TriangleButton(const QIcon &icon, QWidget *parent = NULL)
        : QPushButton(parent), m_icon(icon)
    {
        setIcon(icon);
        setIconSize(QSize(32, 32)); // 设置图标大小
        setFixedSize(32, 32); // 设置按钮固定大小
    }

protected:
    void paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event);
        QPainter painter(this);
        // 绘制图标
        if (!m_icon.isNull()) {
            QSize iconSize = m_icon.actualSize(QSize(32, 32)); // 使用图标的实际大小
            int x = (width() - iconSize.width()) / 2;
            int y = (height() - iconSize.height()) / 2;
            painter.drawPixmap(x, y, m_icon.pixmap(iconSize));
        }

        // 设置绘制笔的颜色和样式
        painter.setPen(Qt::NoPen); // 设置绘制笔为无边框
        painter.setBrush(Qt::transparent); // 设置绘制刷为透明
        // 绘制三角形
        QPolygonF triangle;
        triangle << QPointF(0, height()) << QPointF(width(), height()) << QPointF(width() / 2, 0);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.drawPolygon(triangle);
    }
private:
    QIcon m_icon;
};

class dlgTimeInterval : public QDialog
{
    Q_OBJECT

public:
    explicit dlgTimeInterval(QWidget *parent = 0);
    ~dlgTimeInterval();
    void systemChanged();
    void updateVersion();
    void updateSqliteData();
    void updateWLANData();
    void updateAdminPuttonGeometry();
    void saveSystemStatus();
    void setHostIpPort(QString ip, QString port);
    void setComStatus(int status);
    void setUseTime(int usetime);
    void setIpAndTimeHide(bool enable);
    bool isWLANChanged();
    bool isHostIPChanged();
    bool isCloudConnect();
    bool isMasterConnect();
    QString getGateway(const QString &interfaceName);
    void setWLANData();

    QString m_getTmpip1;
    QString m_getTmpgateway1;
    QString m_getTmpmask1;

    QString m_getTmpip2;
    QString m_getTmpgateway2;
    QString m_getTmpmask2;

    QString m_dateTimeStr;
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

    void on_pushButton_saveCloudIP_clicked();

    void on_pushButton_can1can2_clicked();

    void on_pushButton_can3can4_clicked();

    void on_allStartCheckBox_clicked();

    void on_onlyStartPageCheckBox_clicked();

    void on_onlyStartGroupCheckBox_clicked();

    void on_Can4CheckBox_clicked();

    void on_Can8CheckBox_clicked();

    void on_onlyStartDistributionCheckBox_clicked();

    void on_triangleButton_clicked();

    void on_pushButton_can5can6_clicked();

    void on_pushButton_can7can8_clicked();

    void on_singleWLAN_clicked();

    void on_doubleWLAN_clicked();

    void on_pushButton_saveWlan_clicked();

    void on_autoPageCheckBox_clicked();

private:
    Ui::dlgTimeInterval *ui;
    bool isThreadRunning;
    TriangleButton *m_triangleButton;

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
