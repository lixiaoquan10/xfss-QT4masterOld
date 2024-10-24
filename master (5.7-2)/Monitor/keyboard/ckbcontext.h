#ifndef DISP_INPUT_H
#define DISP_INPUT_H

#include <QObject>
#include <QtGui/QInputContext>
#include "ckeyboard.h"

class CKBContext : public QInputContext
{
    Q_OBJECT
public:
    CKBContext();
    bool filterEvent(const QEvent *event);
    QString identifierName();
    QString language();

    bool isComposing() const;
    void reset();

private slots:
    void sendCharacter(quint32 character);

private:
    CKeyboard *disp_inputkey;
    void updatePosition();
};



#endif // DISP_INPUT_H

