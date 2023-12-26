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
    cout << "map->rows: " << currentMap->rows
         << "\tmap->cols: " << currentMap->cols << endl;
    cout << "player position: (" << playerRow << ", " << playerCol << ")\n";
    cout << endl;
}

void Game::handlePlayerMove(char userInput) {
    int rowMove = 0;
    int colMove = 0;

    switch (userInput) {
        case 'w':
            rowMove = -1;
            break;
        case 's':
            rowMove = 1;
            break;
        case 'a':
            colMove = -1;
            break;
        case 'd':
            colMove = 1;
            break;
        default:
            cout << "Invalid input. Please use 'w', 's', 'a', or 'd'." << endl;
            return;
    }

    string posElement = currentMap->getPosElement((playerRow + rowMove), (playerCol + colMove));
    if (posElement.empty()) {
        cout << "Invalid movement: Out of range." << endl;
    } else {
        dealPosElement(posElement, playerRow, playerCol, rowMove, colMove);
    }
}

void Game::dealPosElement(string posElement, int row, int col, int rowMove, int colMove) {
    switch (posElement[0]) {
        case '#':
            cout << "Invalid movement: Wall." << endl;
            break;
        case 'O':
            // TODO
            break;
        default:
            currentMap->swapElements(row, col, (row + rowMove), (col + colMove));
            playerRow = row + rowMove;
            playerCol = col + colMove;
            break;
    }
}



