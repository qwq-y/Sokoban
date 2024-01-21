#include <string>
#include <map>
#include <vector>
using namespace std;
map<string, string> map2box_name;
vector<string *> level[20];

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
                 "# # # # #\n"; // enter

string L3B1Str = "# # # # # # #\n"
                 "# . . . # . .\n"
                 "# B1 B2 . # P .\n"
                 "# . . . # . .\n"
                 "# . . . # . .\n"
                 "# = - . # . .\n"
                 "# # # # # # #\n"; // enter, go out

string L3B2Str = ". . .\n"
                 "O . .\n"
                 ". . .\n";

string L4B1Str = "# # # # # # # # #\n"
                 "# . . . . . # # #\n"
                 "# . . . . - . = #\n"
                 "# . . . . . # # #\n"
                 "# . . B1 . . # # P\n"
                 "# . . . . . # # #\n"
                 "# . . . . . # # #\n"
                 "# BC1 . # # # # # #\n"
                 "# # # # # # # # #\n"; // Multiple reference(Clone)

string L5B1Str = "# # # # # # # #\n"
                 "# # # # B4 . . #\n"
                 "# . . . # . . #\n"
                 "# P B2 . # . . #\n"
                 "# . . . # . = #\n"
                 "# # # # # # # #\n"; // Multiple reference(Clone)

string L5B2Str = "# # # # # # #\n"
                 "# . . . . . #\n"
                 "# . . B3 . . #\n"
                 ". . . O . . #\n"
                 "# . BC4 . . . #\n"
                 "# . . . . . #\n"
                 "# # # # # # #\n";

string L5B3Str = "# # #\n"
                 "# - #\n"
                 "# - #\n";

string L5B4Str = "# # # #\n"
                 "# . . .\n"
                 "# . . .\n";

string L6B1Str = "# # # # # # # # #\n"
                 "# B1 O B2 B3 O O P O\n"
                 "# # . # # # . . #\n"
                 "# # . . . . . . #\n"
                 "# # . . . . . . #\n"
                 "# # . . . . . . #\n"
                 "# # # # # # # # #\n"; // greedy snake

string L6B2Str = "# # #\n"
                 "# - #\n"
                 "# O #\n";

string L6B3Str = "# # #\n"
                 "# = #\n"
                 "# . #\n";

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
                 "# . . . . . . . . . #\n"; // multiple-infinite

string L8B1Str = "# . . . . . . . . . .\n"
                 "# . . 1I1 . 2I1 . 3I1 . . .\n"
                 "# . . . . . . . . . .\n"
                 "# . . BC1 . P . B1 . . .\n"
                 "# . . . . . . . . . .\n"
                 "# . . . . . # # # # # \n"
                 "# . . . . . # # - - #\n"
                 "# . . . . . # - - # #\n"
                 "# . . . . . . - # # #\n"
                 "# = . . . . # # # # #\n"
                 "# # # # # # # # # # #\n"; // multiple-infinite

string L9B1Str = "# # # #\n"
                 "# B2 B3 #\n"
                 "# # # #\n"; // slide into

string L9B2Str = "# . P\n"
                 "# . .\n"
                 "# = .\n";

string L9B3Str = "O - #\n"
                 ". . #\n"
                 ". . #\n";

string L10B1Str = "# # # #\n"
                  "# B2 B3 #\n"
                  "# # # #\n"; // slide into

string L10B2Str = "# . . P\n"
                  "# . . .\n"
                  "# . . .\n"
                  "# . . .\n"
                  "# . . .\n"
                  "# . . .\n"
                  "# = . .\n";

string L10B3Str = ". . . #\n"
                  ". O - #\n"
                  ". . . #\n";

string L11MAINStr = "# # # # # # #\n"
                    "# . . P . . #\n"
                    "# . BC1 . BC1 . #\n"
                    "# . . . . . #\n"
                    "# . . . . - #\n"
                    "# # # B1 # = #\n"
                    "# # # # # # #\n";

string L11B1Str = ". # . . .\n"
                  ". # # . .\n"
                  ". . . . .\n"
                  ". . . . .\n"
                  ". . . . .\n";
string L11B2Str = "# # #\n"
                  ". . #\n"
                  "# . #\n";

string L11EPSILONStr = ". . . . . B2 .\n"
                       ". . . . . . .\n"
                       ". . . . . . .\n"
                       ". . . . . . .\n"
                       ". . . # . . .\n"
                       ". . . . . . .\n"
                       ". . . . . . .\n";

string L12B1Str = "# . . . . . . . . . . #\n"
                  "# . . . . . . . . . . #\n"
                  "# B1 1I1 2I1 3I1 4I1 5I1 6I1 7I1 8I1 . #\n"
                  "# . . . . P . . . . . #\n"
                  "# . . . . . . . . . . #\n"
                  "# . . . . . . . . . . #\n"
                  "# . . . . . . . . . . #\n"
                  "# . . . . . . . . . . #\n"
                  "# . . . . . . . . . . #\n"
                  "# . . . . . . # # # # #\n"
                  "# . . . . . . # B2 # B3 #\n"
                  "# # # # # # # # # # # # #\n"; // multiple-infinite

string L12B2Str = "# # # # #\n"
                  "# . . B4 #\n"
                  "# . . . #\n"
                  "# 9I1 . . #\n"
                  "# # # # #\n";

string L12B3Str = "# # # # #\n"
                  "# . . = #\n"
                  "# . . . #\n"
                  "# 9E5 . . #\n"
                  "# # # # #\n";

