#include "CSVLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

CSVLoader::CSVLoader(string dataPath) : dataPath(dataPath) {}

vector<string> CSVLoader::parseCSVLine(const string& line) {
    vector<string> fields;
    string field;
    bool inQuotes = false;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            field.erase(remove(field.begin(), field.end(), '\r'), field.end());
            fields.push_back(trim(field));
            field.clear();
        } else {
            field += c;
        }
    }
    field.erase(remove(field.begin(), field.end(), '\r'), field.end());
    fields.push_back(trim(field));
    return fields;
}

string CSVLoader::trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

map<int, Enemy> CSVLoader::loadEnemies() {
    map<int, Enemy> enemies;
    try {
        ifstream file(dataPath + "enemies.csv");
        if (!file.is_open()) { cerr << "Error: cannot open enemies.csv" << endl; return enemies; }
        string line;
        getline(file, line); 
        while (getline(file, line)) {
            if (line.empty()) continue;
            try {
                auto f = parseCSVLine(line);
                if (f.size() < 6) continue;
                int id = stoi(f[0]);
                enemies.emplace(id, Enemy(id, f[1], stoi(f[2]), stoi(f[3]), stoi(f[4]), f[5]));
            } catch (...) { cerr << "Warning: skipping malformed enemy row." << endl; }
        }
    } catch (...) { cerr << "Error loading enemies.csv" << endl; }
    return enemies;
}

map<int, Item> CSVLoader::loadItems() {
    map<int, Item> items;
    try {
        ifstream file(dataPath + "items.csv");
        if (!file.is_open()) { cerr << "Error: cannot open items.csv" << endl; return items; }
        string line;
        getline(file, line);
        while (getline(file, line)) {
            if (line.empty()) continue;
            try {
                auto f = parseCSVLine(line);
                if (f.size() < 6) continue;
                int id = stoi(f[0]);
                items.emplace(id, Item(id, f[1], f[2], f[3], stoi(f[4]), f[5]));
            } catch (...) { cerr << "Warning: skipping malformed item row." << endl; }
        }
    } catch (...) { cerr << "Error loading items.csv" << endl; }
    return items;
}

map<int, Puzzle> CSVLoader::loadPuzzles() {
    map<int, Puzzle> puzzles;
    try {
        ifstream file(dataPath + "puzzles.csv");
        if (!file.is_open()) { cerr << "Error: cannot open puzzles.csv" << endl; return puzzles; }
        string line;
        getline(file, line);
        while (getline(file, line)) {
            if (line.empty()) continue;
            try {
                auto f = parseCSVLine(line);
                if (f.size() < 6) continue;
                int id       = stoi(f[0]);
                string hint  = f[3];
                if (hint.empty()) {
                    cerr << "Warning: puzzle " << id << " has empty hint field." << endl;
                }
                puzzles.emplace(id, Puzzle(id, f[1], f[2], hint, stoi(f[4]), stoi(f[5])));
            } catch (...) { cerr << "Warning: skipping malformed puzzle row." << endl; }
        }
    } catch (...) { cerr << "Error loading puzzles.csv" << endl; }
    return puzzles;
}

map<int, vector<int>> CSVLoader::loadSceneItems() {
    map<int, vector<int>> sceneItems;
    try {
        ifstream file(dataPath + "choices.csv");
        if (!file.is_open()) return sceneItems;
        string line;
        getline(file, line);
        while (getline(file, line)) {
            if (line.empty()) continue;
            try {
                auto f = parseCSVLine(line);
                if (f.size() < 6) continue;
                int parentSceneID = stoi(f[1]);
                string resultType = f[4];
                int resultID      = stoi(f[5]);
                if (resultType == "item" && resultID > 0) {
                    sceneItems[parentSceneID].push_back(resultID);
                }
            } catch (...) {}
        }
    } catch (...) {}
    return sceneItems;
}

static map<int, int> buildSceneEnemyMap(const string& dataPath,
    function<vector<string>(const string&)> parse)
{
    map<int, int> result;
    ifstream file(dataPath + "choices.csv");
    if (!file.is_open()) return result;
    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (line.empty()) continue;
        try {
            auto f = parse(line);
            if (f.size() < 6) continue;
            string resultType = f[4];
            int resultID      = stoi(f[5]);
            if (resultType == "enemy" && resultID > 0) {
                
                int nextScene = stoi(f[3]);
                if (result.find(nextScene) == result.end()) {
                    result[nextScene] = resultID;
                }
            }
        } catch (...) {}
    }
    return result;
}

static map<int, int> buildScenePuzzleMap(const string& dataPath,
    function<vector<string>(const string&)> parse)
{
    map<int, int> result;
    ifstream file(dataPath + "choices.csv");
    if (!file.is_open()) return result;
    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (line.empty()) continue;
        try {
            auto f = parse(line);
            if (f.size() < 6) continue;
            string resultType = f[4];
            int resultID      = stoi(f[5]);
            if (resultType == "puzzle" && resultID > 0) {
                int nextScene = stoi(f[3]);
                if (result.find(nextScene) == result.end()) {
                    result[nextScene] = resultID;
                }
            }
        } catch (...) {}
    }
    return result;
}

