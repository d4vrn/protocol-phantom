#include "Inventory.h"
#include <iostream>
#include <sstream>

using namespace std;

void Inventory::addItem(const Item& item) {
    items.push_back(item);
}

void Inventory::removeItem(const string& name) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (it->getName() == name) {
            items.erase(it);
            return;
        }
    }
}

bool Inventory::hasItem(const string& name) const {
    for (const auto& item : items) {
        if (item.getName() == name) return true;
    }
    return false;
}

bool Inventory::hasPuzzleHintItem() const {
    for (const auto& item : items) {
        if (item.isPuzzleHint()) return true;
    }
    return false;
}

string Inventory::getHintItemName() const {
    for (const auto& item : items) {
        if (item.isPuzzleHint()) return item.getName();
    }
    return "";
}

void Inventory::displayInventory() const {
    if (items.empty()) {
        cout << "  (empty)" << endl;
        return;
    }
    for (const auto& item : items) {
        cout << "  - " << item.getName() << " [" << item.getType() << "]" << endl;
    }
}

const vector<Item>& Inventory::getItems() const {
    return items;
}

string Inventory::serialize() const {
    string result;
    for (size_t i = 0; i < items.size(); ++i) {
        if (i > 0) result += ";";
        result += items[i].serialize();
    }
    return result;
}

void Inventory::deserialize(const string& data, const map<int, Item>& itemMap) {
    items.clear();
    if (data.empty()) return;
    stringstream ss(data);
    string token;
    while (getline(ss, token, ';')) {
        try {
            int id = stoi(token);
            auto it = itemMap.find(id);
            if (it != itemMap.end()) {
                items.push_back(it->second);
            }
        } catch (...) {
            
        }
    }
}
