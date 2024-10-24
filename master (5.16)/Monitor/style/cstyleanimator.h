#ifndef CSTYLEANIMATOR_H
#define CSTYLEANIMATOR_H

#include <QPointer>
#include <QTime>
#include <QBasicTimer>
#include <QStyle>
#include <QPainter>
#include <QWidget>

/*
 * This is a set of helper classes to allow for widget animations in
 * the style. Its mostly taken from Vista style so it should be fully documented
 * there.
 *
 */

class CAnimation
{
public :
    CAnimation() : m_running(true) { }
    virtual ~CAnimation() { }
    QWidget * widget() const { return m_widget; }
    bool running() const { return m_running; }
    const QTime &startTime() const { return m_startTime; }
    void setRunning(bool val) { m_running = val; }
    void setWidget(QWidget *widget) { m_widget = widget; }
    void setStartTime(const QTime &startTime) { m_startTime = startTime; }
    virtual void paint(QPainter *painter, const QStyleOption *option);

protected:
    void drawBlendedImage(QPainter *painter, QRect rect, float value);
    QTime m_startTime;
    QPointer<QWidget> m_widget;
    QImage m_primaryImage;
    QImage m_secondaryImage;
    QImage m_tempImage;
    bool m_running;
};

// Handles state transition animations
class CTransition : public CAnimation
{
public :
    CTransition() : CAnimation() {}
    virtual ~CTransition() {}
    void setDuration(int duration) { m_duration = duration; }
    void setStartImage(const QImage &image) { m_primaryImage = image; }
    void setEndImage(const QImage &image) { m_secondaryImage = image; }
    virtual void paint(QPainter *painter, const QStyleOption *option);
    int duration() const { return m_duration; }
    int m_duration; //set time in ms to complete a state transition
};

class CStyleAnimator : public QObject
{
    Q_OBJECT

public:
    CStyleAnimator(QObject *parent = 0) : QObject(parent) {}

    void timerEvent(QTimerEvent *);
    void startAnimation(CAnimation *);
    void stopAnimation(const QWidget *);
    CAnimation* widgetAnimation(const QWidget *) const;

private:
    QBasicTimer animationTimer;
    QList <CAnimation*> animations;
};
#endif // CSTYLEANIMATOR_H
