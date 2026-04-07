#include "FileHandler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <algorithm>

using namespace std;

FileHandler::FileHandler(string path) : savesFilePath(path) {}

void FileHandler::createSavesFileIfNotExists() {
    ifstream check(savesFilePath);
    if (!check.good()) {
        ofstream out(savesFilePath);
        if (out.is_open()) {
            out << "slotID,playerName,codename,sceneID,health,lives,attackPower,score,pendingEndingSceneID,inventory,lastPlayed,active" << endl;
        }
    }
}

bool FileHandler::hasSaves() {
    try {
        auto dossiers = loadAllDossiers();
        return !dossiers.empty();
    } catch (...) {
        return false;
    }
}

vector<string> FileHandler::parseCSVLine(const string& line) {
    vector<string> fields;
    string field;
    bool inQuotes = false;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            field.erase(remove(field.begin(), field.end(), '\r'), field.end());
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    field.erase(remove(field.begin(), field.end(), '\r'), field.end());
    fields.push_back(field);
    return fields;
}

string FileHandler::escapeCSV(const string& field) {
    if (field.find(',') != string::npos || field.find('"') != string::npos) {
        return "\"" + field + "\"";
    }
    return field;
}

string FileHandler::getCurrentTimestamp() {
    auto now = chrono::system_clock::now();
    time_t t = chrono::system_clock::to_time_t(now);
    tm* tm_info = localtime(&t);
    ostringstream oss;
    oss << put_time(tm_info, "%Y-%m-%dT%H:%M:%S");
    return oss.str();
}

vector<Dossier> FileHandler::loadAllDossiers() {
    vector<Dossier> dossiers;
    try {
        ifstream file(savesFilePath);
        if (!file.is_open()) return dossiers;
        string line;
        getline(file, line); 
        while (getline(file, line)) {
            if (line.empty()) continue;
            try {
                auto fields = parseCSVLine(line);
                if (fields.size() < 12) continue;
                Dossier d;
                d.slotID                = stoi(fields[0]);
                d.playerName            = fields[1];
                d.codename              = fields[2];
                d.sceneID               = stoi(fields[3]);
                d.health                = stoi(fields[4]);
                d.lives                 = stoi(fields[5]);
                d.attackPower           = stoi(fields[6]);
                d.score                 = stoi(fields[7]);
                d.pendingEndingSceneID  = stoi(fields[8]);
                d.inventory             = fields[9];
                d.lastPlayed            = fields[10];
                d.active                = (fields[11] == "1");
                dossiers.push_back(d);
            } catch (...) {
                cerr << "Warning: skipping malformed save row." << endl;
            }
        }
    } catch (...) {
        cerr << "Error reading saves file." << endl;
    }
    return dossiers;
}

void FileHandler::writeAllDossiers(const vector<Dossier>& dossiers) {
    ofstream file(savesFilePath);
    if (!file.is_open()) {
        cerr << "Error: cannot write to saves file." << endl;
        return;
    }
    file << "slotID,playerName,codename,sceneID,health,lives,attackPower,score,pendingEndingSceneID,inventory,lastPlayed,active" << endl;
    for (const auto& d : dossiers) {
        file << d.slotID << ","
             << escapeCSV(d.playerName) << ","
             << escapeCSV(d.codename) << ","
             << d.sceneID << ","
             << d.health << ","
             << d.lives << ","
             << d.attackPower << ","
             << d.score << ","
             << d.pendingEndingSceneID << ","
             << escapeCSV(d.inventory) << ","
             << d.lastPlayed << ","
             << (d.active ? 1 : 0) << endl;
    }
}

void FileHandler::saveDossier(const Player& player, int pendingEndingSceneID) {
    try {
        createSavesFileIfNotExists();
        auto dossiers = loadAllDossiers();

        
        int nextID = 1;
        for (const auto& d : dossiers) {
            if (d.slotID >= nextID) nextID = d.slotID + 1;
        }

        Dossier d;
        d.slotID               = nextID;
        d.playerName           = player.getName();
        d.codename             = player.getCodename();
        d.sceneID              = player.getSceneID();
        d.health               = player.getHealth();
        d.lives                = player.getLives();
        d.attackPower          = player.getAttackPower();
        d.score                = player.getScore();
        d.pendingEndingSceneID = pendingEndingSceneID;
        d.inventory            = player.getInventory().serialize();
        d.lastPlayed           = getCurrentTimestamp();
        d.active               = false;

        dossiers.push_back(d);
        writeAllDossiers(dossiers);
    } catch (...) {
        cerr << "Error saving dossier." << endl;
    }
}

void FileHandler::setActiveTag(int slotID) {
    try {
        auto dossiers = loadAllDossiers();
        for (auto& d : dossiers) {
            d.active = (d.slotID == slotID);
        }
        writeAllDossiers(dossiers);
    } catch (...) {
        cerr << "Error setting active tag." << endl;
    }
}

Dossier FileHandler::getActiveDossier() {
    auto dossiers = loadAllDossiers();
    for (const auto& d : dossiers) {
        if (d.active) return d;
    }
    return getMostRecentDossier();
}

Dossier FileHandler::getMostRecentDossier() {
    auto dossiers = loadAllDossiers();
    if (dossiers.empty()) throw runtime_error("No saved dossiers found.");
    return *max_element(dossiers.begin(), dossiers.end(),
        [](const Dossier& a, const Dossier& b) {
            return a.lastPlayed < b.lastPlayed;
        });
}

void FileHandler::displayDossiers() {
    auto dossiers = loadAllDossiers();
    if (dossiers.empty()) {
        cout << "No saved dossiers." << endl;
        return;
    }
    for (size_t i = 0; i < dossiers.size(); ++i) {
        const auto& d = dossiers[i];
        cout << "[" << (i + 1) << "] Agent " << d.codename
             << " (" << d.playerName << ")"
             << " — Scene " << d.sceneID
             << " — " << d.lastPlayed
             << (d.active ? " [ACTIVE]" : "") << endl;
    }
}
