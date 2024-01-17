//
// Created by 86188 on 2023/12/26.
//

#include <iostream>
#include <map>
#include <cmath>
#include "game.hpp"

using namespace std;

void Game::printState()
{
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
    Move(currentMap, p->get_pos().first, p->get_pos().second, currentMap, newRow, newCol, direction, this_step, 0);
    if (!this_step.empty())
        record.push_back(this_step);
}

bool Game::Move(Map *sm, int sx, int sy, Map *dm, int dx, int dy, int dir, vector<recorder> &this_step, int status) // dir: 1上 2右 3下 4 左,status:-1出，0同地图，1入
{
    // cout << "try move " << map2name[sm] << " " << sx << " " << sy << " into " << map2name[dm] << " " << dx << " " << dy << " direction:" << dir << endl;
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
            if (bbox->get_mark()[1] == mark[1])
            {
                outer_map = bbox->get_map();
                outer_x = bbox->get_pos().first + xMove;
                outer_y = bbox->get_pos().second + yMove;
                break;
            }
        return Move(sm, sx, sy, outer_map, outer_x, outer_y, dir, this_step, -1);
    }
    else if (dm->mapTable[dx][dy]->get_mark()[0] == '#') // 撞墙
        return false;
    else if (dm->mapTable[dx][dy]->get_mark()[0] == 'O' || dm->mapTable[dx][dy]->get_mark()[0] == 'o' || dm->mapTable[dx][dy]->get_mark()[0] == 'B' || dm->mapTable[dx][dy]->get_mark()[0] == 'b' || dm->mapTable[dx][dy]->get_mark()[1] == 'I' || dm->mapTable[dx][dy]->get_mark()[1] == 'i') // 前方还有箱子
        can_move = Move(dm, dx, dy, dm, dx + xMove, dy + yMove, dir, this_step, 0);
    else if (dm->mapTable[dx][dy] == p) // 推到自己了
        push_player = true;

    if (!can_move) // 前面的块不能再往前
    {
        bool can_move_in_next = false; // 记录该块是否可以进入前面的块

        // 先看能否进后面的块，不行再考虑自己能不能吞掉后面的块
        if (dm->mapTable[dx][dy]->get_mark()[0] == 'B' || dm->mapTable[dx][dy]->get_mark()[0] == 'b') // 是可进入箱子
        {
            string name = dm->mapTable[dx][dy]->get_mark();
            if (name[1] == 'C') // 进入克隆盒
                name = "B" + name.substr(2, 1);
            Map *entering_map = name2map[name];
            int entering_rows = entering_map->rows;
            int entering_cols = entering_map->cols;

            if (dir == 1) // 向上进入，检测B箱的下边
            {
                int x = entering_rows - 1;
                int y = (entering_cols - 1) / 2;
                if (status == -1) // 刚出就进，说明是相邻的两个箱子的slide into,不需按中点规则进入
                    if (sm->cols >= entering_cols)
                        y = sy * (entering_cols) / (sm->cols - 1);
                    else
                        y = round((sy + sy + 1) * entering_cols / (2 * sm->cols));
                if (entering_map->mapTable[x][y]->get_mark()[0] == '#') // 是墙，进不去
                    can_move_in_next = false;
                else
                    can_move_in_next = Move(sm, sx, sy, entering_map, x, y, dir, this_step, 1);
            }
            else if (dir == 2) // 向右进入，检测B箱的左边
            {
                int x = (entering_rows - 1) / 2;
                int y = 0;
                if (status == -1) // 刚出就进，说明是相邻的两个箱子的slide into,不需按中点规则进入
                    if (sm->rows >= entering_rows)
                        x = sx * (entering_rows) / (sm->rows - 1);
                    else
                        x = round((sx + sx + 1) * entering_rows / (2 * sm->rows));
                if (entering_map->mapTable[x][y]->get_mark()[0] == '#') // 是墙，进不去
                    can_move_in_next = false;
                else
                    can_move_in_next = Move(sm, sx, sy, entering_map, x, y, dir, this_step, 1);
            }
            else if (dir == 3) // 向下进入，检测B箱的上边
            {
                int x = 0;
                int y = (entering_cols - 1) / 2;
                if (status == -1) // 刚出就进，说明是相邻的两个箱子的slide into,不需按中点规则进入
                    if (sm->cols >= entering_cols)
                        y = sy * (entering_cols) / (sm->cols - 1);
                    else
                        y = round((sy + sy + 1) * entering_cols / (2 * sm->cols));
                if (entering_map->mapTable[x][y]->get_mark()[0] == '#') // 是墙，进不去
                    can_move_in_next = false;
                else
                    can_move_in_next = Move(sm, sx, sy, entering_map, x, y, dir, this_step, 1);
            }
            else if (dir == 4) // 向左进入，检测B箱的右边
            {
                int x = (entering_rows - 1) / 2;
                int y = entering_cols - 1;
                if (status == -1) // 刚出就进，说明是相邻的两个箱子的slide into,不需按中点规则进入
                    if (sm->rows >= entering_rows)
                        x = sx * (entering_rows) / (sm->rows - 1);
                    else
                        x = round((sx + sx + 1) * entering_rows / (2 * sm->rows));
                if (entering_map->mapTable[x][y]->get_mark()[0] == '#') // 是墙，进不去
                    can_move_in_next = false;
                else
                    can_move_in_next = Move(sm, sx, sy, entering_map, x, y, dir, this_step, 1);
            }
        }
        if (can_move_in_next) // 进入了前面的箱子
            return true;
        else // 考虑是否能吞掉后面的块
        {
            if (object->get_mark()[0] != 'B' && object->get_mark()[0] != 'b')
                return false;
            else
            {
                string name = sm->mapTable[sx][sy]->get_mark();
                if (name[1] == 'C') // 自己是克隆盒
                    name = "B" + name.substr(2, 1);
                Map *own_map = name2map[name];
                int own_rows = own_map->rows;
                int own_cols = own_map->cols;
                if (dir == 1) // 向上吃，检测自己的上边
                {
                    if (own_map->mapTable[0][(own_cols - 1) / 2]->get_mark()[0] == '#') // 是墙，进不去
                        return false;
                    else if (!Move(dm, dx, dy, own_map, 0, (own_cols - 1) / 2, 3, this_step, 1)) // 能吃就吃，再走；不能吃直接返回
                        return false;
                }
                else if (dir == 2) // 向右吃，检测自己的右边
                {
                    if (own_map->mapTable[(own_rows - 1) / 2][own_cols - 1]->get_mark()[0] == '#') // 是墙，进不去
                        return false;
                    else if (!Move(dm, dx, dy, own_map, (own_rows - 1) / 2, own_cols - 1, 4, this_step, 1)) // 先吃进去，再走；不能吃直接返回
                        return false;
                }
                else if (dir == 3) // 向下吃，检测自己的下边
                {
                    if (own_map->mapTable[own_rows - 1][(own_cols - 1) / 2]->get_mark()[0] == '#') // 是墙，进不去
                        return false;
                    else if (!Move(dm, dx, dy, own_map, own_rows - 1, (own_cols - 1) / 2, 1, this_step, 1)) // 先吃进去，再走；不能吃直接返回
                        return false;
                }
                else if (dir == 4) // 向左吃，检测自己的左边
                {
                    if (own_map->mapTable[(own_rows - 1) / 2][0]->get_mark()[0] == '#') // 是墙，进不去
                        return false;
                    else if (!Move(dm, dx, dy, own_map, (own_rows - 1) / 2, 0, 2, this_step, 1)) // 先吃进去，再走；不能吃直接返回
                        return false;
                }
            }
        }
    }

    // 单体移动
    if(object == p)
        step_rec.push_back(dir);
    // 离开的点的处理
    if (object != p || push_player == false) // 判定是否推到过自己，如果推到过，那么player移动时就不更新离开的点了
    {
        string initOrigElement = sm->initMapTable[sx][sy]->get_mark();
        if (initOrigElement[0] == 'P' || initOrigElement[0] == 'p' ||
            initOrigElement[0] == 'O' || initOrigElement[0] == 'o' ||
            initOrigElement[0] == 'B' || initOrigElement[0] == 'b' ||
            initOrigElement[1] == 'I' || initOrigElement[1] == 'i')
        {
            recorder rec_leave;
            rec_leave.now = sm;
            rec_leave.x = sx;
            rec_leave.y = sy;
            rec_leave.before = sm->mapTable[sx][sy];
            if (mark[0] == 'o' || mark[0] == 'b' || mark[0] == 'p' || mark[1] == 'i')
                rec_leave.lower = true;
            sm->mapTable[sx][sy] = sm->initMapTable[sx][sy]->get_prepared_empty();
            this_step.push_back(rec_leave);
        }
        else
        { // . - =
            recorder rec_leave;
            rec_leave.now = sm;
            rec_leave.x = sx;
            rec_leave.y = sy;
            rec_leave.before = sm->mapTable[sx][sy];
            if (mark[0] == 'o' || mark[0] == 'b' || mark[0] == 'p' || mark[1] == 'i')
                rec_leave.lower = true;
            sm->mapTable[sx][sy] = sm->initMapTable[sx][sy];
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
        object->set_lower();
    }
    else
    {
        object->set_upper();
    }
    recorder rec_reach;
    rec_reach.now = dm;
    rec_reach.x = dx;
    rec_reach.y = dy;
    rec_reach.before = dm->mapTable[dx][dy];
    if (rec_reach.before->get_mark()[0] == 'o' || rec_reach.before->get_mark()[0] == 'b' || rec_reach.before->get_mark()[0] == 'p' || rec_reach.before->get_mark()[1] == 'i')
        rec_reach.lower = true;
    dm->mapTable[dx][dy] = object;
    object->change_pos(make_pair(dx, dy), dm);
    if (object == p)
        currentMap = dm;
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
    step_rec.pop_back();
    for (int i = last_step.size() - 1; i >= 0; i--)
    {
        recorder move = last_step[i];
        move.now->mapTable[move.x][move.y] = move.before;
        move.before->change_pos(make_pair(move.x, move.y), move.now);
        if (move.lower)
            move.before->set_lower();
        else
            move.before->set_upper();
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
