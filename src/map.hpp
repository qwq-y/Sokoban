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
    vector<vector<string>> initMapTable;  // 初始的地图，不随游戏变化
    Map *father;
    int rows;
    int cols;
    int initialPlayerRow;
    int initialPlayerCol;

    Map(string mapStr, Map *father);

    void initializeMapTable();

    void printMap() const;

    string getPosElement(int row, int col) const;

    string getInitPosElement(int row, int col) const;

};


#endif //PROJ_MAP_H



