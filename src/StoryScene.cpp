#include "StoryScene.h"
#include <iostream>

using namespace std;

StoryScene::StoryScene(int id, string title, string description,
                       bool isEnding, bool isGameOver, int gameOverSceneID)
    : Scene(id, title, description, "story", isEnding, isGameOver, gameOverSceneID) {}

void StoryScene::play(Player& player) {
    displayDivider();
    cout << "   " << title << endl;
    displayDivider();
    cout << endl;
    displayDescription(player);
    cout << endl;
}
