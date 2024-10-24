#include "cdesignview.h"
#include "cminisplitter.h"
#include "cdesigncontainer.h"
#include "cglobal.h"
#include "cdesigntreeview.h"

#include <QVBoxLayout>

CDesignView::CDesignView(QWidget *parent) :
    QWidget(parent)
{
    m_pSpliter = new CMiniSplitter(this);
    CDesignTreeView* treeView = new CDesignTreeView(m_pSpliter);
    treeView->setFixedWidth(treeView->sizeHint().width());
    m_pSpliter->addWidget(treeView);
    m_pSpliter->addWidget(new CDesignContainer(m_pSpliter));

    QVBoxLayout* pLayout = new QVBoxLayout();
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->addWidget(m_pSpliter);
    setLayout(pLayout);
}

CDesignView::~CDesignView()
{
    delete m_pSpliter;
}

