#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <QObject>
#include "hand.h"

/**
 * Group: Kimochi
 * Class: CS3505
 * Date: November 29, 2022
 * Project: A8 Educational App
 *
 * Reviewers: Mike Phelps
 *
 * Built to reflect MIT's perfect strategy blackjack model. Advises what player
 * should do in given game states.
 *
 * 1 represents Aces
 * 21 is not acconted for. If player gets 21 or blackjack no need to ask decision tree. They will stay.
 */
class DecisionTree : public QObject
{
    Q_OBJECT
public:
    //Constructor
    explicit DecisionTree(Hand*, Hand*, QObject *parent = nullptr);
    //Determines best choice in HardTotal game state
    int calculateHardTotalDecision();
    //Determines best choice in SoftTotal game state
    int calculateSoftTotalDecision();
    //Determines if 2 cards should be split
    int calculateSplit();
    //Checks if cards are a pair
    bool isPair();


private:
    //Pointer to vector of players possible hands
    Hand* playerHand_;
    //Pointer to house hand
    Hand* houseHand_;

//These enums represent the possible decision recommendations made
//by decision tree.
enum
{
    HIT = 0,
    STAND = 1,
    DOUBLEHIT = 2,
    DOUBLESTAY = 3,
    SPLIT = 4,
    SPLITANDDOUBLE = 5,
    DONTSPLIT = 6
};

};

#endif
