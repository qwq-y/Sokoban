//
// Created by 86188 on 2023/12/26.
//

#include <iostream>
#include <map>
#include "game.hpp"

using namespace std;

void Game::printState()
{
    cout << endl;
    cout << map2name[currentMap] << ":" << endl;
    cout << endl;
    currentMap->printMap();
    cout << endl;
    for (const auto &print : map2name)
    {
        if (print.first == currentMap)
            continue;
        cout << map2name[print.first] << ":" << endl;
        cout << endl;
        print.first->printMap();
        cout << endl;
    }
    /*cout << "rows: " << currentMap->rows
         << "\tcols: " << currentMap->cols << endl;*/
    cout << "player position: " << map2name[currentMap] << " (" << p->get_pos().first << ", " << p->get_pos().second << ")\n";
    cout << endl;
}

void Game::handlePlayerMove(char userInput)
{
    int rowMove = 0;
    int colMove = 0;
    int direction = 0;

    switch (userInput)
    {
    case 'w':
        rowMove = -1;
        direction = 1;
        break;
    case 's':
        rowMove = 1;
        direction = 3;
        break;
    case 'a':
        colMove = -1;
        direction = 4;
        break;
    case 'd':
        colMove = 1;
        direction = 2;
        break;
    default:
        cout << "Invalid input. Please use 'w', 's', 'a', or 'd'." << endl;
        return;
    }

    int newRow = p->get_pos().first + rowMove;
    int newCol = p->get_pos().second + colMove;

    Move(currentMap, p->get_pos().first, p->get_pos().second, currentMap, newRow, newCol, direction);
}

bool Game::Move(Map *sm, int sx, int sy, Map *dm, int dx, int dy, int dir) // 1上 2右 3下 4 左
{

    cout << "try move " << map2name[sm] << " " << sx << " " << sy << " into " << map2name[dm] << " " << dx << " " << dy << " direction:" << dir << endl;

    int xMove = 0;
    int yMove = 0;
    if (dir == 1)
        xMove = -1;
    else if (dir == 2)
        yMove = 1;
    else if (dir == 3)
        xMove = 1;
    else if (dir == 4)
        yMove = -1;
    bool can_move = true;
    if (dx < 0 || dy < 0 || dx >= dm->rows || dy >= dm->cols) // 出界
    {
        string mark = map2name[sm];
        Map *outer_map;
        int outer_x = 0;
        int outer_y = 0;
        for (Bbox *bbox : B_boxs)
            if (bbox->get_mark() == mark)
            {
                outer_map = bbox->get_map();
                outer_x = bbox->get_pos().first + xMove;
                outer_y = bbox->get_pos().second + yMove;
                break;
            }
        return Move(sm, sx, sy, outer_map, outer_x, outer_y, dir);
    }
    else if (dm->mapTable[dx][dy]->get_mark()[0] == '#') // 撞墙
        return false;
    else if (dm->mapTable[dx][dy]->get_mark()[0] == 'O' || dm->mapTable[dx][dy]->get_mark()[0] == 'o' || dm->mapTable[dx][dy]->get_mark()[0] == 'B' || dm->mapTable[dx][dy]->get_mark()[0] == 'b' || dm->mapTable[dx][dy]->get_mark()[1] == 'I' || dm->mapTable[dx][dy]->get_mark()[1] == 'i') // 前方还有箱子
        can_move = Move(dm, dx, dy, dm, dx + xMove, dy + yMove, dir);

    if (!can_move) // 前面的块不能再往前
    {
        if (dm->mapTable[dx][dy]->get_mark()[0] != 'B' && dm->mapTable[dx][dy]->get_mark()[0] != 'b') // 不是可进入箱子
            return false;                                                                             // 那么这个物体也动不了
        string name = dm->mapTable[dx][dy]->get_mark();
        Map *entering_map = name2map[name];
        int rows = entering_map->rows;
        int cols = entering_map->cols;
        if (dir == 1) // 向上进入，检测B箱的下边
        {
            if (entering_map->mapTable[rows - 1][cols / 2]->get_mark()[0] == '#') // 是墙，进不去
                return false;
            else
                return Move(sm, sx, sy, entering_map, rows - 1, cols / 2, dir);
        }
        else if (dir == 2) // 向右进入，检测B箱的左边
        {
            if (entering_map->mapTable[rows / 2][0]->get_mark()[0] == '#') // 是墙，进不去
                return false;
            else
                return Move(sm, sx, sy, entering_map, rows / 2, 0, dir);
        }
        else if (dir == 3) // 向下进入，检测B箱的上边
        {
            if (entering_map->mapTable[0][cols / 2]->get_mark()[0] == '#') // 是墙，进不去
                return false;
            else
                return Move(sm, sx, sy, entering_map, 0, cols / 2, dir);
        }
        else if (dir == 4) // 向左进入，检测B箱的右边
        {
            if (entering_map->mapTable[rows / 2][cols - 1]->get_mark()[0] == '#') // 是墙，进不去
                return false;
            else
                return Move(sm, sx, sy, entering_map, rows / 2, cols - 1, dir);
        }
    }

    // 单体移动
    Entity *object = sm->mapTable[sx][sy];
    string mark = object->get_mark();
    // 离开的点的处理
    string initOrigElement = sm->initMapTable[sx][sy]->get_mark();
    if (initOrigElement[0] == 'P' || initOrigElement[0] == 'O' ||
        initOrigElement[0] == 'B' || initOrigElement[1] == 'I')
    {
        sm->mapTable[sx][sy] = sm->initMapTable[sx][sy]->get_prepared_empty();
    }
    else
    { // . - =
        sm->mapTable[sx][sy] = sm->initMapTable[sx][sy];
    }
    // 进入的点的处理
    string initDestElement = dm->initMapTable[dx][dy]->get_mark();
    if ((initDestElement == "=" && mark[0] == 'P') ||
        (initDestElement == "-" && (mark[0] == 'O' || mark[0] == 'o' || mark[0] == 'B' || mark[0] == 'b' || mark[1] == 'I' || mark[1] == 'i'))) // 玩家进入检查点、箱子进入目标点
    {
        object->to_lower();
    }
    else
    {
        object->to_upper();
    }
    dm->mapTable[dx][dy] = object;
    object->change_pos(make_pair(dx, dy), dm);
    if (object == p)
        currentMap = dm;
    return true;
}

bool Game::checkWinCondition() const
{
    for (const auto &submap : map2name)
        for (int i = 0; i < submap.first->rows; i++)
        {
            for (int j = 0; j < submap.first->cols; j++)
            {
                Entity *initElement = submap.first->getInitPosElement(i, j);
                Entity *currentElement = submap.first->getPosElement(i, j);
                if (initElement->get_mark()[0] == '-' && (currentElement->get_mark()[0] != 'i' && currentElement->get_mark()[0] != 'b' && currentElement->get_mark()[0] != 'o'))
                {
                    return false;
                }
                if (initElement->get_mark()[0] == '=' && currentElement->get_mark()[0] != 'p')
                {
                    return false;
                }
            }
        }
    return true;
}
