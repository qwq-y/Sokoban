# pragma once
#include <string>

using namespace std;

class Map {
    private:
        string mapStr;
        Map *father;

    public:  
        Map(string mapStr, Map *father) : mapStr(mapStr), father(father) {}

        string getMapStr() const {
            return mapStr;
        };

        Map* getFather() const {
            return father;
        };
};
