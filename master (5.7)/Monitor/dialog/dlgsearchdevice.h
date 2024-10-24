#ifndef DLGSEARCHDEVICE_H
#define DLGSEARCHDEVICE_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
class dlgSearchDevice;
}

class CDevicesFindTableModel;
class dlgSearchDevice : public QDialog
{
    Q_OBJECT

public:
    explicit dlgSearchDevice(QWidget *parent = 0);
    ~dlgSearchDevice();

private slots:
    void slot_on_dClicked(QModelIndex);

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

private:
    Ui::dlgSearchDevice *ui;
    CDevicesFindTableModel* m_model;
};

#endif // DLGSEARCHDEVICE_H
