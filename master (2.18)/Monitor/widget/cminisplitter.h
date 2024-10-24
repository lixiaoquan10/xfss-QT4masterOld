#ifndef CMINISPLITTER_H
#define CMINISPLITTER_H

#include <QSplitter>

class CMiniSplitter : public QSplitter
{
    Q_OBJECT
public:
    CMiniSplitter(QWidget *parent = 0);
    CMiniSplitter(Qt::Orientation orientation);

protected:
    QSplitterHandle *createHandle();
};

#endif // CMINISPLITTER_H
