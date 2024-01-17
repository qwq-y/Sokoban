//
// Created by 86188 on 2023/12/26.
//

#ifndef PROJ_GAME_H
#define PROJ_GAME_H

#include "map.hpp"
#include "recorder.hpp"
using namespace std;
class Game
{
private:
    Map *currentMap;
    map<Map *, string> map2name;
    map<string, Map *> name2map;
    vector<Bbox *> B_boxs;
    vector<Ibox *> inf_boxs;
    vector<vector<recorder>> record;
    vector<int> step_rec;
    Player *p;
    bool push_player = false; // 记录是否推到了自己

public:
    Game(Player *p, map<Map *, string> map2name, map<string, Map *> name2map, vector<Bbox *> B_boxs, vector<Ibox *> inf_boxs) : p(p), map2name(map2name), name2map(name2map), B_boxs(B_boxs), inf_boxs(inf_boxs)
    {
        currentMap = p->get_map();
    }

    void handlePlayerMove(char userInput);

    bool checkWinCondition() const;

    void printState();

    bool Move(Map *sm, int sx, int sy, Map *dm, int dx, int dy, int dir, vector<recorder> &this_step, int status);

    void undo();

    vector<int> get_step_rec() const
    {
        return step_rec;
    }

    vector<vector<recorder>> get_record() const
    {
        return record;
    }

    map<Map *, string> get_map2name() const
    {
        return map2name;
    }

    void set_step_rec(vector<int> steps)
    {
        step_rec = steps;
    }
};

#endif // PROJ_GAME_H
