#include "cstyleanimator.h"

#include <QStyleOption>

CAnimation * CStyleAnimator::widgetAnimation(const QWidget *widget) const
{
    if (!widget)
        return 0;
    foreach (CAnimation *a, animations) {
        if (a->widget() == widget)
            return a;
    }
    return 0;
}

void CAnimation::paint(QPainter *painter, const QStyleOption *option)
{
    Q_UNUSED(option)
    Q_UNUSED(painter)
}

void CAnimation::drawBlendedImage(QPainter *painter, QRect rect, float alpha)
{
    if (m_secondaryImage.isNull() || m_primaryImage.isNull())
        return;

    if (m_tempImage.isNull())
        m_tempImage = m_secondaryImage;

    const int a = qRound(alpha*256);
    const int ia = 256 - a;
    const int sw = m_primaryImage.width();
    const int sh = m_primaryImage.height();
    const int bpl = m_primaryImage.bytesPerLine();
    switch (m_primaryImage.depth()) {
    case 32:
        {
            uchar *mixed_data = m_tempImage.bits();
            const uchar *back_data = m_primaryImage.bits();
            const uchar *front_data = m_secondaryImage.bits();
            for (int sy = 0; sy < sh; sy++) {
                quint32 *mixed = (quint32*)mixed_data;
                const quint32* back = (const quint32*)back_data;
                const quint32* front = (const quint32*)front_data;
                for (int sx = 0; sx < sw; sx++) {
                    quint32 bp = back[sx];
                    quint32 fp = front[sx];
                    mixed[sx] =  qRgba ((qRed(bp)*ia + qRed(fp)*a)>>8,
                                        (qGreen(bp)*ia + qGreen(fp)*a)>>8,
                                        (qBlue(bp)*ia + qBlue(fp)*a)>>8,
                                        (qAlpha(bp)*ia + qAlpha(fp)*a)>>8);
                }
                mixed_data += bpl;
                back_data += bpl;
                front_data += bpl;
            }
        }
    default:
        break;
    }
    painter->drawImage(rect, m_tempImage);
}

void CTransition::paint(QPainter *painter, const QStyleOption *option)
{
    float alpha = 1.0;
    if (m_duration > 0) {
        QTime current = QTime::currentTime();

        if (m_startTime > current)
            m_startTime = current;

        int timeDiff = m_startTime.msecsTo(current);
        alpha = timeDiff/(float)m_duration;
        if (timeDiff > m_duration) {
            m_running = false;
            alpha = 1.0;
        }
    }
    else {
        m_running = false;
    }
    drawBlendedImage(painter, option->rect, alpha);
}

void CStyleAnimator::timerEvent(QTimerEvent *)
{
    for (int i = animations.size() - 1 ; i >= 0 ; --i) {
        if (animations[i]->widget())
            animations[i]->widget()->update();

        if (!animations[i]->widget() ||
            !animations[i]->widget()->isEnabled() ||
            !animations[i]->widget()->isVisible() ||
            animations[i]->widget()->window()->isMinimized() ||
            !animations[i]->running())
        {
            CAnimation *a = animations.takeAt(i);
            delete a;
        }
    }
    if (animations.size() == 0 && animationTimer.isActive()) {
        animationTimer.stop();
    }
}

void CStyleAnimator::stopAnimation(const QWidget *w)
{
    for (int i = animations.size() - 1 ; i >= 0 ; --i) {
        if (animations[i]->widget() == w) {
            CAnimation *a = animations.takeAt(i);
            delete a;
            break;
        }
    }
}

void CStyleAnimator::startAnimation(CAnimation *t)
{
    stopAnimation(t->widget());
    animations.append(t);
    if (animations.size() > 0 && !animationTimer.isActive()) {
        animationTimer.start(35, this);
    }
}
