//
// Created by 86188 on 2023/12/26.
//

#ifndef PROJ_MAP_H
#define PROJ_MAP_H
#include <string>
#include <vector>

using namespace std;

class Map {
public:
    string mapStr;  // 初始的字符串，不随游戏变化
    vector<vector<string>> mapTable;
    Map *father;
    int rows;
    int cols;
    int initialPlayerRow;
    int initialPlayerCol;

    Map(string mapStr, Map *father);

    void initializeMapTable();

    void printMap() const;

    string getPosElement(int row, int col) const;

    void swapElements(int row1, int col1, int row2, int col2);

};


#endif //PROJ_MAP_H



