//
// Created on 2024/1/11.
//
#ifndef PROJ_Entity_H
#define PROJ_Entity_H

#include <string>

using namespace std;

class Map;
class Empty;

class Entity
{
protected:
    string mark;
    int row;
    int col;
    Map *now = nullptr;
    Empty *prepared_empty = nullptr;

public:
    explicit Entity(string mark, int row, int col, Map *now) : mark(move(mark)), row(row), col(col), now(now){};

    /**
     * @brief Gets this Entity's mark
     * @return The mark of this Entity
     */
    string get_mark() const
    {
        return mark;
    }

    /**
     * @brief Gets what type this Entity is
     * @return The char represents the type of this Entity
     */
    char get_type() const
    {
        return mark[0];
    }

    /**
     * @brief Gets which map this Entity now is in
     * @return The map pointer points to the map now it is in
     */
    Map *get_map() const
    {
        return now;
    }

    /**
     * @brief Gets this Entity's place
     * @return The map pointer points to the map now it is in
     */
    pair<int, int> get_pos() const
    {
        return make_pair(row, col);
    }

    /**
     * @brief Change this Entity's position;
     */
    void change_pos(pair<int, int> new_pos, Map *new_map)
    {
        row = new_pos.first;
        col = new_pos.second;
        now = new_map;
    }

    void set_prepared_empty(Empty *tmp)
    {
        prepared_empty = tmp;
    }

    Empty *get_prepared_empty()
    {
        return prepared_empty;
    }

    void set_lower()
    {
        if (isdigit(mark[0]))
            mark[1] = tolower(mark[1]);
        mark[0] = tolower(mark[0]);
    }
    void set_upper()
    {
        if (isdigit(mark[0]))
            mark[1] = toupper(mark[1]);
        mark[0] = toupper(mark[0]);
    }
};

/**
 * @brief The player Entity
 */
class Player : public Entity
{
public:
    Player(string mark, int row, int col, Map *now) : Entity(move(mark), row, col, now) {}
};

/**
 * @brief The Empty place Entity
 */
class Empty : public Entity
{
public:
    Empty(int row, int col, Map *now) : Entity(".", row, col, now) {}
};

/**
 * @brief The Wall Entity
 */
class Wall : public Entity
{
public:
    Wall(int row, int col, Map *now) : Entity("#", row, col, now) {}
};

/**
 * @brief The box destination Entity
 */
class Box_des : public Entity
{
public:
    Box_des(int row, int col, Map *now) : Entity("-", row, col, now) {}
};

/**
 * @brief The Player destination Entity
 */
class P_des : public Entity
{
public:
    P_des(int row, int col, Map *now) : Entity("=", row, col, now) {}
};

/**
 * @brief The O box Entity
 */
class Obox : public Entity
{
public:
    Obox(string mark, int row, int col, Map *now) : Entity(move(mark), row, col, now) {}
};

/**
 * @brief The B box Entity
 */
class Bbox : public Entity
{
public:
    Bbox(string mark, int row, int col, Map *now) : Entity(move(mark), row, col, now) {}
};

/**
 * @brief The I box Entity
 */
class Ibox : public Entity
{
private:
    string belong; // The name of the B box
    int layer;

public:
    Ibox(string mark, int row, int col, Map *now, string belong, int layer) : Entity(move(mark), row, col, now), belong(belong), layer(layer) {}

    /**
     * @brief get the B box this I box belong to and the layer number
     * @return The name of the Bbox this I box belong to and the layer number
     */
    pair<string, int> get_belong_info()
    {
        return make_pair(belong, layer);
    }
};

/**
 * @brief The Clone box Entity
 */
class Clone : public Entity
{
public:
    Clone(string mark, int row, int col, Map *now) : Entity(move(mark), row, col, now) {}
};

/**
 * @brief The Epsilon box Entity
 */
class Epsilon : public Entity
{
public:
    Epsilon(string mark, int row, int col, Map *now) : Entity(move(mark), row, col, now) {}
};

#endif