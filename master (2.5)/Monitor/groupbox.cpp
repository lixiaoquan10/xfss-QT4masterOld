#include "groupbox.h"

GroupBox::GroupBox(QWidget *parent) :
    QGroupBox(parent)
{
//    setFlat(true);
    setMouseTracking(true);
    setStyleSheet("GroupBox {background-color: rgb(215, 220, 225);}");
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

GroupBox::GroupBox(const QString &title, QWidget *parent) :
    QGroupBox(title, parent)
{
//    setFlat(true);
    setMouseTracking(true);
    setStyleSheet("QGroupBox {background-color: rgb(215, 220, 225);}");
}
