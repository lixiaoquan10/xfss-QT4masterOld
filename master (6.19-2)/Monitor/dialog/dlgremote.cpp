#include "dlgremote.h"
#include "ipaddress.h"
#include "global.h"
#include "ctrace.h"

dlgRemote::dlgRemote(QWidget *parent) :
    QDialog(parent)
{
    CTRACE_LOG;
    setFixedSize(230, 90);
    setWindowTitle(tr("Network Setup"));
    setAttribute(Qt::WA_DeleteOnClose);
    Global::instance()->m_HaveDialog = true;

    m_Group = new QGroupBox(tr("Sever IP Address"), this);
    m_IP = new IPAddress(this);
    m_IP->settext(Global::instance()->m_strRemoteAddress);
    m_btnProxy = Global::instance()->CreatePushButton(this, tr("Proxy"), "", "");
    connect(m_btnProxy,SIGNAL(clicked()),this,SLOT(slot_on_btnProxy_Clicked()));
    m_btnOK = Global::instance()->CreatePushButton(this, tr("Ok"), "", "");
    connect(m_btnOK,SIGNAL(clicked()),this,SLOT(slot_on_btnOK_Clicked()));
    QGridLayout *layout = Global::instance()->CreateGridLayout(this,0,0);
    layout->addWidget(m_IP);
    m_Group->setLayout(layout);
    QGridLayout *mainlay = Global::instance()->CreateGridLayout(this,3,3);
    mainlay->addWidget(m_Group,0,0,1,2);
    mainlay->addWidget(m_btnProxy,1,0,1,1);
    mainlay->addWidget(m_btnOK,1,1,1,1);
    this->setLayout(mainlay);
    m_btnOK->setFocus();
}

dlgRemote::~dlgRemote()
{
    CTRACE_LOG;
    delete m_IP;
    delete m_btnOK;
    delete m_btnProxy;
    delete m_Group;
    Global::instance()->setNetRemote(NULL);
    Global::instance()->m_HaveDialog = false;
}

void dlgRemote::slot_on_btnOK_Clicked()
{
    CTRACE_LOG;
    Global::instance()->m_strRemoteAddress = m_IP->text();
    this->accept();
}

void dlgRemote::slot_on_btnProxy_Clicked()
{
    CTRACE_LOG;
    Global::instance()->ClientBusiness()->setProxy();
}