string L12B4Str = "# # # # # # # # # # # #\n"
                  "# # # # # # # # # # # #\n"
                  "# # # # # # # # # # # #\n"
                  "# # # # # # # # # # # #\n"
                  "# # # # # # # # # # # #\n"
                  "# # # # # # # # # # # #\n"
                  ". . . . . . . . . . . #\n"
                  "# # # # # # . . . . . #\n"
                  ". . . . . . . . . . . #\n"
                  ". BC5 BC5 BC5 BC5 BC5 BC5 BC5 BC5 BC5 B5 #\n"
                  "# # # # # # # # # # # #\n"
                  "# # 1E5 2E5 3E5 4E5 5E5 6E5 7E5 8E5 # #\n"
                  "# # # # # # # # # # # #\n";

string L12B5Str = "# # #\n"
                  ". # #\n"
                  "# # .\n";

string L121E5Str = "# # #\n"
                   ". # #\n"
                   "# . #\n";

string L122E5Str = "# # #\n"
                   ". # #\n"
                   "# . .\n";

string L123E5Str = "# . #\n"
                   ". # #\n"
                   "# # #\n";

string L124E5Str = "# # .\n"
                   ". # #\n"
                   "# # #\n";

string L125E5Str = "# . .\n"
                   ". # #\n"
                   "# # #\n";
                   
string L126E5Str = "# . .\n"
                   ". # #\n"
                   "# . #\n";

string L127E5Str = "# . .\n"
                   ". # #\n"
                   "# # .\n";

string L128E5Str = "# # #\n"
                   ". # #\n"
                   "# # #\n";

string L129E5Str = "# . #\n"
                   ". . #\n"
                   "# # #\n";



void build_map2box_name()
{
    level[1].push_back(&L1MStr);
    level[2].push_back(&L2MStr);
    level[2].push_back(&L2B1Str);
    level[3].push_back(&L3B1Str);
    level[3].push_back(&L3B2Str);
    level[4].push_back(&L4B1Str);
    level[5].push_back(&L5B1Str);
    level[5].push_back(&L5B2Str);
    level[5].push_back(&L5B3Str);
    level[5].push_back(&L5B4Str);
    level[6].push_back(&L6B1Str);
    level[6].push_back(&L6B2Str);
    level[6].push_back(&L6B3Str);
    level[7].push_back(&L7B1Str);
    level[8].push_back(&L8B1Str);
    level[9].push_back(&L9B1Str);
    level[9].push_back(&L9B2Str);
    level[9].push_back(&L9B3Str);
    level[10].push_back(&L10B1Str);
    level[10].push_back(&L10B2Str);
    level[10].push_back(&L10B3Str);
    level[11].push_back(&L11MAINStr);
    level[11].push_back(&L11B1Str);
    level[11].push_back(&L11B2Str);
    level[11].push_back(&L11EPSILONStr);
    level[12].push_back(&L12B1Str);
    level[12].push_back(&L12B2Str);
    level[12].push_back(&L12B3Str);
    level[12].push_back(&L12B4Str);
    level[12].push_back(&L12B5Str);
    level[12].push_back(&L121E5Str);
    level[12].push_back(&L122E5Str);
    level[12].push_back(&L123E5Str);
    level[12].push_back(&L124E5Str);
    level[12].push_back(&L125E5Str);
    level[12].push_back(&L126E5Str);
    level[12].push_back(&L127E5Str);
    level[12].push_back(&L128E5Str);
    level[12].push_back(&L129E5Str);

    map2box_name[L1MStr] = "MAIN";
    map2box_name[L2MStr] = "MAIN";
    map2box_name[L2B1Str] = "B1";
    map2box_name[L3B1Str] = "B1";
    map2box_name[L3B2Str] = "B2";
    map2box_name[L4B1Str] = "B1";
    map2box_name[L5B1Str] = "B1";
    map2box_name[L5B2Str] = "B2";
    map2box_name[L5B3Str] = "B3";
    map2box_name[L5B4Str] = "B4";
    map2box_name[L6B1Str] = "B1";
    map2box_name[L6B2Str] = "B2";
    map2box_name[L6B3Str] = "B3";
    map2box_name[L7B1Str] = "B1";
    map2box_name[L8B1Str] = "B1";
    map2box_name[L9B1Str] = "B1";
    map2box_name[L9B2Str] = "B2";
    map2box_name[L9B3Str] = "B3";
    map2box_name[L10B1Str] = "B1";
    map2box_name[L10B2Str] = "B2";
    map2box_name[L10B3Str] = "B3";
    map2box_name[L11MAINStr] = "MAIN";
    map2box_name[L11B1Str] = "B1";
    map2box_name[L11B2Str] = "B2";
    map2box_name[L11EPSILONStr] = "1E1";
    map2box_name[L12B1Str] = "B1";
    map2box_name[L12B2Str] = "B2";
    map2box_name[L12B3Str] = "B3";
    map2box_name[L12B4Str] = "B4";
    map2box_name[L12B5Str] = "B5";
    map2box_name[L121E5Str] = "1E5";
    map2box_name[L122E5Str] = "2E5";
    map2box_name[L123E5Str] = "3E5";
    map2box_name[L124E5Str] = "4E5";
    map2box_name[L125E5Str] = "5E5";
    map2box_name[L126E5Str] = "6E5";
    map2box_name[L127E5Str] = "7E5";
    map2box_name[L128E5Str] = "8E5";
    map2box_name[L129E5Str] = "9E5";
}
