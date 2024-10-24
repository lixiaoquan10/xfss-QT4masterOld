#include "cwmfpaintdevice.h"
#include <QApplication>
#include <QDesktopWidget>

#include "cwmfpaintengine.h"

CWmfPaintDevice::CWmfPaintDevice(const QString &strFileName)
{
    engine = new CWmfPaintEngine(strFileName);
}

CWmfPaintDevice::~CWmfPaintDevice()
{
    delete engine;
}

QPaintEngine * CWmfPaintDevice::paintEngine () const
{
    return engine;
}

int CWmfPaintDevice::metric(PaintDeviceMetric metric ) const
{
    int xdpi = QApplication::desktop()->logicalDpiX();
    int ydpi = QApplication::desktop()->logicalDpiY();

    switch (metric){
        case QPaintDevice::PdmWidth:
            return QSize().width();
        case QPaintDevice::PdmHeight:
            return QSize().height();
        case QPaintDevice::PdmWidthMM:
            return int(25.4*QSize().width()/(double)xdpi);
        case QPaintDevice::PdmHeightMM:
            return int(25.4*QSize().height()/(double)ydpi);
        case QPaintDevice::PdmNumColors:
            return 65536;//should it be millions?
        case QPaintDevice::PdmDepth:
            return 32;
        case QPaintDevice::PdmDpiX:
        case QPaintDevice::PdmPhysicalDpiX:
            return xdpi;
        case QPaintDevice::PdmDpiY:
        case QPaintDevice::PdmPhysicalDpiY:
            return ydpi;
        default:
            qWarning("CWmfPaintDevice::Strange metric asked");
            return 0;
    }
}
