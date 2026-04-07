#include "Item.h"
#include "Player.h"

using namespace std;

Item::Item(int id, string name, string type,
           string effectType, int effectValue, string description)
    : itemID(id), name(name), type(type),
      effectType(effectType), effectValue(effectValue), description(description) {}

void Item::applyEffect(Player& player) {
    if (effectType == "attackBoost") {
        player.addAttackPower(effectValue);
    } else if (effectType == "healthBoost") {
        player.heal(effectValue);
    }
    
}

bool Item::isPuzzleHint() const {
    return effectType == "puzzleHint";
}

string Item::getName() const { return name; }
string Item::getType() const { return type; }
string Item::getEffectType() const { return effectType; }
int Item::getEffectValue() const { return effectValue; }
int Item::getItemID() const { return itemID; }
string Item::getDescription() const { return description; }

string Item::serialize() const {
    return to_string(itemID);
}
