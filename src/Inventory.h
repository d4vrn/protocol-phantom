#pragma once

#include "Item.h"
#include <vector>
#include <map>
#include <string>

class Inventory {
private:
    std::vector<Item> items;

public:
    
    void addItem(const Item& item);

    
    void removeItem(const std::string& name);

    
    bool hasItem(const std::string& name) const;

    
    bool hasPuzzleHintItem() const;

    
    std::string getHintItemName() const;

    
    void displayInventory() const;

    
    const std::vector<Item>& getItems() const;

    
    std::string serialize() const;

    
    void deserialize(const std::string& data, const std::map<int, Item>& itemMap);
};
