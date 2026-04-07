#pragma once

#include "Player.h"
#include <string>

class Scene {
protected:
    int sceneID;
    std::string title;
    std::string description;
    std::string sceneType;
    bool isEnding;
    bool isGameOver;
    int gameOverSceneID;  

public:
    Scene(int id, std::string title, std::string description,
          std::string type, bool isEnding, bool isGameOver, int gameOverSceneID = 0);
    virtual ~Scene() = default;

    
    virtual void play(Player& player) = 0;

    
    
    virtual bool wasSuccessful() const;

    
    
    virtual bool autoAdvance() const;

    
    int getGameOverSceneID() const;

    
    int getSceneID() const;
    std::string getTitle() const;
    std::string getSceneType() const;
    bool getIsEnding() const;
    bool getIsGameOver() const;

protected:
    
    std::string processText(const std::string& text, const Player& player) const;

    
    void displayDescription(const Player& player) const;

    
    std::string wordWrap(const std::string& text, int lineWidth = 60) const;

    
    void displayDivider() const;
};
