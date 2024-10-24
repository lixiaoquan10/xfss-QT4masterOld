#include "ckeyboard.h"
#include "ui_ckeyboard.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include "cglobal.h"

CKeyboard::CKeyboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CKeyboard)
{
    ui->setupUi(this);
    this->setModal(true);
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint*/);
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
            this, SLOT(saveFocusWidget(QWidget*,QWidget*)));


    PushButton_Init();


#if 1
    //signalMapper.setMapping(ui->pushButton_0oint, ui->pushButton_0oint);
    signalMapper.setMapping(ui->pushButton_3, ui->pushButton_3);
    signalMapper.setMapping(ui->pushButton_dian, ui->pushButton_dian);
    signalMapper.setMapping(ui->pushButton_8, ui->pushButton_8);
    signalMapper.setMapping(ui->pushButton_9, ui->pushButton_9);
    signalMapper.setMapping(ui->pushButton_0, ui->pushButton_0);
    signalMapper.setMapping(ui->pushButton_1, ui->pushButton_1);
    signalMapper.setMapping(ui->pushButton_4, ui->pushButton_4);
    signalMapper.setMapping(ui->pushButton_5, ui->pushButton_5);
    signalMapper.setMapping(ui->pushButton_7, ui->pushButton_7);
    signalMapper.setMapping(ui->pushButton_2, ui->pushButton_2);
    signalMapper.setMapping(ui->pushButton_6, ui->pushButton_6);
    signalMapper.setMapping(ui->pushButton_delete, ui->pushButton_delete);
#endif

#if 1
    //connect(ui->pushButton_0oint, SIGNAL(clicked(bool)), &signalMapper, SLOT(map()));
    connect(ui->pushButton_3, SIGNAL(clicked(bool)), &signalMapper, SLOT(map()));
    connect(ui->pushButton_dian, SIGNAL(clicked(bool)), &signalMapper, SLOT(map()));
    connect(ui->pushButton_8, SIGNAL(clicked(bool)), &signalMapper, SLOT(map()));
    connect(ui->pushButton_9, SIGNAL(clicked(bool)), &signalMapper, SLOT(map()));
    connect(ui->pushButton_0, SIGNAL(clicked(bool)), &signalMapper, SLOT(map()));
    connect(ui->pushButton_1, SIGNAL(clicked(bool)), &signalMapper, SLOT(map()));
    connect(ui->pushButton_4, SIGNAL(clicked(bool)), &signalMapper, SLOT(map()));
    connect(ui->pushButton_5, SIGNAL(clicked(bool)), &signalMapper, SLOT(map()));
    connect(ui->pushButton_7, SIGNAL(clicked(bool)), &signalMapper, SLOT(map()));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), &signalMapper, SLOT(map()));
    connect(ui->pushButton_6, SIGNAL(clicked(bool)), &signalMapper, SLOT(map()));
    connect(ui->pushButton_delete, SIGNAL(clicked(bool)), &signalMapper, SLOT(map()));
#endif
    connect(&signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(buttonClicked(QWidget*)));
}

bool CKeyboard::event(QEvent *ev)
{
    switch(ev->type()) {
    case QEvent::WindowActivate:
        if (lastFocusWidget)
        {
            lastFocusWidget->activateWindow();
        }

        break;
    default:
        break;
    }
    return QWidget::event(ev);
}

void CKeyboard::saveFocusWidget(QWidget *oldFocus, QWidget *newFocus)
{
    qDebug() << "CKeyboard::saveFocusWidget" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    Q_UNUSED(oldFocus);
    if (newFocus != 0 && !this->isActiveWindow())
    {
        lastFocusWidget = newFocus;
    }
}

void CKeyboard::buttonClicked(QWidget *w)
{
    qDebug() << "CKeyboard::buttonClicked" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QPushButton *button = static_cast<QPushButton*>(w);
    if (!button) return;
    quint32 chr = qvariant_cast<quint32>(w->property("buttonValue"));
    if(w->property("buttonValue") == quint32(Qt::Key_Backspace))
    {
        ui->LineEdit_Echo->backspace();//删除选中文本或光标前的一个字符，默认光标在最后
        emit characterGenerated(chr);

        return;
    }
    ui->LineEdit_Echo->insert(button->text());//候选字母输入框插入字母
    emit characterGenerated(chr);
}


CKeyboard::~CKeyboard()
{
    delete ui;
}

QWidget *CKeyboard::getFocusedWidget()
{
    qDebug() << "CKeyboard::getFocusedWidget" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(CGlobal::instance()->m_isdialogMousePress && CGlobal::instance()->m_lineEditwidget)
            return CGlobal::instance()->m_lineEditwidget;
    return lastFocusWidget;
}

void CKeyboard::on_pushButton_close_clicked()
{
    qDebug() << "CKeyboard::on_pushButton_close_clicked" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    ui->LineEdit_Echo->clear();
    PushButton_Init();
    this->hide();
}


void CKeyboard::PushButton_Init()
{
    qDebug() << "CKeyboard::PushButton_Init" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    ui->pushButton_delete->setProperty("buttonValue", QVariant(quint32(Qt::Key_Backspace)));
    ui->pushButton_3->setProperty("buttonValue", QChar('3'));
    ui->pushButton_dian->setProperty("buttonValue", QChar('.'));
    ui->pushButton_8->setProperty("buttonValue", QChar('8'));
    ui->pushButton_9->setProperty("buttonValue", QChar('9'));
    ui->pushButton_0->setProperty("buttonValue", QChar('0'));
    ui->pushButton_1->setProperty("buttonValue", QChar('1'));
    ui->pushButton_4->setProperty("buttonValue", QChar('4'));
    ui->pushButton_5->setProperty("buttonValue", QChar('5'));
    ui->pushButton_7->setProperty("buttonValue", QChar('7'));
    ui->pushButton_2->setProperty("buttonValue", QChar('2'));
    ui->pushButton_6->setProperty("buttonValue", QChar('6'));
}


