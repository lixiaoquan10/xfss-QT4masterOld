#ifndef DLGCHANGEADD_H
#define DLGCHANGEADD_H

#include <QDialog>

namespace Ui {
class dlgChangeAdd;
}
class QModelIndex;
class CDevice;
class CProgramDeviceView;


class dlgChangeAdd : public QDialog
{
    Q_OBJECT

public:
    explicit dlgChangeAdd(QWidget *parent = 0);
    ~dlgChangeAdd();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::dlgChangeAdd *ui;
    CDevice* m_device;
};

#endif // DLGCHANGEADD_H
