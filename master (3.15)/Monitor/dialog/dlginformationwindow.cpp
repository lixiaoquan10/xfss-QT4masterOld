#include "cglobal.h"


dlgInformationWindow::dlgInformationWindow(QWidget *parent) :
    QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowOpacity(0.8);
    this->setFixedSize(400, 200);
    this->setStyleSheet("background-color:rgb(170, 170, 170)");
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);


    movie = new QMovie(this);

    m_PushButton = new QPushButton(this);
    m_PushButton->setFixedHeight(35);
    m_PushButton->setFixedWidth(80);
    m_PushButton->setText("关闭");
    connect(m_PushButton,SIGNAL(clicked(bool)),this,SLOT(slot_close()));


    m_Icon = new QLabel;
    m_Description = new QLabel;
    m_Description->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_Description->setAlignment(Qt::AlignCenter);


    m_VBoxLayout = new QVBoxLayout;

    m_HBoxLayout = new QHBoxLayout;
    m_HBoxLayout->addStretch();
    m_HBoxLayout->addWidget(m_PushButton);
    m_HBoxLayout->addStretch();

    m_VBoxLayout->addWidget(m_Description);
    m_VBoxLayout->addLayout(m_HBoxLayout);

    setLayout(m_VBoxLayout);

}

dlgInformationWindow::~dlgInformationWindow()
{
    delete m_Icon;
    delete m_Description;
    delete m_HBoxLayout;
    delete m_VBoxLayout;
    delete m_PushButton;
    CGlobal::instance()->setInformationWindow(NULL);
}

void dlgInformationWindow::slot_close()
{
    dlgAdministrator m_Administrator;
    m_Administrator.setWindowTitle("用户验证");
    m_Administrator.setUserDisabled(true);
    m_Administrator.setWindowFlags(Qt::WindowStaysOnTopHint);
    if(QDialog::Accepted == m_Administrator.exec()) {
        CMsgNull m_MsgNUll;
        if(CGlobal::instance()->m_SystemYearCheck)
        {
            CGlobal::instance()->ClientBusiness()->exeCommand(NCT_YearCheckEnd, &m_MsgNUll);
            CGlobal::instance()->m_SystemYearCheck = false;
        }

        else if(CGlobal::instance()->m_SystemMonthCheck)
        {
            CGlobal::instance()->ClientBusiness()->exeCommand(NCT_MonthCheckEnd, &m_MsgNUll);
            CGlobal::instance()->m_SystemMonthCheck = false;
        }
        CGlobal::instance()->InformationWindow()->hide();
    } else {
        return;
    }

}

void dlgInformationWindow::setMovie(QString filePath)
{
    movie->stop();
    movie->setFileName(filePath);
    movie->setScaledSize(QSize(128,128));
    m_Icon->setMovie(movie);
    movie->start();
}

void dlgInformationWindow::hideCloseButton()
{
    m_PushButton->hide();
}

void dlgInformationWindow::showCloseButton()
{
    m_PushButton->show();
}

void dlgInformationWindow::setDescription(QString description)
{
    m_Description->setText(description);
    repaint();
}

void dlgInformationWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
}

void dlgInformationWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() != Qt::Key_Escape)
        QDialog::keyPressEvent(event);
}
