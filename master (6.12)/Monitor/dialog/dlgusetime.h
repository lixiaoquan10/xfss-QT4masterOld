#ifndef DLGUSETIME_H
#define DLGUSETIME_H

#include <QDialog>

namespace Ui {
class dlgUseTime;
}

class dlgUseTime : public QDialog
{
    Q_OBJECT

public:
    explicit dlgUseTime(QWidget *parent = 0);
    ~dlgUseTime();
    bool VerifyPowerCode(QString strPowerCode, int nLocalNumber, int &nDayCounts);
    int MyPow(int dishu, int mi);
    void setlabelMessage();

private slots:
    void on_m_btnOk_clicked();

    void on_m_btnCancel_clicked();

private:
    Ui::dlgUseTime *ui;
    qint64 m_localNumber;
    int m_useDay;
};

#endif // DLGUSETIME_H
