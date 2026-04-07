#pragma once

#include "Scene.h"
#include "Puzzle.h"

class PuzzleScene : public Scene {
private:
    Puzzle puzzle;

public:
    PuzzleScene(int id, std::string title, std::string description,
                bool isEnding, bool isGameOver, Puzzle puzzle, int gameOverSceneID = 0);

    
    void play(Player& player) override;

    
    bool wasSolved() const;

    bool wasSuccessful() const override;
    bool autoAdvance() const override;

private:
    bool solved;
};
