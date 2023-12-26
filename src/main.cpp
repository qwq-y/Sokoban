#include <string>
#include <iostream>
#include "map.hpp"
#include "game.hpp"

using namespace std;

extern const string L1B1Str;
extern const string L2B1Str;

int main() {
    string mapStr = L2B1Str;

    Map *map = new Map(mapStr, nullptr);
    Game *game = new Game(map);

    game->printState();

    char userInput;

    while (true) {
        std::cout << "Enter your move (w/a/s/d):";
        userInput = cin.get();
        cin.ignore();  // 忽略换行符
        userInput = tolower(userInput);

        cout << "input: " << userInput << endl;

        if (userInput == 'q') { // 使用 q 退出
            std::cout << "\nExiting.\n";
            break;
        } else {
            game->handlePlayerMove(userInput);
            game->printState();
        }
    }

    return 0;
}



