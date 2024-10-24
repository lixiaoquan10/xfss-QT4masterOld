#ifndef DLGDEVICEFORBIDFAULT_H
#define DLGDEVICEFORBIDFAULT_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <QTreeWidgetItem>
#include "cglobal.h"
#include "dm/ccontroller.h"

namespace Ui {
    class dlgdeviceForbidFault;
}

class CGlobal;
class CController;
class CDeviceForbidFaultlistmodel;
class dlgdeviceForbidFault : public QDialog
{
    Q_OBJECT
public:
    explicit dlgdeviceForbidFault(QWidget *parent = 0);
    ~dlgdeviceForbidFault();
    void addDeviceForbidFaultInfo(CDevice* device);
    void clearDeviceForbidFaultInfo();
private slots:
    void on_pushButton_close_clicked();

private:
    Ui::dlgdeviceForbidFault *ui;
    CDeviceForbidFaultlistmodel* m_model;
    int m_count;
};


#endif // DLGDEVICEFORBIDFAULT_H
