#ifndef CPATHDELEGATE_H
#define CPATHDELEGATE_H

#include <QItemDelegate>

class CPathDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CPathDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // CPATHDELEGATE_H
