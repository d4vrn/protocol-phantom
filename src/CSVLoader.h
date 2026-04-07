#pragma once

#include "Scene.h"
#include "CombatScene.h"
#include "PuzzleScene.h"
#include "ItemScene.h"
#include "StoryScene.h"
#include "Enemy.h"
#include "Item.h"
#include "Puzzle.h"
#include <map>
#include <vector>
#include <string>

class CSVLoader {
private:
    std::string dataPath;

public:
    CSVLoader(std::string dataPath);

    
    std::map<int, Scene*> loadScenes(
        const std::map<int, Enemy>& enemies,
        const std::map<int, Puzzle>& puzzles,
        const std::map<int, Item>& items,
        const std::map<int, std::vector<int>>& sceneItems);

    
    std::map<int, std::vector<std::pair<std::string, int>>> loadChoices();

    
    std::map<int, Enemy> loadEnemies();

    
    std::map<int, Item> loadItems();

    
    std::map<int, Puzzle> loadPuzzles();

    
    std::map<int, std::vector<int>> loadSceneItems();

private:
    
    std::vector<std::string> parseCSVLine(const std::string& line);

    
    std::string trim(const std::string& s);
};
