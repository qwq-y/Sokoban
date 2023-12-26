//
// Created by 86188 on 2023/12/26.
//

#include <iostream>
#include "game.hpp"

using namespace std;

Game::Game(Map *initialMap) : currentMap(initialMap) {
    playerRow = initialMap->initialPlayerRow;
    playerCol = initialMap->initialPlayerCol;
}

void Game::printState() {
    cout << endl;
    currentMap->printMap();
    cout << "map->rows: " << currentMap->rows << "\tmap->cols: " << currentMap->cols << endl;
    cout << "player position: (" << playerRow << ", " << playerCol << ")\n";
    cout << endl;
}

void  Game::handlePlayerMove(char userInput) {
    int newRow = playerRow;
    int newCol = playerCol;

    switch (userInput) {
        case 'w':
            newRow--;
            break;
        case 's':
            newRow++;
            break;
        case 'a':
            newCol--;
            break;
        case 'd':
            newCol++;
            break;
        default:
            cout << "Invalid input. Please use 'w', 's', 'a', or 'd'." << endl;
            return;
    }

    cout << "new pos: (" << newRow << ", " << newCol << ")\n";


}




