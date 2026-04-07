#pragma once

#include "Inventory.h"
#include <string>

constexpr int DEFAULT_HEALTH = 100;
constexpr int DEFAULT_LIVES  = 3;
constexpr int DEFAULT_ATTACK = 10;
constexpr int MAX_HEALTH     = 150;
constexpr int MAX_ATTACK     = 30;

class Player {
private:
    std::string name;
    std::string codename;
    int health;
    int lives;
    int attackPower;
    int score;
    int currentSceneID;
    Inventory inventory;

public:
    Player();
    Player(std::string name, std::string codename);

    
    void takeDamage(int amount);
    void heal(int amount);
    void addAttackPower(int amount);
    void loseLife();          
    void addScore(int points);

    
    bool isAlive() const;    
    bool hasLives() const;   

    
    void displayStats() const;

    
    Inventory& getInventory();
    const Inventory& getInventory() const;
    void addItem(const Item& item);
    bool hasItem(const std::string& name) const;
    bool hasPuzzleHintItem() const;
    std::string getHintItemName() const;

    
    std::string getName() const;
    std::string getCodename() const;
    int getHealth() const;
    int getLives() const;
    int getAttackPower() const;
    int getScore() const;
    int getSceneID() const;

    
    void setHealth(int h);
    void setLives(int l);
    void setAttackPower(int a);
    void setScore(int s);
    void setSceneID(int id);
};
