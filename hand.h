#ifndef HAND_H
#define HAND_H

#include <iostream>
#include <vector>
#include "card.h"

/**
 * Group: Kimochi
 * Class: CS3505
 * Date: December 8, 2022
 * Project: A8 Educational App
 *
 * Reviewers: Mike Phelps
 *
 * This class represents one list of cards (hand of cards) for the dealer or player.
 * In the case of the player, hand(s) are held inside of a vector because a player
 * can hold mulitple hands at one time (in the case of a split).
 */
class Hand
{
public:
    /* empty constructor*/
    Hand();

    /**
     * @brief get number of cards in the hand
     * @return number of cards in the hand
     */
    int getNumCards();

    /**
     * @brief get the card at the specified index
     * @return the card at the specified index
     */
    Card getCard(int index);

    /**
     * @brief removes the last card in the hand
     */
    void removeLastCard();

    /**
     * @brief add card to the hand
     */
    void addCard(Card card);

    /**
     * @brief get number of aces in the hand
     * @return number of aces
     */
    int getNumAces();

    /**
     * @brief get the sum of card Values Excluding Ace
     * @return the sum of card values excluding ace (J,Q, K count as 10)
     */
    int getValueExcludingAce();

    /**
     * @brief get the decision card (non-ace) card from the inital hand
     * @return the decision card (non-ace card) value (J,Q, K count as 10)
     */
    int getDecisionCard();

    /**
     * @brief Ace can be 11 or 1, so player can potentially vary the value of his hand. This method gets
     * the optimal value as close to 21 as possible that is achievable with the current hand
     * @return the resultant value of the hand.
     */
    int getResultantValue();


private:
    //vector of Cards which represents a hand
    std::vector<Card> hand_;

};

#endif // HAND_H
