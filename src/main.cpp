#include <string>
#include <iostream>
#include "map.hpp"

using namespace std;

extern const string L1B1Str;

int main() {
    Map *L1B1Map = new Map(L1B1Str, nullptr);
    cout << L1B1Map->getMapStr() << endl;

    return 0;
}
