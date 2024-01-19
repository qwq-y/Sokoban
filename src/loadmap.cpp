#include <map>
#include "map.hpp"
#include "game.hpp"
using namespace std;

extern vector<string *> level[20];
extern map<string, string> map2box_name;
extern int lev;

Game *initial_game()
{
    map<Map *, string> map2name;
    map<string, Map *> name2map;
    vector<Bbox *> B_boxs;
    vector<Ibox *> inf_boxs;
    Player *p;
    for (string *mapStr : level[lev])
    {
        Map *map = new Map(p, *mapStr, B_boxs, inf_boxs);
        map2name[map] = map2box_name[*mapStr];
        name2map[map2box_name[*mapStr]] = map;
    }
    Game *game = new Game(p, map2name, name2map, B_boxs, inf_boxs);
    return game;
}