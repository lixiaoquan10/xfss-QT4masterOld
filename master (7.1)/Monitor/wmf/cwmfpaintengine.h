#ifndef CWMFPAINTENGINE_H
#define CWMFPAINTENGINE_H

#include <QPaintEngine>
namespace Libwmf {
    class WmfPainterBackend;
}

class CWmfPaintEngine : public QPaintEngine
{
public:
    explicit CWmfPaintEngine(const QString& strFileName);
    virtual ~CWmfPaintEngine();

    virtual bool begin(QPaintDevice *pdev);
    virtual bool end();

    virtual void updateState(const QPaintEngineState &state);

    virtual void drawRects(const QRect *rects, int rectCount);
    virtual void drawRects(const QRectF *rects, int rectCount);

    virtual void drawLines(const QLine *lines, int lineCount);
    virtual void drawLines(const QLineF *lines, int lineCount);

    virtual void drawEllipse(const QRectF &r);
    virtual void drawEllipse(const QRect &r);

    virtual void drawPath(const QPainterPath &path);

    virtual void drawPoints(const QPointF *points, int pointCount);
    virtual void drawPoints(const QPoint *points, int pointCount);

    virtual void drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode);
    virtual void drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode);

    virtual void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr);
    virtual void drawTextItem(const QPointF &p, const QTextItem &textItem);
    virtual void drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s);
    virtual void drawImage(const QRectF &r, const QImage &pm, const QRectF &sr,
                           Qt::ImageConversionFlags flags = Qt::AutoColor);

    virtual QPoint coordinateOffset() const;
    virtual Type type() const;
signals:
    
public slots:
private:
    Libwmf::WmfPainterBackend* m_pBackend;
};

#endif // CWMFPAINTENGINE_H
