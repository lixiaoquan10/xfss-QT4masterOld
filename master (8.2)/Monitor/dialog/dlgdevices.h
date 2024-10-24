#ifndef DLGDEVICES_H
#define DLGDEVICES_H

#include <QDialog>

namespace Ui {
class dlgDevices;
}

class CDeviceListModel;
class CDeviceTypeDelegate;
class QMenu;
class QModelIndex;
class CMyIconStyle;
class CDevice;
class CMsgDeviceInfo;
class QToolButton;
class dlgDevices : public QDialog
{
    Q_OBJECT

public:
    explicit dlgDevices(QWidget *parent = 0);
    ~dlgDevices();
    void getDeviceInfo();
    void lampBatStatusUpload();
    void setDevicePage(int page);
    QMenu* createTableMenu();
private slots:
//    void timeOut();
    void slot_StartTest();
    void slot_StopTest();
    void slot_StartEmergency();
    void slot_StopEmergency();
    void slot_CloseBatteryOut();
    void slot_BatteryOutKeep();
    void slot_deviceUse();
    void slot_deviceForbid();
//    void slot_DeviceInfo();
    void on_tableView_clicked(const QModelIndex &index);

    void on_ReadBatteryStatus_Button_clicked();

    void on_pageScrollBar_valueChanged(int value);

    void on_nextPage_Button_clicked();

    void on_previousPage_Button_clicked();

private:
    CDeviceListModel* m_model;
    Ui::dlgDevices *ui;
    QStringList m_tabList;
//    QTimer* m_time;
    QMenu *m_menuTable;
    const QModelIndex *m_index;
    CMyIconStyle *m_style;
    QString m_strSheet;
    QString m_strSheetCheck;
    QToolButton* m_readDevicePower;
    int m_maxPage;
    int m_deviceNumber;
};

#endif // DLGDEVICES_H
