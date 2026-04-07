#pragma once

#include <string>

class Puzzle {
private:
    int puzzleID;
    std::string question;
    std::string answer;      
    std::string hint;
    int scoreReward;
    int rewardItemID;

public:
    Puzzle(int id, std::string question, std::string answer,
           std::string hint, int scoreReward, int rewardItemID);

    
    bool checkAnswer(const std::string& input) const;

    
    std::string getQuestion() const;
    std::string getHint() const;
    int getScoreReward() const;
    int getRewardItemID() const;
    int getPuzzleID() const;
};
