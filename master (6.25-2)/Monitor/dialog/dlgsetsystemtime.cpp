#include "dlgsetsystemtime.h"
#include "ui_dlgsetsystemtime.h"
#include "global.h"
#include "ctrace.h"
#include <QMessageBox>
#include <QTimer>
#include <QDebug>

#ifdef Q_OS_WIN
    #include "windows.h"
#elif (defined Q_OS_LINUX)

#endif

dlgSetSystemTime::dlgSetSystemTime(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgSetSystemTime)
{
    CTRACE_LOG;
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Set DateTime"));
    Global::instance()->m_HaveDialog = true;

    QDateTime dateTime;
    dateTime.setDate(dateTime.currentDateTime().date());
    dateTime.setTime(dateTime.currentDateTime().time());
    ui->dateEdit->setDateTime(dateTime);
    ui->timeEdit->setDateTime(dateTime);
}

dlgSetSystemTime::~dlgSetSystemTime()
{
    CTRACE_LOG;
    delete ui;
    Global::instance()->setSystemTime(NULL);
    Global::instance()->m_HaveDialog = false;
}

void dlgSetSystemTime::on_ok_button_clicked()
{
    CTRACE_LOG;
    QDate date = ui->dateEdit->date();
    QTime time = ui->timeEdit->time();

    if(setDateTime(date, time))
    {
        QMessageBox::information(this, tr("information"), tr("Set a time success!"));
        this->close();
    }
    else
    {
        QMessageBox::information(this, tr("information"), tr("Set a time failed!"));
    }
}

void dlgSetSystemTime::on_cancel_button_clicked()
{
    CTRACE_LOG;
    this->close();
}

bool dlgSetSystemTime::setDateTime(const QDate& date, const QTime& time)
{
    CTRACE_LOG;
    bool b = false;
#ifdef Q_OS_WIN
    SYSTEMTIME st;
    GetLocalTime(&st);

    st.wYear = date.year();
    st.wMonth = date.month();
    st.wDay = date.day();
    st.wDayOfWeek = date.dayOfWeek();
    st.wHour = time.hour();
    st.wMinute = time.minute();
    st.wSecond = time.second();

    b = SetLocalTime(&st);
#elif (defined Q_OS_LINUX)
    QString str = QString("date -s %1/%2/%3").arg(date.month()).arg(date.day()).arg(date.year());
    system(str.toLatin1().data());
    str = QString("date -s %1:%2:%3").arg(time.hour()).arg(time.minute()).arg(time.second());
    system(str.toLatin1().data());
    system("clock -w");
    b = true;
#endif
    return b;
}
