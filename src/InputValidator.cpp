#include "InputValidator.h"
#include <iostream>
#include <algorithm>
#include <cctype>

using namespace std;

static constexpr int MAX_INPUT_LENGTH = 100;

string InputValidator::trim(const string& s) {
    string result = s;
    
    result.erase(remove(result.begin(), result.end(), '\r'), result.end());
    size_t start = result.find_first_not_of(" \t\n");
    if (start == string::npos) return "";
    size_t end = result.find_last_not_of(" \t\n");
    return result.substr(start, end - start + 1);
}

string InputValidator::toLower(const string& s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

static string readLine(const string& reprompt = "") {
    string input;
    getline(cin, input);
    
    input.erase(remove(input.begin(), input.end(), '\r'), input.end());
    if ((int)input.size() > MAX_INPUT_LENGTH) {
        cout << "Input too long (max " << MAX_INPUT_LENGTH << " characters).";
        if (!reprompt.empty()) cout << " " << reprompt;
        cout << endl;
        return ""; 
    }
    
    size_t start = input.find_first_not_of(" \t\n");
    if (start == string::npos) return "";
    size_t end = input.find_last_not_of(" \t\n");
    return input.substr(start, end - start + 1);
}

int InputValidator::getMenuChoice(int max) {
    while (true) {
        string input = readLine("Please enter a number between 1 and "
                                + to_string(max) + ": ");

        if (input.empty()) {
            cout << "Please enter a number between 1 and " << max << ": ";
            continue;
        }

        
        bool allDigits = true;
        for (char c : input) {
            if (!isdigit(c)) { allDigits = false; break; }
        }
        if (!allDigits) {
            cout << "Numbers only. Please enter a number between 1 and " << max << ": ";
            continue;
        }

        try {
            int choice = stoi(input);
            if (choice < 1 || choice > max) {
                cout << "Please enter a number between 1 and " << max << ": ";
                continue;
            }
            return choice;
        } catch (...) {
            cout << "Invalid input. Please enter a number between 1 and " << max << ": ";
        }
    }
}

string InputValidator::getPlayerName() {
    while (true) {
        string input = readLine("Enter your name: ");

        if (input.empty()) {
            cout << "Name cannot be empty. Enter your name: ";
            continue;
        }

        bool valid = true;
        for (char c : input) {
            if (!isalpha(c) && c != ' ' && c != '-') { valid = false; break; }
        }
        if (!valid) {
            cout << "Letters, spaces, and hyphens only. Enter your name: ";
            continue;
        }

        return input;
    }
}

bool InputValidator::getYesNo() {
    while (true) {
        string input = readLine("Please enter y or n: ");
        input = toLower(input);

        if (input == "y" || input == "yes") return true;
        if (input == "n" || input == "no")  return false;

        cout << "Please enter y or n: ";
    }
}

string InputValidator::getPuzzleAnswer() {
    while (true) {
        string input = readLine("Answer cannot be empty. Your answer: ");

        if (input.empty()) {
            cout << "Answer cannot be empty. Your answer: ";
            continue;
        }

        return input;
    }
}

int InputValidator::getItemChoice(int max) {
    while (true) {
        string input = readLine("Please enter a number between 1 and "
                                + to_string(max) + ": ");

        if (input.empty()) {
            cout << "Please enter a number between 1 and " << max << ": ";
            continue;
        }

        
        bool allDigits = true;
        for (char c : input) {
            if (!isdigit(c)) { allDigits = false; break; }
        }
        if (!allDigits) {
            cout << "Numbers only. Please enter a number between 1 and " << max << ": ";
            continue;
        }

        try {
            int choice = stoi(input);
            if (choice < 1 || choice > max) {
                cout << "Please enter a number between 1 and " << max << ": ";
                continue;
            }
            return choice;
        } catch (...) {
            cout << "Invalid input. Please enter a number between 1 and " << max << ": ";
        }
    }
}

int InputValidator::getSlotChoice(int count) {
    while (true) {
        string input = readLine();

        if (input.empty()) return -1;

        
        bool allDigits = true;
        for (char c : input) {
            if (!isdigit(c)) { allDigits = false; break; }
        }
        if (!allDigits) {
            cout << "Numbers only, or press Enter for latest: ";
            continue;
        }

        try {
            int choice = stoi(input);
            if (choice < 1 || choice > count) {
                cout << "Please enter a number between 1 and " << count
                     << ", or press Enter for latest: ";
                continue;
            }
            return choice;
        } catch (...) {
            cout << "Invalid input. Enter a number or press Enter for latest: ";
        }
    }
}
