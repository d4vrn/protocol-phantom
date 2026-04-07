#include "Scene.h"
#include "Display.h"
#include <iostream>
#include <sstream>

using namespace std;

Scene::Scene(int id, string title, string description,
             string type, bool isEnding, bool isGameOver, int gameOverSceneID)
    : sceneID(id), title(title), description(description),
      sceneType(type), isEnding(isEnding), isGameOver(isGameOver),
      gameOverSceneID(gameOverSceneID) {}

bool Scene::wasSuccessful() const { return true; }
bool Scene::autoAdvance() const { return false; }
int Scene::getGameOverSceneID() const { return gameOverSceneID; }

string Scene::processText(const string& text, const Player& player) const {
    string result = text;
    
    size_t pos = 0;
    const string nameToken = "[playerName]";
    while ((pos = result.find(nameToken, pos)) != string::npos) {
        result.replace(pos, nameToken.size(), player.getName());
        pos += player.getName().size();
    }
    
    pos = 0;
    const string codeToken = "[codename]";
    while ((pos = result.find(codeToken, pos)) != string::npos) {
        result.replace(pos, codeToken.size(), player.getCodename());
        pos += player.getCodename().size();
    }
    return result;
}

string Scene::wordWrap(const string& text, int lineWidth) const {
    string result;
    istringstream words(text);
    string word;
    int currentLen = 0;

    while (words >> word) {
        if (currentLen == 0) {
            result += word;
            currentLen = word.size();
        } else if (currentLen + 1 + (int)word.size() <= lineWidth) {
            result += ' ';
            result += word;
            currentLen += 1 + word.size();
        } else {
            result += '\n';
            result += word;
            currentLen = word.size();
        }
    }
    return result;
}

void Scene::displayDescription(const Player& player) const {
    Display::printWrapped(processText(description, player), Display::SPEED_NORMAL, Display::WHITE);
}

void Scene::displayDivider() const {
    Display::divider('=');
}

int Scene::getSceneID() const { return sceneID; }
string Scene::getTitle() const { return title; }
string Scene::getSceneType() const { return sceneType; }
bool Scene::getIsEnding() const { return isEnding; }
bool Scene::getIsGameOver() const { return isGameOver; }
