#include "Enemy.h"

using namespace std;

Enemy::Enemy(int id, string name, int health, int attackPower,
             int scoreReward, string description)
    : enemyID(id), name(name), health(health), attackPower(attackPower),
      scoreReward(scoreReward), description(description) {}

string Enemy::getName() const { return name; }
int Enemy::getHealth() const { return health; }
int Enemy::getAttackPower() const { return attackPower; }
int Enemy::getScoreReward() const { return scoreReward; }
string Enemy::getDescription() const { return description; }
int Enemy::getEnemyID() const { return enemyID; }
