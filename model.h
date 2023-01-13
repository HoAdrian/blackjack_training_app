#ifndef MODEL_H
#define MODEL_H

#include "card.h"
#include "hand.h"
#include "directevaluation.h"
#include "carddeck.h"
#include "decisiontree.h"
#include <QObject>
#include <QTimer>

/**
 * Group: Kimochi
 * Class: CS3505
 * Date: November 18, 2022
 * Project: A8 Educational App
 *
 * Reviewers Bella Miller, Siyu Wang
 *
 * This class represents the model with all the logic functionality for the Blackjack educational game
 */

class Model : public QObject
{
    Q_OBJECT

public:
    /** Initializes Model */
    explicit Model(QObject *parent = nullptr);

private:
    CardDeck cards;
    Hand houseHand;
    Hand playerHand;
    std::vector<Hand> playerHands_;
    int numberOfPlayerHands;
    int currentPlayerHandIndex;
    int playerTotal;
    int houseTotal;
    DecisionTree decisionTree;
    QString playersLastChoice;
    DirectEvaluation evalAgent;
    int wins;
    int rounds;

    /**
     * @brief generates appropriate advice message
     * using the players last turn, and the MIT models
     * suggested move
     * @return string of advice to be displayed
     */
    QString generateAdvice(int);

    /**
     * @brief isPair checks to see if the first two dealt cards have the same value
     * @return a bool whether they are the same or not
     */
    bool isPair();

    /**
     * @brief get a card from the deck, add the card value to the player total,
     * emit a signal with the card to the view to display
     */
    void dealPlayerCard();

    /**
     * @brief get a card from the deck, add the card value to the house total,
     * emit a signal with the card to the view to display
     * @param hidden the dealer's second card is hidden until the player is
     * done hitting. This checks to make sure the dealer doesn't get
     * a blackjack (win for dealer unless player gets blackjack too)
     */
    void dealHouseCard(bool hidden);

    /**
     * @brief isFinalHand Checks if the player is acting on the final hand. If so calls the end of
     * round logic, otherwise moves on to the next hand after storing the current hand in the
     * list of player hands
     */
    void isFinalHand();

    /**
     * @brief When round is over(player reached 21 or stood) emit
     * a signal saying number of wins.
     */
    void endRoundLogic();

    /**
     * @brief CalculateNumWins Go through all the playerhands and compare the totals
     * to the house total and then add up the wins and losses
     * @return The number of wins. Negative if more losses than wins
     */
    int CalculateNumWins();

    /**
     * @brief append a pair of game state and reward to the trajectory of the evaluation agent
     * @param reward an int representing a win/loss
     */
    void updateEvalAgent(int reward);

public: signals:
    void cardSignal(Card card, QString cardLabel);
    void moveFeedbackSignal(QString advice);
    void whoWinsSignal(int winLoss, int wins, int rounds);
    void standButtonSignal(bool);
    void hitButtonSignal(bool);
    void splitButtonSignal(bool);
    void resetPlayerUISignal();
    void plotValueMapSignal(std::map<std::vector<int>, float> valueMap);
    void updateSplitIndex(int);

public slots:
    /**
     * @brief Deal two cards to house and player, only one house card shows
     */
    void startGameDeal();

    /**
     * @brief resetRoundSlot Set up all the variables and initialize all information
     * to the beginning of a new round
     */
    void resetRoundSlot();

    /**
     * @brief deal a card to the player, if the playertotal
     * goes above 21 disable the hit button and call the playerstandslot
     */
    void playerHitSlot();

    /**
     * @brief Disable stand/hit button, and check if split, start end of round logic
     */
    void playerStandSlot();

    /**
     * @brief updateUIAdvice Compare the current hard and soft totals of the player and house, and compare to
     * the decision tree based on MIT's perfect blackjack strategy
     */
    void updateUIAdvice();

    /**
     * @brief Disable split button, add a hand with the first card to
     * playerHands, and a card with the second hand to playerHands. Reset the
     * UI to go with the first hand and continue gameplay
     */
    void playerSplitSlot();
};

#endif // MODEL_H
