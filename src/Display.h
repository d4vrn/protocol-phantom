#pragma once
#include <string>

namespace Display {
    
    const std::string RESET   = "\033[0m";
    const std::string WHITE   = "\033[37m";
    const std::string GREEN   = "\033[32m";
    const std::string RED     = "\033[31m";
    const std::string YELLOW  = "\033[33m";
    const std::string CYAN    = "\033[34m";  

    
    const int SPEED_SLOW      = 50;   
    const int SPEED_NORMAL    = 30;   
    const int SPEED_FAST      = 15;   
    const int SPEED_MEDIUM    = 25;   
    const int SPEED_BRIEFING  = 35;   
    const int SPEED_ACT       = 40;   

    
    const int PAUSE_ELLIPSIS  = 600;  
    const int PAUSE_PARAGRAPH = 300;  
    const int PAUSE_COMBAT    = 400;  
    const int PAUSE_ENDING    = 800;  

    void print(const std::string& text, int speedMs, const std::string& colour = WHITE);
    void println(const std::string& text, int speedMs, const std::string& colour = WHITE);
    void instant(const std::string& text, const std::string& colour = WHITE);
    void pauseMs(int ms);
    void printWrapped(const std::string& text, int speedMs, const std::string& colour, int width = 60);
    void divider(char c = '=', int width = 60);
}
