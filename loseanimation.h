#ifndef LOSEANIMATION_H
#define LOSEANIMATION_H

#include <QWidget>
#include <Box2D/Box2D.h>
#include <QTimer>

class LoseAnimation : public QWidget
{
    Q_OBJECT
public:
    explicit LoseAnimation(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);

signals:

public slots:
     void updateWorld();

 private:
     b2World world;
     b2Body* body;
     QTimer timer;
     QImage image;

 public:
     void displayLoseAnimation();
     enum _moveState {
         MS_STOP,
         MS_UP,
         MS_DOWN
     };

     _moveState moveState;


};

#endif // LOSEANIMATION_H
