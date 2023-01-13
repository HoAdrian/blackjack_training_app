#include "card.h"

Card::Card(int value, int suit):
    value(value),
    suit(suit)
{
}


int Card::getValue(){
    if (value > 10)
        return 10;
    return value;
}

int Card::getSuit()
{
    return suit;
}

QString Card::getCardValue(){
    switch(value) {
    case 1:
        return "A";
        break;
    case 11:
        return "J";
        break;
    case 12:
        return "Q";
        break;
    case 13:
        return "K";
        break;
    default:
        return QString::number(value);
    }
}
