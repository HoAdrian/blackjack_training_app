#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "model.h"
#include "qtimer.h"
#include "valueplotwindow.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QInputDialog>
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>
#include <QPaintEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class GameWindow; }
QT_END_NAMESPACE

/**
 * Group: Kimochi
 * Class: CS3505
 * Date: November 18, 2022
 * Project: A8 Educational App
 *
 * Reviewers: Bella Miller, Yunsu Kwon
 *
 * @brief This class is the View for our educational app and handles
 * all UI updates and displays for the blackjack game.
 */
class GameWindow : public QMainWindow
{
    Q_OBJECT
public:
    GameWindow(Model& model, QWidget *parent = nullptr);
    ~GameWindow();

private:
    Ui::GameWindow *ui;
    QGraphicsScene *graphic_;
    QTimer timer;
    QWidget * savedPageData;
    QTimer animationTimer;
    QTimer gameOverTimer;
    QPixmap gameOverImage;
    QPixmap semiTransparentGameOver;
    QPixmap landingPage;
    ValuePlotWindow valuePlotWindow;
    bool movingTowardsRight = true;
    bool stop = false;
    int playerMoney;
    int originalBetAmount;
    int betAmount;

    /**
     * @brief Provides the correct image path for the given suit.
     * @param suit An int representing a card's suite.
     * 0 = clubs, 1 = spades, 2 = hearts, 3 = diamonds
     * @return the stylesheet string that is the image associated with the suit
     */
    QString decideCardBackgroundImage(int suit);

    /**
     * @brief startRound Calls a reset of the UI and lets the model
     * know another round is starting
     */
    void startRound();

    /**
     * @brief resetUI All the stulesheet settings that set the UI back to the
     * beginning of gameplay
     */
    void resetUI();

    /**
     * @brief all the stylesheet setting that sets the player UI back to the beginning of gameplay.
    */
    void resetPlayerUI();

    /**
     * @brief startAnotherRound Displays a question message box asking if user wants to start another
     * round
     */
    void startAnotherRound();

    /**
     * @brief gameOverFlash Displays the 2D Physics game over animation to create a flash
     */
    void gameOverFlash();

    /**
     * @brief gameOverLabelShow Hides the 2D Physics game over animation
     */
    void gameOverLabelShow();

    /**
     * @brief paintEvent Update the paint event for drawing
     * @param pe The paint event the method is called on
     */
    void paintEvent(QPaintEvent* pe);


signals:
    /**
     * @brief startGameDealSignals start of a deal
     */
    void startGameDealSignal();

    /**
     * @brief resetRoundSignals another round
     */
    void resetRoundSignal();

private slots:

    /**
     * @brief Set the text of the card with it's value, and the background image to its suit,
     * and then display the card on the cardLabel in the UI
     * @param card The provided card from the deck, dealt to the house or player
     * @param cardLabel the label name of current card
     */
    void showCardSlot(Card card, QString cardLabel);

    /**
     * @brief Display who is winner after each game play is done
     * @param whosWinner is to check who is win in the game
     */
    void winLoseDisplaySlot(int whosWinner, int wins, int rounds);

    /**
     * @brief showMoveAdviceSlot displays the advice from the model to the UI
     * @param advice The feedback on wether the user followed MIT's perfect strategy,
     * and the method that should've been taken
     */
    void showMoveAdviceSlot(QString advice);

    /**
     * @brief on_dificulltyCheckBox_stateChanged When the PhaseII is clicked,
     * increase the difficulty by allowing split and double down to be seen
     * @param arg1 The state of the check box
     */
    void on_dificulltyCheckBox_stateChanged(int arg1);

    /**
     * @brief Enables or Disables the hit button
     * @param state A bool to setEnabled to
     */
    void hitButtonStateSlot(bool);

    /**
     * @brief Enables or Disables the stand button
     * @param state A bool to setEnabled to
     */
    void standButtonStateSlot(bool);

    /**
     * @brief Enables or Disables the split button
     * @param state A bool to setEnabled to
     */
    void splitButtonStateSlot(bool state);

    /**
     * @brief When start is clicked, move to the game screen and
     * set initialize the first round
     */
    void on_StartButton_clicked();

    /**
     * @brief When rule is clicked, move to the rule screen and
     * can see the explanation of the game rule.
     */
    void on_RuleButton_clicked();

    /**
     * @brief When deal is clicked, send the start game deal signal to model,
     * disable deal button and enable hit/stand buttons
     */
    void on_dealButton_clicked();

    /**
     * @brief pop up window, player can place their bet(between 1 and their monetary amt)
    */
    void on_betButton_clicked();

    /**
     * @brief Go back to the game in it's previous state
     */
    void on_BackToGameButton_clicked();

    /**
     * @brief When next button is clicked, move to the ruleDetail page and
     * can see the detail explanation of the game rule.
     */
    void on_NextButton_clicked();

    /**
     * @brief When previous button is clicked, move to the first rule page and
     * can see the explanation of the game rule.
     */
    void on_PreviousButton_clicked();

    /**
     * @brief onTimeout For the 2D Physics display of blackjack animation label
     * to move across the screen
     */
    void onTimeout();

    /**
     * @brief on_showValuePlotButton_clicked Show the expectations plot in a new window
     */
    void on_showValuePlotButton_clicked();

    /**
     * @brief on_doubleDownButton_clicked double the bet amount
     */
    void on_doubleDownButton_clicked();

    /**
     * @brief updateSplitSlot display the current deck index for the split deck
    */
    void updateSplitSlot(int);


};
#endif // MAINWINDOW_H
