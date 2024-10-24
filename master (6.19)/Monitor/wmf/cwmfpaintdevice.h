#ifndef CWMFPAINTDEVICE_H
#define CWMFPAINTDEVICE_H

#include <QPaintDevice>
class CWmfPaintEngine;
class CWmfPaintDevice : public QPaintDevice
{
public:
    CWmfPaintDevice(const QString& strFileName);
    ~CWmfPaintDevice();
    virtual QPaintEngine * paintEngine () const;
protected:
    virtual int metric ( PaintDeviceMetric ) const;

private:
    CWmfPaintEngine* engine;
};

#endif // CWMFPAINTDEVICE_H
