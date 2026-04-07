#include "ItemScene.h"
#include "InputValidator.h"
#include <iostream>

using namespace std;

ItemScene::ItemScene(int id, string title, string description,
                     bool isEnding, bool isGameOver, vector<Item> items, int gameOverSceneID)
    : Scene(id, title, description, "item", isEnding, isGameOver, gameOverSceneID),
      choiceItems(items) {}

void ItemScene::play(Player& player) {
    displayDivider();
    cout << "   " << title << endl;
    displayDivider();
    cout << endl;
    displayDescription(player);
    cout << endl;

    cout << string(60, '-') << endl;
    player.displayStats();
    cout << string(60, '-') << endl;
    cout << endl;

    
    for (size_t i = 0; i < choiceItems.size(); ++i) {
        const Item& item = choiceItems[i];
        cout << "[" << (i + 1) << "] " << item.getName() << endl;
        cout << "    " << item.getDescription() << endl;
    }
    cout << endl;

    cout << "Choose [1] or [2]: ";
    int choice = InputValidator::getItemChoice(static_cast<int>(choiceItems.size()));

    const Item& chosen = choiceItems[choice - 1];

    
    Item chosenCopy = chosen;
    chosenCopy.applyEffect(player);
    player.addItem(chosenCopy);

    cout << endl << "Item acquired: " << chosen.getName() << "." << endl;
    if (chosen.getEffectType() == "attackBoost") {
        cout << "Attack power increased by " << chosen.getEffectValue() << "." << endl;
    } else if (chosen.getEffectType() == "healthBoost") {
        cout << "Health restored by " << chosen.getEffectValue() << "." << endl;
    } else if (chosen.getEffectType() == "puzzleHint") {
        cout << "This item will reveal clues in puzzle scenarios." << endl;
    }
    cout << endl;
}

bool ItemScene::autoAdvance() const { return true; }
