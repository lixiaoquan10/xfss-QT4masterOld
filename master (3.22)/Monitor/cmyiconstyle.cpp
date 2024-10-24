#include "cmyiconstyle.h"

CMyIconStyle::CMyIconStyle() :
    QPlastiqueStyle()
{
}

int CMyIconStyle::pixelMetric(QStyle::PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    int n = QPlastiqueStyle::pixelMetric(metric, option, widget);
    if(metric == QStyle::PM_SmallIconSize)
        n = 32;
    return n;
}
