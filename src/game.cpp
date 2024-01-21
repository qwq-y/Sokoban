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
    for (const auto &print : map2name)
    {
        if (print.first == currentMap)
            continue;
        cout << map2name[print.first] << ":" << endl;
        cout << endl;
        print.first->printMap();
        cout << endl;
    }
    cout << map2name[currentMap] << ":" << endl;
    cout << endl;
    currentMap->printMap();
    cout << endl;
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
    case 'r':
        reset();
        return;
    default:
        cout << "Invalid input. Please use 'w', 's', 'a', 'd', 'z' or 'r'" << endl;
        return;
    }

    int newRow = p->get_pos().first + rowMove;
    int newCol = p->get_pos().second + colMove;
    vector<recorder> this_step;
    vector<Entity *> entities;
    push_itself = false;
    itself = nullptr;
    Move(currentMap, p->get_pos().first, p->get_pos().second, currentMap, newRow, newCol, direction, this_step, 0, false, 0, 0, entities);
    if (!this_step.empty())
        record.push_back(this_step);
}

bool Game::Move(Map *sm, int sx, int sy, Map *dm, int dx, int dy, int dir, vector<recorder> &this_step, int status, bool open_void, int inf_layer, int epi_layer, vector<Entity *> &entities) // dir: 1上 2右 3下 4 左,status:-1出，0同地图，1入
{
    //cout << "try move " << map2name[sm] << " " << sx << " " << sy << " into " << map2name[dm] << " " << dx << " " << dy << " direction:" << dir << " " << epi_layer << endl;
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
        bool find = false;
        for (Bbox *bbox : B_boxs)
            if (bbox->get_mark()[1] == mark[1])
            {
                find = true;
                outer_map = bbox->get_map();
                outer_x = bbox->get_pos().first + xMove;
                outer_y = bbox->get_pos().second + yMove;
                break;
            }
        if (!find && mark.size() == 3)
            for (Epsilon *epsilon : epi_boxs)
            {
                if (epsilon->get_mark()[2] == mark[2])
                {
                    find = true;
                    outer_map = epsilon->get_map();
                    outer_x = epsilon->get_pos().first + xMove;
                    outer_y = epsilon->get_pos().second + yMove;
                    break;
                }
            }
        if (find) // 核查是否无限
        {
            if ((outer_x < 0 || outer_y < 0 || outer_x >= outer_map->rows || outer_y >= outer_map->cols) && outer_map == dm) // 再次出界且地图没变,加一重无限
            {
                find = false;
                for (Ibox *ibox : inf_boxs)
                    if (ibox->get_belong_info().first == mark && ibox->get_belong_info().second == inf_layer + 1)
                    {
                        find = true;
                        outer_map = ibox->get_map();
                        outer_x = ibox->get_pos().first + xMove;
                        outer_y = ibox->get_pos().second + yMove;
                        break;
                    }
                if (!find)
                    mark = to_string(inf_layer + 1) + "I" + mark[1];
                else
                    return Move(sm, sx, sy, outer_map, outer_x, outer_y, dir, this_step, -1, false, inf_layer + 1, 0, entities);
            }
        }
        if (!find) // 出到虚空了，创建虚空
        {
            if (have_void)
                return Move(sm, sx, sy, name2map["VOID"], 4 + xMove, 4 + yMove, dir, this_step, -1, false, inf_layer + 1, 0, entities);
            string voidStr = make_void_str(mark);
            Map *void_map = new Map(p, voidStr, B_boxs, inf_boxs, epi_boxs);
            map2name[void_map] = "VOID";
            name2map["VOID"] = void_map;
            outer_map = void_map;
            outer_x = 4 + xMove;
            outer_y = 4 + yMove;
            have_void = true;
            return Move(sm, sx, sy, outer_map, outer_x, outer_y, dir, this_step, -1, true, inf_layer + 1, 0, entities);
        }
        return Move(sm, sx, sy, outer_map, outer_x, outer_y, dir, this_step, -1, false, 0, 0, entities);
    }
    else if (dm->mapTable[dx][dy]->get_mark()[0] == '#') // 撞墙
        return false;
    for (Entity *entity : entities)
        if (entity == dm->mapTable[dx][dy]) // 贪吃蛇出现
        {
            itself = entity;
            push_itself = true;
            break;
        }
    if (!push_itself && (dm->mapTable[dx][dy]->get_mark()[0] == 'O' || dm->mapTable[dx][dy]->get_mark()[0] == 'o' || dm->mapTable[dx][dy]->get_mark()[0] == 'B' || dm->mapTable[dx][dy]->get_mark()[0] == 'b' || dm->mapTable[dx][dy]->get_mark()[1] == 'I' || dm->mapTable[dx][dy]->get_mark()[1] == 'i' || dm->mapTable[dx][dy]->get_mark()[1] == 'E' || dm->mapTable[dx][dy]->get_mark()[1] == 'e')) // 前方还有箱子
    {
        entities.push_back(sm->mapTable[sx][sy]);
        can_move = Move(dm, dx, dy, dm, dx + xMove, dy + yMove, dir, this_step, 0, false, 0, 0, entities);
    }
    if (!can_move && !push_itself) // 前面的块不能再往前,且后面没有出现贪吃蛇
    {
        if (map2name[sm] != "VOID") // 虚空无法进入任何块内
        {
            bool can_move_in_next = false; // 记录该块是否可以进入后面的块
            // 先看能否进后面的块，不行再考虑自己能不能吞掉后面的块
            if (dm->mapTable[dx][dy]->get_mark()[0] == 'B' || dm->mapTable[dx][dy]->get_mark()[0] == 'b') // 是可进入箱子
            {
                string name = dm->mapTable[dx][dy]->get_mark();
                if (name[1] == 'C') // 进入克隆盒
                    name = "B" + name.substr(2, 1);
                else
                    name = "B" + name.substr(1, 1);
                Map *entering_map = name2map[name];
                int entering_rows = entering_map->rows;
                int entering_cols = entering_map->cols;
                if (dir == 1) // 向上进入，检测B箱的下边
                {
                    int x = entering_rows - 1;
                    int y = (entering_cols - 1) / 2;
                    if (status == -1) // 刚出就进，说明是相邻的两个箱子的slide into,不需按中点规则进入
                        if (sm->cols >= entering_cols)
                            y = (sy + sy + 1) * (entering_cols) / (sm->cols * 2);
                        else
                            y = round((sy + sy + 1) * entering_cols / (2 * sm->cols));
                    if (entering_map->mapTable[x][y]->get_mark()[0] == '#') // 是墙，进不去
                        can_move_in_next = false;
                    else if (((entering_map == dm) || (epi_layer && entering_map == name2map["B" + map2name[dm].substr(2, 1)])) && x == dx && y == dy) // 无限进入，进入1-epsilon
                    {
                        string epi_mark = to_string(epi_layer + 1) + "E" + map2name[entering_map][1];
                        int epi_x = name2map[epi_mark]->rows - 1;
                        int epi_y = (name2map[epi_mark]->cols - 1) / 2;
                        can_move_in_next = Move(sm, sx, sy, name2map[epi_mark], epi_x, epi_y, dir, this_step, 1, false, 0, epi_layer + 1, entities);
                    }
                    else
                        can_move_in_next = Move(sm, sx, sy, entering_map, x, y, dir, this_step, 1, false, 0, 0, entities);
                }
                else if (dir == 2) // 向右进入，检测B箱的左边
                {
                    int x = (entering_rows - 1) / 2;
                    int y = 0;
                    if (status == -1) // 刚出就进，说明是相邻的两个箱子的slide into,不需按中点规则进入
                        if (sm->rows >= entering_rows)
                            x = (sx + sx + 1) * (entering_rows) / (sm->rows * 2);
                        else
                            x = round((sx + sx + 1) * entering_rows / (2 * sm->rows));
                    if (entering_map->mapTable[x][y]->get_mark()[0] == '#') // 是墙，进不去
                        can_move_in_next = false;
                    else if (((entering_map == dm) || (epi_layer && entering_map == name2map["B" + map2name[dm].substr(2, 1)])) && x == dx && y == dy) // 无限进入，进入epsilon
                    {
                        string epi_mark = to_string(epi_layer + 1) + "E" + map2name[entering_map][1];
                        int epi_x = (name2map[epi_mark]->rows - 1) / 2;
                        int epi_y = 0;
                        can_move_in_next = Move(sm, sx, sy, name2map[epi_mark], epi_x, epi_y, dir, this_step, 1, false, 0, epi_layer + 1, entities);
                    }
                    else
                        can_move_in_next = Move(sm, sx, sy, entering_map, x, y, dir, this_step, 1, false, 0, 0, entities);
                }
                else if (dir == 3) // 向下进入，检测B箱的上边
                {
                    int x = 0;
                    int y = (entering_cols - 1) / 2;
                    if (status == -1) // 刚出就进，说明是相邻的两个箱子的slide into,不需按中点规则进入
                        if (sm->cols >= entering_cols)
                            y = (sy + sy + 1) * (entering_cols) / (sm->cols * 2);
                        else
                            y = round((sy + sy + 1) * entering_cols / (2 * sm->cols));
                    if (entering_map->mapTable[x][y]->get_mark()[0] == '#') // 是墙，进不去
                        can_move_in_next = false;
                    else if (((entering_map == dm) || (epi_layer && entering_map == name2map["B" + map2name[dm].substr(2, 1)])) && x == dx && y == dy) // 无限进入，进入epsilon
                    {
                        string epi_mark = to_string(epi_layer + 1) + "E" + map2name[entering_map][1];
                        int epi_x = 0;
                        int epi_y = (name2map[epi_mark]->cols - 1) / 2;
                        can_move_in_next = Move(sm, sx, sy, name2map[epi_mark], epi_x, epi_y, dir, this_step, 1, false, 0, epi_layer + 1, entities);
                    }
                    else
                        can_move_in_next = Move(sm, sx, sy, entering_map, x, y, dir, this_step, 1, false, 0, 0, entities);
                }
                else if (dir == 4) // 向左进入，检测B箱的右边
                {
                    int x = (entering_rows - 1) / 2;
                    int y = entering_cols - 1;
                    if (status == -1) // 刚出就进，说明是相邻的两个箱子的slide into,不需按中点规则进入
                        if (sm->rows >= entering_rows)
                            x = (sx + sx + 1) * (entering_rows) / (sm->rows * 2);
                        else
                            x = round((sx + sx + 1) * entering_rows / (2 * sm->rows));
                    if (entering_map->mapTable[x][y]->get_mark()[0] == '#') // 是墙，进不去
                        can_move_in_next = false;
                    else if (((entering_map == dm) || (epi_layer && entering_map == name2map["B" + map2name[dm].substr(2, 1)])) && x == dx && y == dy) // 无限进入，进入epsilon
                    {
                        string epi_mark = to_string(epi_layer + 1) + "E" + map2name[entering_map][1];
                        int epi_x = (name2map[epi_mark]->rows - 1) / 2;
                        int epi_y = name2map[epi_mark]->cols - 1;
                        can_move_in_next = Move(sm, sx, sy, name2map[epi_mark], epi_x, epi_y, dir, this_step, 1, false, 0, epi_layer + 1, entities);
                    }
                    else
                        can_move_in_next = Move(sm, sx, sy, entering_map, x, y, dir, this_step, 1, false, 0, 0, entities);
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
                        else if (!Move(dm, dx, dy, own_map, 0, (own_cols - 1) / 2, 3, this_step, 1, false, 0, 0, entities)) // 能吃就吃，再走；不能吃直接返回
                            return false;
                    }
                    else if (dir == 2) // 向右吃，检测自己的右边
                    {
                        if (own_map->mapTable[(own_rows - 1) / 2][own_cols - 1]->get_mark()[0] == '#') // 是墙，进不去
                            return false;
                        else if (!Move(dm, dx, dy, own_map, (own_rows - 1) / 2, own_cols - 1, 4, this_step, 1, false, 0, 0, entities)) // 先吃进去，再走；不能吃直接返回
                            return false;
                    }
                    else if (dir == 3) // 向下吃，检测自己的下边
                    {
                        if (own_map->mapTable[own_rows - 1][(own_cols - 1) / 2]->get_mark()[0] == '#') // 是墙，进不去
                            return false;
                        else if (!Move(dm, dx, dy, own_map, own_rows - 1, (own_cols - 1) / 2, 1, this_step, 1, false, 0, 0, entities)) // 先吃进去，再走；不能吃直接返回
                            return false;
                    }
                    else if (dir == 4) // 向左吃，检测自己的左边
                    {
                        if (own_map->mapTable[(own_rows - 1) / 2][0]->get_mark()[0] == '#') // 是墙，进不去
                            return false;
                        else if (!Move(dm, dx, dy, own_map, (own_rows - 1) / 2, 0, 2, this_step, 1, false, 0, 0, entities)) // 先吃进去，再走；不能吃直接返回
                            return false;
                    }
                }
            }
        }
        else
            return false;
    }
    else if (!can_move && push_itself) /// 不能动，但是后面贪吃蛇了，那么此块不动
        return false;

    // 单体移动
    if (object == p)
        step_rec.push_back(dir);
    // 离开的点的处理
    if (object != itself || !push_itself) // 判定是否贪吃蛇，如果贪吃蛇，判断自己是不是蛇尾，如果是，则不更新离开的点
    {
        string initOrigElement = sm->initMapTable[sx][sy]->get_mark();
        if (initOrigElement[0] == 'P' || initOrigElement[0] == 'p' ||
            initOrigElement[0] == 'O' || initOrigElement[0] == 'o' ||
            initOrigElement[0] == 'B' || initOrigElement[0] == 'b' ||
            initOrigElement[1] == 'I' || initOrigElement[1] == 'i' ||
            initOrigElement[1] == 'E' || initOrigElement[1] == 'e')
        {
            recorder rec_leave;
            rec_leave.now = sm;
            rec_leave.x = sx;
            rec_leave.y = sy;
            rec_leave.before = sm->mapTable[sx][sy];
            if (mark[0] == 'o' || mark[0] == 'b' || mark[0] == 'p' || mark[1] == 'i')
                rec_leave.lower = true;
            sm->mapTable[sx][sy] = sm->initMapTable[sx][sy]->get_prepared_empty();
            rec_leave.openVoid = open_void;
            this_step.push_back(rec_leave);
        }
        else
        { // . - =
            recorder rec_leave;
            rec_leave.now = sm;
            rec_leave.x = sx;
            rec_leave.y = sy;
            rec_leave.before = sm->mapTable[sx][sy];
            rec_leave.openVoid = open_void;
            if (mark[0] == 'o' || mark[0] == 'b' || mark[0] == 'p' || mark[1] == 'i')
                rec_leave.lower = true;
            sm->mapTable[sx][sy] = sm->initMapTable[sx][sy];
            this_step.push_back(rec_leave);
        }
    }
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
    if (push_itself && itself == object) // 返回false，但不重置状态，保留push_itself供之前的块识别，避免因为返回false而被前面的块吃
        return false;
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

        if (move.openVoid) // 这一步创建的虚空，所以需要销毁
        {
            Map *void_map = name2map["VOID"];
            string mark = void_map->initMapTable[4][4]->get_mark();
            if (mark[0] == 'B' || mark[0] == 'b')
                B_boxs.erase(
                    remove_if(B_boxs.begin(), B_boxs.end(), [mark](const Bbox *bboxPtr)
                              { return bboxPtr->get_mark() == mark; }),
                    B_boxs.end());
            else if (mark[1] == 'I' || mark[1] == 'i')
                inf_boxs.erase(
                    remove_if(inf_boxs.begin(), inf_boxs.end(), [mark](const Ibox *iboxPtr)
                              { return iboxPtr->get_mark() == mark; }),
                    inf_boxs.end());
            map<string, Map *>::iterator it1 = name2map.find("VOID");
            map<Map *, string>::iterator it2 = map2name.find(void_map);
            if (it1 != name2map.end())
                name2map.erase(it1);
            if (it2 != map2name.end())
                map2name.erase(it2);
            have_void = false;
        }
        if (move.clearVoid) // 这一步是reset，消除过虚空，undo这一步需要复原虚空。之前的map并没有消失，而是存在了这一个recorder的now里，所以直接将地图加回两个map里，再把虚空中心的箱子加回B_boxs或者inf_boxs即可
        {
            Map *void_map = move.now;
            have_void = true;
            map2name[void_map] = "VOID";
            name2map["VOID"] = void_map;
            if (void_map->initMapTable[4][4]->get_mark()[0] == 'B' || void_map->initMapTable[4][4]->get_mark()[0] == 'b')
                B_boxs.push_back((Bbox *)void_map->initMapTable[4][4]);
            else if (void_map->initMapTable[4][4]->get_mark()[1] == 'I' || void_map->initMapTable[4][4]->get_mark()[1] == 'i')
                inf_boxs.push_back((Ibox *)void_map->initMapTable[4][4]);
        }
    }
}

