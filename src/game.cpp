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
    case 'z':
        undo();
        return;
    default:
        cout << "Invalid input. Please use 'w', 's', 'a', 'd' or 'z'" << endl;
        return;
    }

    int newRow = p->get_pos().first + rowMove;
    int newCol = p->get_pos().second + colMove;
    vector<recorder> this_step;
    Move(currentMap, p->get_pos().first, p->get_pos().second, currentMap, newRow, newCol, direction, this_step);
    if (!this_step.empty())
        record.push_back(this_step);
}

bool Game::Move(Map *sm, int sx, int sy, Map *dm, int dx, int dy, int dir, vector<recorder> &this_step) // 1上 2右 3下 4 左
{
    cout << "try move " << map2name[sm] << " " << sx << " " << sy << " into " << map2name[dm] << " " << dx << " " << dy << " direction:" << dir << endl;
    Entity *object = sm->mapTable[sx][sy];
    string mark = object->get_mark();
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
        return Move(sm, sx, sy, outer_map, outer_x, outer_y, dir, this_step);
    }
    else if (dm->mapTable[dx][dy]->get_mark()[0] == '#') // 撞墙
        return false;
    else if (dm->mapTable[dx][dy]->get_mark()[0] == 'O' || dm->mapTable[dx][dy]->get_mark()[0] == 'o' || dm->mapTable[dx][dy]->get_mark()[0] == 'B' || dm->mapTable[dx][dy]->get_mark()[0] == 'b' || dm->mapTable[dx][dy]->get_mark()[1] == 'I' || dm->mapTable[dx][dy]->get_mark()[1] == 'i') // 前方还有箱子
        can_move = Move(dm, dx, dy, dm, dx + xMove, dy + yMove, dir, this_step);
    else if (dm->mapTable[dx][dy] == p) // 推到自己了
        push_player = true;

    if (!can_move) // 前面的块不能再往前
    {
        bool can_move_in_next = false; // 记录该块是否可以进入前面的块

        // 先看能否进后面的块，不行再考虑自己能不能吞掉后面的块
        if (dm->mapTable[dx][dy]->get_mark()[0] == 'B' || dm->mapTable[dx][dy]->get_mark()[0] == 'b') // 是可进入箱子
        {
            Map *entering_map = name2map[dm->mapTable[dx][dy]->get_mark()];
            int entering_rows = entering_map->rows;
            int entering_cols = entering_map->cols;
            if (dir == 1) // 向上进入，检测B箱的下边
            {
                if (entering_map->mapTable[entering_rows - 1][entering_cols / 2]->get_mark()[0] == '#') // 是墙，进不去
                    can_move_in_next = false;
                else
                    can_move_in_next = Move(sm, sx, sy, entering_map, entering_rows - 1, entering_cols / 2, dir, this_step);
            }
            else if (dir == 2) // 向右进入，检测B箱的左边
            {
                if (entering_map->mapTable[entering_rows / 2][0]->get_mark()[0] == '#') // 是墙，进不去
                    can_move_in_next = false;
                else
                    can_move_in_next = Move(sm, sx, sy, entering_map, entering_rows / 2, 0, dir, this_step);
            }
            else if (dir == 3) // 向下进入，检测B箱的上边
            {
                if (entering_map->mapTable[0][entering_cols / 2]->get_mark()[0] == '#') // 是墙，进不去
                    can_move_in_next = false;
                else
                    can_move_in_next = Move(sm, sx, sy, entering_map, 0, entering_cols / 2, dir, this_step);
            }
            else if (dir == 4) // 向左进入，检测B箱的右边
            {
                if (entering_map->mapTable[entering_rows / 2][entering_cols - 1]->get_mark()[0] == '#') // 是墙，进不去
                    can_move_in_next = false;
                else
                    can_move_in_next = Move(sm, sx, sy, entering_map, entering_rows / 2, entering_cols - 1, dir, this_step);
            }
        }
        if (can_move_in_next)//进入了前面的箱子
            return true;
        else // 考虑是否能吞掉后面的块
        {
            if (object->get_mark()[0] != 'B' && object->get_mark()[0] != 'b')
            {
                cout << object->get_mark() << endl;
                return false;
            }
            else
            {
                Map *own_map = name2map[sm->mapTable[sx][sy]->get_mark()];
                int own_rows = own_map->rows;
                int own_cols = own_map->cols;
                if (dir == 1) // 向上吃，检测自己的上边
                {
                    if (own_map->mapTable[0][own_cols / 2]->get_mark()[0] == '#') // 是墙，进不去
                        return false;
                    else if (!Move(dm, dx, dy, own_map, 0, own_cols / 2, 3, this_step)) // 能吃就吃，再走；不能吃直接返回
                        return false;
                }
                else if (dir == 2) // 向右吃，检测自己的右边
                {
                    if (own_map->mapTable[own_rows / 2][own_cols - 1]->get_mark()[0] == '#') // 是墙，进不去
                        return false;
                    else if (!Move(dm, dx, dy, own_map, own_rows / 2, own_cols - 1, 4, this_step)) // 先吃进去，再走；不能吃直接返回
                        return false;
                }
                else if (dir == 3) // 向下吃，检测自己的下边
                {
                    if (own_map->mapTable[own_rows - 1][own_cols / 2]->get_mark()[0] == '#') // 是墙，进不去
                        return false;
                    else if (!Move(dm, dx, dy, own_map, own_rows - 1, own_cols / 2, 1, this_step)) // 先吃进去，再走；不能吃直接返回
                        return false;
                }
                else if (dir == 4) // 向左吃，检测自己的左边
                {
                    if (own_map->mapTable[own_rows / 2][0]->get_mark()[0] == '#') // 是墙，进不去
                        return false;
                    else if (!Move(dm, dx, dy, own_map, own_rows / 2, 0, 2, this_step)) // 先吃进去，再走；不能吃直接返回
                        return false;
                }
            }
        }
    }

    // 单体移动

    // 离开的点的处理
    if (object != p || push_player == false) // 判定是否推到过自己，如果推到过，那么player移动时就不更新离开的点了
    {
        string initOrigElement = sm->initMapTable[sx][sy]->get_mark();
        if (initOrigElement[0] == 'P' || initOrigElement[0] == 'O' ||
            initOrigElement[0] == 'B' || initOrigElement[1] == 'I')
        {
            recorder rec_leave;
            rec_leave.now = sm;
            rec_leave.x = sx;
            rec_leave.y = sy;
            rec_leave.before = sm->mapTable[sx][sy];
            sm->mapTable[sx][sy] = sm->initMapTable[sx][sy]->get_prepared_empty();
            rec_leave.after = sm->mapTable[sx][sy];
            this_step.push_back(rec_leave);
        }
        else
        { // . - =
            recorder rec_leave;
            rec_leave.now = sm;
            rec_leave.x = sx;
            rec_leave.y = sy;
            rec_leave.before = sm->mapTable[sx][sy];
            sm->mapTable[sx][sy] = sm->initMapTable[sx][sy];
            rec_leave.after = sm->mapTable[sx][sy];
            this_step.push_back(rec_leave);
        }
    }
    else
        push_player = false; // 重置该状态，以便下一次使用
    // 进入的点的处理
    string initDestElement = dm->initMapTable[dx][dy]->get_mark();
    if ((initDestElement == "=" && mark[0] == 'P') ||
        (initDestElement == "-" && (mark[0] == 'O' || mark[0] == 'o' || mark[0] == 'B' || mark[0] == 'b' || mark[1] == 'I' || mark[1] == 'i'))) // 玩家进入检查点、箱子进入目标点
    {
        object = object->get_lower();
    }
    else
    {
        object = object->get_upper();
    }
    if (initDestElement == "=" && mark[0] == 'P')
        p = (Player *)p->get_lower();
    else if (initDestElement != "=" && mark[0] == 'p')
        p = (Player *)p->get_upper();
    recorder rec_reach;
    rec_reach.now = dm;
    rec_reach.x = dx;
    rec_reach.y = dy;
    rec_reach.before = dm->mapTable[dx][dy];
    dm->mapTable[dx][dy] = object;
    object->change_pos(make_pair(dx, dy), dm);
    if (object == p)
        currentMap = dm;
    rec_reach.after = dm->mapTable[dx][dy];
    this_step.push_back(rec_reach);
    return true;
}

void Game::undo()
{
    if (record.empty())
    {
        cout << "You are in the initial place, you cannot undo again." << endl;
        return;
    }
    cout << "undo 1 step" << endl;
    vector<recorder> last_step = record.back();
    record.pop_back();
    for (int i = last_step.size() - 1; i >= 0; i--)
    {
        recorder move = last_step[i];
        move.now->mapTable[move.x][move.y] = move.before;
        move.before->change_pos(make_pair(move.x, move.y), move.now);
        if (move.before == p)
            currentMap = move.now;
    }
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
