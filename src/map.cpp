//
// Created by 86188 on 2023/12/26.
//

#include "map.hpp"
#include <iostream>
#include <sstream>
#include <map>

using namespace std;

Map::Map(Player *&p, string mapStr, vector<Bbox *> &B_boxs, vector<Ibox *> &inf_boxs) : mapStr(mapStr)
{
    // 计算行数、列数和总数，获取玩家初始位置
    initializeMapTable(p, B_boxs, inf_boxs);
    rows = mapTable.size();
    cols = mapTable[0].size();
}

void Map::initializeMapTable(Player *&p, vector<Bbox *> &B_boxs, vector<Ibox *> &inf_boxs)
{
    istringstream iss(mapStr);
    string lineStr;
    int row = 0;
    int col = 0;
    while (getline(iss, lineStr))
    {
        vector<Entity *> lineVec;
        string element = "";
        int test = 0;
        lineStr += '\n';
        for (char c : lineStr)
        {
            if (c == ' ' || c == '\n') // deal with the element just read
            {
                if (element[0] == 'P')
                {
                    Player *player_up = new Player("P", row, col, this);
                    Player *player_low = new Player("p", row, col, this);
                    player_up->set_lower(player_low);
                    player_low->set_upper(player_up);
                    Empty *empty = new Empty(row, col, this);
                    player_up->set_prepared_empty(empty);
                    player_low->set_prepared_empty(empty);
                    lineVec.push_back(player_up);
                    p = player_up;
                }
                else if (element[0] == 'O')
                {
                    Obox *obox_up = new Obox("O", row, col, this);
                    Obox *obox_low = new Obox("o", row, col, this);
                    obox_up->set_lower(obox_low);
                    obox_low->set_upper(obox_up);
                    Empty *empty = new Empty(row, col, this);
                    obox_up->set_prepared_empty(empty);
                    obox_low->set_prepared_empty(empty);
                    lineVec.push_back(obox_up);
                }
                else if (element[0] == '#')
                {
                    Wall *wall = new Wall(row, col, this);
                    lineVec.push_back(wall);
                }
                else if (element[0] == '.')
                {
                    Empty *empty = new Empty(row, col, this);
                    lineVec.push_back(empty);
                }
                else if (element[0] == '-')
                {
                    Box_des *box_des = new Box_des(row, col, this);
                    lineVec.push_back(box_des);
                }
                else if (element[0] == '=')
                {
                    P_des *p_des = new P_des(row, col, this);
                    lineVec.push_back(p_des);
                }
                else if (element[0] == 'B')
                {
                    Bbox *bbox_up = new Bbox(element, row, col, this);
                    element[0] = tolower(element[0]);
                    Bbox *bbox_low = new Bbox(element, row, col, this);
                    bbox_up->set_lower(bbox_low);
                    bbox_low->set_upper(bbox_up);
                    B_boxs.push_back(bbox_up);
                    Empty *empty = new Empty(row, col, this);
                    bbox_up->set_prepared_empty(empty);
                    bbox_low->set_prepared_empty(empty);
                    lineVec.push_back(bbox_up);
                }
                else if (isdigit(element[0]))
                {
                    Ibox *ibox_up = new Ibox(element, row, col, this, "B" + element[2], (int)(element[0] - '0'));
                    element[1] = tolower(element[1]);
                    Ibox *ibox_low = new Ibox(element, row, col, this, "B" + element[2], (int)(element[0] - '0'));
                    inf_boxs.push_back(ibox_up);
                    Empty *empty = new Empty(row, col, this);
                    ibox_up->set_prepared_empty(empty);
                    ibox_low->set_prepared_empty(empty);
                    lineVec.push_back(ibox_up);
                }
                element = "";
                col++;
            }
            else
                element += c;
        }

        mapTable.push_back(lineVec);
        initMapTable.push_back(lineVec);
        row++;
        col = 0;
    }
}

void Map::printMap() const
{
    for (int i = 0; i < mapTable.size(); i++)
    {
        for (int j = 0; j < mapTable[i].size(); j++)
        {
            cout << mapTable[i][j]->get_mark();
            if (mapTable[i][j]->get_mark().size() == 1)
                cout << " ";
        }
        cout << endl;
    }
}

Entity *Map::getPosElement(int row, int col) const
{
    // 检查是否超出地图范围，如不超出则返回该位置的元素
    if (row < 0 || col < 0 || row >= rows || col >= cols)
    {
        return nullptr;
    }
    return mapTable[row][col];
}

Entity *Map::getInitPosElement(int row, int col) const
{
    if (row < 0 || col < 0 || row >= rows || col >= cols)
    {
        return nullptr;
    }
    return initMapTable[row][col];
}
