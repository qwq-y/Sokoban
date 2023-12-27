#include <string>
#include <iostream>
#include "map.hpp"
#include "game.hpp"

using namespace std;

extern const string L1B1Str;
extern const string L2B1Str;

int main() {
    string mapStr = L1B1Str;

    Map *map = new Map(mapStr, nullptr);
    Game *game = new Game(map);

    char userInput;

    while (true) {
        game->printState();
        cout << "Enter your move (w/a/s/d) or quit (q):";

        userInput = cin.get();
        cin.ignore();  // 忽略换行符
        userInput = tolower(userInput);

        // 处理无效输入
        if (!(userInput == 'w' || userInput == 'a' || userInput == 's' || userInput == 'd' || userInput == 'q')) {
            std::cout << "Invalid input. Please enter 'w', 'a', 's', 'd', or 'q'." << std::endl;
            continue;  // 继续循环，等待有效输入
        }

        if (userInput == 'q') { // 使用 q 退出
            cout << "\nQuit.\n";
            break;
        } else {
            game->handlePlayerMove(userInput);
            if (game->checkWinCondition()) {
                cout << "\nYou won!\n";
                break;
            }
        }
    }

    return 0;
}



