#include "hand.h"

Hand::Hand()
{
}


/**
 * @brief used to get the number of cards in a hand.
 * @return int value reflecting the number of cards in the hand_ vector.
 */
int Hand::getNumCards(){
    return hand_.size();
}

/**
 * @brief Used to return the card object at a specific index in the hand vector.
 * @return Card object at index in hand vector.
 */
Card Hand::getCard(int index){
    return hand_[index];
}


/**
 * @brief Used to get the non-Ace value in a deck with 2 cards. Used by DecisionTree
 * for branch direction. This method cannot be called unless a hand_ has at least 2 values.
 * Currently it is not called anywhere in the code where there is less than 2 cards.
 * @return returns the int value of the decision card in hand.
 */
int Hand::getDecisionCard(){
    //If the first card is an ace
    if(hand_[0].getValue() == 1){
        //return the second card value
        return hand_[1].getValue();
    }
    //otherwise return the first card value
    else{
        return hand_[0].getValue();
    }
}

/**
 * @brief Adds a card to the hand. Takes in the card object you want
 * added to the hand.
 */
void Hand::addCard(Card card){
   hand_.push_back(card);
}

/**
 * @brief Removes the last card from a hand
 */
void Hand:: removeLastCard(){
    hand_.pop_back();
}


/**
 * @brief Returns the number of Aces in hand.
 * @return int number of aces in a hand.
 */
int Hand::getNumAces(){
    int numAce = 0;
    for(unsigned int i=0; i<hand_.size(); i++){
        Card card = hand_[i];
        if(card.getValue()==1){
            numAce+=1;
        }
    }

    return numAce;
}


/**
 * @brief Used to get the value of hand excluding the value of the aces
 * @return returns sum of all values minus the ace cards.
 */
int Hand::getValueExcludingAce(){
    int sum = 0;
    for(unsigned int i=0; i<hand_.size(); i++){
        Card card = hand_[i];
        if(card.getValue()!=1){
            sum+=card.getValue();
        }
    }

    return sum;
}

/**
 * @brief Ace can be 11 or 1, so player can potentially vary the value of his hand. This method gets
 * the optimal value as close to 21 as possible that is achievable with the current hand
 * @return the resultant value of the hand.
 */
int Hand::getResultantValue(){
    int numAce = getNumAces();
    int valueNoAce = getValueExcludingAce();

    if(valueNoAce+numAce>21){
        return valueNoAce+numAce;
    }

    if(numAce==0){
        return valueNoAce;
    }else if(numAce==1){
        int options[2] = {valueNoAce + 1, valueNoAce + 11};
        int bestValue=0;
        for(int i=0; i<2; i++){
            if(options[i]<=21 && options[i]>bestValue){
                bestValue = options[i];
            }
        }
        return bestValue;
    }else if(numAce==2){
        int options[2] = {valueNoAce + 2, valueNoAce + 12};
        int bestValue=0;
        for(int i=0; i<2; i++){
            if(options[i]<=21 && options[i]>bestValue){
                bestValue = options[i];
            }
        }
        return bestValue;
    }else if(numAce==3){
        int options[2] = {valueNoAce + 3, valueNoAce + 13};
        int bestValue=0;
        for(int i=0; i<2; i++){
            if(options[i]<=21 && options[i]>bestValue){
                bestValue = options[i];
            }
        }
        return bestValue;
    }else{
        //numAce==4
        int options[2] = {valueNoAce + 4, valueNoAce + 14};
        int bestValue=0;
        for(int i=0; i<2; i++){
            if(options[i]<=21 && options[i]>bestValue){
                bestValue = options[i];
            }
        }
        return bestValue;
    }
}
