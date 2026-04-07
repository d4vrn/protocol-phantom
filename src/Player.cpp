#include "Player.h"
#include <iostream>

using namespace std;

Player::Player()
    : name("Unknown"), codename("000"),
      health(DEFAULT_HEALTH), lives(DEFAULT_LIVES),
      attackPower(DEFAULT_ATTACK), score(0), currentSceneID(1) {}

Player::Player(string name, string codename)
    : name(name), codename(codename),
      health(DEFAULT_HEALTH), lives(DEFAULT_LIVES),
      attackPower(DEFAULT_ATTACK), score(0), currentSceneID(1) {}

void Player::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

void Player::heal(int amount) {
    health = min(health + amount, 150);
}

void Player::addAttackPower(int amount) {
    attackPower = min(attackPower + amount, 30);
}

void Player::loseLife() {
    lives--;
    health = DEFAULT_HEALTH;
}

void Player::addScore(int points) {
    score += points;
}

bool Player::isAlive() const {
    return health > 0;
}

bool Player::hasLives() const {
    return lives > 0;
}

void Player::displayStats() const {
    cout << "Agent " << codename
         << " | Health: " << health
         << " | Lives: " << lives
         << " | Attack: " << attackPower
         << " | Score: " << score << endl;
}

Inventory& Player::getInventory() {
    return inventory;
}

const Inventory& Player::getInventory() const {
    return inventory;
}

void Player::addItem(const Item& item) {
    inventory.addItem(item);
}

bool Player::hasItem(const string& name) const {
    return inventory.hasItem(name);
}

bool Player::hasPuzzleHintItem() const {
    return inventory.hasPuzzleHintItem();
}

string Player::getHintItemName() const {
    return inventory.getHintItemName();
}

string Player::getName() const { return name; }
string Player::getCodename() const { return codename; }
int Player::getHealth() const { return health; }
int Player::getLives() const { return lives; }
int Player::getAttackPower() const { return attackPower; }
int Player::getScore() const { return score; }
int Player::getSceneID() const { return currentSceneID; }

void Player::setHealth(int h) { health = h; }
void Player::setLives(int l) { lives = l; }
void Player::setAttackPower(int a) { attackPower = a; }
void Player::setScore(int s) { score = s; }
void Player::setSceneID(int id) { currentSceneID = id; }
