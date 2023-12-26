//
// Created by 86188 on 2023/12/26.
//

#ifndef PROJ_GAME_H
#define PROJ_GAME_H

#include "map.hpp"

class Game {
private:
    Map *currentMap;
    int playerRow;   // index 从 0 开始，左上角为 (0, 0)
    int playerCol;

public:
    Game(Map *initialMap);

    void handlePlayerMove(char userInput);

    bool checkWinCondition() const;

    bool isValidPosition(int row, int col) const;

    void printState();
};


#endif //PROJ_GAME_H
