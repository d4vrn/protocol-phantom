#pragma once

#include "Scene.h"
#include "Enemy.h"
#include <random>

class CombatScene : public Scene {
private:
    Enemy enemy;
    bool combatWon;
    std::mt19937 rng;

public:
    CombatScene(int id, std::string title, std::string description,
                bool isEnding, bool isGameOver, Enemy enemy, int gameOverSceneID = 0);

    
    void play(Player& player) override;

    
    const Enemy& getEnemy() const;

    bool wasSuccessful() const override;
    bool autoAdvance() const override;

private:
    
    bool resolveCombat(Player& player);
};
