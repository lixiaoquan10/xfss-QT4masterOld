#ifndef CSTYLEDBAR_H
#define CSTYLEDBAR_H

#include <QWidget>

class CStyledBar : public QWidget
{
    Q_OBJECT
public:
    CStyledBar(QWidget *parent = 0);
    void setSingleRow(bool singleRow);
    void setTandA(bool b);
    bool isSingleRow() const;

    void setLightColored(bool lightColored);
    bool isLightColored() const;

protected:
    void paintEvent(QPaintEvent *event);
};

class CStyledSeparator : public QWidget
{
    Q_OBJECT
public:
    CStyledSeparator(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // CSTYLEDBAR_H
