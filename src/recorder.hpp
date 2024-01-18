//
// Created on 2024/1/11.
//
#ifndef PROJ_RECORDER_H
#define PROJ_RECORDER_H

#include "map.hpp"
#include "Entity.hpp"

struct recorder
{
    Map *now;
    int x, y;
    Entity *before;
    bool lower = false; // false大写，true小写
    bool openVoid = false;
    bool clearVoid = false;
};

#endif