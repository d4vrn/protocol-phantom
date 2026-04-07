#include "Puzzle.h"
#include <algorithm>
#include <cctype>

using namespace std;

Puzzle::Puzzle(int id, string question, string answer,
               string hint, int scoreReward, int rewardItemID)
    : puzzleID(id), question(question), hint(hint),
      scoreReward(scoreReward), rewardItemID(rewardItemID) {
    
    string lower = answer;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    this->answer = lower;
}

bool Puzzle::checkAnswer(const string& input) const {
    string normalised = input;
    string storedAnswer = answer;

    
    normalised.erase(0, normalised.find_first_not_of(" \t\r\n"));
    normalised.erase(normalised.find_last_not_of(" \t\r\n") + 1);

    
    storedAnswer.erase(0, storedAnswer.find_first_not_of(" \t\r\n"));
    storedAnswer.erase(storedAnswer.find_last_not_of(" \t\r\n") + 1);

    
    transform(normalised.begin(),    normalised.end(),    normalised.begin(),    ::tolower);
    transform(storedAnswer.begin(),  storedAnswer.end(),  storedAnswer.begin(),  ::tolower);

    return normalised == storedAnswer;
}

string Puzzle::getQuestion() const { return question; }
string Puzzle::getHint() const { return hint; }
int Puzzle::getScoreReward() const { return scoreReward; }
int Puzzle::getRewardItemID() const { return rewardItemID; }
int Puzzle::getPuzzleID() const { return puzzleID; }
