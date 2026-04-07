#pragma once

#include "Player.h"
#include <string>
#include <vector>

struct Dossier {
    int slotID;
    std::string playerName;
    std::string codename;
    int sceneID;
    int health;
    int lives;
    int attackPower;
    int score;
    int pendingEndingSceneID;  
    std::string inventory;     
    std::string lastPlayed;    
    bool active;
};

class FileHandler {
private:
    std::string savesFilePath;

public:
    FileHandler(std::string path);

    
    void saveDossier(const Player& player, int pendingEndingSceneID);

    
    std::vector<Dossier> loadAllDossiers();

    
    void setActiveTag(int slotID);

    
    Dossier getActiveDossier();

    
    Dossier getMostRecentDossier();

    
    void displayDossiers();

    
    bool hasSaves();

    
    void createSavesFileIfNotExists();

private:
    
    std::string getCurrentTimestamp();

    
    std::vector<std::string> parseCSVLine(const std::string& line);

    
    std::string escapeCSV(const std::string& field);

    
    void writeAllDossiers(const std::vector<Dossier>& dossiers);
};
