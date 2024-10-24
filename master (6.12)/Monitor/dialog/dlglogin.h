#ifndef DLGLOGIN_H
#define DLGLOGIN_H

#include <QDialog>

namespace Ui {
class dlgLogin;
}

class dlgLogin : public QDialog
{
    Q_OBJECT

public:
    explicit dlgLogin(QWidget *parent = 0);
    ~dlgLogin();

    QString userName;
    QString passWord;

private slots:
    void on_m_btnLogin_clicked();
    void on_m_btnCancel_clicked();
    void on_m_btnNetSet_clicked();

private:
    Ui::dlgLogin *ui;
};

#endif // DLGLOGIN_H
