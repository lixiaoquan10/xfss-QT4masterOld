#include "cwmfpaintengine.h"
#include "libwmf/WmfPainterBackend.h"

CWmfPaintEngine::CWmfPaintEngine(const QString& strFileName) :
    QPaintEngine()
{
    m_pBackend = new Libwmf::WmfPainterBackend(QSizeF());
    m_pBackend->load(strFileName);
}

CWmfPaintEngine::~CWmfPaintEngine()
{
    delete m_pBackend;
}

bool CWmfPaintEngine::begin(QPaintDevice *pdev)
{
    return true;
}

bool CWmfPaintEngine::end()
{
    return true;
}

void CWmfPaintEngine::updateState(const QPaintEngineState &state)
{
}

void CWmfPaintEngine::drawRects(const QRect *rects, int rectCount)
{
}

void CWmfPaintEngine::drawRects(const QRectF *rects, int rectCount)
{
}

void CWmfPaintEngine::drawLines(const QLine *lines, int lineCount)
{
}

void CWmfPaintEngine::drawLines(const QLineF *lines, int lineCount)
{
}

void CWmfPaintEngine::drawEllipse(const QRectF &r)
{
}

void CWmfPaintEngine::drawEllipse(const QRect &r)
{
}

void CWmfPaintEngine::drawPath(const QPainterPath &path)
{
}

void CWmfPaintEngine::drawPoints(const QPointF *points, int pointCount)
{
}

void CWmfPaintEngine::drawPoints(const QPoint *points, int pointCount)
{
}

void CWmfPaintEngine::drawPolygon(const QPointF *points, int pointCount, QPaintEngine::PolygonDrawMode mode)
{
}

void CWmfPaintEngine::drawPolygon(const QPoint *points, int pointCount, QPaintEngine::PolygonDrawMode mode)
{
}

void CWmfPaintEngine::drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr)
{
}

void CWmfPaintEngine::drawTextItem(const QPointF &p, const QTextItem &textItem)
{
}

void CWmfPaintEngine::drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s)
{
}

void CWmfPaintEngine::drawImage(const QRectF &r, const QImage &pm, const QRectF &sr, Qt::ImageConversionFlags flags)
{
}

QPoint CWmfPaintEngine::coordinateOffset() const
{
    return QPoint();
}

QPaintEngine::Type CWmfPaintEngine::type() const
{
    return User;
}
