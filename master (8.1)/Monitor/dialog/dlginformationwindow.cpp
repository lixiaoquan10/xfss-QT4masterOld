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

    QFont font;
    font.setPointSize(12); // 设置字体大小为12像素

    m_liftTime = new QLabel(this);
    m_liftTime->setText("剩余时间：");
    m_liftTime->setFont(font);

    m_PushButton = new QPushButton(this);
    m_PushButton->setFixedHeight(35);
    m_PushButton->setFixedWidth(80);
    m_PushButton->setText("关闭");
    m_PushButton->setFont(font);
    connect(m_PushButton,SIGNAL(clicked(bool)),this,SLOT(slot_close()));


    m_Icon = new QLabel;
    m_Description = new QLabel;
    m_Description->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_Description->setAlignment(Qt::AlignCenter);


    m_VBoxLayout = new QVBoxLayout;

    m_HBoxLayout1 = new QHBoxLayout;
    m_HBoxLayout1->addStretch();
    m_HBoxLayout1->addWidget(m_liftTime);
    m_HBoxLayout1->addStretch();

    m_HBoxLayout2 = new QHBoxLayout;
    m_HBoxLayout2->addStretch();
    m_HBoxLayout2->addWidget(m_PushButton);
    m_HBoxLayout2->addStretch();

    m_VBoxLayout->addWidget(m_Description);
    m_VBoxLayout->addLayout(m_HBoxLayout1);
    m_VBoxLayout->addLayout(m_HBoxLayout2);

    setLayout(m_VBoxLayout);

}

dlgInformationWindow::~dlgInformationWindow()
{
    delete m_Icon;
    delete m_Description;
    delete m_HBoxLayout1;
    delete m_HBoxLayout2;
    delete m_VBoxLayout;
    delete m_liftTime;
    delete m_PushButton;
    CGlobal::instance()->setInformationWindow(NULL);
}

void dlgInformationWindow::slot_close()
{
    qDebug() << "dlgInformationWindow::slot_close"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
    qDebug() << "dlgInformationWindow::setMovie"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    movie->stop();
    movie->setFileName(filePath);
    movie->setScaledSize(QSize(128,128));
    m_Icon->setMovie(movie);
    movie->start();
}

void dlgInformationWindow::hideCloseButton()
{
    qDebug() << "dlgInformationWindow::hideCloseButton"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_liftTime->hide();
    m_PushButton->hide();
}

void dlgInformationWindow::setLiftTime(QString str)
{
    qDebug() << "dlgInformationWindow::setLiftTime"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_liftTime->setText("剩余时间：" + str);
}

void dlgInformationWindow::showCloseButton()
{
    qDebug() << "dlgInformationWindow::showCloseButton"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_liftTime->show();
    m_PushButton->show();
}

void dlgInformationWindow::setDescription(QString description)
{
    qDebug() << "dlgInformationWindow::setDescription"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
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
