// TypeFast typing game by AbstractUltra

#include <iostream>
#include <unistd.h>
#include <vector>
#include <future>
#include <cstring>
#include <climits>

bool debugMode;
bool perfReport = true;

struct Player {
    std::vector<int>data;
    int streak = 0;
    int level = 1;
    int lives = 3;
    int highScore = 0;
    std::string name;
};

struct ProvidedArgs {
    bool perfReport;
    bool debugMode;
};

void clearScreen() {
    std::cout << "\033[2J"
              << "\033[H";
}

void getReadyScreen(std::string name) {
    for (int i = 5;i>0;--i) {
        clearScreen();
        std::cout << "Get ready, "
                  << name
                  << "!"
                  << std::endl
                  << i
                  << std::endl;
        sleep(1);
    }
}

int requiredWPM(int playerLevel) {
    int i = playerLevel * 5;
    return i;
}

double wpm;

bool performanceReport(int playerLevel) {
    bool result;
    if (wpm >= requiredWPM(playerLevel)) {
        result = true;
    } else {
        result = false;
    }
    return result;
}

std::vector<int>mainGame(int playerLevel, std::string playerName, int playerHealth, int playerHighscore) {
    std::vector<int>results(2);
    //Debug Start
    if (debugMode) {
        char usrin;
        bool dbgmenu;
        std::cout << "Enter dbg menu? ";
        std::cin >> usrin;
        std::cin.ignore();
        if (usrin == 'y') {
            dbgmenu = true;
        } else {
            dbgmenu = false;
        }
        if (dbgmenu) {
            std::cout << "Enter level ";
            std::cin >> results[0];
            std::cout << "Subtract playerhealth? (value 0 is yes) ";
            std::cin >> results[1];
            std::cerr << "Pretend there is a game here, this is currently only for testing :)" << std::endl;
            return results;
        }
    }
    //Debug End
    std::vector<std::string>sentences{
        "i am typing a sentence",
        "this is a very short sentence",
        "i am a very fast typer",
        "the quick brown fox jumps over the lazy dog",
        "i will beat my highscore",
        "i am typing very fast right now",
        "why am i playing this",
        "i am very intelligent",
        "i Am uNlUCkY1!1!!!111!!!!!11",
        "1",
        "i am running out of ideas"
    };
    srand(time(0));
    std::string stringToType = sentences[rand()%sentences.size()];
    clearScreen();
    std::cout << playerName << "    " << playerHealth << "HP" << "    " << "Highscore: " << playerHighscore << "\n"
              << "Level " << playerLevel << "\n"
              << "Required Speed: " << requiredWPM(playerLevel) << "WPM\n\n"
              << "Type:\n"
              << stringToType
              << "\n\n>> ";
    std::string userInput;
    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
    getline(std::cin, userInput);
    std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double>totalTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime-startTime);
    int wordCount = 1;
    for (int i = 0;i<userInput.length();++i) {
        if (userInput.at(i) == ' ') {
            ++wordCount;
        }
    }
    wpm = (wordCount/totalTime.count()) * 60;
    bool speedPass = true;
    if (!performanceReport(playerLevel)) {
        speedPass = false;
    }
    if (userInput == stringToType && speedPass) {
        results[0] = 0;
        results[1] = 1;
    } else {
        results[0] = 1;
        results[1] = 0;
    }
    if (perfReport) {
        clearScreen();
        std::cout << "Your speed was " << wpm << "WPM." << std::endl;
        if (wpm >= requiredWPM(playerLevel) && results[0] == 0) {
            std::cout << "Pass!" << std::endl;
        } else if (userInput == stringToType && wpm < requiredWPM(playerLevel)) {
            std::cout << "Fail! " << requiredWPM(playerLevel) << "WPM is required to pass at level " << playerLevel
                      << "!" << std::endl;
        } else {
            std::cout << "Fail! Inaccurate typing!" << std::endl;
        }
        sleep(5);
    }
    return results;
}

