#ifndef CDESIGNSPLASHVIEW_H
#define CDESIGNSPLASHVIEW_H

#include <QWidget>

class QLabel;
class CStyledBar;
class CDevice;
class QMenu;
class CMyIconStyle;
class QScrollArea;
class QSlider;
class QToolButton;
class QScrollBar;
class CFirePoint;
class CDesignSplashView : public QWidget
{
    Q_OBJECT
public:
    explicit CDesignSplashView(QWidget *parent = 0);
    ~CDesignSplashView();
    void setPixmap(QPixmap background);
    QPointF rotatePoint(const QPointF& point, const QPointF& center, double angle);
    void setPrimitiveItem(int layerID);

    void setFirePointPrimitiveItem(QPixmap* background, CFirePoint* firepoint);
    void setLampToFirePoint(QPixmap* background, CFirePoint* firepoint, CDevice* lamp);

    void setLampConnectionArrow(QPixmap* background, CDevice* startLamp, CDevice* endLamp, int joint);

    void setArrowPrimitiveItem(QPixmap* background, CDevice* device);
    void setLeftArrowPrimitiveItem(QPixmap* background, CDevice* device);
    void setRightArrowPrimitiveItem(QPixmap* background, CDevice* device);
    void setLeftFPrimitiveItem(QPixmap* background, CDevice* device);
    void setFloorPrimitiveItem(QPixmap* background, CDevice* device);
    void setExitPrimitiveItem(QPixmap* background, CDevice* device);
    void setChangeExitPrimitiveItem(QPixmap* background, CDevice* device);
    void setLightPrimitiveItem(QPixmap* background, CDevice* device);
    void setOneUndergroundPrimitiveItem(QPixmap* background, CDevice* device);
    void setDoubleUndergroundPrimitiveItem(QPixmap* background, CDevice* device);

    QMenu* createTableMenu(CDevice* device);

    void ScrollBarChange(int valueX, int valueY);
    void setPageStateUpdate(bool enble) { m_isEnblePageStateUpdate = enble; }
    void sliderReset();

    void isHideSlider(bool istrue);

public slots:
    void slot_setPrimitiveItem();
    void setMainPicture();
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void slot_spliderValueChanged(int value);
    void slot_setSliderChanged();
    void slot_enlarge();
    void slot_reduction();
    void onHorizontalScroll(int value);
    void onVerticalScroll(int value);
    void slot_Lighten();
    void slot_CloseLight();
    void slot_AllClose();
    void slot_AllBright();
    void slot_Left();
    void slot_Right();

    void slot_DefaultLeft();
    void slot_DefaultRight();
    void slot_DefaultAll();
    void slot_DefaultOff();

    void slot_Reverse();
    void slot_NoReverse();

    void slot_StartTest();
    void slot_StopTest();
    void slot_StartEmergency();
    void slot_StopEmergency();
    void slot_CloseBatteryOut();
    void slot_BatteryOutKeep();
    void slot_DeviceInfo();
private:
    CStyledBar* m_pToolBar;
    QLabel* m_pView;
    QLabel* m_label;
    QToolButton* m_enlarge;
    QScrollBar* m_verticalSlider;
    QToolButton* m_reduction;
    int m_previousScrollY;
    int m_previousScrollX;

    QTimer* m_slidertimer;
    int m_sliderValue;

    QMenu *m_menuTable;
    CMyIconStyle *m_style;
    QString m_strSheet;
    QString m_strSheetCheck;
    CDevice* m_device;
    QScrollArea* m_scrollArea;
    QTimer* m_timer;
    bool m_isEnblePageStateUpdate;
    //布局视图滑动操作相关变量
    int lastX;
    int lastY;
    bool m_moveEnd;
    //布局视图放大缩小相关变量
    bool m_isChanged;
    int m_changeValue;
    QLabel* m_labelAddress;
};

#endif // CDESIGNSPLASHVIEW_H
