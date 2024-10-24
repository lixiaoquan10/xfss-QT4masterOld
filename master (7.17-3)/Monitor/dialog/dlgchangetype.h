#ifndef DLGCHANGETYPE_H
#define DLGCHANGETYPE_H

#include <QDialog>

namespace Ui {
class dlgChangeType;
}
class QModelIndex;
class CDevice;
class CProgramDeviceView;
class CDeviceType;

class dlgChangeType : public QDialog
{
    Q_OBJECT

public:
    explicit dlgChangeType(QWidget *parent = 0);
    ~dlgChangeType();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::dlgChangeType *ui;
    CDevice* m_device;
};

#endif // DLGCHANGETYPE_H
