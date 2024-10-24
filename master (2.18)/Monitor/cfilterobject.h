#ifndef CFILTEROBJECT_H
#define CFILTEROBJECT_H

#include <QObject>

class CFilterObject : public QObject
{
    Q_OBJECT
public:
    explicit CFilterObject(QObject *parent = 0);
    bool eventFilter(QObject* watched, QEvent* event);
signals:
    void keyMouseChanged();
};

#endif // CFILTEROBJECT_H
