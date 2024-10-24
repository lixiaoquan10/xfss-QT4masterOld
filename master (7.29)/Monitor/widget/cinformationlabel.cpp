#include "cinformationlabel.h"

#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QResizeEvent>
#include <QDebug>
#include <QDateTime>

CInformationLabel::CInformationLabel(QWidget *parent) :
    QLabel(parent), m_currentParent(parent)
{
    setStyleSheet("background-color: rgba(173,172,220,140); color: red");
    setFont(QFont("宋体", 24));
    setMouseTracking(true);
    setAlignment(Qt::AlignCenter);
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    hide();
}

void CInformationLabel::setInformation(const QString &info)
{
    qDebug() << "CInformationLabel::setInformation" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    setText(info);
    if(info.isEmpty())
        hide();
    else
        show();
}

void CInformationLabel::resetParentWidget(QWidget *parent)
{
    qDebug() << "CInformationLabel::resetParentWidget" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_currentParent = parent;
    setParent(parent);
    resetSize();
    if(parent && !this->text().isEmpty())
        show();
    else
        hide();
}

void CInformationLabel::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    resetSize();
}

void CInformationLabel::resetSize()
{
    qDebug() << "CInformationLabel::resetSize" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QWidget* parent = this->parentWidget();
    if(parent){
        setFixedWidth(500);//parent->size().width());
        move(parent->size().width()-500, 0);
    }else{
        move(0, 0);
    }
}

