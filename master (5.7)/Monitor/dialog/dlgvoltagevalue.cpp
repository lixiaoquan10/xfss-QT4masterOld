#include "dlgvoltagevalue.h"
#include "ui_dlgvoltagevalue.h"
#include "business/cclientbusiness.h"
#include "dm/cdistribution.h"
#include "dm/cloop.h"
#include "dm/cdevice.h"
#include "cglobal.h"

#include <QTimer>

dlgVoltageValue::dlgVoltageValue(const CLoop *loop, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgVoltageValue),
    m_loop(loop), m_x(loop->deviceCount()), m_needUpdate(false)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("View voltage value"));
    initCustomPlot(ui->customPlot);
    connectLoopDevice(m_loop, true);
    updateVoltageValue();
    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    m_timer->start(3300);
    timerUpdate();
}

dlgVoltageValue::~dlgVoltageValue()
{
    connectLoopDevice(m_loop, false);
    delete ui;
}

void dlgVoltageValue::initCustomPlot(QCustomPlot *customPlot)
{
    for(int i = 0; i < m_x.size(); ++i)
        m_x[i] = i + 1;

    m_bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(m_bars);
    m_bars->setWidth(0.8);
    m_bars->setPen(Qt::NoPen);
    m_bars->setBrush(QColor(10, 140, 70, 160));

    // set some pens, brushes and backgrounds:
    customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickLabelColor(Qt::white);
    customPlot->yAxis->setTickLabelColor(Qt::white);
    customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridVisible(true);
    customPlot->yAxis->grid()->setSubGridVisible(true);
    customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    customPlot->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    customPlot->axisRect()->setBackground(axisRectGradient);

    //  customPlot->rescaleAxes();
    customPlot->xAxis->setRange(0.5, m_x.size() + 0.5);
    customPlot->yAxis->setRange(0, 40);
}

void dlgVoltageValue::connectLoopDevice(const CLoop *loop, bool enable)
{
    for(int i = 0; i < loop->deviceCount(); ++i)
    {
        if(enable)
            connect(loop->device(i), SIGNAL(objectStatusChanged(short,unsigned char,unsigned char,uint)), this, SLOT(loopDeviceStatusChanged(short,unsigned char,unsigned char,uint)));
        else
            disconnect(loop->device(i), SIGNAL(objectStatusChanged(short,unsigned char,unsigned char,uint)), this, SLOT(loopDeviceStatusChanged(short,unsigned char,unsigned char,uint)));
    }
}

void dlgVoltageValue::updateVoltageValue()
{
    int count = m_loop->deviceCount();
    QVector<double> data(count);
    for(int i = 0; i < count; ++i)
    {
        int n = m_loop->device(i)->getStatus(OBJS_VoltageValue);
        data[i] = n > 0 ? (42.9*n/256.0+1.8) : 0;
    }
    m_bars->setData(m_x, data);
    ui->customPlot->replot();
    m_needUpdate = false;
}

void dlgVoltageValue::timerUpdate()
{
    static int n = 3;
    n++;
    if(n >= 3)
    {
        n = 0;
        CMsgDeviceInfo msgDeviceInfo;
        msgDeviceInfo.deviceInfo.insert(DI_DistributionId, m_loop->distribution()->distributionId());
        msgDeviceInfo.deviceInfo.insert(DI_LoopId, m_loop->loopId());
        CGlobal::instance()->ClientBusiness()->exeCommand(NCT_GetVoltageValue, &msgDeviceInfo);
    }
    if(m_needUpdate)
        updateVoltageValue();
}

void dlgVoltageValue::loopDeviceStatusChanged(short status, unsigned char oldValue, unsigned char newValue, unsigned int time)
{
    Q_UNUSED(oldValue);
    Q_UNUSED(newValue);
    Q_UNUSED(time);
    if(status == OBJS_VoltageValue)
        m_needUpdate = true;
}
