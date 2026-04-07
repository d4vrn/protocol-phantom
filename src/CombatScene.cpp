#include "CombatScene.h"
#include "Display.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <string>

using namespace std;

CombatScene::CombatScene(int id, string title, string description,
                         bool isEnding, bool isGameOver, Enemy enemy, int gameOverSceneID)
    : Scene(id, title, description, "combat", isEnding, isGameOver, gameOverSceneID),
      enemy(enemy), combatWon(false),
      rng(chrono::steady_clock::now().time_since_epoch().count()) {}

void CombatScene::play(Player& player) {
    displayDivider();
    Display::instant("   " + title, Display::CYAN);
    displayDivider();
    cout << endl;
    displayDescription(player);
    cout << endl;

    Display::divider('-');
    player.displayStats();
    Display::divider('-');
    cout << endl;

    Display::instant("ENEMY: " + enemy.getName(), Display::CYAN);
    Display::printWrapped(enemy.getDescription(), Display::SPEED_FAST, Display::CYAN);
    Display::instant("Enemy Attack Power: " + to_string(enemy.getAttackPower()), Display::CYAN);
    cout << endl;

    combatWon = resolveCombat(player);
}

bool CombatScene::resolveCombat(Player& player) {
    uniform_int_distribution<int> dist(1, 10);

    int enemyHealth        = enemy.getHealth();
    const string enemyName = enemy.getName();

    
    for (const Item& item : player.getInventory().getItems()) {
        if (item.getType() == "weapon") {
            Display::instant("You draw your " + item.getName() + ".", Display::CYAN);
            cout << endl;
            break;
        }
    }

    bool fled = false;

    for (int round = 1; round <= 3; ++round) {
        Display::divider('-');
        cout << endl;
        Display::instant("ROUND " + to_string(round), Display::CYAN);

        int playerRoll  = dist(rng);
        int enemyRoll   = dist(rng);
        int playerScore = player.getAttackPower() + playerRoll;
        int enemyScore  = enemy.getAttackPower()  + enemyRoll;

        Display::println("You:     " + to_string(player.getAttackPower()) + " + " +
                         to_string(playerRoll) + " = " + to_string(playerScore),
                         Display::SPEED_FAST, Display::WHITE);
        Display::println(enemyName + ": " + to_string(enemy.getAttackPower()) + " + " +
                         to_string(enemyRoll) + " = " + to_string(enemyScore),
                         Display::SPEED_FAST, Display::WHITE);

        if (playerScore > enemyScore) {
            int dmgToEnemy  = max(1, player.getAttackPower() / 2);
            int dmgToPlayer = max(1, enemy.getAttackPower()  / 2);
            enemyHealth -= dmgToEnemy;
            player.takeDamage(dmgToPlayer);
            Display::println("You land a hit.", Display::SPEED_FAST, Display::GREEN);
            Display::pauseMs(Display::PAUSE_COMBAT);
        } else {
            int dmgToPlayer = max(1, enemy.getAttackPower() / 2);
            player.takeDamage(dmgToPlayer);
            Display::println("You take a hit.", Display::SPEED_FAST, Display::RED);
            Display::pauseMs(Display::PAUSE_COMBAT);
        }

        Display::instant("Your health: " + to_string(player.getHealth()) +
                         " | " + enemyName + " health: " + to_string(enemyHealth),
                         Display::RED);
        cout << endl;

        
        if (player.getHealth() <= 0 || enemyHealth <= 0) break;

        
        if (round < 3) {
            string choice;
            while (true) {
                Display::instant("[1] Continue fighting", Display::CYAN);
                Display::instant("[flee] Attempt to flee — costs 10 health, no score reward", Display::CYAN);
                cout << Display::CYAN << "> " << Display::RESET;
                getline(cin, choice);

                
                auto start = choice.find_first_not_of(" \t\r\n");
                if (start == string::npos) {
                    choice = "";
                } else {
                    choice = choice.substr(start,
                        choice.find_last_not_of(" \t\r\n") - start + 1);
                }

                if (choice == "1" || choice == "flee") break;
                Display::instant("Invalid input. Enter 1 to continue or 'flee' to escape.", Display::CYAN);
            }

            if (choice == "flee") {
                player.takeDamage(10);
                Display::println("You break away from the fight. It costs you.", Display::SPEED_FAST, Display::YELLOW);
                cout << endl;
                fled = true;
                break;
            }
        }
    }

    
    if (fled) return true;

    
    if (player.getHealth() <= 0) return false;

    
    if (enemyHealth <= 0) {
        Display::instant(enemyName + " is down. +" + to_string(enemy.getScoreReward()) + " points.", Display::GREEN);
        player.addScore(enemy.getScoreReward());
        cout << endl;
        return true;
    }

    
    if (player.getHealth() > enemyHealth) {
        Display::instant(enemyName + " is down. +" + to_string(enemy.getScoreReward()) + " points.", Display::GREEN);
        player.addScore(enemy.getScoreReward());
        cout << endl;
        return true;
    }

    return false;
}

const Enemy& CombatScene::getEnemy() const {
    return enemy;
}

bool CombatScene::wasSuccessful() const { return combatWon; }
bool CombatScene::autoAdvance() const   { return true; }
