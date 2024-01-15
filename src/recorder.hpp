//
// Created on 2024/1/11.
//
#ifndef PROJ_RECORDER_H
#define PROJ_RECORDER_H

#include "map.hpp"
#include "Entity.hpp"

struct recorder
{
    Map* now;
    int x, y;
    Entity *before;
    Entity *after;
};

#endif