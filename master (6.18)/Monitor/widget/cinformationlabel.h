#ifndef CINFORMATIONLABEL_H
#define CINFORMATIONLABEL_H

#include <QLabel>

class CInformationLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CInformationLabel(QWidget *parent = 0);
    void setInformation(const QString& info);
    void resetParentWidget(QWidget* parent);
protected:
    void resizeEvent(QResizeEvent* event);
private:
    void resetSize();
private:
    QWidget* m_currentParent;
};


#endif // CINFORMATIONLABEL_H
