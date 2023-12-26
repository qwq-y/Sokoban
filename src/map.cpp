//
// Created by 86188 on 2023/12/26.
//

#include "map.hpp"
#include <iostream>

Map::Map(string mapStr, Map *father) : mapStr(mapStr), father(father) {
    // 计算行数和列数，获取玩家初始位置
    int cnt = 0;
    for (char c : mapStr) {
        if (c == ' ') {
            continue;
        }
        if (c == 'P' || c == 'p') {
            initialPlayerRow = rows;
            initialPlayerCol = cnt - rows * cols;
        }
        if (c == '\n') {
            rows++;
            if (cols == 0) {
                cols = cnt;
            }
        } else {
            cnt++;
        }
    }
}

void Map::printMap() const {
    std::cout << mapStr;
}

int Map::getPosElement(int row, int col) const {

}

