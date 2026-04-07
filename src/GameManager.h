#pragma once

#include "Player.h"
#include "Scene.h"
#include "StoryScene.h"
#include "FileHandler.h"
#include "CSVLoader.h"
#include <map>
#include <vector>
#include <string>
#include <random>

class GameManager {
private:
    Player player;
    std::map<int, Scene*> scenes;
    std::map<int, std::vector<std::pair<std::string, int>>> choices; 
    FileHandler fileHandler;
    CSVLoader csvLoader;
    bool isRunning;        
    bool gameLoop_active;  
    int currentSceneID;
    int pendingEndingSceneID;  
    bool hasSavedGames;
    std::mt19937 rng;          

public:
    GameManager();
    ~GameManager();  

    
    void run();

private:
    
    void showMainMenu();
    void newGame();
    void continueGame();
    void loadGame();
    void howToPlay();
    void showCredits();
    void exitGame();

    
    void gameLoop();
    void processScene(int sceneID);
    void presentChoices(int sceneID);
    int getPlayerChoice(int sceneID);

    
    void handleLivesCheck(int gameOverSceneID);
    void handleEnding(int endingSceneID);

    
    std::string generateCodename(const std::string& name);

    
    void saveGame();
    void loadPlayerFromDossier(const Dossier& d);

    
    void clearScreen();
    void waitForEnter();
    void printTitle();
    void showActTransition(int act);
    void showInventory();
    std::string toLower(const std::string& s);
    std::string trim(const std::string& s);
};
