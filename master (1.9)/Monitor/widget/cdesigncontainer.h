#ifndef CDESIGNCONTAINER_H
#define CDESIGNCONTAINER_H

#include <QWidget>

class QStackedLayout;
class CDesignContainer : public QWidget
{
    Q_OBJECT
public:
    explicit CDesignContainer(QWidget *parent = 0);
    ~CDesignContainer();
    void setCurrentView(int index);
    
signals:
    
public slots:
private:
    QStackedLayout* m_pLayout;
};

#endif // CDESIGNCONTAINER_H
