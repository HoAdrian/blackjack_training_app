
#ifndef DIRECTEVALUATION_H
#define DIRECTEVALUATION_H
#include <iostream>
#include <map>
#include <vector>

/**
 * Group: Kimochi
 * Class: CS3505
 * Date: November 20, 2022
 * Project: A8 Educational App
 *
 * Reviewers: Shing Hei Ho (Adrian)
 *
 * @brief This class is for learning the likelihood of winning the blackjack game given you are
 * following your current strategy. Each state of the game is define to be a vector of size 3,
 * where vector[0] is the player hand's value excluding ace, vector[1] is the card value of the dealer's revealed
 * card, vector[2] is the number of aces of the player (0 or 1). At each state, the player does some action, and then
 * receives a reward. If player wins, reward=+1; if player loses, reward=-1; otherwise, reward=0
 */

class DirectEvaluation
{
    public:
        /*empty constructor*/
        DirectEvaluation();

        /*update values_ and returns_ when an episonde ends*/
        void updateValueEpsiodeEnd();

        /*add a state-reward tuple defined by the parameters to the state-reward trajectory*/
        void appendStateReward(int playerCardValueExcludeAce, int dealerCard, int numAce, float reward);

        /*returns the map of state values*/
        std::map<std::vector<int>, float> getStateValues();

    private:
        //discount factor of Markov Decision Process
        float gamma_ = 0.8;

        //a vector of state and reward tuples of one round of game
        std::vector< std::tuple< std::vector<int>, float > > stateRewardTraj_;

        //map from state to a list of returns from each the game history
        std::map<std::vector<int>, std::vector<float>> stateReturns_;

        //map from state to the average returns computed from the entire the game history
        std::map<std::vector<int>, float> stateValues_;
};

#endif // DIRECTEVALUATION_H
