#include "cprogramcontainer.h"
#include "cglobal.h"
#include "cdesignsplashview.h"
#include "cprogramprojectview.h"
#include "cprogramcanportview.h"
#include "cprogramloopview.h"
#include "cprogramdistributionview.h"
#include "cprogramdeviceview.h"

#include <QStackedLayout>

CProgramContainer::CProgramContainer(QWidget *parent) :
    QWidget(parent)
{
    CGlobal::instance()->setProgramContainer(this);

    m_layout = new QStackedLayout(this);

    ///// 0
    CDesignSplashView* pSplashView = new CDesignSplashView();
    m_layout->addWidget(pSplashView);

    ///// 1
    CProgramControllerView* pControllerView = new CProgramControllerView();
    m_layout->addWidget(pControllerView);

    ///// 2
    CProgramCanportView* pCanportView = new CProgramCanportView();
    m_layout->addWidget(pCanportView);

    ///// 3
    CProgramDistributionView* pDistributionView = new CProgramDistributionView();
    m_layout->addWidget(pDistributionView);

    ///// 4
    CProgramDeviceView* pDeviceView = new CProgramDeviceView();
    m_layout->addWidget(pDeviceView);


    setCurrentView();
}

CProgramContainer::~CProgramContainer()
{
    CGlobal::instance()->setProgramContainer(0);
    delete m_layout;
}

void CProgramContainer::setCurrentView(int index)
{
    m_layout->setCurrentIndex(index);
}

