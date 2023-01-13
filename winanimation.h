#ifndef WINANIMATION_H
#define WINANIMATION_H

#include <QWidget>
#include <Box2D/Box2D.h>
#include <QTimer>

class WinAnimation : public QWidget
{
    Q_OBJECT
public:
    explicit WinAnimation(QWidget *parent = nullptr);
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
      void displayWinAnimation();

};

#endif // WINANIMATION_H
