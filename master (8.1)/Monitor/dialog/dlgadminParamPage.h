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
    void setParamDatabase();
private slots:
    void on_SoundFCheckBox_clicked();

    void on_SoundECheckBox_clicked();

    void on_MonthCheckBox_clicked();

    void on_YearCheckBox_clicked();

    void on_pushButton_month_clicked();

    void on_pushButton_year_clicked();

    void on_HideLinkLineCheckBox_clicked();

    void on_HideFirePointCheckBox_clicked();

    void on_ShieldUnknownFirePointCheckBox_clicked();

    void on_ShieldArrowLampLightFaultCheckBox_clicked();

    void on_ShieldLightLampLightFaultCheckBox_clicked();

    void on_ShieldUndergroundLampLightFaultCheckBox_clicked();

    void on_ShieldLampComFaultCheckBox_clicked();

    void on_ShieldCanDeviceComFaultCheckBox_clicked();

    void on_ShieldLoginComFaultCheckBox_clicked();

    void on_ShieldMainPowerFaultEmergencyCheckBox_clicked();

    void on_HideBatPowerFaultShowCheckBox_clicked();

    void on_OldLampcheckBox_clicked();

private:
    Ui::dlgadminParamPage *ui;
};

#endif // DLGADMINPARAMPAGE_H
