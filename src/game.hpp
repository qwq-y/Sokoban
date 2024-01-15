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
    int num_of_inplace_box;
    map<Map *, string> map2name;
    map<string, Map *> name2map;
    vector<Bbox *> B_boxs;
    vector<Ibox *> inf_boxs;
    vector<vector<recorder>> record;
    Player *p;

public:
    Game(Player *p, map<Map *, string> map2name, map<string, Map *> name2map, vector<Bbox *> B_boxs, vector<Ibox *> inf_boxs) : p(p), map2name(map2name), name2map(name2map), B_boxs(B_boxs), inf_boxs(inf_boxs)
    {
        currentMap = p->get_map();
    }

    void handlePlayerMove(char userInput);

    bool checkWinCondition() const;

    void printState();

    bool Move(Map *sm, int sx, int sy, Map *dm, int dx, int dy, int dir,vector<recorder> &this_step);

    void undo();
};

#endif // PROJ_GAME_H
