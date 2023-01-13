#include "gamewindow.h"
#include <QApplication>

/**
 * Group: Kimochi
 * Class: CS3505
 * Project: A8 Educational App
 * Date: November 18, 2022
 *
 * Main class to start up application
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model model;
    GameWindow w(model);
    w.show();
    return a.exec();
}
