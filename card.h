#ifndef CARD_H
#define CARD_H

#include <QString>
/**
 * Group: Kimochi
 * Class: CS3505
 * Date: November 18, 2022
 * Project: A8 Educational App
 *
 * Reviewers: Yunsu Kwon
 *
 * @brief This class represents a single card.
 */
class Card
{
public:
    /**
     * Creates a card. J, Q, K values are 11-13 for the purpose of visualization.
     * @param value The card value 1 through 13. 1 = Ace, 11 = Jack, 12 = Queen, 13 = King
     * @param suit The card suit 0 through 3. 0 = Club, 1 = Spade, 2 = Heart, 3 = Diamond
    */
    Card(int value, int suit);
    ~Card() {}

    /**
     * @brief get numerical value of the card e.g. 1 is Ace. J, Q, K are 10.
     * @return the value
     */
    int getValue();

    /**
     * @brief get numerical value of the suit. 0 = Club, 1 = Spade, 2 = Heart, 3 = Diamond
     * @return the suit value
     */
    int getSuit();

    /**
     * @brief getCardValue returns the string representation of the card based
     * on it's value. This is the number or letter that would be seen on a
     * card in a regular playing deck
     * @return The representation of the card for display
     */
    QString getCardValue();

private:
    int value = 0;
    int suit = 0;

};

#endif // CARD_H