map<int, vector<pair<string, int>>> CSVLoader::loadChoices() {
    map<int, vector<pair<string, int>>> choices;
    try {
        ifstream file(dataPath + "choices.csv");
        if (!file.is_open()) { cerr << "Error: cannot open choices.csv" << endl; return choices; }
        string line;
        getline(file, line);
        while (getline(file, line)) {
            if (line.empty()) continue;
            try {
                auto f = parseCSVLine(line);
                if (f.size() < 4) continue;
                int parentScene   = stoi(f[1]);
                string choiceText = f[2];
                int nextScene     = stoi(f[3]);
                choices[parentScene].emplace_back(choiceText, nextScene);
            } catch (...) { cerr << "Warning: skipping malformed choice row." << endl; }
        }
    } catch (...) { cerr << "Error loading choices.csv" << endl; }
    return choices;
}

map<int, Scene*> CSVLoader::loadScenes(
    const map<int, Enemy>& enemies,
    const map<int, Puzzle>& puzzles,
    const map<int, Item>& items,
    const map<int, vector<int>>& sceneItems)
{
    map<int, Scene*> scenes;

    
    auto parseFunc = [this](const string& line) { return parseCSVLine(line); };
    auto sceneEnemyMap  = buildSceneEnemyMap(dataPath, parseFunc);
    auto scenePuzzleMap = buildScenePuzzleMap(dataPath, parseFunc);

    
    
    static const map<int, int> gameOverMap = {
        {3,  103}, {4,  101}, {7,  102}, {8,  102},
        {11, 101}, {14, 103}, {15, 103}, {16, 104},
        {19, 105}, {20, 103}, {22, 106}, {23, 106},
        {26, 107}, {27, 107}, {29, 107}, {30, 108}
    };

    
    
    
    
    
    if (scenePuzzleMap.find(15) == scenePuzzleMap.end()) scenePuzzleMap[15] = 4;
    if (scenePuzzleMap.find(20) == scenePuzzleMap.end()) scenePuzzleMap[20] = 5;
    if (scenePuzzleMap.find(23) == scenePuzzleMap.end()) scenePuzzleMap[23] = 6;

    
    
    
    if (sceneEnemyMap.find(16) == sceneEnemyMap.end()) sceneEnemyMap[16] = 4; 
    if (sceneEnemyMap.find(22) == sceneEnemyMap.end()) sceneEnemyMap[22] = 6; 

    try {
        ifstream file(dataPath + "scenes.csv");
        if (!file.is_open()) { cerr << "Error: cannot open scenes.csv" << endl; return scenes; }
        string line;
        getline(file, line);
        while (getline(file, line)) {
            if (line.empty()) continue;
            try {
                auto f = parseCSVLine(line);
                if (f.size() < 6) continue;
                int id          = stoi(f[0]);
                string title    = f[1];
                string desc     = f[2];
                string type     = f[3];
                bool isEnd      = (f[4] == "1");
                bool isGO       = (f[5] == "1");

                Scene* scene = nullptr;

                if (type == "combat") {
                    
                    Enemy enemy = enemies.empty() ? Enemy(0,"Unknown",30,5,0,"") : enemies.begin()->second;
                    auto it = sceneEnemyMap.find(id);
                    if (it != sceneEnemyMap.end()) {
                        auto eit = enemies.find(it->second);
                        if (eit != enemies.end()) enemy = eit->second;
                    }
                    int goSceneID = 0;
                    auto goIt = gameOverMap.find(id);
                    if (goIt != gameOverMap.end()) goSceneID = goIt->second;
                    scene = new CombatScene(id, title, desc, isEnd, isGO, enemy, goSceneID);

                } else if (type == "puzzle") {
                    Puzzle puzzle(0, "", "", "", 0, 0);
                    auto it = scenePuzzleMap.find(id);
                    if (it != scenePuzzleMap.end()) {
                        auto pit = puzzles.find(it->second);
                        if (pit != puzzles.end()) puzzle = pit->second;
                    }
                    int goSceneID = 0;
                    auto goIt = gameOverMap.find(id);
                    if (goIt != gameOverMap.end()) goSceneID = goIt->second;
                    scene = new PuzzleScene(id, title, desc, isEnd, isGO, puzzle, goSceneID);

                } else if (type == "item") {
                    vector<Item> itemChoices;
                    auto it = sceneItems.find(id);
                    if (it != sceneItems.end()) {
                        for (int itemID : it->second) {
                            auto jt = items.find(itemID);
                            if (jt != items.end()) itemChoices.push_back(jt->second);
                        }
                    }
                    scene = new ItemScene(id, title, desc, isEnd, isGO, itemChoices);

                } else {
                    
                    scene = new StoryScene(id, title, desc, isEnd, isGO);
                }

                if (scene) scenes[id] = scene;

            } catch (...) { cerr << "Warning: skipping malformed scene row." << endl; }
        }
    } catch (...) { cerr << "Error loading scenes.csv" << endl; }
    return scenes;
}
