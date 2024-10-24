#include "cstyledbar.h"
#include "cstylehelper.h"

#include <QVariant>
#include <QPainter>
#include <QPixmapCache>
#include <QStyle>
#include <QStyleOption>

CStyledBar::CStyledBar(QWidget *parent)
    : QWidget(parent)
{
    setProperty("panelwidget", true);
    //setProperty("panelwidget_singlerow", true);
    setFixedHeight(32);
    setProperty("lightColored", false);
    setTandA(false);
}

void CStyledBar::setSingleRow(bool singleRow)
{
    setProperty("panelwidget_singlerow", singleRow);
}

void CStyledBar::setTandA(bool b)
{
    setProperty("zsyf", b);
}

bool CStyledBar::isSingleRow() const
{
    return property("panelwidget_singlerow").toBool();
}

void CStyledBar::setLightColored(bool lightColored)
{
    setProperty("lightColored", lightColored);
}

bool CStyledBar::isLightColored() const
{
    return property("lightColored").toBool();
}

void CStyledBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    QStyleOption option;
    option.rect = rect();
    option.state = QStyle::State_Horizontal;
    style()->drawControl(QStyle::CE_ToolBar, &option, &painter, this);
    if (property("zsyf").toBool()) {
        painter.drawImage(2, 2, QImage(":/images/main.jpg").scaled(60, 26));
    }
}

CStyledSeparator::CStyledSeparator(QWidget *parent)
    : QWidget(parent)
{
    setFixedWidth(10);
}

void CStyledSeparator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    QStyleOption option;
    option.rect = rect();
    option.state = QStyle::State_Horizontal;
    option.palette = palette();    
    style()->drawPrimitive(QStyle::PE_IndicatorToolBarSeparator, &option, &painter, this);
}
