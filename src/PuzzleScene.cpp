#include "PuzzleScene.h"
#include "Display.h"
#include "InputValidator.h"
#include <iostream>

using namespace std;

PuzzleScene::PuzzleScene(int id, string title, string description,
                         bool isEnding, bool isGameOver, Puzzle puzzle, int gameOverSceneID)
    : Scene(id, title, description, "puzzle", isEnding, isGameOver, gameOverSceneID),
      puzzle(puzzle), solved(false) {}

void PuzzleScene::play(Player& player) {
    displayDivider();
    Display::instant("   " + title, Display::CYAN);
    displayDivider();
    cout << endl;
    displayDescription(player);
    cout << endl;

    
    if (player.hasPuzzleHintItem()) {
        string hint = puzzle.getHint();
        if (hint.empty()) {
            cerr << "Debug: getHint() returned empty for puzzle " << puzzle.getPuzzleID() << endl;
        }
        Display::println("[Your " + player.getHintItemName() +
                         " highlights a clue: " + hint + "]",
                         Display::SPEED_MEDIUM, Display::YELLOW);
        cout << endl;
    }

    Display::divider('-');
    player.displayStats();
    Display::divider('-');
    cout << endl;

    Display::println(puzzle.getQuestion(), Display::SPEED_MEDIUM, Display::WHITE);
    cout << Display::CYAN << "Your answer: " << Display::RESET;

    string input = InputValidator::getPuzzleAnswer();

    if (puzzle.checkAnswer(input)) {
        cout << endl;
        Display::instant("Correct.", Display::GREEN);
        player.addScore(puzzle.getScoreReward());
        Display::instant("+" + to_string(puzzle.getScoreReward()) + " points.", Display::GREEN);
        solved = true;
    } else {
        cout << endl;
        Display::instant("Wrong.", Display::RED);
        solved = false;
        
        player.takeDamage(DEFAULT_HEALTH); 
    }
    cout << endl;
}

bool PuzzleScene::wasSolved() const {
    return solved;
}

bool PuzzleScene::wasSuccessful() const { return solved; }
bool PuzzleScene::autoAdvance() const   { return true; }
