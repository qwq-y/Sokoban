//
// Created by 86188 on 2023/12/26.
//

#include "map.hpp"
#include <iostream>
#include <sstream>

using namespace std;

Map::Map(string mapStr, Map *father) : mapStr(mapStr), father(father) {
    // 计算行数、列数和总数，获取玩家初始位置
    initializeMapTable();
    rows = mapTable.size();
    cols = mapTable[0].size();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (mapTable[i][j] == "P") {
                initialPlayerRow = i;
                initialPlayerCol = j;
                return;
            }
        }
    }
}

void Map::initializeMapTable() {
    istringstream iss(mapStr);
    string lineStr;

    while (getline(iss, lineStr)) {
        vector<string> lineVec;

        string element;
        bool isConsecutive = false;
        for (char c: lineStr) {
            if (c == ' ' || c == '\n') {
                continue;
            }
            if (isConsecutive) {
                isConsecutive = false;
                element += c;
                lineVec.push_back(element);
            } else if (c == 'B' || c == 'I') {
                isConsecutive = true;
                element = c;
            } else {
                element = c;
                lineVec.push_back(element);
            }
        }

        mapTable.push_back(lineVec);
    }
}

void Map::printMap() const {
    for (int i = 0; i < mapTable.size(); i++) {
        for (int j = 0; j < mapTable[i].size(); j++) {
            cout << mapTable[i][j];
            if (mapTable[i][j].length() == 1) {
                cout << " ";
            }
        }
        cout << endl;
    }
}

string Map::getPosElement(int row, int col) const {
    // 检查是否超出地图范围，如不超出则返回该位置的元素
    if (row < 0 || col < 0 || row >= rows || col >= cols) {
        return "";
    }
    return mapTable[row][col];
}

void Map::swapElements(int row1, int col1, int row2, int col2) {
    cout << "swap: (" << row1 << ", " << col1 << ") " << mapTable[row1][col1]
         << "\t(" << row2 << ", " << col2 << ") " << mapTable[row2][col2] << endl;
    swap(mapTable[row1][col1], mapTable[row2][col2]);
}


