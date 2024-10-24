#include "cglobal.h"


CDesignContainer::CDesignContainer(QWidget *parent) :
    QWidget(parent)
{
    CGlobal::instance()->setDesignContainer(this);
    m_pLayout = new QStackedLayout(this);
    ///// 0
    m_pLayout->addWidget(new CDesignSplashView());
//    ///// 1
//    m_pLayout->addWidget(new CProgramDeviceView());
    setCurrentView(0);
}

CDesignContainer::~CDesignContainer()
{
    CGlobal::instance()->setDesignContainer(0);
    delete m_pLayout;
}

void CDesignContainer::setCurrentView(int index)
{
    m_pLayout->setCurrentIndex(index);
}
