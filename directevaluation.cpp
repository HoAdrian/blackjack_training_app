#include "directevaluation.h"

DirectEvaluation::DirectEvaluation()
{
    //empty
}

void DirectEvaluation::updateValueEpsiodeEnd(){
    float currentReturn = 0;
    // calculate each return (accumulated rewards) from each state
    for (int i=stateRewardTraj_.size()-1; i>=0; i--){
        std::tuple stateReward = stateRewardTraj_[i];

        std::vector<int> state = std::get<0>(stateReward);
        float reward = std::get<1>(stateReward);

        currentReturn = gamma_*currentReturn + reward;

        stateReturns_[state].push_back(currentReturn);
     }

     //update values
    for (const auto & [state,returns] : stateReturns_){
        float totalReturns = 0;
        int numReturns = returns.size();
        for(unsigned int i=0; i<returns.size(); i++){
            totalReturns+=returns[i];
        }

        //map the state to its average return from the expereicne episodes (game history)
        stateValues_[state] = totalReturns/numReturns;
    }

    //reset state-reward trajectory
    stateRewardTraj_.clear();
}

void DirectEvaluation::appendStateReward(int playerCardValueExcludeAce, int dealerCard, int numAce, float reward){
    std::vector<int> state {playerCardValueExcludeAce, dealerCard, numAce};
    std::tuple<std::vector<int>, float> stateRewardPair(state, reward);
    stateRewardTraj_.push_back(stateRewardPair);
}

std::map<std::vector<int>, float> DirectEvaluation::getStateValues(){
    return stateValues_;
}
