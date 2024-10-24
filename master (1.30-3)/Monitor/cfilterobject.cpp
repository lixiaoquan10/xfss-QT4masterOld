#include "cfilterobject.h"

#include <QEvent>
#include <QDebug>

CFilterObject::CFilterObject(QObject *parent) :
    QObject(parent)
{
}

bool CFilterObject::eventFilter(QObject *watched, QEvent *event)
{
	Q_UNUSED(watched);
    int n = event->type();
    if(n > 1 && n < 8)
    {
        emit keyMouseChanged();
    }
    return false;
}
