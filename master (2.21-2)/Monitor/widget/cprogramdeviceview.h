#ifndef CPROGRAMDEVICEVIEW_H
#define CPROGRAMDEVICEVIEW_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class CProgramDeviceView;
}
enum LampType{
    NCT_NULL = 0,
    NCT_LightType = 1,                       // 照明灯
    NCT_ArrowLeftType = 2,                   // 左向标志灯
    NCT_ArrowRightType = 3,                  // 右向标志灯
    NCT_ArrowDoubleType = 4                 // 双向标志灯
};

class CLoop;
class CDevice;
class CDistribution;
class CDeviceModel;
class QModelIndex;
class QMenu;
class CMyIconStyle;
class CMsgDeviceInfo;
class CProgramDeviceView : public QWidget
{
    Q_OBJECT
public:
    explicit CProgramDeviceView(QWidget *parent = 0);
    ~CProgramDeviceView();
    void setLoop(CLoop* loop);
    CLoop* loop();
    CDevice* getDevices(const QModelIndex& index) const;
    QMenu *createTableMenu(int lamptype, CDevice* device);
    int type(int typeId);
    const QModelIndex* getDevice() {return m_index;}
    bool isNewLamp(CDevice* device);
    void setUpdateEnble();
private slots:
    void slot_ChangeAdd();
    void slot_ChangeDeviceType();
    void slot_Lighten();
    void slot_CloseLight();
    void slot_AllClose();
    void slot_Left();
    void slot_Right();
    void slot_StartTest();
    void slot_StopTest();
    void slot_StartEmergency();
    void slot_StopEmergency();
    void slot_CloseBatteryOut();
    void slot_BatteryOutKeep();
    void slot_uploadOldLamp();
    void on_tableView_clicked(const QModelIndex &index);
private:
    bool eventFilter(QObject *target, QEvent *event);
private:
    Ui::CProgramDeviceView *ui;
    CDeviceModel*m_model;
    QMenu *m_menuTable;
    const QModelIndex *m_index;
    CMyIconStyle *m_style;
    QString m_strSheet;
    QString m_strSheetCheck;
};


#endif // CPROGRAMDEVICEVIEW_H
