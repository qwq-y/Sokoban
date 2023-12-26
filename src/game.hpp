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

    void dealPosElement(string posElement, int row, int col, int rowMove, int colMove);

    void moveElement(int row, int col, int rowMove, int colMove);

    bool checkWinCondition() const;

    int isValidPosition(int row, int col) const;

    void printState();
};


#endif //PROJ_GAME_H
