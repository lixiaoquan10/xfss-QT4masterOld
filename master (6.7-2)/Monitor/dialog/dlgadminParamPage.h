#ifndef DLGADMINPARAMPAGE_H
#define DLGADMINPARAMPAGE_H

#include <QDialog>

namespace Ui {
class dlgadminParamPage;
}

class dlgadminParamPage : public QDialog
{
    Q_OBJECT

public:
    explicit dlgadminParamPage(QWidget *parent = 0);
    ~dlgadminParamPage();
    void updateSqliteData();
    void systemChanged();
private slots:
    void on_SoundFCheckBox_clicked();

    void on_SoundECheckBox_clicked();

    void on_MonthCheckBox_clicked();

    void on_YearCheckBox_clicked();

    void on_MonthTimeSlider_valueChanged(int value);

private:
    Ui::dlgadminParamPage *ui;
};

#endif // DLGADMINPARAMPAGE_H
