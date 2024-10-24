#include "cminisplitter.h"
#include "style/cstylehelper.h"

#include <QPaintEvent>
#include <QPainter>
#include <QSplitterHandle>

class CMiniSplitterHandle : public QSplitterHandle
{
public:
    CMiniSplitterHandle(Qt::Orientation orientation, QSplitter *parent)
            : QSplitterHandle(orientation, parent)
    {
        setMask(QRegion(contentsRect()));
        setAttribute(Qt::WA_MouseNoMask, true);
    }
protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
};

void CMiniSplitterHandle::resizeEvent(QResizeEvent *event)
{
    if (orientation() == Qt::Horizontal)
        setContentsMargins(2, 0, 2, 0);
    else
        setContentsMargins(0, 2, 0, 2);
    setMask(QRegion(contentsRect()));
    QSplitterHandle::resizeEvent(event);
}

void CMiniSplitterHandle::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(event->rect(), CStyleHelper::borderColor());
}

QSplitterHandle *CMiniSplitter::createHandle()
{
    return new CMiniSplitterHandle(orientation(), this);
}

CMiniSplitter::CMiniSplitter(QWidget *parent)
    : QSplitter(parent)
{
    setHandleWidth(1);
    setChildrenCollapsible(true);
    setProperty("minisplitter", true);
}

CMiniSplitter::CMiniSplitter(Qt::Orientation orientation)
    : QSplitter(orientation)
{
    setHandleWidth(1);
    setChildrenCollapsible(true);
    setProperty("minisplitter", true);
}
