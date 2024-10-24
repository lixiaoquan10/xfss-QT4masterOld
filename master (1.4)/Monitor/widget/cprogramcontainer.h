#ifndef CPROGRAMCONTAINER_H
#define CPROGRAMCONTAINER_H

#include <QWidget>

class QStackedLayout;
class CProgramContainer : public QWidget
{
    Q_OBJECT
public:
    explicit CProgramContainer(QWidget *parent = 0);
    ~CProgramContainer();
    void setCurrentView(int index = 0);
private:
    QStackedLayout* m_layout;
};

#endif // CPROGRAMCONTAINER_H
