#pragma once

#include <string>

class InputValidator {
public:
    
    
    static int getMenuChoice(int max);

    
    static std::string getPlayerName();

    
    
    static bool getYesNo();

    
    static std::string getPuzzleAnswer();

    
    static int getItemChoice(int max);

    
    
    static int getSlotChoice(int count);

private:
    
    static std::string trim(const std::string& s);

    
    static std::string toLower(const std::string& s);
};
