//
// Created by 86188 on 2023/12/26.
//

#ifndef PROJ_MAP_H
#define PROJ_MAP_H
#include <string>

using namespace std;

class Map {
public:
    string mapStr;
    Map *father;
    int rows;
    int cols;
    int initialPlayerRow;
    int initialPlayerCol;

    Map(string mapStr, Map *father);

    void printMap() const;

    int getPosElement(int row, int col) const;

    enum MapElement {
        WALL = '#',
        EMPTY = '.',
        BOX = 'O',
        PLAYER = 'P',
        STORAGE_POINT = '-',
        CHECK_POINT = '=',
        BOX_STORED= 'o',
        PLAYER_CHECKED = 'p'
    };
};


#endif //PROJ_MAP_H



