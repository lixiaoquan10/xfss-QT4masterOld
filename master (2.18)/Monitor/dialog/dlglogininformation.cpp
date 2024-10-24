#include "dlglogininformation.h"
#include "widget/cprogramview.h"
#include "widget/cprogramtreeview.h"
#include "widget/cprogramcontainer.h"
#include "dm/cloop.h"
#include "cglobal.h"

#include <QVBoxLayout>
#include <QMenu>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>


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

