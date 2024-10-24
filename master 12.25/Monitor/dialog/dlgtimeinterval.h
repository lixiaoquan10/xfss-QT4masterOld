#ifndef DLGTIMEINTERVAL_H
#define DLGTIMEINTERVAL_H

#include <QDialog>
#include <QProcess>
#include <QMessageBox>
#include <QThread>
#include <QDebug>
#include <QFile>

namespace Ui {
class dlgTimeInterval;
}

class dlgTimeInterval : public QDialog
{
    Q_OBJECT

public:
    explicit dlgTimeInterval(QWidget *parent = 0);
    ~dlgTimeInterval();
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
    void setIPData(const QString& ipAddress, const QString& subnetMask, const QString& gateway);
private slots:
    void on_m_ButtonSet_clicked();

    void on_MonthTimeSlider_valueChanged(int value);

    void on_pushButton_clicked();

    void on_pushButton_UseTime_clicked();

    void on_m_ButtonTimeSet_clicked();

    void openDlgTimeInterval();

    void on_pingButton_clicked();

    void slot_pingFinished(double successRate);


private:
    Ui::dlgTimeInterval *ui;
    bool isThreadRunning;
    QString m_getTmpip;
    QString m_getTmpmask;
    QString m_getTmpgateway;

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
    void threadFinished();
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
    void run()
    {
        int receivedPackets = 0; // 收到的数据包数
        double successRate = 0.0;

        QString command = "ping -c 5 " + m_pingIP;
        QProcess process;
        process.start(command);
        process.waitForFinished();
        QString output = process.readAllStandardOutput();


        // 解析输出结果并发射信号
        QStringList lines = output.split("\n");// 存储Ping命令输出的每行结果
        foreach (QString line, lines) {
            if (line.contains("bytes from")) {
                receivedPackets++;
            }
        }
        successRate = (static_cast<double>(receivedPackets) / 5) * 100;
        emit pingFinished(successRate);  // 发送成功率信号
    }
private:
    QString m_pingIP;
};

#endif // DLGTIMEINTERVAL_H
