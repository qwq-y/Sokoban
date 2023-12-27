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
    cout << "rows: " << currentMap->rows
         << "\tcols: " << currentMap->cols << endl;
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

    int newRow = playerRow + rowMove;
    int newCol = playerCol + colMove;

    string posElement = currentMap->getPosElement(newRow, newCol);
    if (posElement.empty()) {
        cout << "Invalid movement: Out of range." << endl;
    } else {
        dealPosElement(posElement, playerRow, playerCol, newRow, newCol);
    }
}

void Game::dealPosElement(string posElement, int row, int col, int newRow, int newCol) {
    switch (posElement[0]) {
        case '#':
            cout << "Invalid movement: Wall." << endl;
            break;
        case 'B':
        case 'b':
        case 'I':
        case 'i':
        case 'O':
        case 'o':
            // 目前把 O I B 类箱子都当作普通箱子处理
            if (!updateMapToPushBox(newRow, newCol)) {  // 目前只有遇到 # 会返回 false，未考虑贪吃蛇、自含、出界等
                cout << "Invalid movement: Wall." << endl;
            }
            break;
        default:   // . - =
            updateMapToMove(newRow, newCol);
            break;
    }
}

void Game::updateMapToMove(int newRow, int newCol) {
    // 玩家行走一步，更新地图
    string initOrigElement = currentMap->initMapTable[playerRow][playerCol];
    if (initOrigElement[0] == 'P' || initOrigElement[0] == 'O' ||
        initOrigElement[0] == 'B' || initOrigElement[0] == 'I') {
        currentMap->mapTable[playerRow][playerCol] = '.';
    } else {  // . - =
        currentMap->mapTable[playerRow][playerCol] = initOrigElement;
    }

    string initDestElement = currentMap->initMapTable[newRow][newCol];
    if (initDestElement == "=") {
        currentMap->mapTable[newRow][newCol] = 'p';
    } else {
        currentMap->mapTable[newRow][newCol] = 'P';
    }

    playerRow = newRow;
    playerCol = newCol;
}

bool Game::updateMapToPushBox(int newRow, int newCol) {
    int rowMove = newRow - playerRow;
    int colMove = newCol - playerCol;
    int rowPointer = newRow;
    int colPointer = newCol;
    char charPointer = currentMap->mapTable[rowPointer][colPointer][0];
    vector<pair<int, int>> boxPositions;

    while (charPointer == 'O' || charPointer == 'o' ||
           charPointer == 'B' || charPointer == 'b' ||
           charPointer == 'I' || charPointer == 'i') {

        boxPositions.push_back(std::make_pair(rowPointer, colPointer));

        rowPointer = rowPointer + rowMove;
        colPointer = colPointer + colMove;
        charPointer = currentMap->mapTable[rowPointer][colPointer][0];
    }

    if (charPointer == '#') {
        return false;
    }

    // 移动所有箱子
    for (pair<int, int> &pos: boxPositions) {
        string box = currentMap->mapTable[pos.first][pos.second];
        string initDest = currentMap->initMapTable[pos.first + rowMove][pos.second + colMove];

        cout << "box " << box << ": (" << pos.first << ", " << pos.second << ")" << endl;

        if (initDest[0] == '-') {
            box[0] = tolower(box[0]);
        } else {
            box[0] = toupper(box[0]);
        }
        currentMap->mapTable[pos.first + rowMove][pos.second + colMove] = box;
    }

    // 移动玩家
    updateMapToMove(newRow, newCol);
    playerRow = newRow;
    playerCol = newCol;

    return true;
}

bool Game::checkWinCondition() const {
    // 目前只考虑了单张地图
    for (int i = 0; i < currentMap->rows; i++) {
        for (int j = 0; j < currentMap->cols; j++) {
            string initElement = currentMap->getInitPosElement(i, j);
            string currentElement = currentMap->getPosElement(i, j);
            if (initElement[0] == '-' && (currentElement[0] != 'i' && currentElement[0] != 'b' && currentElement[0] != 'o')) {
                return false;
            }
            if (initElement[0] == '=' && currentElement[0] != 'p') {
                return false;
            }
        }
    }
    return true;
}


