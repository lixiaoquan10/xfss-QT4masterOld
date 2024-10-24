#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QHash>
class CLayer;
class CDevice;
class GroupBox;
class QTabWidget;
class QTableView;
class Widget : public QWidget
{
    Q_OBJECT
    
public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    void switchLayer();
    void createTimer();// Create timer
    void keyPressEvent(QKeyEvent  *event);
    int  normalSwitch(QList<CLayer*> &listLayer, int num);

protected:
    void closeEvent(QCloseEvent *e);

public slots:
    void slotLEDTimer();
    void slotEventChange();

private:
    int     m_iDelayCut;

    QTimer  *m_timerLED;//LEDµÆ¶¨Ê±Æ÷
    GroupBox *topBox;
    QGridLayout *topLayout;
    GroupBox *bottomBox;
    QGridLayout *bottomLayout;

};

#endif // WIDGET_H
