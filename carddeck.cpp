#include "carddeck.h"

CardDeck::CardDeck()
{
    buildDeck();
    shuffleDeck();
}


Card CardDeck::getCard()
{
    // Initialize seed randomly
    srand(time(0));

    // Random index to get card from
    int randomIndex = (rand() % (cards.size()));
    Card card = cards[randomIndex];

    cards.erase(cards.begin() + randomIndex);

    return card;
}


void CardDeck::buildDeck()
{
    //add 52 cards to the deck
    for(int suit = 0; suit < 4; suit++){
        //fills the cards with their actual value, A=1, Jack = 11
        for(int value = 1; value < 14; value++){
            cards.push_back(Card(value, suit));
        }
    }
}

void CardDeck::shuffleDeck()
{
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(cards), std::end(cards), rng);
}
