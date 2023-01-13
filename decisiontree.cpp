#include "decisiontree.h"

/**
 * @brief Initialized with pointers to the dealers hand and the players hand
 */
DecisionTree::DecisionTree(Hand* playerHand, Hand* houseHand, QObject *parent)
    : QObject{parent},
      playerHand_ (playerHand),
      houseHand_ (houseHand)
{
}

/**
 * @brief Determines correct decision in a given hardTotal game state
 * @return correct output advice associated with enum int value
 */
int DecisionTree::calculateHardTotalDecision(){
    //If first decision after split, always hit.
    if(playerHand_->getNumCards() == 1){
        return HIT;
    }
    //If we have only 2 cards and they are a pair, and the advise is to split,
    //return the split advice, otherwise return the normal advice
    if((isPair()) && (calculateSplit() < 6)){
        return calculateSplit();
    }

     int houseHandTopCardValue = houseHand_->getCard(0).getValue();
     int hardTotalSum = playerHand_->getResultantValue();
    switch(houseHandTopCardValue) {
    case 2:
        if(hardTotalSum >= 13) {
            return STAND;
        }
        else if(hardTotalSum == 12 || hardTotalSum < 10 ) {
            return HIT;
        }
        else {
            return DOUBLEHIT;
        }
        break;
    case 3:
        if(hardTotalSum >= 13) {
            return STAND;
        }
        else if(hardTotalSum == 12 || hardTotalSum < 9 ) {
            return HIT;
        }
        else {
            return DOUBLEHIT;
        }
        break;
    case 4:
    case 5:
    case 6:
        if(hardTotalSum >= 12) {
            return STAND;
        }
        else if(hardTotalSum < 9) {
            return HIT;
        }
        else {
            return DOUBLEHIT;
        }
        break;
    case 7:
    case 8:
    case 9:
        if(hardTotalSum > 16) {
            return STAND;
        }
        else if(hardTotalSum == 11 || hardTotalSum == 10) {
            return DOUBLEHIT;
        }
        else {
            return HIT;
        }
        break;
    case 10:
    case 1:
        if(hardTotalSum > 16) {
            return STAND;
        }
        else if(hardTotalSum == 11) {
            return DOUBLEHIT;
        }
        else {
            return HIT;
        }
        break;
    }

    //Will never hit this line assuming inputs are correct
    return HIT;
}

/**
 * @brief Determines correct decision in a given soft total game state.
 * @return correct output advice associated with enum int value.
 */
int DecisionTree::calculateSoftTotalDecision(){
    //If first decision after split, always hit.
    if(playerHand_->getNumCards() == 1){
        return HIT;
    }
    //If we have only 2 cards and they are a pair, and the advise is to split,
    //return the split advice, otherwise return the normal advice
    if((isPair()) && (calculateSplit() < 6)){
        return calculateSplit();
    }
    int decisionCard = playerHand_->getDecisionCard();
    //dealers 0th index card value
    int houseHandTopCardValue = houseHand_->getCard(0).getValue();
    switch(houseHandTopCardValue) {
    case 2:
        if(decisionCard >= 8) {
            return STAND;
        }
        else if (decisionCard == 7) {
            return DOUBLESTAY;
        }
        else {
            return HIT;
        }
        break;
    case 3:
        if(decisionCard >= 8) {
            return STAND;
        }
        else if(decisionCard == 7) {
            return DOUBLESTAY;
        }
        else if(decisionCard == 6) {
            return DOUBLEHIT;
        }
        else {
            return HIT;
        }
        break;
    case 4:
        if(decisionCard >= 8) {
            return STAND;
        }
        else if(decisionCard == 7) {
            return DOUBLESTAY;
        }
        else if(decisionCard < 4) {
            return HIT;
        }
        else {
            return DOUBLEHIT;
        }
        break;
    case 5:
        if(decisionCard >= 8) {
            return STAND;
        }
        else if(decisionCard == 7) {
            return DOUBLESTAY;
        }
        else {
            return DOUBLEHIT;
        }
        break;
    case 6:
        if(decisionCard >= 9) {
            return STAND;
        }
        else if(decisionCard == 7 || decisionCard == 8) {
            return DOUBLESTAY;
        }
        else {
            return DOUBLEHIT;
        }
        break;
    case 7:
    case 8:
        if(decisionCard >= 7) {
            return STAND;
        }
        else {
            return HIT;
        }
        break;
    case 9:
    case 10:
    case 1:
        if(decisionCard >= 8) {
            return STAND;
        }
        else {
            return HIT;
        }
        break;
    }

    //Will never hit this line assuming inputs are correct
    return HIT;
}


/**
 * @brief Determines if cards in playersHand is a pair. Must be only 2 cards
 * and their values have to be identical.
 * @return bool value T if is a pair, F otherwise
 */
bool DecisionTree::isPair(){
    //Can only be a pair if it is 2 cards and they are the same
    if((playerHand_->getNumCards() == 2) && (playerHand_->getCard(0).getValue() == playerHand_->getCard(1).getValue())){
        return true;
    }
    else{
        return false;
    }
}


/**
 * @brief Determines correct decision if player hand has a pir
 * @return correct output advice associated with enum int value.
 */
int DecisionTree::calculateSplit(){
    int splitCardVal = playerHand_->getCard(0).getValue();
    int houseHandTopCardValue = houseHand_->getCard(0).getValue();

    switch(houseHandTopCardValue){
    case 2:
        if(splitCardVal == 10 || splitCardVal == 4 || splitCardVal == 5){
            return DONTSPLIT;
        }
        else if(splitCardVal == 2 || splitCardVal == 3 || splitCardVal == 6){
            return SPLITANDDOUBLE;
        }
        else{
            return SPLIT;
        }
        break;
    case 3:
        if(splitCardVal == 10 || splitCardVal == 4 || splitCardVal == 5){
            return DONTSPLIT;
        }
        else if(splitCardVal == 2 || splitCardVal == 3){
            return SPLITANDDOUBLE;
        }
        else{
            return SPLIT;
        }
        break;
    case 4:
        if(splitCardVal == 10 || splitCardVal == 4 || splitCardVal == 5){
            return DONTSPLIT;
        }
        else{
            return SPLIT;
        }
        break;
    case 5:
    case 6:
        if(splitCardVal == 10 || splitCardVal == 5){
            return DONTSPLIT;
        }
        else if(splitCardVal == 4){
            return SPLITANDDOUBLE;
        }
        else{
            return SPLIT;
        }
        break;
    case 7:
        if(splitCardVal == 4 || splitCardVal == 5 || splitCardVal == 6 || splitCardVal == 9 || splitCardVal == 10){
            return DONTSPLIT;
        }
        else{
            return SPLIT;
        }
        break;
    case 8:
    case 9:
        if(splitCardVal == 1 || splitCardVal == 8 || splitCardVal == 9){
            return SPLIT;
        }
        else{
            return DONTSPLIT;
        }
        break;
    case 10:
    case 1:
        if(splitCardVal == 1 || splitCardVal == 8){
            return SPLIT;
        }
        else{
            return DONTSPLIT;
        }
        break;
    }

    //Will never hit this line assuming inputs are correct
    return DONTSPLIT;
}
