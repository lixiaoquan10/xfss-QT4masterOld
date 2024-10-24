#ifndef CDESIGNVIEW_H
#define CDESIGNVIEW_H

#include <QWidget>

class CMiniSplitter;
class CDesignView : public QWidget
{
    Q_OBJECT
public:
    explicit CDesignView(QWidget *parent = 0);
    virtual ~CDesignView();
signals:

private:
    CMiniSplitter* m_pSpliter;
};

#endif // CDESIGNVIEW_H
