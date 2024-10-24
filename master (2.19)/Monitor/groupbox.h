#ifndef GROUPBOX_H
#define GROUPBOX_H

#include <QMouseEvent>
#include <QGroupBox>

class GroupBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit GroupBox(QWidget *parent = 0);
    explicit GroupBox(const QString &title, QWidget *parent = 0);
    
signals:
    
public slots:
    
};

#endif // GROUPBOX_H
