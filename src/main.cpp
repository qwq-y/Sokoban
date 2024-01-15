#include <string>
#include <iostream>
#include <map>
#include "map.hpp"
#include "game.hpp"

using namespace std;

extern map<string, string> map2box_name;
extern vector<string *> level[9];
extern void build_map2box_name();

int main()
{
    build_map2box_name();
    int lev = 0;
    cout << "Please choose the level(1~8):";
    cin >> lev;
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
    char userInput;
    cin.ignore();
    while (true)
    {
        game->printState();
        cout << "Enter your move (w/a/s/d/z) or quit (q):";
        userInput = cin.get();
        cin.ignore(); // 忽略换行符
        userInput = tolower(userInput);
        // 处理无效输入
        if (!(userInput == 'w' || userInput == 'a' || userInput == 's' || userInput == 'd' || userInput == 'q' || userInput == 'z'))
        {
            std::cout << "Invalid input. Please enter 'w', 'a', 's', 'd', 'z', or 'q'." << std::endl;
            continue; // 继续循环，等待有效输入
        }
        if (userInput == 'q')
        { // 使用 q 退出
            cout << "\nQuit.\n";
            break;
        }
        else
        {
            game->handlePlayerMove(userInput);
            if (game->checkWinCondition())
            {
                cout << "\nYou won!\n";
                break;
            }
        }
    }

    return 0;
}
