#ifndef CARDDECK_H
#define CARDDECK_H

#include <iostream>
#include<vector>
#include <algorithm>
#include <random>
#include "card.h"

using std::vector;


/**
 * Group: Kimochi
 * Class: CS3505
 * Date: November 18, 2022
 * Project: A8 Educational App
 *
 * Reviewers: Bella Miller
 *
 * @brief This class represents a deck of cards.
 */
class CardDeck
{
public:
    CardDeck();

    /**
     *  Gets a random card from the deck, and then removes it from the deck
     *  @return A Card with a value and a suit
    */
    Card getCard();


private:
    /** A vector representing the cards in the deck. */
    vector<Card> cards;

    /**
     * @brief CardDeck Builds a standard deck of cards with the cards shuffled randomly
     */
    void buildDeck();

    /**
     *  Randomizes the deck of cards.
     *  @ref https://stackoverflow.com/questions/6926433/how-to-shuffle-a-stdvector
    */
    void shuffleDeck();
};

#endif // CARDDECK_H
