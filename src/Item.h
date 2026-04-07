#pragma once

#include <string>

class Player;

class Item {
private:
    int itemID;
    std::string name;
    std::string type;        
    std::string effectType;  
    int effectValue;
    std::string description;

public:
    Item(int id, std::string name, std::string type,
         std::string effectType, int effectValue, std::string description);

    
    void applyEffect(Player& player);

    
    bool isPuzzleHint() const;

    
    std::string getName() const;
    std::string getType() const;
    std::string getEffectType() const;
    int getEffectValue() const;
    int getItemID() const;
    std::string getDescription() const;

    
    std::string serialize() const;
};
