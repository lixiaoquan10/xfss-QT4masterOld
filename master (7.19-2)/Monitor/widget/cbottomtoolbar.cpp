#include "cglobal.h"

CBottomToolBar::CBottomToolBar(QWidget *parent) :
    QWidget(parent)
{
    createToolBar();
    setUser();
    setAuto();
}

CBottomToolBar::~CBottomToolBar()
{
    delete m_labelCurrentTime;
    delete m_labelEvacuationMode;
    delete m_labelUserLevel;
    delete m_labelCurrentZoneName;
    delete m_labelZoneName;
}

void CBottomToolBar::createToolBar()
{
    qDebug() << "CBottomToolBar::createToolBar" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QFont font;
    font.setPointSize(12);
    QPalette pa;
    QLabel *lbmsg = new QLabel("    系统消息:", this);
    lbmsg->setFixedWidth(100);
    lbmsg->setFont(font);

    CGlobal::instance()->m_labelSystemMsg->setFont(font);
    pa.setColor(QPalette::WindowText,QColor(12, 180, 3));
    CGlobal::instance()->m_labelSystemMsg->setPalette(pa);

    QFrame *vline1 = new QFrame(this);
    vline1->setFrameShape(QFrame::VLine);
    vline1->setFrameShadow(QFrame::Raised);
    vline1->setLineWidth(0);
    vline1->setMidLineWidth(1);


    m_labelCurrentZoneName = new QLabel("当前布局:", this);
    m_labelCurrentZoneName->setFont(font);
    m_labelZoneName = new QLabel(this);
    pa.setColor(QPalette::WindowText,Qt::blue);
    m_labelZoneName->setPalette(pa);
    m_labelZoneName->setFont(font);

    QFrame *vline2 = new QFrame(this);
    vline2->setFrameShape(QFrame::VLine);
    vline2->setFrameShadow(QFrame::Raised);
    vline2->setLineWidth(0);
    vline2->setMidLineWidth(1);

    QLabel *lbuser = new QLabel("用户权限:", this);
    lbuser->setFixedWidth(80);
    lbuser->setFont(font);

    m_labelUserLevel = new QLabel(this);
    pa.setColor(QPalette::WindowText,Qt::blue);
    m_labelUserLevel->setPalette(pa);
    m_labelUserLevel->setFixedWidth(150);
    m_labelUserLevel->setAlignment(Qt::AlignCenter);
    m_labelUserLevel->setFont(font);

    QFrame *vline3 = new QFrame(this);
    vline3->setFrameShape(QFrame::VLine);
    vline3->setFrameShadow(QFrame::Raised);
    vline3->setLineWidth(0);
    vline3->setMidLineWidth(1);

    QLabel *lbmode = new QLabel("疏散模式:", this);
    lbmode->setFixedWidth(80);
    lbmode->setFont(font);

    m_labelEvacuationMode = new QLabel(this);
    pa.setColor(QPalette::WindowText,Qt::blue);
    m_labelEvacuationMode->setPalette(pa);
    m_labelEvacuationMode->setFixedWidth(150);
    m_labelEvacuationMode->setAlignment(Qt::AlignCenter);
    m_labelEvacuationMode->setFont(font);

    QFrame *vline4 = new QFrame(this);
    vline4->setFrameShape(QFrame::VLine);
    vline4->setFrameShadow(QFrame::Raised);
    vline4->setLineWidth(0);
    vline4->setMidLineWidth(1);

    m_labelCurrentTime = new QLabel(getCurrentDateTime(), this);
    m_labelCurrentTime->setFixedWidth(300);
    m_labelCurrentTime->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_labelCurrentTime->setFont(font);

    QSpacerItem *bottomSpacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *bottomSpacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    m_bottomToolBox = CGlobal::instance()->CreateHBoxLayout();
    m_bottomToolBox->setSpacing(0);

    m_bottomToolBox->addWidget(lbmsg);
    m_bottomToolBox->addWidget(CGlobal::instance()->m_labelSystemMsg);
    m_bottomToolBox->addItem(bottomSpacerItem);
    m_bottomToolBox->addWidget(vline1);
    m_bottomToolBox->addWidget(m_labelCurrentZoneName);
    m_bottomToolBox->addWidget(m_labelZoneName);
    m_bottomToolBox->addItem(bottomSpacerItem1);
    m_bottomToolBox->addWidget(vline2);

    m_bottomToolBox->addWidget(lbuser);
    m_bottomToolBox->addWidget(m_labelUserLevel);
    m_bottomToolBox->addWidget(vline3);
    m_bottomToolBox->addWidget(lbmode);
    m_bottomToolBox->addWidget(m_labelEvacuationMode);
    m_bottomToolBox->addWidget(vline4);
    m_bottomToolBox->addWidget(m_labelCurrentTime, 0, Qt::AlignRight);

    this->setLayout(m_bottomToolBox);
}

QString CBottomToolBar::getCurrentDateTime()
{
    qDebug() << "CBottomToolBar::getCurrentDateTime" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString strWeek = "";
    switch(QDate::currentDate().dayOfWeek())
    {
        case Qt::Monday:
            strWeek = "星期一";
            break;
        case Qt::Tuesday:
            strWeek = "星期二";
            break;
        case Qt::Wednesday:
            strWeek = "星期三";
            break;
        case Qt::Thursday:
            strWeek = "星期四";
            break;
        case Qt::Friday:
            strWeek = "星期五";
            break;
        case Qt::Saturday:
            strWeek = "星期六";
            break;
        case Qt::Sunday:
            strWeek = "星期日";
            break;
        default:
            break;
    }
    QString strdatetime = QDateTime::currentDateTime().toString(DATETIME_FORMAT) + " " + strWeek;
    return strdatetime;
}

void CBottomToolBar::setCurrentDateTime()
{
    m_labelCurrentTime->setText(getCurrentDateTime());
}

void CBottomToolBar::setUser()
{
    qDebug() << "CBottomToolBar::setUser" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(CGlobal::instance()->m_UserType == CGlobal::User)
        m_labelUserLevel->setText(tr(""));
}

void CBottomToolBar::setAdministrator()
{
    qDebug() << "CBottomToolBar::setAdministrator" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(CGlobal::instance()->m_UserType == CGlobal::OnDuty)
        m_labelUserLevel->setText("值班员");
    else if(CGlobal::instance()->m_UserType == CGlobal::Operator)
        m_labelUserLevel->setText("操作员");
    else if(CGlobal::instance()->m_UserType == CGlobal::Administrator)
        m_labelUserLevel->setText("管理员");
    else if(CGlobal::instance()->m_UserType == CGlobal::SuperAdministrator)
        m_labelUserLevel->setText("SuperAdmin");
}


void CBottomToolBar::setManual()
{
    qDebug() << "CBottomToolBar::setManual" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_EvacuationMode = CGlobal::Manual;
    m_labelEvacuationMode->setText("手动");
}

void CBottomToolBar::setAuto()
{
    qDebug() << "CBottomToolBar::setAuto" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CGlobal::instance()->m_EvacuationMode = CGlobal::Auto;
    m_labelEvacuationMode->setText("自动");
}

