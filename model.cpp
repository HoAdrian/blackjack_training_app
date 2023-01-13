#include "model.h"

Model::Model(QObject *parent)
    : QObject{parent} ,
      cards(CardDeck()),
      decisionTree(&playerHand, &houseHand),
      houseHand(Hand())
{
    //Add initial (empty) player hand to playerHands
    playerHands_.push_back(Hand());

    //set playerHand to the first hand in playerHandsVector
    playerHand = playerHands_.at(0);
    numberOfPlayerHands = 1;
    currentPlayerHandIndex = 0;
    wins = 0;
    rounds = 0;
}


void Model::updateEvalAgent(int reward){
    int playerValueNoAce = playerHand.getValueExcludingAce();
    int dealerCard = houseHand.getCard(0).getValue();
    int numAce = playerHand.getNumAces();
    evalAgent.appendStateReward(playerValueNoAce, dealerCard, numAce, reward);
}


void Model::startGameDeal()
{
    dealHouseCard(false);
    dealHouseCard(true);
    dealPlayerCard();
    dealPlayerCard();

    if(playerTotal == 21){
        //BLACKJACK!
        emit whoWinsSignal(100, wins, rounds);
    }

}


void Model::resetRoundSlot(){
    playerTotal = 0;
    houseTotal = 0;
    //clear hands vector. add new first hand, set player hand to first vector.
    playerHands_.clear();
    playerHands_.push_back(Hand());
    playerHand = playerHands_.at(0);
    numberOfPlayerHands = 1;
    currentPlayerHandIndex = 0;
    houseHand = Hand();
    cards = CardDeck();
}


void Model::dealPlayerCard()
{
    //get card from deck
    Card card = cards.getCard();
    std::cout<<"CARD BEING ADDED: "<< card.getValue()<<std::endl;
    playerHand.addCard(card);
    playerTotal = playerHand.getResultantValue();

    QString cardName = "playerCard";
    cardName.append(QString::number(playerHand.getNumCards()));

    emit cardSignal(card, cardName);

    std::cout<< "       playerTotal: "<<playerTotal<<std::endl;

    //if the card dealt makes it a pair of two identical cards
    //enable split.
    if(isPair()){
        emit splitButtonSignal(true);
    }

}


void Model::dealHouseCard(bool hidden)
{
    Card card = cards.getCard();
    houseHand.addCard(card);
    houseTotal = houseHand.getResultantValue();

    std::cout<< "       houseTotal: "<<houseTotal<<std::endl;

    if(!hidden){
        QString cardName = "houseCard";
        cardName.append(QString::number(houseHand.getNumCards()));
        emit cardSignal(card, cardName);
    }
}


void Model::isFinalHand(){
    if((currentPlayerHandIndex+1 == numberOfPlayerHands)){
        //if player is acting on final hand, safe to end round logic
        playerHands_[currentPlayerHandIndex] = playerHand;
        endRoundLogic();
    }
    else{
        //If other unevaluated hands move on to evaluate them
        playerHands_[currentPlayerHandIndex] = playerHand;
        currentPlayerHandIndex++;
        //set player's hand to their next hand
        playerHand = playerHands_[currentPlayerHandIndex];
        //set the UI to the firstCard and reset UI
        emit resetPlayerUISignal();
        emit cardSignal(playerHand.getCard(0), "playerCard1");
        //update split ui
        emit updateSplitIndex(currentPlayerHandIndex);
    }
}


void Model::playerHitSlot()
{
    //Used to write players move in advice message.
    playersLastChoice = "hit";
    //Update advice from MIT's perfect strategy
    updateUIAdvice();

    updateEvalAgent(0);

    dealPlayerCard();

    if (playerTotal >= 21) {
        //for split
        isFinalHand();
        //need to reset totals??
    }

}


void Model::playerStandSlot()
{
    //Used to write players move in advice message (for now).
    playersLastChoice = "stand";
    //Update advice from MIT's perfect strategy
    updateUIAdvice();

    //for split
    isFinalHand();

}


