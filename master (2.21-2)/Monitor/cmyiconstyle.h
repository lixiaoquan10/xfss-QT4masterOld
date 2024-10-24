#ifndef CMYICONSTYLE_H
#define CMYICONSTYLE_H

#include <QPlastiqueStyle>

class CMyIconStyle : public QPlastiqueStyle
{
    Q_OBJECT
public:
    CMyIconStyle();
    int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const;
};

#endif // CMYICONSTYLE_H
