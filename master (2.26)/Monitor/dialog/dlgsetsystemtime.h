#ifndef DLGSETSYSTEMTIME_H
#define DLGSETSYSTEMTIME_H

#include <QDialog>
#include <QDateTime>

namespace Ui {
class dlgSetSystemTime;
}

class dlgSetSystemTime : public QDialog
{
    Q_OBJECT

public:
    explicit dlgSetSystemTime(QWidget *parent = 0);
    ~dlgSetSystemTime();

    bool setDateTime(const QDate& date, const QTime& time);

private slots:
    void on_ok_button_clicked();
    void on_cancel_button_clicked();

private:
    Ui::dlgSetSystemTime *ui;
};

#endif // DLGSETSYSTEMTIME_H
