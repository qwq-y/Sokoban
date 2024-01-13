#include <string>
#include <map>
#include <vector>
using namespace std;
map<string,string> map2box_name;
vector<string*> level[9];

string L1MStr = "# # # # # # # #\n"
                "# . . . . = . #\n"
                "# . . . . - - #\n"
                "# . . # . # # #\n"
                "# . O . O . . #\n"
                "# . . . P . . #\n"
                "# # # # # # # #\n";

string L2MStr = "# # # # # # # # #\n"
                "# . . = . . # - #\n"
                "# . . . . . . . #\n"
                "# . . . . . # # #\n"
                "# . B1 . O . . . #\n"
                "# . . . . . . . #\n"
                "# . P . . . . . #\n"
                "# . . . # # . . #\n"
                "# # # # # # # # #\n";

string L2B1Str = "# # . # #\n"
                 "# # . # #\n"
                 ". . . # #\n"
                 "# # # # #\n"
                 "# # # # #\n";

string L3B1Str = "# # # # # # #\n"
                 "# . . . # . .\n"
                 "# B1 B2 . # P .\n"
                 "# . . . # . .\n"
                 "# = - . # . .\n"
                 "# # # # # # #\n";

string L3B2Str = ". . .\n"
                 "O . .\n"
                 ". . .\n";

string L4B1Str = "# . . . #\n"
                 "# P . B2 #\n"
                 "# . . . #\n"
                 "# # # # #\n";

string L4B2Str = "B3 # . . #\n"
                 ". . . . #\n"
                 "# # # # #\n";

string L4B3Str = "# # . # #\n"
                 ". . . . #\n"
                 ". . . . #\n"
                 ". . O - #\n"
                 "# # # # #\n";

string L4VoidStr = ". . . . .\n"
                      ". . . . .\n"
                      ". . = . .\n"
                      ". . . . .\n"
                      ". . . . .\n";

string L5B1Str = "# # # # # # # # #\n"
                 "# B1 O B2 B3 O O P O\n"
                 "# # . # # # . . #\n"
                 "# # . . . . . . #\n"
                 "# # . . . . . . #\n"
                 "# # . . . . . . #\n"
                 "# # # # # # # # #\n";

string L5B2Str = "# # #\n"
                 "# - #\n"
                 "# O #\n";

string L5B3Str = "# # #\n"
                 "# = #\n"
                 "# . #\n";

string L6B1Str = "# # # # # # # #\n"
                 "# # # # B4 . . #\n"
                 "# . . . # . . #\n"
                 "# P B2 . # . . #\n"
                 "# . . . # . = #\n"
                 "# # # # # # # #\n";

string L6B2Str = "# # # # # # #\n"
                 "# . . . . . #\n"
                 "# . . B3 . . #\n"
                 ". . . O . . #\n"
                 "# . B4 . . . #\n"
                 "# . . . . . #\n"
                 "# # # # # # #\n";

string L6B3Str = "# # #\n"
                 "# - #\n"
                 "# - #\n";

string L6B4Str = "# # # #\n"
                 "# . . .\n"
                 "# . . .\n";

string L7B1Str = "# # # # # # # # # # #\n"
                 "# . . . . # # # # # #\n"
                 "# . . . . # # # # # #\n"
                 "# . . . . . . . . . #\n"
                 "# . 1I1 . . # . . . . #\n"
                 "# . . . . # . . . . #\n"
                 "# . B1 . . # . - - = #\n"
                 "# . . . . # # # # # #\n"
                 "# . P . . . . . . . #\n"
                 "# . . . . . . . . . #\n"
                 "# . . . . . . . . . #\n";

string L8B1Str = "# . . . . . . . #\n"
                 "# . . . . . . . #\n"
                 "# B1 . . P . . 1I1 #\n"
                 "# . . - = - . . #\n"
                 "# . . . . . . . #\n"
                 "# . . . . . . . #\n"
                 "# . . # 2I1 . . . #\n"
                 "# # # # # # # # #\n";

void build_map2box_name()
{
    level[1].push_back(&L1MStr);
    level[2].push_back(&L2MStr);
    level[2].push_back(&L2B1Str);
    level[3].push_back(&L3B1Str);
    level[3].push_back(&L3B2Str);
    level[4].push_back(&L4B1Str);
    level[4].push_back(&L4B2Str);
    level[4].push_back(&L4B3Str);
    level[4].push_back(&L4VoidStr);
    level[5].push_back(&L5B1Str);
    level[5].push_back(&L5B2Str);
    level[5].push_back(&L5B3Str);
    level[6].push_back(&L6B1Str);
    level[6].push_back(&L6B2Str);
    level[6].push_back(&L6B3Str);
    level[6].push_back(&L6B4Str);
    level[7].push_back(&L7B1Str);
    level[8].push_back(&L8B1Str);

    map2box_name[L1MStr] = "MAIN";
    map2box_name[L2MStr] = "MAIN";
    map2box_name[L2B1Str] = "B1";
    map2box_name[L3B1Str] = "B1";
    map2box_name[L3B2Str] = "B2";
    map2box_name[L4B1Str] = "B1";
    map2box_name[L4B2Str] = "B2";
    map2box_name[L4B3Str] = "B3";
    map2box_name[L4VoidStr] = "VOID";
    map2box_name[L5B1Str] = "B1";
    map2box_name[L5B2Str] = "B2";
    map2box_name[L5B3Str] = "B3";
    map2box_name[L6B1Str] = "B1";
    map2box_name[L6B2Str] = "B2";
    map2box_name[L6B3Str] = "B3";
    map2box_name[L7B1Str] = "B1";
    map2box_name[L8B1Str] = "B1";
}
