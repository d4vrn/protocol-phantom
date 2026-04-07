#include "Display.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

using namespace std;

void Display::print(const string& text, int speedMs, const string& colour) {
    cout << colour;
    for (char c : text) {
        cout << c << flush;
        
        if (c == '.' && text.find("...") != string::npos)
            this_thread::sleep_for(chrono::milliseconds(PAUSE_ELLIPSIS / 3));
        else
            this_thread::sleep_for(chrono::milliseconds(speedMs));
    }
    cout << RESET;
}

void Display::println(const string& text, int speedMs, const string& colour) {
    print(text, speedMs, colour);
    cout << endl;
    pauseMs(PAUSE_PARAGRAPH);
}

void Display::instant(const string& text, const string& colour) {
    cout << colour << text << RESET << endl;
}

void Display::pauseMs(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}

void Display::printWrapped(const string& text, int speedMs, const string& colour, int width) {
    istringstream words(text);
    string word;
    string line;

    while (words >> word) {
        
        if (!line.empty() && (int)(line.length() + 1 + word.length()) > width) {
            println(line, speedMs, colour);
            line = word;
        } else {
            if (!line.empty()) line += " ";
            line += word;
        }
    }
    if (!line.empty()) println(line, speedMs, colour);
}

void Display::divider(char c, int width) {
    instant(string(width, c), CYAN);
}
