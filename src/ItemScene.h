#pragma once

#include "Scene.h"
#include "Item.h"
#include <vector>

class ItemScene : public Scene {
private:
    std::vector<Item> choiceItems;  

public:
    ItemScene(int id, std::string title, std::string description,
              bool isEnding, bool isGameOver, std::vector<Item> items, int gameOverSceneID = 0);

    
    void play(Player& player) override;

    bool autoAdvance() const override;
};
