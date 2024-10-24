#ifndef DLGVOLTAGEVALUE_H
#define DLGVOLTAGEVALUE_H

#include <QDialog>
#include <QVector>

namespace Ui {
class dlgVoltageValue;
}

class QTimer;
class CLoop;
class QCPBars;
class QCustomPlot;
class dlgVoltageValue : public QDialog
{
    Q_OBJECT

public:
    explicit dlgVoltageValue(const CLoop* loop, QWidget *parent = 0);
    ~dlgVoltageValue();
private:
    void initCustomPlot(QCustomPlot *customPlot);
    void connectLoopDevice(const CLoop *loop, bool enable);
    void updateVoltageValue();
private slots:
    void timerUpdate();
    void loopDeviceStatusChanged(short status, unsigned char oldValue, unsigned char newValue, unsigned int time);
private:
    Ui::dlgVoltageValue *ui;
    QTimer* m_timer;
    const CLoop* m_loop;
    QCPBars* m_bars;
    QVector<double> m_x;
    bool m_needUpdate;
};

#endif // DLGVOLTAGEVALUE_H