int main(int argc, char *argv[]) {
    ProvidedArgs providedArgs;
    std::string dbgCheck = "";
    if (argc > 1) {
        for (int i = 1;i<argc;++i) {
            std::string argument = argv[i];
            if (argument == "-noperfreport") {
                providedArgs.perfReport = true;
                perfReport = false;
            } else if (argument == "-debugmode") {
                providedArgs.debugMode = true;
                dbgCheck = "dbg";
            }
        }
    }
    if (!providedArgs.perfReport) {
        char userInput;
        std::cout << "Enable performance report? (Y/N)\n>> ";
        std::cin >> userInput;
        userInput = tolower(userInput);
        if (userInput == 'y') {
            perfReport = true;
        } else if (userInput == 'n') {
            perfReport = false;
        }
    }
    clearScreen();
    bool dualPlayer;
    int playerTurn;
    while (true) {
        std::cout << "Are there 2 players? (Y/N)\n>> ";
        char userInput;
        std::cin >> userInput;
        userInput = tolower(userInput);
        //std::cin.ignore(INT_MAX);
        if (userInput == 'y') {
            dualPlayer = true;
            break;
        } else if (userInput == 'n') {
            dualPlayer = false;
            break;
        } else if (userInput == 'd') {
            dbgCheck += 'd';
            std::cout << "Invalid selection!" << std::endl;
        } else if (userInput == 'b') {
            dbgCheck += 'b';
            std::cout << "Invalid selection!" << std::endl;
        } else if (userInput == 'g') {
            dbgCheck += 'g';
            std::cout << "Invalid selection!" << std::endl;
            if (dbgCheck == "dbg") {
                break;
            }
        } else {
            std::cout << "Invalid selection!" << std::endl;
        }
    }
    if (dbgCheck == "dbg") {
        while (true) {
            std::string code;
            std::cout << "!";
            std::cin >> code;
            if (code == "debug") {
                debugMode = true;
                break;
            } else if (code == "debug2") {
                debugMode = true;
                dualPlayer = true;
                break;
            }
        }
    }
    Player player1;
    Player player2;
    clearScreen();
    std::cout << "What is Player 1's name?\n>> ";
    std::cin.ignore();
    getline(std::cin, player1.name);
    if (dualPlayer) {
        clearScreen();
        std::cout << "What is Player 2's name?\n>> ";
        getline(std::cin, player2.name);
    }
    getReadyScreen(player1.name);
    while (true) {
        while (player1.lives > 0) {
            player1.data = mainGame(player1.level, player1.name, player1.lives, player1.highScore);
            if (player1.data[0] == 0) {
                ++player1.level;
                ++player1.streak;
                if (player1.streak == 5) {
                    ++player1.lives;
                }
            }
            if (player1.data[1] == 0) {
                --player1.lives;
                player1.streak = 0;
            }
            if (player1.data[1] == 0 && player1.lives > 0) {
                getReadyScreen(player1.name);
            }
            if (player1.level > player1.highScore) {
                player1.highScore = player1.level;
            }
        }
        clearScreen();
        std::cout << "Game Over!";
        if (dualPlayer) {
            getReadyScreen(player2.name);
            while (player2.lives > 0) {
                player2.data = mainGame(player2.level, player2.name, player2.lives, player2.highScore);
                if (player2.data[0] == 0) {
                    ++player2.level;
                    ++player2.streak;
                    if (player2.streak == 5) {
                        ++player2.lives;
                    }
                }
                if (player2.data[1] == 0) {
                    --player2.lives;
                    player2.streak = 0;
                }
                if (player2.data[1] == 0 && player2.lives > 0) {
                    getReadyScreen(player2.name);
                }
                if (player2.level > player2.highScore) {
                    player2.highScore = player2.level;
                }
            }
            clearScreen();
            std::cout << "Game Over!" << std::endl;
        }
        char usrSelect;
        clearScreen();
        std::cout << "Try Again?" << std::endl;
        while (true) {
            std::cout << ">> ";
            std::cin >> usrSelect;
            std::cin.ignore();
            usrSelect = tolower(usrSelect);
            if (usrSelect == 'n') {
                break;
            } else if (usrSelect == 'y') {
                player1.level = 1;
                player2.level = 1;
                player1.lives = 3;
                player2.lives = 3;
                getReadyScreen(player1.name);
                break;
            } else {
                std::cout << "Invalid Selection!" << std::endl;
            }
        }
        if (usrSelect == 'n') {
            break;
        }
    }
    if (debugMode) {
        std::cerr << "Player 1 has " << player1.lives << " lives" << std::endl;
        std::cerr << "Player 1's highscore is " << player1.highScore << std::endl;
        std::cerr << "Player 2 has " << player2.lives << " lives" << std::endl;
        std::cerr << "Player 2's highscore is " << player2.highScore << std::endl;
    }
}