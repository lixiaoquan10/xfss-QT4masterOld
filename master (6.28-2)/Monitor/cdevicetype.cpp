#include "cglobal.h"

CDeviceType::CDeviceType(QObject *parent) :
    QObject(parent), m_type(0)
{

}

CDeviceType::CDeviceType(const int type, const QString name, QObject *parent) :
    QObject(parent), m_type(type), m_name(name)
{

}

void CDeviceType::setRawImgReal(const QString &str)
{
    m_rawImgReal = CGlobal::loadRawData(str);
    m_hashRealPixs.clear();
}

QByteArray &CDeviceType::rawImgReal()
{
    return m_rawImgReal;
}

void CDeviceType::setRawImgStandard(const QString &str)
{
    m_rawImgStandard = CGlobal::loadRawData(str);
    m_hashRealPixs.clear();
}

QByteArray &CDeviceType::rawImgStandard()
{
    return m_rawImgStandard;
}

QPixmap CDeviceType::imageStandard(qreal s) const
{
    return loadImageFromData(s, m_rawImgStandard, false);
}

QPixmap CDeviceType::imageReal(qreal s) const
{
    return loadImageFromData(s, m_rawImgReal, true);
}

QPixmap CDeviceType::image(qreal s)
{
    return m_rawImgReal.isEmpty() ? (m_rawImgStandard.isEmpty() ? imageReal(s) : imageStandard(s)) : imageReal(s);
}

QPixmap CDeviceType::loadImageFromData(qreal s, const QByteArray &data, bool bReal) const
{
    QString strkey = QString::number(s, 'f');
    QHash<QString, QPixmap>& hash = bReal ? m_hashRealPixs : m_hashStandardPixs;
    if (!hash.contains(strkey)) {
        QSvgRenderer svg;
        if (svg.load(data)) {
            QRectF rect(0.0f, 0.0f, svg.viewBoxF().width() * s, svg.viewBoxF().height() * s);
            QPixmap pix(rect.width(), rect.height());
            pix.fill(QColor(0,0,0,0));            
            QPainter painter;
//            painter.setCompositionMode(QPainter::CompositionMode_Destination);
            painter.begin(&pix);
            svg.render(&painter, rect);
            painter.end();
            hash[strkey] = pix;
        } else {
            QPixmap pix;
            if (pix.loadFromData(data)) {
                hash[strkey] = pix.scaled(pix.width() * s, pix.height() * s);
            } else {
                QSvgRenderer svgUndefined;
                svgUndefined.load(QLatin1String(":/images/devtype/0.svg"));
                QRectF rect(0.0f, 0.0f, svgUndefined.viewBoxF().width() * s, svgUndefined.viewBoxF().height() * s);
                QPixmap pixUndefined(rect.width(), rect.height());
                pixUndefined.fill(QColor(0,0,0,0));
                QPainter painter;
//                painter.setCompositionMode(QPainter::CompositionMode_Destination);
                painter.begin(&pixUndefined);
                svgUndefined.render(&painter, rect);
                painter.end();
                hash[strkey] = pixUndefined;
            }
        }
    }
    return hash[strkey];
}
