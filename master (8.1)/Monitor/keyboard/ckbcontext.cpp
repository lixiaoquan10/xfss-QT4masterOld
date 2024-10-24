#include "ckbcontext.h"
#include "QWSServer"
#include <QPointer>
#include <QDebug>

CKBContext::CKBContext()
{
    disp_inputkey = new CKeyboard;
    connect(disp_inputkey, SIGNAL(characterGenerated(quint32)), this, SLOT(sendCharacter(quint32)));
//    connect(disp_inputkey, SIGNAL(chineseGenerated(QString)), this, SLOT(sendChinese(QString)));
}

bool CKBContext::filterEvent(const QEvent *event)
{
    if (event->type() == QEvent::RequestSoftwareInputPanel)
    {
        updatePosition();
        disp_inputkey->show();
        return true;
    }
    else if (event->type() == QEvent::CloseSoftwareInputPanel)
    {
        disp_inputkey->hide();
        return true;
    }
    return false;
}

QString CKBContext::identifierName()
{
    return "InputPanelContext";
}

void CKBContext::reset()
{

}

bool CKBContext::isComposing() const
{
    return false;
}

QString CKBContext::language()
{
    return "en_US";
}

void CKBContext::sendCharacter(quint32 character)
{
    qDebug() << "CKBContext::sendCharacter" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
 //   QPointer<QWidget> w = focusWidget();
    QPointer<QWidget> w = disp_inputkey->getFocusedWidget();
    if (!w) return;

    QKeyEvent keyPress(QEvent::KeyPress, character, Qt::NoModifier, QString(character));
    QApplication::sendEvent(w, &keyPress);

    if (!w) return;
    QKeyEvent keyRelease(QEvent::KeyRelease, character, Qt::NoModifier, QString());
    QApplication::sendEvent(w, &keyRelease);

    //if(character.toAscii()==3)
    //{
        //QWSServer::sendKeyEvent(0x01000003,Qt::Key_Backspace,Qt::NoModifier,true,false);
    //}
}



void CKBContext::updatePosition()
{
    qDebug() << "CKBContext::updatePosition" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QWidget *widget = disp_inputkey->getFocusedWidget();
    if (!widget) return;
    QRect widgetRect = widget->rect();
    QPoint panelPos = QPoint(widgetRect.left(), widgetRect.bottom()+30);
    panelPos = widget->mapToGlobal(panelPos);
    if (panelPos.x() + disp_inputkey->width() > 1366)
    {
        panelPos.setX(1356 - disp_inputkey->width());
    }
    if (panelPos.y() + disp_inputkey->height() > 768)
    {
        panelPos.setY(panelPos.y() - disp_inputkey->height() - 25);
    }
    disp_inputkey->move(panelPos);
}