void Game::reset()
{
    vector<recorder> this_step;
    for (const auto &print : map2name)
    {
        Map *now = print.first;
        for (int i = 0; i < now->mapTable.size(); i++)
        {
            for (int j = 0; j < now->mapTable[i].size(); j++)
            {
                if (now->mapTable[i][j] != now->initMapTable[i][j])
                {
                    recorder rec;
                    rec.now = now;
                    rec.x = i;
                    rec.y = j;
                    rec.before = now->mapTable[i][j];
                    if (rec.before->get_mark()[0] == 'o' || rec.before->get_mark()[0] == 'b' || rec.before->get_mark()[1] == 'i' || rec.before->get_mark()[0] == 'p')
                        rec.lower = true;
                    this_step.push_back(rec);
                    if (now->mapTable[i][j]->get_mark()[0] == 'o' || now->mapTable[i][j]->get_mark()[0] == 'b' || now->mapTable[i][j]->get_mark()[0] == 'p' || now->mapTable[i][j]->get_mark()[1] == 'i')
                        now->mapTable[i][j]->set_upper();
                    now->mapTable[i][j] = now->initMapTable[i][j];
                    now->mapTable[i][j]->change_pos(make_pair(i, j), now);
                }
            }
        }
        if (have_void && map2name[now] == "VOID")
            this_step[this_step.size() - 1].clearVoid = true;
    }
    if (have_void)
    {
        have_void = false;
        Map *void_map = name2map["VOID"];
        string mark = void_map->initMapTable[4][4]->get_mark();
        if (mark[0] == 'B' || mark[0] == 'b')
            B_boxs.erase(
                remove_if(B_boxs.begin(), B_boxs.end(), [mark](const Bbox *bboxPtr)
                          { return bboxPtr->get_mark() == mark; }),
                B_boxs.end());
        else if (mark[1] == 'I' || mark[1] == 'i')
            inf_boxs.erase(
                remove_if(inf_boxs.begin(), inf_boxs.end(), [mark](const Ibox *iboxPtr)
                          { return iboxPtr->get_mark() == mark; }),
                inf_boxs.end());
        map<string, Map *>::iterator it1 = name2map.find("VOID");
        map<Map *, string>::iterator it2 = map2name.find(void_map);
        if (it1 != name2map.end())
            name2map.erase(it1);
        if (it2 != map2name.end())
            map2name.erase(it2);
    }
    record.push_back(this_step);
    currentMap = p->get_map();
}

string Game::make_void_str(string mark)
{
    string void_map;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
        {
            if (i == 4 && j == 4)
                void_map += mark;
            else if (i == 0 || j == 0 || i == 8 || j == 8)
                void_map += "#";
            else
                void_map += ".";
            if (j == 8)
                void_map += "\n";
            else
                void_map += " ";
        }
    return void_map;
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
                if (initElement->get_mark()[0] == '-' && (currentElement->get_mark()[1] != 'i' && currentElement->get_mark()[0] != 'b' && currentElement->get_mark()[0] != 'o'))
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
