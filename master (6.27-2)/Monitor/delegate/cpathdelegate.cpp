#include "cpathdelegate.h"

CPathDelegate::CPathDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void CPathDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem  viewOption(option);
    viewOption.palette.setColor(QPalette::HighlightedText, index.data(Qt::TextColorRole).value<QColor>());
    QVariant tmp = index.data(Qt::BackgroundRole);
    if(tmp.isValid())
    {
        QColor oldColor1(viewOption.palette.color(QPalette::Highlight));
        QColor oldColor2(tmp.value<QBrush>().color());
        QColor newColor((oldColor1.red() + oldColor2.red()) / 2,
                        (oldColor1.green() + oldColor2.green()) / 2,
                        (oldColor1.blue() + oldColor2.blue()) / 2);
        viewOption.palette.setColor(QPalette::Highlight, newColor);
    }
    QItemDelegate::paint(painter, viewOption, index);
}
