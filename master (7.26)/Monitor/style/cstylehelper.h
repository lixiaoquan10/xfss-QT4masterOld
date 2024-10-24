#ifndef CSTYLEHELPER_H
#define CSTYLEHELPER_H

#include <QColor>
#include <QStyle>
#include <QSet>

QT_BEGIN_NAMESPACE
class QPalette;
class QPainter;
class QRect;
// Note, this is exported but in a private header as qtopengl depends on it.
// We should consider adding this as a public helper function.
void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

// Helper class holding all custom color values

class CStyleHelper
{
public:
    static const unsigned int DEFAULT_BASE_COLOR = 0x666666;

    // Height of the project explorer navigation bar
    static int navigationWidgetHeight() { return 28; }
    static qreal sidebarFontSize();
    static QPalette sidebarFontPalette(const QPalette &original);

    // This is our color table, all colors derive from baseColor
    static QColor requestedBaseColor() { return m_requestedBaseColor; }
    static QColor baseColor(bool lightColored = false);
    static QColor panelTextColor(bool lightColored = false);
    static QColor highlightColor(bool lightColored = false);
    static QColor shadowColor(bool lightColored = false);
    static QColor borderColor(bool lightColored = false);
    static QColor buttonTextColor() { return QColor(0x4c4c4c); }
    static QColor mergedColors(const QColor &colorA, const QColor &colorB, int factor = 50);

    static QColor sidebarHighlight() { return QColor(255, 255, 255, 40); }
    static QColor sidebarShadow() { return QColor(0, 0, 0, 40); }

    // Sets the base color and makes sure all top level widgets are updated
    static void setBaseColor(const QColor &color);

    // Draws a shaded anti-aliased arrow
    static void drawArrow(QStyle::PrimitiveElement element, QPainter *painter, const QStyleOption *option);

    // Gradients used for panels
    static void horizontalGradient(QPainter *painter, const QRect &spanRect, const QRect &clipRect, bool lightColored = false);
    static void verticalGradient(QPainter *painter, const QRect &spanRect, const QRect &clipRect, bool lightColored = false);
    static void menuGradient(QPainter *painter, const QRect &spanRect, const QRect &clipRect);
    static bool usePixmapCache() { return true; }

    static void drawIconWithShadow(const QIcon &icon, const QRect &rect, QPainter *p, QIcon::Mode iconMode,
                                   int radius = 3, const QColor &color = QColor(0, 0, 0, 130),
                                   const QPoint &offset = QPoint(1, -2));
    static void drawCornerImage(const QImage &img, QPainter *painter, QRect rect,
                         int left = 0, int top = 0, int right = 0, int bottom = 0);

    static void tintImage(QImage &img, const QColor &tintColor);

    static int clamp(float x);
    static void horizontalGradientHelper(QPainter *p, const QRect &spanRect, const
    QRect &rect, bool lightColored);
    static void menuGradientHelper(QPainter *p, const QRect &spanRect, const QRect &rect);
    static void verticalGradientHelper(QPainter *p, const QRect &spanRect, const QRect &rect, bool lightColored);

    // zrx
    static void addAllowWidget(const QWidget* widget) { if(widget) m_allowWidgets.insert(widget); }
    static void delAllowWidget(const QWidget* widget) { if(widget) m_allowWidgets.remove(widget); }
    static bool hasAllowWidget(const QWidget* widget) { if(widget) return m_allowWidgets.contains(widget); else return false; }
private:
    static QColor m_baseColor;
    static QColor m_requestedBaseColor;
    static QSet<const QWidget*> m_allowWidgets;
};

#endif // CSTYLEHELPER_H
