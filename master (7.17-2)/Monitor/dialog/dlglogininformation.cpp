#include "cglobal.h"
#include "dlglogininformation.h"


dlgLoginInformation::dlgLoginInformation(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout* pLayout = new QVBoxLayout();
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->addWidget(new CProgramView());
    setLayout(pLayout);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    CGlobal::instance()->programContainer()->setMaximumWidth(1366);
    CGlobal::instance()->programContainer()->setMaximumHeight(661);
}

