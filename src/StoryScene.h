#pragma once

#include "Scene.h"

class StoryScene : public Scene {
public:
    StoryScene(int id, std::string title, std::string description,
               bool isEnding, bool isGameOver, int gameOverSceneID = 0);

    
    void play(Player& player) override;
};
