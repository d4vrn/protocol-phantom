#pragma once

#include <string>

class Enemy {
private:
    int enemyID;
    std::string name;
    int health;
    int attackPower;
    int scoreReward;
    std::string description;

public:
    Enemy(int id, std::string name, int health, int attackPower,
          int scoreReward, std::string description);

    
    std::string getName() const;
    int getHealth() const;
    int getAttackPower() const;
    int getScoreReward() const;
    std::string getDescription() const;
    int getEnemyID() const;
};
