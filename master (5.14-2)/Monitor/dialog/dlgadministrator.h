#ifndef DLGADMINISTRATOR_H
#define DLGADMINISTRATOR_H

#include <QDialog>

namespace Ui {
class dlgAdministrator;
}

class dlgAdministrator : public QDialog
{
    Q_OBJECT

public:
    explicit dlgAdministrator(QWidget *parent = 0);
    ~dlgAdministrator();
    void setUserDisabled(bool enable);
    void verifyPassword();
private slots:

    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_0_clicked();

    void on_pushButton_empty_clicked();

private:
    Ui::dlgAdministrator *ui;
    QString m_password;
    int m_count;
};

#endif // DLGADMINISTRATOR_H
