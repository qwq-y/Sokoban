#include <string>
#include <iostream>
#include "map.hpp"
#include "game.hpp"

using namespace std;

extern const string L1B1Str;

int main() {
    string mapStr = L1B1Str;

    Map *map = new Map(mapStr, nullptr);
    Game *game = new Game(map);

    game->printState();

    char userInput;

    while (true) {
        std::cout << "Enter your move (w/a/s/d):";
        userInput = cin.get();
        cin.ignore();  // 忽略换行符

        if (userInput == 'q' || userInput == 'Q') { // 使用 q/Q 退出
            std::cout << "\nExiting.\n";
            break;
        } else {
            game->handlePlayerMove(userInput);
        }
    }

    return 0;
}



