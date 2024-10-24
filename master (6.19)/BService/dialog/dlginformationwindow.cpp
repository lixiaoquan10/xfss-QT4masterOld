#include "dlginformationwindow.h"
#include "cglobal.h"
//#include <windows.h>
#include <QSizePolicy>
#include <QCloseEvent>

dlgInformationWindow::dlgInformationWindow(QWidget *parent) :
    QDialog(parent)
{
    CGlobal::instance()->setDlgInformationWindow(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowOpacity(0.85);
    this->setFixedSize(500, 220);
    this->setStyleSheet("background-color:rgb(170, 170, 170)");
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);


    movie = new QMovie(this);

//    m_ProgressBar = new QProgressBar(this);

    m_VBoxLayout = new QVBoxLayout(this);
    m_HBoxLayout = new QHBoxLayout;

    m_Icon = new QLabel;
    m_Description = new QLabel;
    m_Description->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_HBoxLayout->addWidget(m_Icon);
    m_HBoxLayout->addWidget(m_Description);

    m_VBoxLayout->addLayout(m_HBoxLayout);
//    m_VBoxLayout->addWidget(m_ProgressBar);
//    m_ProgressBar->hide();

    this->setLayout(m_VBoxLayout);

}

dlgInformationWindow::~dlgInformationWindow()
{
    CGlobal::instance()->setDlgInformationWindow(NULL);
    delete m_Icon;
    delete m_Description;
    delete m_HBoxLayout;
    delete m_VBoxLayout;
//    delete m_ProgressBar;
    delete movie;
}

void dlgInformationWindow::setMovie(QString filePath)
{
    movie->stop();
    movie->setFileName(filePath);
    m_Icon->setMovie(movie);
    movie->start();
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
