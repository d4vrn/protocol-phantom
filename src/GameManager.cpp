#include "GameManager.h"
#include "CombatScene.h"
#include "PuzzleScene.h"
#include "ItemScene.h"
#include "InputValidator.h"
#include "Display.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <limits>

using namespace std;

GameManager::GameManager()
    : fileHandler("../data/saves.csv"),
      csvLoader("../data/"),
      isRunning(false),
      gameLoop_active(false),
      currentSceneID(1),
      pendingEndingSceneID(33),
      hasSavedGames(false),
      rng(chrono::steady_clock::now().time_since_epoch().count())
{
    
    auto enemies    = csvLoader.loadEnemies();
    auto items      = csvLoader.loadItems();
    auto puzzles    = csvLoader.loadPuzzles();
    auto sceneItems = csvLoader.loadSceneItems();
    choices = csvLoader.loadChoices();
    scenes  = csvLoader.loadScenes(enemies, puzzles, items, sceneItems);

    fileHandler.createSavesFileIfNotExists();
    hasSavedGames = fileHandler.hasSaves();
}

GameManager::~GameManager() {
    for (auto& pair : scenes) {
        delete pair.second;
    }
}

void GameManager::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void GameManager::waitForEnter() {
    cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void GameManager::printTitle() {
    clearScreen();
    Display::instant("", Display::GREEN);
    Display::instant("╔════════════════════════════════════════════════════════════╗", Display::GREEN);
    Display::instant("║                                                            ║", Display::GREEN);
    Display::instant("║              [ P R O T O C O L  P H A N T O M ]            ║", Display::GREEN);
    Display::instant("║                                                            ║", Display::GREEN);
    Display::instant("║     CLASSIFICATION: TOP SECRET — KGB EYES ONLY             ║", Display::GREEN);
    Display::instant("║     ACCESS LEVEL:   GHOST OPERATIVE                        ║", Display::GREEN);
    Display::instant("║     STATUS:         MISSION ACTIVE                         ║", Display::GREEN);
    Display::instant("║                                                            ║", Display::GREEN);
    Display::instant("║     WARNING: UNAUTHORISED ACCESS IS PUNISHABLE             ║", Display::GREEN);
    Display::instant("║     BY DEATH UNDER ARTICLE 58 OF THE SOVIET                ║", Display::GREEN);
    Display::instant("║     CRIMINAL CODE.                                         ║", Display::GREEN);
    Display::instant("║                                                            ║", Display::GREEN);
    Display::instant("╚════════════════════════════════════════════════════════════╝", Display::GREEN);
    Display::instant("", Display::RESET);
}

void GameManager::showActTransition(int act) {
    static const string subtitles[] = {
        "THE MAKING OF A GHOST",
        "THE INFILTRATION",
        "UNRAVELLING THE SECRET",
        "PROTOCOL PHANTOM"
    };
    const string& sub = subtitles[act - 1];

    clearScreen();
    Display::divider('=');
    cout << endl;
    Display::println("ACT " + to_string(act) + " — " + sub, Display::SPEED_ACT, Display::YELLOW);
    cout << endl;
    Display::divider('=');
    cout << endl;
    waitForEnter();
}

void GameManager::showInventory() {
    const auto& items = player.getInventory().getItems();

    cout << endl;
    Display::instant("INVENTORY — AGENT " + player.getCodename(), Display::CYAN);
    Display::divider('=');

    if (items.empty()) {
        Display::instant("  (empty)", Display::CYAN);
    } else {
        int i = 1;
        for (const Item& item : items) {
            
            string typeStr;
            if      (item.getType() == "weapon")     typeStr = "Weapon";
            else if (item.getType() == "clothing")   typeStr = "Clothing";
            else if (item.getType() == "consumable") typeStr = "Consumable";
            else if (item.getType() == "key")        typeStr = "Key Item";
            else                                     typeStr = item.getType();

            
            string effectStr;
            if      (item.getEffectType() == "attackBoost")
                effectStr = "Attack +" + to_string(item.getEffectValue());
            else if (item.getEffectType() == "healthBoost")
                effectStr = "Health +" + to_string(item.getEffectValue());
            else if (item.getEffectType() == "puzzleHint")
                effectStr = "Reveals puzzle hints";

            string line = "[" + to_string(i++) + "] " + item.getName() + " — " + typeStr;
            if (!effectStr.empty()) line += " — " + effectStr;
            Display::instant(line, Display::CYAN);
        }
    }

    Display::divider('=');
    cout << endl;
}

string GameManager::toLower(const string& s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

string GameManager::trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

string GameManager::generateCodename(const string& name) {
    
    string upper = name;
    transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

    
    string digits;
    for (char c : upper) {
        switch (c) {
            case 'O': digits += '0'; break;
            case 'I': digits += '1'; break;
            case 'Z': digits += '2'; break;
            case 'E': digits += '3'; break;
            case 'A': digits += '4'; break;
            case 'S': digits += '5'; break;
            case 'T': digits += '7'; break;
            case 'B': digits += '8'; break;
            default: break;
        }
    }

    
    uniform_int_distribution<int> digitDist(0, 9);
    while (digits.size() < 3) {
        digits = to_string(digitDist(rng)) + digits;
    }

    
    reverse(digits.begin(), digits.end());

    return digits;
}

void GameManager::run() {
    isRunning = true;
    showMainMenu();
}

void GameManager::showMainMenu() {
    while (isRunning) {
        clearScreen();
        printTitle();
        cout << endl;
        cout << "A Cold War spy thriller. 1943. You are the only thing" << endl;
        cout << "standing between a railway cannon and the city of Stalinets." << endl;
        cout << endl;

        hasSavedGames = fileHandler.hasSaves();

        if (hasSavedGames) {
            Display::instant("1. Continue",    Display::CYAN);
            Display::instant("2. New Game",    Display::CYAN);
            Display::instant("3. Load Game",   Display::CYAN);
            Display::instant("4. How to Play", Display::CYAN);
            Display::instant("5. Credits",     Display::CYAN);
            Display::instant("6. Exit",        Display::CYAN);
        } else {
            Display::instant("1. New Game",    Display::CYAN);
            Display::instant("2. How to Play", Display::CYAN);
            Display::instant("3. Credits",     Display::CYAN);
            Display::instant("4. Exit",        Display::CYAN);
        }

        cout << endl;
        cout << Display::CYAN << "Your choice: " << Display::RESET;
        int choice = InputValidator::getMenuChoice(hasSavedGames ? 6 : 4);

        if (hasSavedGames) {
            if      (choice == 1) continueGame();
            else if (choice == 2) newGame();
            else if (choice == 3) loadGame();
            else if (choice == 4) howToPlay();
            else if (choice == 5) showCredits();
            else if (choice == 6) exitGame();
        } else {
            if      (choice == 1) newGame();
            else if (choice == 2) howToPlay();
            else if (choice == 3) showCredits();
            else if (choice == 4) exitGame();
        }
    }
}

void GameManager::newGame() {
    clearScreen();
    printTitle();
    cout << endl;

    cout << Display::CYAN << "Enter your name: " << Display::RESET;
    string name = InputValidator::getPlayerName();

    string codename = generateCodename(name);
    player = Player(name, codename);

    cout << endl;
    Display::instant("Welcome, " + name + ". Your codename is " + codename + ".", Display::CYAN);
    Display::instant("Your mission begins now.", Display::CYAN);
    cout << endl;
    waitForEnter();

    clearScreen();
    Display::divider('=');
    Display::instant("   FIELD BRIEFING — AGENT " + player.getCodename(), Display::CYAN);
    Display::divider('=');
    cout << endl;
    Display::instant("Before you deploy, your handler leaves you with this:", Display::CYAN);
    cout << endl;
    Display::println("  — Names matter. Pay attention to who is mentioned and where. Photographs too.", Display::SPEED_BRIEFING, Display::YELLOW);
    Display::println("  — Dates are never accidental. Officers are creatures of habit.", Display::SPEED_BRIEFING, Display::YELLOW);
    Display::println("  — Numbers on walls, stamps, documents — nothing is decoration.", Display::SPEED_BRIEFING, Display::YELLOW);
    Display::println("  — Some words are German. A spy who cannot translate is a dead spy.", Display::SPEED_BRIEFING, Display::YELLOW);
    Display::println("  — Your cover identity is memorised. Trust what you know.", Display::SPEED_BRIEFING, Display::YELLOW);
    Display::println("  — Key items in your inventory may reveal what your eyes miss. Type [inv] if you are stuck.", Display::SPEED_BRIEFING, Display::YELLOW);
    cout << endl;
    Display::instant("This briefing will not be repeated.", Display::CYAN);
    cout << endl;
    Display::instant("Press Enter to begin your mission.", Display::CYAN);
    waitForEnter();

    currentSceneID = 1;
    pendingEndingSceneID = 33;
    gameLoop();
}

void GameManager::continueGame() {
    if (!hasSavedGames) {
        clearScreen();
        printTitle();
        cout << endl;
        Display::instant("No saved progress found. Start a New Game first.", Display::CYAN);
        cout << endl;
        waitForEnter();
        return;
    }
    try {
        Dossier d = fileHandler.getActiveDossier();
        loadPlayerFromDossier(d);
        clearScreen();
        printTitle();
        cout << endl;
        Display::instant("Resuming Agent " + player.getCodename() + "'s progress...", Display::CYAN);
        cout << endl;
        waitForEnter();
        gameLoop();
    } catch (const exception& e) {
        cerr << "Error loading dossier: " << e.what() << endl;
        waitForEnter();
    }
}

void GameManager::loadGame() {
    clearScreen();
    printTitle();
    cout << endl;
    cout << "LOAD GAME" << endl;
    cout << string(60, '-') << endl;
    fileHandler.displayDossiers();
    cout << endl;
    cout << "Enter number to select, or press Enter to load latest: ";

    auto dossiers = fileHandler.loadAllDossiers();
    int slot = InputValidator::getSlotChoice(static_cast<int>(dossiers.size()));

    if (slot == -1) {
        try {
            Dossier d = fileHandler.getMostRecentDossier();
            loadPlayerFromDossier(d);
            Display::instant("Dossier loaded. Return to menu and select Continue to play.", Display::CYAN);
        } catch (...) {
            Display::instant("No dossiers found.", Display::CYAN);
        }
    } else {
        fileHandler.setActiveTag(dossiers[slot - 1].slotID);
        loadPlayerFromDossier(dossiers[slot - 1]);
        Display::instant("Dossier loaded. Return to menu and select Continue to play.", Display::CYAN);
    }
    cout << endl;
    waitForEnter();
}

void GameManager::howToPlay() {
    clearScreen();
    cout << string(60, '=') << endl;
    cout << "   PROTOCOL PHANTOM — HOW TO PLAY" << endl;
    cout << string(60, '=') << endl;
    cout << endl;
    cout << "You are a Soviet spy embedded in the German Wehrmacht." << endl;
    cout << "Navigate four acts of decisions, combat, and puzzles." << endl;
    cout << endl;
    cout << "CHOICES: At each scenario you will face two options." << endl;
    cout << "         Choose wisely." << endl;
    cout << endl;
    cout << "COMBAT:  Your attack power versus the enemy's — with a" << endl;
    cout << "         random element. Collect weapons to increase your" << endl;
    cout << "         attack power." << endl;
    cout << endl;
    cout << "PUZZLES: Each puzzle is different. You may face:" << endl;
    cout << "         — Questions answerable from the scene around you" << endl;
    cout << "         — Anagrams — unscramble the letters" << endl;
    cout << "         — Riddles — think laterally" << endl;
    cout << "         — Math — calculate from numbers in the scene" << endl;
    cout << endl;
    cout << "         Read everything carefully. The answer is always" << endl;
    cout << "         findable without prior knowledge." << endl;
    cout << endl;
    cout << "         Key items reveal hints. Type [inv] to check" << endl;
    cout << "         what you are carrying." << endl;
    cout << endl;
    cout << "ITEMS:   Weapons boost attack. Clothing and food restore" << endl;
    cout << "         health. Key items unlock hints and story paths." << endl;
    cout << endl;
    cout << "LIVES:   You have 3 lives. Lose them all and the mission" << endl;
    cout << "         fails." << endl;
    cout << endl;
    cout << "INV:     Type 'inv' at any choice prompt to view your" << endl;
    cout << "         current inventory." << endl;
    cout << endl;
    cout << "SAVE:    Type 'save' at any choice prompt to save your" << endl;
    cout << "         progress." << endl;
    cout << endl;
    cout << "EXIT:    Type 'exit' at any choice prompt to return to" << endl;
    cout << "         the main menu. You will be asked if you want" << endl;
    cout << "         to save first." << endl;
    cout << endl;
    waitForEnter();
}

void GameManager::showCredits() {
    clearScreen();
    printTitle();
    cout << endl;
    cout << "Developed for IY453 Software Design and Implementation" << endl;
    cout << endl;
    cout << "Inspired by:" << endl;
    cout << "  John le Carre — spy fiction and moral ambiguity" << endl;
    cout << "  The Schwerer Gustav — German WWII railway gun" << endl;
    cout << "  Cold War espionage history" << endl;
    cout << endl;
    waitForEnter();
}

void GameManager::exitGame() {
    cout << endl;
    cout << Display::CYAN << "Are you sure you want to exit? (y/n): " << Display::RESET;
    if (InputValidator::getYesNo()) {
        isRunning = false;
        Display::instant("Goodbye.", Display::CYAN);
    }
}

void GameManager::gameLoop() {
    gameLoop_active = true;
    while (gameLoop_active) {
        if (scenes.find(currentSceneID) == scenes.end()) {
            cerr << "Error: scene " << currentSceneID << " not found." << endl;
            return;
        }
        processScene(currentSceneID);
    }
}

void GameManager::processScene(int sceneID) {
    auto it = scenes.find(sceneID);
    if (it == scenes.end()) {
        cerr << "Error: scene " << sceneID << " not found." << endl;
        return;
    }
    Scene* scene = it->second;
    player.setSceneID(sceneID);
    currentSceneID = sceneID;

    
    if (sceneID == 1)  showActTransition(1);
    if (sceneID == 11) showActTransition(2);
    if (sceneID == 22) showActTransition(3);
    if (sceneID == 32) showActTransition(4);

    clearScreen();
    scene->play(player);

    
    if (scene->getIsEnding()) {
        handleEnding(sceneID);
        return;
    }

    
    if (scene->getIsGameOver()) {
        
        if (sceneID == 108) {
            processScene(35);
            return;
        }
        handleLivesCheck(sceneID);
        return;
    }

    
    if (!scene->wasSuccessful()) {
        handleLivesCheck(scene->getGameOverSceneID());
        return;
    }

    
    if (sceneID == 31) {
        
    }

    
    if (sceneID == 32) {
        processScene(pendingEndingSceneID);
        return;
    }

    
    if (sceneID == 15 && scene->wasSuccessful()) {
        currentSceneID = 16;
        return;
    }

    presentChoices(sceneID);
}

void GameManager::presentChoices(int sceneID) {
    auto it = choices.find(sceneID);

    
    if (it == choices.end() || it->second.empty()) {
        
        return;
    }

    const auto& sceneChoices = it->second;

    
    Display::divider('-');
    Display::instant("Agent " + player.getCodename() +
                     " | Health: " + to_string(player.getHealth()) +
                     " | Lives: "  + to_string(player.getLives()) +
                     " | Attack: " + to_string(player.getAttackPower()) +
                     " | Score: "  + to_string(player.getScore()), Display::CYAN);
    Display::divider('-');
    cout << endl;

    Scene* scene = scenes.at(sceneID);

    
    if (scene->autoAdvance()) {
        if (!sceneChoices.empty()) {
            currentSceneID = sceneChoices[0].second;
        }
        return;
    }

    
    if (sceneID == 6 && !player.hasItem("Officer's Greatcoat")) {
        Display::instant("Without officer's rank insignia you cannot use the fast-track lane.", Display::CYAN);
        Display::instant("You join the enlisted queue.", Display::CYAN);
        cout << endl;
        waitForEnter();
        currentSceneID = 8; 
        return;
    }

    
    auto printChoices = [&]() {
        for (size_t i = 0; i < sceneChoices.size(); ++i) {
            Display::instant("[" + to_string(i + 1) + "] " + sceneChoices[i].first, Display::CYAN);
        }
        Display::instant("[inv]  View inventory", Display::CYAN);
        Display::instant("[save] Save progress",  Display::CYAN);
        Display::instant("[exit] Exit to main menu", Display::CYAN);
        cout << endl;
    };

    
    printChoices();

    while (true) {
        cout << Display::CYAN << "Your choice: " << Display::RESET;
        string input;
        getline(cin, input);
        input = trim(toLower(input));

        if (input == "inv") {
            showInventory();
            continue;
        }

        if (input == "save") {
            saveGame();
            printChoices();
            continue;
        }

        if (input == "exit") {
            cout << endl;
            cout << Display::CYAN << "Save before exiting? (y/n): " << Display::RESET;
            if (InputValidator::getYesNo()) {
                saveGame();
            }
            Display::instant("Goodbye, Agent " + player.getCodename() + ".", Display::CYAN);
            gameLoop_active = false;
            return;
        }

        
        int choice = 0;
        try { choice = stoi(input); } catch (...) {}

        if (choice < 1 || choice > static_cast<int>(sceneChoices.size())) {
            Display::instant("Enter a number between 1 and " +
                             to_string(sceneChoices.size()) + ", or a command.", Display::CYAN);
            continue;
        }

        int nextSceneID = sceneChoices[choice - 1].second;

        
        if (sceneID == 31) {
            string choiceText = toLower(sceneChoices[choice - 1].first);
            if (choiceText.find("tunnel") != string::npos) {
                pendingEndingSceneID = 34;
            } else {
                pendingEndingSceneID = 33;
            }
        }

        currentSceneID = nextSceneID;
        return;
    }
}

void GameManager::handleLivesCheck(int gameOverSceneID) {
    
    auto it = scenes.find(gameOverSceneID);
    if (it != scenes.end()) {
        clearScreen();
        it->second->play(player);
    }

    if (player.getLives() > 0) {
        player.loseLife();
        Display::instant("Barely alive, Agent " + player.getCodename() +
                         ". " + to_string(player.getLives()) + " lives remaining.", Display::CYAN);
        Display::instant("The mission continues.", Display::CYAN);
        cout << endl;
        waitForEnter();
        
        processScene(currentSceneID);
    } else {
        Display::instant("Your lives are gone. The mission is over.", Display::CYAN);
        cout << endl;
        waitForEnter();
        processScene(35); 
    }
}

void GameManager::handleEnding(int endingSceneID) {
    string colour;
    if      (endingSceneID == 33) colour = Display::GREEN;
    else if (endingSceneID == 34) colour = Display::YELLOW;
    else                          colour = Display::RED;

    cout << endl;
    waitForEnter();

    clearScreen();
    Display::divider('=');
    Display::instant("   MISSION COMPLETE — PROTOCOL PHANTOM", colour);
    Display::divider('=');
    Display::instant("Agent " + player.getCodename() + " | " + player.getName(), colour);
    Display::instant("Final Score: " + to_string(player.getScore()), colour);
    if      (endingSceneID == 33)
        Display::instant("  Outcome:      MISSION SUCCESS", Display::GREEN);
    else if (endingSceneID == 34)
        Display::instant("  Outcome:      BITTERSWEET — THE CITY LIVES", Display::YELLOW);
    else
        Display::instant("  Outcome:      MISSION FAILED", Display::RED);
    Display::divider('=');
    cout << endl;

    waitForEnter();
    
    
    gameLoop_active = false;
}

void GameManager::saveGame() {
    try {
        player.setSceneID(currentSceneID);
        fileHandler.saveDossier(player, pendingEndingSceneID);
        hasSavedGames = true;
        Display::instant("Dossier saved. Agent " + player.getCodename() + "'s progress recorded.", Display::CYAN);
        cout << endl;
    } catch (...) {
        cerr << "Error saving game." << endl;
    }
}

void GameManager::loadPlayerFromDossier(const Dossier& d) {
    
    player = Player(d.playerName, d.codename);
    player.setHealth(d.health);
    player.setLives(d.lives);
    player.setAttackPower(d.attackPower);
    player.setScore(d.score);
    player.setSceneID(d.sceneID);
    currentSceneID = d.sceneID;
    pendingEndingSceneID = d.pendingEndingSceneID;

    
    auto items = csvLoader.loadItems();
    player.getInventory().deserialize(d.inventory, items);
}

