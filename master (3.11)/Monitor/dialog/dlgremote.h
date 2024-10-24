#ifndef DLGREMOTE_H
#define DLGREMOTE_H

#include <QDialog>
#include <QPushButton>
#include <QGroupBox>

class IPAddress;
class dlgRemote : public QDialog
{
    Q_OBJECT
public:
    explicit dlgRemote(QWidget *parent = 0);
    ~dlgRemote();

    IPAddress   *m_IP;
    QPushButton *m_btnOK;
    QPushButton *m_btnProxy;
    QGroupBox   *m_Group;
    
signals:
    
public slots:
    void slot_on_btnOK_Clicked();
    void slot_on_btnProxy_Clicked();
    
};

#endif // DLGREMOTE_H
