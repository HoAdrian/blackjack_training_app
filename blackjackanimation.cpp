#include "blackjackanimation.h"

blackJackAnimation::blackJackAnimation(QWidget *parent) :
    QGraphicsView(parent)
{
    QPixmap image(":/new/images/blackJack.png");

    connect(&animationTimer, &QTimer::timeout, this, &blackJackAnimation::onTimeout);

    animationTimer.start(100);

}

void blackJackAnimation::onTimeout() {
    QPoint oldPos =
}