void Model::playerSplitSlot()
{
    //need to do something with num player hands/current player hand index?
    emit splitButtonSignal(false);

    //Used to write players move in advice message (for now).
    playersLastChoice = "Split";
    //Update advice from MIT's perfect strategy
    updateUIAdvice();

    //add last card from player hand to a newHand
    Hand newHand;
    newHand.addCard(playerHand.getCard(1));
    playerHand.removeLastCard();
    //add the new hand to the list of hands
    playerHands_.push_back(newHand);
    numberOfPlayerHands++;

    //set current hand index somewhere??
    //redraw
    emit resetPlayerUISignal();
    emit cardSignal(playerHand.getCard(0), "playerCard1");

    //update split ui
    emit updateSplitIndex(currentPlayerHandIndex);

}


void Model::updateUIAdvice(){
    QString advice;
    //Only calculate soft total decision on first decision, and if has an ace
    if(playerHand.getNumCards() <= 2 && playerHand.getNumAces() > 0){
        advice = generateAdvice(decisionTree.calculateSoftTotalDecision());
    }
    else{
        //Otherwise just calculate as hard total with enforced deck value
        advice = generateAdvice(decisionTree.calculateHardTotalDecision());
    }
    emit moveFeedbackSignal(advice);
}

bool Model::isPair(){

    if((playerHand.getNumCards() == 2) && (playerHand.getCard(0).getValue() == playerHand.getCard(1).getValue())){
        return true;
    }
    else{
        return false;
    }
}



void Model::endRoundLogic()
{
    emit standButtonSignal(false);
    emit hitButtonSignal(false);

    //If player Busts, house wins automatically
 //   if(playerTotal <= 21){
        // else need to show the hidden house card
        emit cardSignal(houseHand.getCard(1), "houseCard2");
        //house hits until total > 17
        while(houseTotal < 17){
            dealHouseCard(false);
        }
  //  }

    int numWins = CalculateNumWins();
    emit whoWinsSignal(numWins, wins, rounds);


    //Modify Expectations in graph based on previous wins/losses
    evalAgent.updateValueEpsiodeEnd();
    std::map<std::vector<int>, float> valMap = evalAgent.getStateValues();

    //notify the GUI to plot value map
    emit plotValueMapSignal(valMap);

    //print map
    for(const auto& e : valMap){
        std::cout<<"("<<e.first[0]<<", "<<e.first[1]<<", "<<e.first[2]<<")"<<":"<<e.second<<std::endl;
    }

}


int Model::CalculateNumWins(){
    int numWins = 0;
    rounds++;
    for(int i = 0; i < numberOfPlayerHands; i++){
        playerHand = playerHands_.at(i);
        int playerHandTotal = playerHand.getResultantValue();
        std::cout<<"!!ERL playerHandTotal: "<<playerHandTotal<<std::endl;
        std::cout<<"!!ERL houseTotal: "<<houseTotal<<std::endl;

        if(playerHandTotal > 21){
            updateEvalAgent(-1);
            numWins -= 1;
        }
        else if(houseTotal > 21){
            updateEvalAgent(1);
            numWins += 1;
            wins++;
        }
        else if(playerHandTotal == houseTotal){
            updateEvalAgent(0);
            //Don't add or subtract, its a tie.
        }
        else if(playerHandTotal > houseTotal){
            updateEvalAgent(1);
            numWins += 1;
            wins++;
        }
        else{
            updateEvalAgent(-1);
            //house must be higher than player
            numWins -= 1;
        }
    }

    return numWins;
}


QString Model::generateAdvice(int treeOutput){
    if(treeOutput == 0){
        return "should: HIT, did:  " + playersLastChoice;
    }
    else if(treeOutput == 1){
        return "should: STAND, did: " + playersLastChoice;
    }
    else if(treeOutput == 2){
        return "Double down if you can, otherwise hit, did: " + playersLastChoice;
    }
    else if(treeOutput == 3){
        return "Double down if you can, otherwise stay, did: " + playersLastChoice;
    }
    else if(treeOutput == 4){
        return "Split your cards, did: " + playersLastChoice;
    }
    else if(treeOutput == 5){
        return "Split your cards and double down, did: " + playersLastChoice;
    }
    else{
        return "STAY";
    }
}
