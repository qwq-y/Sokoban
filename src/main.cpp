#include <string>
#include <iostream>
#include "map.hpp"

using namespace std;

extern string defaultStr;

int main() {
    Map *defaultMap = new Map(defaultStr, nullptr);
    cout << defaultMap->getMapStr() << endl;

    return 0;
}
