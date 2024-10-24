#ifndef DLGADMINPARAM_H
#define DLGADMINPARAM_H

#include <QDialog>

namespace Ui {
class dlgadminParam;
}
class dlgadminParam : public QDialog
{
    Q_OBJECT

public:
    explicit dlgadminParam(QWidget *parent = 0);
    ~dlgadminParam();
    bool VerifyPowerCode(QString strPowerCode, int nLocalNumber, int &nDayCounts);

private slots:
    void on_m_btnOk_clicked();

    void on_m_btnCancel_clicked();

private:
    Ui::dlgadminParam *ui;
    qint64 m_localNumber;
    int m_useDay;
};

#endif // DLGADMINPARAM_H
