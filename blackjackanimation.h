#ifndef BLACKJACKANIMATION_H
#define BLACKJACKANIMATION_H

#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QGraphicsView>
#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QLabel>

class blackJackAnimation : public QGraphicsView
{

    Q_OBJECT

public:
    explicit blackJackAnimation(QWidget *parent = nullptr);
    ~blackJackAnimation();
    QTimer animationTimer;
    QLabel label;


protected slots:
    void onTimeout();


};

#endif // BLACKJACKANIMATION_H
#endif // GAMEWINDOW_H

