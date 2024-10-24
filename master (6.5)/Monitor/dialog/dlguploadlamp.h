#ifndef DLGUPLOADLAMP_H
#define DLGUPLOADLAMP_H

#include <QDialog>

namespace Ui {
class dlgUploadLamp;
}
class QModelIndex;
class CDevice;
class CProgramDeviceView;


class dlgUploadLamp : public QDialog
{
    Q_OBJECT

public:
    explicit dlgUploadLamp(QWidget *parent = 0);
    ~dlgUploadLamp();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_cancal_clicked();

private:
    Ui::dlgUploadLamp *ui;
    CDevice* m_device;
};

#endif // DLGUPLOADLAMP_H
