//
// Created by 86188 on 2023/12/26.
//

#ifndef PROJ_MAP_H
#define PROJ_MAP_H
#include <string>
#include <vector>
#include <map>
#include "Entity.hpp"

using namespace std;

class Map
{
public:
    string mapStr; // 初始的字符串，不随游戏变化
    vector<vector<Entity *>> mapTable;
    vector<vector<Entity *>> initMapTable; // 初始的地图，不随游戏变化
    int rows;
    int cols;

    Map(Player *&p, string mapStr, vector<Bbox*> &B_boxs, vector<Ibox*> &inf_boxs);

    void initializeMapTable(Player *&p, vector<Bbox*> &B_boxs, vector<Ibox*> &inf_boxs);

    void printMap() const;

    Entity *getPosElement(int row, int col) const;

    Entity *getInitPosElement(int row, int col) const;

    // 比较函数，用于 std::map 的比较
    bool operator<(const Map &other) const
    {
        return std::tie(rows, cols) < std::tie(other.rows, other.cols);
    }
};

#endif // PROJ_MAP_H
